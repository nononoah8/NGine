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

#include "Input.h"
#include "Shader.h"
#include "Mesh.h"
#include "Shapes/Cube.h"
#include "Shapes/Sphere.h"
#include "Renderer.h"

#include "Engine.hpp"


int main(int argc, char* argv[]) {
    Engine engine = Engine();

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Main render loop
    engine.GameLoop();

    // Clean up
    SDL_Quit();

    return 0;
}

/*
Next steps:
* UI Framework (Dear ImGui)
* Textures on cube/sphere and shapes
* Skybox
* Add physics engine - https://github.com/jrouwe/JoltPhysics, https://github.com/NVIDIA-Omniverse/PhysX, https://github.com/bulletphysics/bullet3, etc.
* Paticle System
* HUD stuff

Misc stuff can add:
* Frame cap you can choose
*/