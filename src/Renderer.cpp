//
//  Renderer.cpp
//  game_engine
//
//  Created by Noah Dujovny on 2/3/25.
//

#include <iostream>
#include "Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Input.h"

// Define static members
int Renderer::x_resolution = 640;
int Renderer::y_resolution = 360;
SDL_Window* Renderer::window = nullptr;

float Renderer::clear_color_r = 1.0f;
float Renderer::clear_color_g = 1.0f;
float Renderer::clear_color_b = 1.0f;

glm::vec2 Renderer::camera_size;
glm::vec3 Renderer::camera_pos;
float Renderer::zoom;

glm::vec3 Renderer::cameraFront(0.0f, 0.0f, -1.0f);
glm::vec3 Renderer::cameraUp(0.0f, 1.0f, 0.0f);
float Renderer::cameraYaw = -90.0f;
float Renderer::cameraPitch = 0.0f;


SDL_GLContext Renderer::glContext = nullptr;

void Renderer::LoadRenderer(int x_res, int y_res, int r, int g, int b, glm::vec2 cam_size, float z, glm::vec2 cam_pos) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Set OpenGL attributes BEFORE window creation
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    
    x_resolution = x_res;
    y_resolution = y_res;

    clear_color_r = r / 255.0f;
    clear_color_g = g / 255.0f;
    clear_color_b = b / 255.0f;

    camera_size = cam_size;
    camera_pos = glm::vec3(cam_pos, 1.0f);
    zoom = z;

    camera_pos = glm::vec3(cam_pos.x, cam_pos.y, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraYaw = -90.0f;
    cameraPitch = 0.0f;
    
}

void Renderer::RenderWindow(const std::string& title) {
    if(!window) {
        window = SDL_CreateWindow(title.c_str(), 100, 100, x_resolution, y_resolution, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    }
    
    if (!window) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        std::exit(1);
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        std::exit(1);
    }

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        std::exit(1);
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Renderer::Update() {
    SDL_Event e;
    
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return false;
        }
        Input::ProcessEvent(e);
    }

    return true;  // Continue game loop
}

void Renderer::ClearScreen() {
    glClearColor(clear_color_r, clear_color_g, clear_color_b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SwapBuffers() {
    SDL_GL_SwapWindow(window);
}

void Renderer::DestroyScreen() {
    if (glContext) {
        SDL_GL_DeleteContext(glContext);
        glContext = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}

float Renderer::GetZoom() {
    return zoom;
}

glm::vec2 Renderer::GetCamSize() {
    return camera_size;
}

glm::vec3 Renderer::GetCamPos() {
    return camera_pos;
}


void Renderer::SetCamPos(float x, float y, float z) {
    camera_pos.x = x;
    camera_pos.y = y;
    camera_pos.z = z;
}

float Renderer::GetCamPosX() {
    return camera_pos.x;
}

float Renderer::GetCamPosY() {
    return camera_pos.y;
}

float Renderer::GetCamPosZ() {
    return camera_pos.z;
}

void Renderer::SetCamZoom(float z) {
    zoom = z;
}

float Renderer::GetCamZoom() {
    return zoom;
}

// Camera Front direction
glm::vec3 Renderer::GetCameraFront() {
    return cameraFront;
}

void Renderer::SetCameraFront(const glm::vec3& front) {
    cameraFront = glm::normalize(front);
}

// Camera Up vector
glm::vec3 Renderer::GetCameraUp() {
    return cameraUp;
}

void Renderer::SetCameraUp(const glm::vec3& up) {
    cameraUp = glm::normalize(up);
}

// Camera rotation angles
float Renderer::GetCameraYaw() {
    return cameraYaw;
}

void Renderer::SetCameraYaw(float yaw) {
    cameraYaw = yaw;
}

float Renderer::GetCameraPitch() {
    return cameraPitch;
}

void Renderer::SetCameraPitch(float pitch) {
    // Clamp pitch to avoid gimbal lock
    cameraPitch = glm::clamp(pitch, -89.0f, 89.0f);
}

// Update camera direction vector from Euler angles (yaw and pitch)
void Renderer::UpdateCameraDirection() {
    glm::vec3 front;
    front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    front.y = sin(glm::radians(cameraPitch));
    front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront = glm::normalize(front);
}

// Get the camera view matrix
glm::mat4 Renderer::GetViewMatrix() {
    return glm::lookAt(
        camera_pos,
        camera_pos + cameraFront,
        cameraUp
    );
}

void Renderer::Cleanup() {
    SDL_GL_DeleteContext(glContext);
}