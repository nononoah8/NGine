#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

struct Texture {
  unsigned int id;
  std::string type;
  std::string path;
};

struct Material {
  unsigned int diffuseMap;
  unsigned int specularMap;
  unsigned int normalMap;
  float shininess;
};

class Mesh {
private:
  GLuint VAO, VBO, EBO;
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  unsigned int vertexCount, indexCount;
  bool hasTextureCoords;

public:
  Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
  Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);

  ~Mesh();

  void Draw(unsigned int shaderProgram) const;

  // Prevent copying
  Mesh(const Mesh&) = delete;
  Mesh& operator=(const Mesh&) = delete;
  
  // Allow moving
  Mesh(Mesh&& other) noexcept;
  Mesh& operator=(Mesh&& other) noexcept;
};


#endif // MESH_H