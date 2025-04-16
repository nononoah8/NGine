#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "GameObject.h"
#include "Renderer.h"
#include "Shader.h"

#include "Shapes/Cube.h"
#include "Shapes/Sphere.h"

void GameObject::Draw(GLuint shaderProgram, GLint modelLoc) {
  if (!isActive || !mesh) return;

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, scale);
  
  // Pass model matrix to shader
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  
  // Set material properties
  glUniform3fv(glGetUniformLocation(shaderProgram, "material.ambient"), 1, glm::value_ptr(material.ambient));
  glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1, glm::value_ptr(material.diffuse));
  glUniform3fv(glGetUniformLocation(shaderProgram, "material.specular"), 1, glm::value_ptr(material.specular));
  glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), material.shininess);
  
  // Set texture usage flag
  glUniform1i(glGetUniformLocation(shaderProgram, "material.useTexture"), material.useTexture);

  if (material.useTexture) {
    // Activate texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.diffuseMap);
    glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuseMap"), 0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, material.specularMap);
    glUniform1i(glGetUniformLocation(shaderProgram, "material.specularMap"), 1);
  }


  // Set the color
  glUniform3fv(glGetUniformLocation(shaderProgram, "ourColor"), 1, glm::value_ptr(color));
  
  // Draw the mesh
  mesh->Draw(shaderProgram);

  // Clean up by unbinding textures if we used them
  if (material.useTexture) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
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

std::shared_ptr<GameObject> GameObject::LoadModel(const std::string& path) {
  auto gameObject = std::make_shared<GameObject>();
  // This would be implemented once you integrate Assimp
  // gameObject->mesh = AssimpModelLoader::LoadModel(path);
  return gameObject;
}