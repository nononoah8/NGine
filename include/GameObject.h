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
    mesh(nullptr),
    name(""),
    material(Material()) {}

  // Core properties
  glm::vec3 position;
  glm::vec3 rotation;  // In radians
  glm::vec3 scale;
  glm::vec3 color;
  bool isActive = true;
  std::shared_ptr<Mesh> mesh;
  std::string name;

  struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    unsigned int diffuseMap;
    unsigned int specularMap;
    bool useTexture;

    // Constructor with default values
    Material() : 
        ambient(glm::vec3(0.1f)),
        diffuse(glm::vec3(0.8f)),
        specular(glm::vec3(0.5f)),
        shininess(32.0f),
        diffuseMap(UINT_MAX),
        specularMap(UINT_MAX),
        useTexture(false) {}
  };

  Material material;

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

  void SetMaterial(const Material& newMaterial) {
    material = newMaterial;
  }

  void SetMetallic(float value = 0.8f) {
    // Preset for metallic surfaces
    material.ambient = color * 0.1f;
    material.diffuse = color * 0.6f;
    material.specular = glm::vec3(value);
    material.shininess = 64.0f;
  }

  void SetPlastic(float value = 0.5f) {
    // Preset for plastic surfaces
    material.ambient = color * 0.1f;
    material.diffuse = color * 0.9f;
    material.specular = glm::vec3(value);
    material.shininess = 16.0f;
  }

  // Add these methods to GameObject class
  void SetDiffuseTexture(unsigned int textureId) {
    material.diffuseMap = textureId;
    material.useTexture = true;
  }

  void SetSpecularTexture(unsigned int textureId) {
    material.specularMap = textureId;
  }

  void DisableTextures() {
    material.useTexture = false;
  }
private:
};

#endif // GAMEOBJECT_H