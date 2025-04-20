#include "LightComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

std::vector<std::shared_ptr<LightComponent>> LightComponent::lights;

void LightComponent::ApplyAllLightsToShader(unsigned int shaderProgram) {
  
  glUniform1i(glGetUniformLocation(shaderProgram, "numLights"), static_cast<int>(lights.size()));
  
  // Apply each light to the shader with its own uniform location
  for (size_t i = 0; i < lights.size(); i++) {
    std::string uniformPrefix = "lights[" + std::to_string(i) + "]";
    lights[i]->ApplyToShader(shaderProgram, uniformPrefix);
  }
}

void LightComponent::ApplyToShader(unsigned int shaderProgram, const std::string& uniformName) const {
  // Set light type
  glUniform1i(glGetUniformLocation(shaderProgram, (uniformName + ".type").c_str()), static_cast<int>(lightType));

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
