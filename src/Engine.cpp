//
//  Engine.cpp
//  game_engine
//
//  Created by Noah Dujovny on 2/10/25.
//

#include "Engine.hpp"
#include "Input.h"
#include "ComponentManager.h"
#include "ComponentDB.hpp"
#include "EventSystem.h"

#include "Renderer.h"
#include "Shader.h"

#include "Mesh.h"
#include "Shapes/Cube.h"
#include "Shapes/Sphere.h"
#include "GameObject.h"

bool DEBUG = false;

// Engine initialization.
Engine::Engine() {
    if (!std::filesystem::exists("resources/")) {
        std::cout << "error: resources/ missing";
        std::exit(0);
    }
    else if (!std::filesystem::exists("resources/game.config")) {
        std::cout << "error: resources/game.config missing";
        std::exit(0);
    }

    // Start and initialize the lua shi
    ComponentManager::Initialize();
    ComponentDB::Init();

    SetupInitialProps();

    // Create Shader program
    shaderProgram = std::make_shared<Shader>("shaders/vertex/vertex.glsl", "shaders/fragment/fragment.glsl");
    std::cout << "made shader program" << std::endl;
    if (!shaderProgram->GetID()) {
        // SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(Renderer::window);
        SDL_Quit();
        exit(1);
    }

    SetupShaderUniforms();
}

// Engine's gameloop.
// Will run until SDL receivs a quit command.
void Engine::GameLoop() {
    bool running = true;
    
    // Set to wireframe mode if debug flag is true
    if(DEBUG) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Create camera matrices once (or update when camera changes)
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f), // Camera position
        glm::vec3(0.0f, 0.0f, 0.0f), // Look target
        glm::vec3(0.0f, 1.0f, 0.0f)  // Up vector
    );
    
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (float)renderingSettings.cameraSize.x / (float)renderingSettings.cameraSize.y,
        0.1f, 
        100.0f
    );

    while (running) {
        // Process input events
        if (Scene::load_new_scene) {
            current_scene.~Scene();
            current_scene = Scene();
            current_scene.LoadScene(Scene::scene_to_load);
            Scene::load_new_scene = false;
        }

        // Process events
        running = Renderer::Update();

        // Clear screen for renderer
        Renderer::ClearScreen();

        shaderProgram->Use();

        // Set view and projection (camera) uniforms
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // UpdateGame();


        // Render 3d scene objects


        // Render all of the queued stuff
        // ImageDB::RenderAndClearImages();

        // Render all of the queued text
        // TextDB::RenderText();

        // Render all of the queued pixels
        // ImageDB::RenderAndClearPixels();

        // Process pending event subscriptions
        EventSystem::ProcessPendingChanges();
        
        // Swap buffers at the end
        Renderer::SwapBuffers();

        Input::LateUpdate();
    }

    TextDB::Shutdown();
    AudioDB::Shutdown();
} 


void Engine::SetupInitialProps() {
    std::cout << "setting up init props" << std::endl;
    rapidjson::Document doc;
    ReadJsonFile("resources/game.config", doc);

    
    // game title
    game_title = getJsonStringOrDefault(doc, "game_title", "");

    std::string font_name = getJsonStringOrDefault(doc, "font", "");
    
    std::string initial_scene = getJsonStringOrDefault(doc, "initial_scene", "");
    if (initial_scene.empty()) {
        std::cout << "error: initial_scene unspecified";
        exit(0);
    }

    // Setup the custom camera
    if(std::filesystem::exists("resources/rendering.config")) {
        ReadJsonFile("resources/rendering.config", doc);
        renderingSettings.cameraSize.x = getJsonIntOrDefault(doc, "x_resolution", 640);
        renderingSettings.cameraSize.y = getJsonIntOrDefault(doc, "y_resolution", 360);

        renderingSettings.colorR = getJsonIntOrDefault(doc, "clear_color_r", 255);
        renderingSettings.colorG = getJsonIntOrDefault(doc, "clear_color_g", 255);
        renderingSettings.colorB = getJsonIntOrDefault(doc, "clear_color_b", 255);
        renderingSettings.zoomFactor = getJsonFloatOrDefault(doc, "zoom_factor", 1.0f);
        DEBUG = getJsonBoolOrDefault(doc, "debug", false);
    }else{
        renderingSettings.cameraSize.x = 640;
        renderingSettings.cameraSize.y = 360;
    }

    Renderer::LoadRenderer(renderingSettings.cameraSize.x, renderingSettings.cameraSize.y, renderingSettings.colorR, renderingSettings.colorG, renderingSettings.colorB, renderingSettings.cameraSize, renderingSettings.zoomFactor, renderingSettings.cameraPos);
    Renderer::RenderWindow(game_title);

    //TODO: Make sure scene loading and everything works properly.
    // current_scene = Scene();
    // current_scene.LoadScene(initial_scene);
    
    // TextDB::Init(Renderer::renderer);
    // Input::Init();
    // AudioDB::Init();
}


void Engine::UpdateGame() {
    current_scene.Update();
}

// In Engine.cpp, at the end of the Engine constructor or in a separate method
void Engine::SetupShaderUniforms() {
    // Make sure the shader is valid
    if (!shaderProgram || !shaderProgram->GetID()) {
        std::cerr << "Cannot setup uniforms: Invalid shader program" << std::endl;
        return;
    }
    
    // Get shader program ID
    GLuint programID = shaderProgram->GetID();
    
    // Store uniform locations
    modelLoc = glGetUniformLocation(programID, "model");
    viewLoc = glGetUniformLocation(programID, "view");
    projectionLoc = glGetUniformLocation(programID, "projection");
    
    // Add error checking if needed
    if (modelLoc == -1) {
        std::cerr << "Warning: Uniform 'model' not found in shader" << std::endl;
    }

    if (viewLoc == -1) {
        std::cerr << "Warning: Uniform 'view' not found in shader" << std::endl;
    }

    if (projectionLoc == -1) {
        std::cerr << "Warning: Uniform 'projection' not found in shader" << std::endl;
    }
}
