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
      );
  };
}

#endif // CUBE_H