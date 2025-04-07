#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Mesh.h"
#include "Shapes/Cube.h"


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

    // Create Shader program
    Shader shaderProgram("shaders/vertex/triangle.vert", "shaders/fragment/triangle.frag");
    if (!shaderProgram.GetID()) {
        std::cerr << "Failed to create shader program" << std::endl;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Get the location of the uniforms
    GLuint programID = shaderProgram.GetID();
    GLint iTimeLocation = glGetUniformLocation(programID, "iTime");
    GLint iResolutionLocation = glGetUniformLocation(programID, "iResolution");
    GLint modelLoc = glGetUniformLocation(programID, "model");
    
    if (iTimeLocation == -1 || iResolutionLocation == -1 || modelLoc == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM::NOT_FOUND" << std::endl;
    }
    int iTime = 0;

    // Create a cube
    std::unique_ptr<Mesh> cube = Shape::Cube::Create(1.0f, glm::vec3(1.0f, 1.0f, 0.0f));


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

        // Clear the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader program and set uniforms
        shaderProgram.Use();
        glUniform1i(iTimeLocation, iTime);
        glUniform2f(iResolutionLocation, (float)width, (float)height);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Draw the cube
        cube->Draw();

        // Swap buffers
        SDL_GL_SwapWindow(window);

        ++iTime;
    }

    // Clean up
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

/*
Next steps:
* Make more shapes and make them more accessible and easier to use by creating a mesh class, and a shape generator
* Need to have a light and camera manager, and make a renderer manager.
* Also have a transform class
* Implement the input manager, scene manager, and engine manager, and others.
* Can use https://github.com/assimp/assimp - assimp for asset importing for models and whatnot.
* Add physics engine - https://github.com/jrouwe/JoltPhysics, https://github.com/NVIDIA-Omniverse/PhysX, https://github.com/bulletphysics/bullet3, etc.
* Entity-Component system
* Debugging framework
* Paticle System
* UI Framework (Dear ImGui)
*/