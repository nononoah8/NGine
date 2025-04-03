#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Function to load shader source from a file
std::string loadShaderSource(const char* filePath) {
    std::ifstream shaderFile(filePath);
    if (!shaderFile.is_open()) {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return "";
    }
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    return shaderStream.str();
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Set SDL to use OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create an SDL window
    int width = 800;
    int height = 600;
    SDL_Window* window = SDL_CreateWindow(
        "SDL + OpenGL", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        width, 
        height, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create an OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Load and compile the vertex shader
    std::string vertexShaderSource = loadShaderSource("shaders/vertex/triangle.vert");
    const char* vertexShaderCode = vertexShaderSource.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertexShader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    std::string fragmentShaderSource = loadShaderSource("shaders/fragment/triangle.frag");
    const char* fragmentShaderCode = fragmentShaderSource.c_str();
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Clean up the fragment shader (no longer needed after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Define vertices with positions and colors
    float vertices[] = {
        // Position (XYZ)    // Color (RGB)
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // Bottom-left (red)
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // Bottom-right (green)
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // Top-left (blue)
        0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f, // Top-right (yellow)
        // Repeat for back vertices with different colors
        -0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 1.0f, // Back bottom-left (magenta)
        0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, // Back bottom-right (cyan)
        -0.5f,  0.5f, 0.5f,  0.5f, 0.5f, 1.0f, // Back top-left (light blue)
        0.5f,  0.5f, 0.5f,  1.0f, 0.5f, 0.5f  // Back top-right (light red)
    };

    // Define indices to form the faces of a cube (12 triangles)
    unsigned int indices[] = {
        // Front face
        0, 1, 2,
        1, 3, 2,
        // Back face
        4, 5, 6,
        5, 7, 6,
        // Left face
        0, 2, 4,
        2, 6, 4,
        // Right face
        1, 3, 5,
        3, 7, 5,
        // Bottom face
        0, 1, 4,
        1, 5, 4,
        // Top face
        2, 3, 6,
        3, 7, 6
    };

    // Create a Vertex Buffer Object (VBO) and Vertex Array Object (VAO)
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind and set the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Get the location of the "time" uniform
    GLint iTimeLocation = glGetUniformLocation(shaderProgram, "iTime");
    GLint iResolutionLocation = glGetUniformLocation(shaderProgram, "iResolution");
    if (iTimeLocation == -1 || iResolutionLocation == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM::NOT_FOUND" << std::endl;
    }
    int iTime = 0;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // At the top of main(), add or modify these variables
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    float rotationSpeed = 0.002f;  // Reduce this value to make rotation slower
    float positionX = 0.0f;
    float positionY = 0.0f;
    float positionZ = 0.0f;
    float moveSpeed = 0.001f;

    // Main render loop
    bool running = true;
    SDL_Event event;
    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                width = event.window.data1;
                height = event.window.data2;
                glViewport(0, 0, width, height);
            }
        }

        // Check keyboard state for continuous input
        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        if (keyState[SDL_SCANCODE_A]) {
            rotationY -= rotationSpeed;  // Rotate left around Y-axis
        }
        if (keyState[SDL_SCANCODE_D]) {
            rotationY += rotationSpeed;  // Rotate right around Y-axis
        }
        if (keyState[SDL_SCANCODE_W]) {
            rotationX -= rotationSpeed;  // Rotate up around X-axis
        }
        if (keyState[SDL_SCANCODE_S]) {
            rotationX += rotationSpeed;  // Rotate down around X-axis
        }

        // After getting uniform locations
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");

        if (keyState[SDL_SCANCODE_LEFT]) {
            positionX -= moveSpeed;  // Move left
        }
        if (keyState[SDL_SCANCODE_RIGHT]) {
            positionX += moveSpeed;  // Move right
        }
        if (keyState[SDL_SCANCODE_UP]) {
            positionY += moveSpeed;  // Move up
        }
        if (keyState[SDL_SCANCODE_DOWN]) {
            positionY -= moveSpeed;  // Move down
        }

        // Create combined transformation matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(positionX, positionY, positionZ));
        model = glm::rotate(model, rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the triangle
        glUseProgram(shaderProgram);

        // Pass uniform values to the shader
        glUniform1i(iTimeLocation, iTime);
        glUniform2f(iResolutionLocation, (float)width, (float)height);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Swap buffers
        SDL_GL_SwapWindow(window);

        ++iTime;
    }

    // Clean up
    glDeleteBuffers(1, &EBO);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}