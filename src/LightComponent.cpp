#include "LightComponent.h"

#include <glad/glad.h>

LightComponent LightComponent::CreateDirectionalLight(
  const glm::vec3& direction, 
  const glm::vec3& color,
  float intensity) {
  
  LightComponent light;
  light.type = LightType::DIRECTIONAL;
  light.direction = glm::normalize(direction);
  light.color = color;
  light.intensity = intensity;
  light.castShadows = true; // Directional lights typically cast shadows
  return light;
}

LightComponent LightComponent::CreatePointLight(
  const glm::vec3& position,
  const glm::vec3& color,
  float intensity,
  float constant,
  float linear,
  float quadratic) {
  
  LightComponent light;
  light.type = LightType::POINT;
  light.position = position;
  light.color = color;
  light.intensity = intensity;
  light.constant = constant;
  light.linear = linear;
  light.quadratic = quadratic;
  return light;
}

LightComponent LightComponent::CreateSpotLight(
  const glm::vec3& position,
  const glm::vec3& direction,
  const glm::vec3& color,
  float intensity,
  float innerCutoff,
  float outerCutoff) {
  
  LightComponent light;
  light.type = LightType::SPOT;
  light.position = position;
  light.direction = glm::normalize(direction);
  light.color = color;
  light.intensity = intensity;
  light.innerCutoff = innerCutoff;
  light.outerCutoff = outerCutoff;
  return light;
}

void LightComponent::ApplyToShader(unsigned int shaderProgram, const std::string& uniformName) const {
  return;
}
