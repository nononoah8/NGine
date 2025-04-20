#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

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
  unsigned int diffuseMap = UINT_MAX;
  unsigned int specularMap = UINT_MAX;
  unsigned int normalMap = UINT_MAX;

  // Material properties
  glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);  // Ka
  glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);  // Kd
  glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f); // Ks
  float shininess = 32.0f;                          // Ns
  
  // Control flag
  bool useTexture = false;
};

class Mesh {
private:
public:
  GLuint VAO, VBO, EBO;
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  unsigned int vertexCount, indexCount;
  bool hasTextureCoords;
  Material material;

  Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
  Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures, Material material);

  ~Mesh();

  void Draw(unsigned int shaderProgram) const;

  // Prevent copying
  Mesh(const Mesh&) = delete;
  Mesh& operator=(const Mesh&) = delete;
};


#endif // MESH_H