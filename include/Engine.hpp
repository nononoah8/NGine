//
//  Engine.hpp
//  game_engine
//
//  Created by Noah Dujovny on 1/20/25.
//
#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include <iostream>
#include <array>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <sstream>

#include "ReadJSON.h"
#include "SceneDB.hpp"
#include "TemplateDB.hpp"
#include "TextDB.h"
#include "AudioDB.h"

#include "glm/glm.hpp"
#include "Lua/lua.hpp"

// Grouping structs

struct RenderingSettings {
    glm::ivec2 cameraSize;
    glm::vec2 cameraPos = glm::vec2(0, 0);
    int colorR = 255;
    int colorG = 255;
    int colorB = 255;
    float zoomFactor=1.0f;
    float easeFactor=1.0f;
};

// Engine Class: runs the game engine
class Engine {
public:
    Engine();
    void GameLoop();
    std::vector<std::string> OrderDialogue();
    void SetupInitialProps();

    // Game loop functions
    void UpdateGame();

    bool are_intro_images = false;
    bool are_intro_text = false;
    bool is_player_actor=false;

    Scene current_scene;
private:
    std::string game_title;

    RenderingSettings renderingSettings;

    lua_State* lua_state = nullptr;
};

#endif
