#include "LightComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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
  // Set light type
  glUniform1i(glGetUniformLocation(shaderProgram, (uniformName + ".type").c_str()), static_cast<int>(type));

  // Set position and direction
  glUniform3fv(glGetUniformLocation(shaderProgram, (uniformName + ".position").c_str()), 1, glm::value_ptr(position));
  glUniform3fv(glGetUniformLocation(shaderProgram, (uniformName + ".direction").c_str()), 1, glm::value_ptr(direction));

  // Set color and intensity
  glUniform3fv(glGetUniformLocation(shaderProgram, (uniformName + ".color").c_str()), 1, glm::value_ptr(color));
  glUniform1f(glGetUniformLocation(shaderProgram, (uniformName + ".intensity").c_str()), intensity);

  // Set attenuation factors (primarily used by point and spot lights)
  glUniform1f(glGetUniformLocation(shaderProgram, (uniformName + ".constant").c_str()), constant);
  glUniform1f(glGetUniformLocation(shaderProgram, (uniformName + ".linear").c_str()), linear);
  glUniform1f(glGetUniformLocation(shaderProgram, (uniformName + ".quadratic").c_str()), quadratic);

  // Set spotlight parameters (convert degrees to cosine values for shader efficiency)
  glUniform1f(glGetUniformLocation(shaderProgram, (uniformName + ".innerCutoff").c_str()), glm::cos(glm::radians(innerCutoff)));
  glUniform1f(glGetUniformLocation(shaderProgram, (uniformName + ".outerCutoff").c_str()), glm::cos(glm::radians(outerCutoff)));
}
