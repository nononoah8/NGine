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

    // Initialize the physics world

    SetupInitialProps();
}

// Engine's gameloop.
// Will run until SDL receivs a quit command.
void Engine::GameLoop() {
    bool running = true;

    while (running) {
        if (Scene::load_new_scene) {
            current_scene.~Scene();
            current_scene = Scene();
            current_scene.LoadScene(Scene::scene_to_load);
            Scene::load_new_scene = false;
        }

        // running = Renderer::Update();

        // SDL_SetRenderDrawColor(Renderer::renderer, renderingSettings.colorR, renderingSettings.colorG, renderingSettings.colorB, 255);
        // SDL_RenderClear(Renderer::renderer);

        UpdateGame();

        // Render all of the queued stuff
        // ImageDB::RenderAndClearImages();

        // Render all of the queued text
        TextDB::RenderText();

        // Render all of the queued pixels
        // ImageDB::RenderAndClearPixels();

        // Process pending event subscriptions
        EventSystem::ProcessPendingChanges();
        
        // Helper::SDL_RenderPresent(Renderer::renderer);

        Input::LateUpdate();
    }

    TextDB::Shutdown();
    AudioDB::Shutdown();
} 


void Engine::SetupInitialProps() {
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
        renderingSettings.easeFactor = getJsonFloatOrDefault(doc, "cam_ease_factor", 1.0f);
    }else{
        renderingSettings.cameraSize.x = 640;
        renderingSettings.cameraSize.y = 360;
    }

    // Renderer::LoadRenderer(renderingSettings.cameraSize.x, renderingSettings.cameraSize.y, renderingSettings.colorR, renderingSettings.colorG, renderingSettings.colorB, renderingSettings.cameraSize, renderingSettings.zoomFactor, renderingSettings.cameraPos);
    // Renderer::RenderWindow(game_title);

    current_scene = Scene();
    current_scene.LoadScene(initial_scene);
    
    // TextDB::Init(Renderer::renderer);
    Input::Init();
    AudioDB::Init();
}


void Engine::UpdateGame() {
    current_scene.Update();
}

