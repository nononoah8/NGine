#ifndef GAMEOBJECTDB_H
#define GAMEOBJECTDB_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shapes/Sphere.h"
#include "Shapes/Cube.h"
#include "Mesh.h"
#include "GameObject.h"

class GameObjectDB {
public:
  static void Init();
  static void Shutdown();

  // GameObject management
  static std::shared_ptr<GameObject> CreateCube(float size = 1.0f, const glm::vec3& position = glm::vec3(0.0f));
  static std::shared_ptr<GameObject> CreateSphere(float radius = 1.0f, const glm::vec3& color = glm::vec3(1.0f), int segments = 16, const glm::vec3& position = glm::vec3(0.0f));
  static std::shared_ptr<GameObject> LoadModel(const std::string& path, const glm::vec3& position = glm::vec3(0.0f));
  
  // Queue an object for rendering
  static void QueueForRendering(const std::shared_ptr<GameObject>& gameObject);

  // Render all queued objects and clear the queue
  static void RenderAndClearObjects(GLuint shaderProgram, GLint modelLoc);
  
  // Update all game objects
  static void UpdateAll(float deltaTime);
private:
  static std::vector<std::shared_ptr<GameObject>> allGameObjects;
  static std::vector<std::shared_ptr<GameObject>> renderQueue;
  static std::unordered_map<std::string, std::shared_ptr<GameObject>> gameObjectMap;
};

#endif // GAMEOBJECTDB_H