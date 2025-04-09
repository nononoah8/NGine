//
//  Renderer.h
//  game_engine
//
//  Created by Noah Dujovny on 2/3/25.
//
#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

#include "glm/glm.hpp"
#include "glad/glad.h"

class Renderer {
public:
    static void LoadRenderer(int x_res, int y_res, int r, int g, int b, glm::vec2 cam_size, float z, glm::vec2 cam_pos);
    
    static void RenderWindow(const std::string& title);
    
    static bool Update();
    
    static void ClearScreen();
    static void SwapBuffers();
    
    static void DestroyScreen();

    // Camera API
    static float GetZoom();
    static glm::vec2 GetCamSize();
    static glm::vec2 GetCamPos();
    static void SetCamPos(float x, float y);
    static float GetCamPosX();
    static float GetCamPosY();
    static void SetCamZoom(float z);
    static float GetCamZoom();

    static void Cleanup();

    static SDL_GLContext GetContext() { return glContext; };

    static SDL_Window* window;

    static int x_resolution;
    static int y_resolution;
private:
    static glm::vec2 camera_size;
    static glm::vec2 camera_pos;
    static float zoom;
    static float clear_color_r;
    static float clear_color_g;
    static float clear_color_b;

    static SDL_GLContext glContext;
};

#endif
