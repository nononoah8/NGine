#ifndef PLANE_H
#define PLANE_H

#include "Mesh.h"
#include <memory>
#include <glm/glm.hpp>

namespace Shape {
  class Plane {
    public:
    /**
     * Create a plane mesh.
     * 
     * @param width The width of the plane (x-axis)
     * @param length The length of the plane (z-axis)
     * @param color The color of the plane (RGB)
     * @return A unique pointer to a new Mesh object
     */
    static std::unique_ptr<Mesh> Create(
      float width = 1.0f,
      float length = 1.0f,
      const glm::vec3& color = glm::vec3(1.0f)
    ) {
      float halfWidth = width / 2.0f;
      float halfLength = length / 2.0f;
    
      std::vector<float> vertices = {
        // Format: X, Y, Z, R, G, B, NX, NY, NZ
        
        // Plane with normal pointing up (Y+)
        -halfWidth, 0.0f, -halfLength, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f, // Bottom-left
          halfWidth, 0.0f, -halfLength, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f, // Bottom-right
        -halfWidth, 0.0f,  halfLength, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f, // Top-left
          halfWidth, 0.0f,  halfLength, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f  // Top-right
      };
      
      std::vector<unsigned int> indices = {
        // Two triangles to form the plane
        0, 1, 2,  // First triangle
        1, 3, 2   // Second triangle
      };
      
      return std::make_unique<Mesh>(vertices, indices);
    }
    
    /**
     * Create a plane mesh with texture coordinates.
     * 
     * @param width The width of the plane (x-axis)
     * @param length The length of the plane (z-axis)
     * @param color The color of the plane (RGB)
     * @param textures The textures to apply to the plane
     * @return A unique pointer to a new Mesh object
     */
    static std::unique_ptr<Mesh> CreateTextured(
      float width = 1.0f,
      float length = 1.0f,
      const glm::vec3& color = glm::vec3(1.0f),
      const std::vector<Texture>& textures = {}
    ) {
      float halfWidth = width / 2.0f;
      float halfLength = length / 2.0f;
    
      std::vector<float> vertices = {
        // Format: X, Y, Z, R, G, B, NX, NY, NZ, U, V
        
        // Plane with normal pointing up (Y+)
        -halfWidth, 0.0f, -halfLength, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
          halfWidth, 0.0f, -halfLength, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom-right
        -halfWidth, 0.0f,  halfLength, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Top-left
          halfWidth, 0.0f,  halfLength, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f  // Top-right
      };
      
      std::vector<unsigned int> indices = {
        // Two triangles to form the plane
        0, 1, 2,  // First triangle
        1, 3, 2   // Second triangle
      };

      Material material;
      material.ambient = color * 0.1f;
      material.diffuse = color;
      material.specular = glm::vec3(0.5f);
      material.shininess = 32.0f;
      material.useTexture = !textures.empty();
      
      
      return std::make_unique<Mesh>(vertices, indices, textures, material);
    }
  };
}


#endif