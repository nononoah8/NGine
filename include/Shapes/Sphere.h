#ifndef SPHERE_H
#define SPHERE_H

#include "Mesh.h"
#include <vector>
#include <memory>
#include <cmath>
#include <glm/glm.hpp>

namespace Shape {
  class Sphere {
    public:
    /**
     * Create a sphere mesh.
     * 
     * @param radius The radius size of sphere
     * @param color The color of the sphere (RGB)
     * @param segments The number of segments for sphere resolution
     * @return A unique pointer to a new Mesh object
     */
    static std::unique_ptr<Mesh> Create(float radius = 1.0f, 
                                        const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f),
                                        int segments = 16) {
        // Generate sphere vertices and indices
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        // Generate the vertices
        for(int y = 0; y <= segments; ++y) {
          for(int x = 0; x <= segments; ++x) {
            float xSegment = (float)x / (float)segments;
            float ySegment = (float)y / (float)segments;

            float xPos = glm::cos(xSegment * 2.0f * M_PI) * glm::sin(ySegment * M_PI) * radius;
            float yPos = glm::cos(ySegment * M_PI) * radius;
            float zPos = glm::sin(xSegment * 2.0f * M_PI) * glm::sin(ySegment * M_PI) * radius;

            // Position
            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);

            // Color
            vertices.push_back(color.r);
            vertices.push_back(color.g);
            vertices.push_back(color.b);
          }
        }

        // Generate the indices
        for(int y = 0; y < segments; ++y) {
          for(int x = 0; x < segments; ++x) {
            unsigned int current = y * (segments + 1) + x;
            unsigned int next = current + 1;
            unsigned int bottom = (y + 1) * (segments + 1) + x;
            unsigned int bottomNext = bottom + 1;

            // First triangle (top-left, bottom-left, top-right)
            indices.push_back(current);
            indices.push_back(bottom);
            indices.push_back(next);
            
            // Second triangle (top-right, bottom-left, bottom-right)
            indices.push_back(next);
            indices.push_back(bottom);
            indices.push_back(bottomNext);
          }
        }

        return std::make_unique<Mesh>(vertices, indices);
      }
  };
}

#endif // SPHERE_H