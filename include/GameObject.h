#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include "Mesh.h"

class GameObject {
public:
  GameObject() : 
    position(0.0f, 0.0f, 0.0f),
    rotation(0.0f, 0.0f, 0.0f),
    scale(1.0f, 1.0f, 1.0f),
    color(1.0f, 1.0f, 1.0f),
    isActive(true),
    mesh(nullptr) {}

  // Core properties
  glm::vec3 position;
  glm::vec3 rotation;  // In radians
  glm::vec3 scale;
  glm::vec3 color;
  bool isActive = true;
  std::shared_ptr<Mesh> mesh;

  // Core methods
  void Draw(GLuint shaderProgram, GLint modelLoc);
  virtual void Update(float deltaTime);

  // Transformation helpers
  glm::mat4 GetModelMatrix() const;

  // Factory methods for easy creation
  static std::shared_ptr<GameObject> CreateCube(float size = 1.0f);
  static std::shared_ptr<GameObject> CreateSphere(float radius = 1.0f, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), int segments = 16);
  static std::shared_ptr<GameObject> LoadModel(const std::string& path);

  void SetPosition(glm::vec3 pos) { position = pos; }
  void SetPosition(float x, float y, float z) { position = glm::vec3(x, y, z); }
  glm::vec3 GetPosition() { return position; }
private:
};

#endif // GAMEOBJECT_H