#include <iostream>
#include <algorithm>

#include <glad/glad.h>

#include "GameObjectDB.h"
#include "Renderer.h"

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

// Static member initialization
std::vector<std::shared_ptr<GameObject>> GameObjectDB::allGameObjects;
std::vector<std::shared_ptr<GameObject>> GameObjectDB::renderQueue;
std::unordered_map<std::string, std::shared_ptr<GameObject>> GameObjectDB::gameObjectMap;
static std::unordered_map<std::string, unsigned int> textureCache;

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
  // Generate a unique key for this cube
  std::string key = "cube_" + std::to_string(size);
  
  // Check if we already have this cube
  if(gameObjectMap.find(key) != gameObjectMap.end()) {
    auto gameObject = std::make_shared<GameObject>(*gameObjectMap[key]);
    gameObject->position = position; // Update position
    allGameObjects.push_back(gameObject);
    return gameObject;
  }
  
  // Create a new cube if not found
  auto gameObject = std::make_shared<GameObject>();
  gameObject->position = position;
  gameObject->mesh = Shape::Cube::Create(size);
  
  // Cache the cube
  gameObjectMap[key] = gameObject;
  allGameObjects.push_back(gameObject);
  return gameObject;
}

std::shared_ptr<GameObject> GameObjectDB::CreateSphere(float radius, const glm::vec3& color, int segments, const glm::vec3& position) {
  // Generate a unique key for this sphere
  std::string key = "sphere_" + std::to_string(radius) + "_" + 
                   std::to_string(color.r) + "_" + std::to_string(color.g) + "_" + 
                   std::to_string(color.b) + "_" + std::to_string(segments);
  
  // Check if we already have this sphere
  if(gameObjectMap.find(key) != gameObjectMap.end()) {
    auto gameObject = std::make_shared<GameObject>(*gameObjectMap[key]);
    gameObject->position = position; // Update position
    allGameObjects.push_back(gameObject);
    return gameObject;
  }
  
  // Create a new sphere if not found
  auto gameObject = std::make_shared<GameObject>();
  gameObject->position = position;
  gameObject->color = color;
  gameObject->mesh = Shape::Sphere::Create(radius, color, segments);
  
  // Cache the sphere
  gameObjectMap[key] = gameObject;
  allGameObjects.push_back(gameObject);
  return gameObject;
}

std::shared_ptr<GameObject> GameObjectDB::CreatePlane(float width, float length, const glm::vec3& color, const glm::vec3& position) {
  // Generate a unique key for this plane
  std::string key = "plane_" + std::to_string(width) + "_" + std::to_string(length) + "_" +
                   std::to_string(color.r) + "_" + std::to_string(color.g) + "_" + 
                   std::to_string(color.b);
  
  // Check if we already have this plane
  if(gameObjectMap.find(key) != gameObjectMap.end()) {
    auto gameObject = std::make_shared<GameObject>(*gameObjectMap[key]);
    gameObject->position = position; // Update position
    allGameObjects.push_back(gameObject);
    return gameObject;
  }
  
  // Create a new plane if not found
  auto gameObject = std::make_shared<GameObject>();
  gameObject->position = position;
  gameObject->color = color;
  gameObject->mesh = Shape::Plane::Create(width, length, color);
  
  // Set default material properties
  gameObject->material.ambient = color * 0.1f;
  gameObject->material.diffuse = color * 0.8f;
  gameObject->material.specular = glm::vec3(0.5f);
  gameObject->material.shininess = 32.0f;
  
  // Cache the plane
  gameObjectMap[key] = gameObject;
  allGameObjects.push_back(gameObject);
  return gameObject;
}

std::shared_ptr<GameObject> GameObjectDB::CreateTexturedPlane(const std::string& texturePath, float width, float length, const glm::vec3& color, const glm::vec3& position) {
  // Generate a unique key for this textured plane
  std::string key = "tplane_" + texturePath + "_" + std::to_string(width) + "_" + std::to_string(length);
  
  // Check if we already have this textured plane
  if(gameObjectMap.find(key) != gameObjectMap.end()) {
    auto gameObject = std::make_shared<GameObject>(*gameObjectMap[key]);
    gameObject->position = position; // Update position
    allGameObjects.push_back(gameObject);
    return gameObject;
  }
  
  // Create a new textured plane if not found
  auto gameObject = std::make_shared<GameObject>();
  gameObject->position = position;
  gameObject->color = color;
  
  // Check if texture is already loaded
  unsigned int textureID;
  if(textureCache.find(texturePath) != textureCache.end()) {
    textureID = textureCache[texturePath];
  } else {
    // Load the texture if not cached
    std::string textureFolderPath = "resources/textures";
    textureID = TextureFromFile(texturePath.c_str(), textureFolderPath, false);
    textureCache[texturePath] = textureID; // Cache the loaded texture
  }
  
  // Create texture object for the mesh
  Texture diffuseTexture;
  diffuseTexture.id = textureID;
  diffuseTexture.type = "texture_diffuse";
  diffuseTexture.path = texturePath;
  
  std::vector<Texture> textures = {diffuseTexture};
  
  // Create the mesh with texture
  gameObject->mesh = Shape::Plane::CreateTextured(width, length, color, textures);
  
  // Set material properties
  gameObject->material.ambient = color * 0.1f;
  gameObject->material.diffuse = color;
  gameObject->material.specular = glm::vec3(0.5f);
  gameObject->material.shininess = 32.0f;
  gameObject->material.useTexture = true;
  gameObject->material.diffuseMap = textureID;
  
  // Cache the textured plane
  gameObjectMap[key] = gameObject;
  allGameObjects.push_back(gameObject);
  return gameObject;
}

std::shared_ptr<GameObject> GameObjectDB::LoadModel(const std::string& name, const glm::vec3& position, const glm::vec3& scale) {
  // The key should include the model name only
  std::string key = "model_" + name;
  
  if(gameObjectMap.find(key) != gameObjectMap.end()) {
    // Create a COPY of the cached model (not reuse the same one)
    auto gameObject = std::make_shared<GameObject>(*gameObjectMap[key]);
    gameObject->position = position;
    gameObject->scale = scale;
    gameObject->isModel = true;

    allGameObjects.push_back(gameObject);
    return gameObject;
  } else {
    auto gameObject = GameObject::LoadModel(name, scale);
    gameObject->position = position;
    gameObject->isModel = true;
    
    // Cache the model template (not the positioned instance)
    gameObjectMap[key] = gameObject;
    allGameObjects.push_back(gameObject);
    return gameObject;
  }
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