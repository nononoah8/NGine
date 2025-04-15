#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "Component.h"
#include <glm/glm.hpp>
#include <string>
#include <memory>

enum class LightType {
  DIRECTIONAL, // Light comes from a direction
  POINT,       // Light emits in all directions
  SPOT         // Light emits in a cone
};

class LightComponent : public Component {
private:
  LightType lightType;
  glm::vec3 position;
  glm::vec3 direction;
  glm::vec3 color;
  float intensity;

  // Attenuation factors (how light fades with distance)
  float constant;
  float linear;
  float quadratic;

  // Spotlight-specific parameters
  float innerCutoff;
  float outerCutoff;

  std::string name;
public:
  LightComponent() :
    Component(),
    lightType(LightType::POINT),
    position(0.0f, 0.0f, 0.0f),
    direction(0.0f, -1.0f, 0.0f),
    color(1.0f, 1.0f, 1.0f),
    intensity(1.0f),
    constant(1.0f),
    linear(0.09f),
    quadratic(0.032f),
    innerCutoff(12.5f),
    outerCutoff(17.5f),
    name("Light") {
      type = "Light";
    }

    static void RegisterLight(std::shared_ptr<LightComponent> light) {
      lights.push_back(light);
    }

    static void UnregisterLight(std::shared_ptr<LightComponent> light) {
      auto it = std::find(lights.begin(), lights.end(), light);
      if (it != lights.end()) {
        lights.erase(it);
      }
    }

    static void ApplyAllLightsToShader(unsigned int shaderProgram);

    static std::vector<std::shared_ptr<LightComponent>> lights;

    // Getters and setters
    int GetType() const { return static_cast<int>(lightType); }
    void SetType(LightType newType) { lightType = newType; }
    
    glm::vec3 GetPosition() const { return position; }
    void SetPosition(const glm::vec3& newPos) { position = newPos; }
    
    glm::vec3 GetDirection() const { return direction; }
    void SetDirection(const glm::vec3& newDir) { direction = glm::normalize(newDir); }
    
    glm::vec3 GetColor() const { return color; }
    void SetColor(const glm::vec3& newColor) { color = newColor; }
    
    float GetIntensity() const { return intensity; }
    void SetIntensity(float newIntensity) { intensity = newIntensity; }
    
    // Attenuation getters/setters
    float GetConstant() const { return constant; }
    void SetConstant(float value) { constant = value; }
    
    float GetLinear() const { return linear; }
    void SetLinear(float value) { linear = value; }
    
    float GetQuadratic() const { return quadratic; }
    void SetQuadratic(float value) { quadratic = value; }
    
    // Spotlight getters/setters
    float GetInnerCutoff() const { return innerCutoff; }
    void SetInnerCutoff(float degrees) { innerCutoff = degrees; }
    
    float GetOuterCutoff() const { return outerCutoff; }
    void SetOuterCutoff(float degrees) { outerCutoff = degrees; }
    
    std::string GetName() const { return name; }
    void SetName(const std::string& newName) { name = newName; }
    
    // Methods to apply this light to a shader
    void ApplyToShader(unsigned int shaderProgram, const std::string& uniformName) const;
};

#endif // LIGHTCOMPONENT_H