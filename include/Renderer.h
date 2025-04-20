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
    static glm::vec3 GetCamPos();
    static void SetCamPos(float x, float y, float z);
    static float GetCamPosX();
    static float GetCamPosY();
    static float GetCamPosZ();
    static void SetCamZoom(float z);
    static float GetCamZoom();

    // Camera Direction API
    static glm::vec3 GetCameraFront();
    static void SetCameraFront(const glm::vec3& front);
    static glm::vec3 GetCameraUp();
    static void SetCameraUp(const glm::vec3& up);
    static float GetCameraYaw();
    static void SetCameraYaw(float yaw);
    static float GetCameraPitch();
    static void SetCameraPitch(float pitch);

    // Update camera direction based on yaw and pitch
    static void UpdateCameraDirection();

    // Get the view matrix for forward-looking camera
    static glm::mat4 GetViewMatrix();
    

    static void Cleanup();

    static SDL_GLContext GetContext() { return glContext; };

    static SDL_Window* window;

    static int x_resolution;
    static int y_resolution;
private:
    static glm::vec2 camera_size;
    static glm::vec3 camera_pos;
    static float zoom;
    static float clear_color_r;
    static float clear_color_g;
    static float clear_color_b;
    static glm::vec3 cameraFront;
    static glm::vec3 cameraUp;
    static float cameraYaw;
    static float cameraPitch;

    static SDL_GLContext glContext;
};

#endif
