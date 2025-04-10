#include <iostream>
#include <algorithm>

#include <glad/glad.h>

#include "GameObjectDB.h"
#include "Renderer.h"

// Static member initialization
std::vector<std::shared_ptr<GameObject>> GameObjectDB::allGameObjects;
std::vector<std::shared_ptr<GameObject>> GameObjectDB::renderQueue;

void GameObjectDB::Init() {
  // Clear any existing objects
  allGameObjects.clear();
  renderQueue.clear();
}

void GameObjectDB::Shutdown() {
  // Clean up all game objects
  allGameObjects.clear();
  renderQueue.clear();
}

std::shared_ptr<GameObject> GameObjectDB::CreateCube(float size, const glm::vec3& position) {
  auto gameObject = std::make_shared<GameObject>();
  gameObject->position = position;
  gameObject->mesh = Shape::Cube::Create(size);
  
  allGameObjects.push_back(gameObject);
  return gameObject;
}

std::shared_ptr<GameObject> GameObjectDB::CreateSphere(float radius, const glm::vec3& color, int segments, const glm::vec3& position) {
  auto gameObject = std::make_shared<GameObject>();
  gameObject->position = position;
  gameObject->color = color;
  gameObject->mesh = Shape::Sphere::Create(radius, color, segments);
  
  allGameObjects.push_back(gameObject);
  return gameObject;
}

std::shared_ptr<GameObject> GameObjectDB::LoadModel(const std::string& path, const glm::vec3& position) {
  auto gameObject = GameObject::LoadModel(path);
  gameObject->position = position;
  
  allGameObjects.push_back(gameObject);
  return gameObject;
}

void GameObjectDB::RenderAndClearObjects(GLuint shaderProgram, GLint modelLoc) {

  // Enable depth testing for proper 3D rendering
  glEnable(GL_DEPTH_TEST);
  
  // Render all objects in the queue
  for (auto& gameObject : renderQueue) {
    gameObject->Draw(shaderProgram, modelLoc);
  }
  
  // Clear the queue after rendering
  renderQueue.clear();
  allGameObjects.clear();
}

void GameObjectDB::UpdateAll(float deltaTime) {
  // Update all game objects and queue them for rendering
  for (auto& gameObject : allGameObjects) {
    if (gameObject->isActive) {
      // Update the object
      gameObject->Update(deltaTime);
      
      // Add some animation if desired
      // gameObject->rotation.y += deltaTime * 0.5f;
      
      // Queue for rendering
      QueueForRendering(gameObject);
    }
  }
}


void GameObjectDB::QueueForRendering(const std::shared_ptr<GameObject>& gameObject) {
  if (gameObject && gameObject->isActive && gameObject->mesh) {
    renderQueue.push_back(gameObject);
  }
}