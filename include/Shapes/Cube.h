#ifndef CUBE_H
#define CUBE_H

#include "Mesh.h"
#include <memory>
#include <glm/glm.hpp>

namespace Shape {
  class Cube {
    public:
      /**
       * Create a cube mesh.
       * 
       * @param size The length of each side of the cube
       * @param color The color of the cube (RGB)
       * @return A unique pointer to a new Mesh object
       */
      static std::unique_ptr<Mesh> Create(
        float size = 1.0f, 
        const glm::vec3& color = glm::vec3(1.0f)
      ) {
        float halfSize = size / 2.0f;
      
        std::vector<float> vertices = {
          // Position (XYZ)               // Color (RGB)
         -halfSize, -halfSize, -halfSize, color.r, color.g, color.b, // 0: Bottom-left-front
          halfSize, -halfSize, -halfSize, color.r, color.g, color.b, // 1: Bottom-right-front
         -halfSize,  halfSize, -halfSize, color.r, color.g, color.b, // 2: Top-left-front
          halfSize,  halfSize, -halfSize, color.r, color.g, color.b, // 3: Top-right-front
         -halfSize, -halfSize,  halfSize, color.r, color.g, color.b, // 4: Bottom-left-back
          halfSize, -halfSize,  halfSize, color.r, color.g, color.b, // 5: Bottom-right-back
         -halfSize,  halfSize,  halfSize, color.r, color.g, color.b, // 6: Top-left-back
          halfSize,  halfSize,  halfSize, color.r, color.g, color.b  // 7: Top-right-back
        };
        
        std::vector<unsigned int> indices = {
          // Front face
          0, 1, 2, 1, 3, 2,
          // Back face
          4, 5, 6, 5, 7, 6,
          // Left face
          0, 2, 4, 2, 6, 4,
          // Right face
          1, 3, 5, 3, 7, 5,
          // Bottom face
          0, 1, 4, 1, 5, 4,
          // Top face
          2, 3, 6, 3, 7, 6
        };
        
        return std::make_unique<Mesh>(vertices, indices);
      }
  };
}

#endif // CUBE_H