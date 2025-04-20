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
          // Format: X, Y, Z, R, G, B, NX, NY, NZ
          
          // Front face (normal: 0, 0, -1)
          -halfSize, -halfSize, -halfSize, color.r, color.g, color.b, 0.0f, 0.0f, -1.0f, // Bottom-left
           halfSize, -halfSize, -halfSize, color.r, color.g, color.b, 0.0f, 0.0f, -1.0f, // Bottom-right
          -halfSize,  halfSize, -halfSize, color.r, color.g, color.b, 0.0f, 0.0f, -1.0f, // Top-left
           halfSize,  halfSize, -halfSize, color.r, color.g, color.b, 0.0f, 0.0f, -1.0f, // Top-right
          
          // Back face (normal: 0, 0, 1)
          -halfSize, -halfSize,  halfSize, color.r, color.g, color.b, 0.0f, 0.0f, 1.0f,
           halfSize, -halfSize,  halfSize, color.r, color.g, color.b, 0.0f, 0.0f, 1.0f,
          -halfSize,  halfSize,  halfSize, color.r, color.g, color.b, 0.0f, 0.0f, 1.0f,
           halfSize,  halfSize,  halfSize, color.r, color.g, color.b, 0.0f, 0.0f, 1.0f,
          
          // Left face (normal: -1, 0, 0)
          -halfSize, -halfSize, -halfSize, color.r, color.g, color.b, -1.0f, 0.0f, 0.0f,
          -halfSize,  halfSize, -halfSize, color.r, color.g, color.b, -1.0f, 0.0f, 0.0f,
          -halfSize, -halfSize,  halfSize, color.r, color.g, color.b, -1.0f, 0.0f, 0.0f,
          -halfSize,  halfSize,  halfSize, color.r, color.g, color.b, -1.0f, 0.0f, 0.0f,
          
          // Right face (normal: 1, 0, 0)
           halfSize, -halfSize, -halfSize, color.r, color.g, color.b, 1.0f, 0.0f, 0.0f,
           halfSize,  halfSize, -halfSize, color.r, color.g, color.b, 1.0f, 0.0f, 0.0f,
           halfSize, -halfSize,  halfSize, color.r, color.g, color.b, 1.0f, 0.0f, 0.0f,
           halfSize,  halfSize,  halfSize, color.r, color.g, color.b, 1.0f, 0.0f, 0.0f,
          
          // Bottom face (normal: 0, -1, 0)
          -halfSize, -halfSize, -halfSize, color.r, color.g, color.b, 0.0f, -1.0f, 0.0f,
           halfSize, -halfSize, -halfSize, color.r, color.g, color.b, 0.0f, -1.0f, 0.0f,
          -halfSize, -halfSize,  halfSize, color.r, color.g, color.b, 0.0f, -1.0f, 0.0f,
           halfSize, -halfSize,  halfSize, color.r, color.g, color.b, 0.0f, -1.0f, 0.0f,
          
          // Top face (normal: 0, 1, 0)
          -halfSize,  halfSize, -halfSize, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f,
           halfSize,  halfSize, -halfSize, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f,
          -halfSize,  halfSize,  halfSize, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f,
           halfSize,  halfSize,  halfSize, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f
        };
        
        std::vector<unsigned int> indices = {
          // Front face
          0, 1, 2, 1, 3, 2,
          // Back face
          4, 5, 6, 5, 7, 6,
          // Left face
          8, 9, 10, 9, 11, 10,
          // Right face
          12, 13, 14, 13, 15, 14,
          // Bottom face
          16, 17, 18, 17, 19, 18,
          // Top face
          20, 21, 22, 21, 23, 22
        };
        
        return std::make_unique<Mesh>(vertices, indices);
      }
  };
}

#endif // CUBE_H