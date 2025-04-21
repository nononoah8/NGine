#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "GameObject.h"
#include "Renderer.h"
#include "Shader.h"

#include "Shapes/Cube.h"
#include "Shapes/Sphere.h"
#include "Shapes/Plane.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

void GameObject::Draw(GLuint shaderProgram, GLint modelLoc) {
  if (!isActive || !mesh) return;

  // Set up model matrix
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, scale);
  
  // Pass model matrix to shader
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

  // Set the vertex color
  glUniform3fv(glGetUniformLocation(shaderProgram, "ourColor"), 1, glm::value_ptr(color));
  
  // For models, we let each mesh handle its own materials
  if(isModel) {
    this->model->Draw(shaderProgram);
  } else {
    // For basic shapes, transfer the GameObject material to the mesh
    mesh->material = material;
    mesh->Draw(shaderProgram);
  }
}

void GameObject::Update(float deltaTime) {
  // Base implementation - can be overridden in derived classes
  // For example, to add rotation or movement
}

glm::mat4 GameObject::GetModelMatrix() const {
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  
  // Apply transformations in order: scale, rotate, translate
  modelMatrix = glm::translate(modelMatrix, position);
  
  // Apply rotations around each axis
  modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
  modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
  modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
  
  modelMatrix = glm::scale(modelMatrix, scale);
  
  return modelMatrix;
}

std::shared_ptr<GameObject> GameObject::CreateCube(float size) {
  auto gameObject = std::make_shared<GameObject>();
  gameObject->mesh = Shape::Cube::Create(size);
  return gameObject;
}

std::shared_ptr<GameObject> GameObject::CreateSphere(float radius, glm::vec3 color, int segments) {
  auto gameObject = std::make_shared<GameObject>();
  gameObject->mesh = Shape::Sphere::Create(radius, color, segments);
  return gameObject;
}

std::shared_ptr<GameObject> GameObject::LoadModel(const std::string& name) {
  auto gameObject = std::make_shared<GameObject>();

  std::string fp = "resources/models/" + name + "/" + name + ".obj";
  
  auto model = std::make_shared<Model>(fp);
  gameObject->model = model;

  // If model has meshes, use the first one for the GameObject
  if (!model->meshes.empty()) {
    gameObject->mesh = model->meshes[0];
    
    // Set default material properties based on the model's textures
    if (!model->meshes[0]->textures.empty()) {
      gameObject->material.useTexture = true;
      
      // Find diffuse and specular maps if they exist
      for (const auto& texture : model->meshes[0]->textures) {
        if (texture.type == "texture_diffuse") {
          gameObject->material.diffuseMap = texture.id;
        }
        else if (texture.type == "texture_specular") {
          gameObject->material.specularMap = texture.id;
        }
      }
      
      // Set material properties
      gameObject->material.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
      gameObject->material.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
      gameObject->material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
      gameObject->material.shininess = 32.0f;
    }
  }
  else {
    std::cerr << "Warning: Model loaded but contains no meshes: " << fp << std::endl;
  }

  return gameObject;
}

std::shared_ptr<GameObject> GameObject::CreatePlane(float width, float length, glm::vec3 color) {
  auto gameObject = std::make_shared<GameObject>();
  gameObject->mesh = Shape::Plane::Create(width, length, color);
  gameObject->color = color;

  // Set default material properties
  gameObject->material.ambient = color * 0.1f;
  gameObject->material.diffuse = color * 0.8f;
  gameObject->material.specular = glm::vec3(0.5f);
  gameObject->material.shininess = 32.0f;

  return gameObject;
}

std::shared_ptr<GameObject> GameObject::CreateTexturedPlane(
    const std::string& texturePath, 
    float width, 
    float length, 
    glm::vec3 color) {
  
  // Create texture
  std::string textureFolderPath = "resources/textures";
  unsigned int textureID = TextureFromFile(texturePath.c_str(), textureFolderPath, false);
  
  // Create texture object for the mesh
  Texture diffuseTexture;
  diffuseTexture.id = textureID;
  diffuseTexture.type = "texture_diffuse";
  diffuseTexture.path = texturePath;
  
  std::vector<Texture> textures = {diffuseTexture};
  
  // Create the plane with texture coordinates
  auto gameObject = std::make_shared<GameObject>();
  gameObject->mesh = Shape::Plane::CreateTextured(width, length, color, textures);
  gameObject->color = color;
  
  // Set material properties with texture
  gameObject->material.ambient = color * 0.1f;
  gameObject->material.diffuse = color;
  gameObject->material.specular = glm::vec3(0.5f);
  gameObject->material.shininess = 32.0f;
  gameObject->material.useTexture = true;
  gameObject->material.diffuseMap = textureID;

  return gameObject;
}