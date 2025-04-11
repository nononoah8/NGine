#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include <glm/glm.hpp>
#include <string>

enum class LightType {
  DIRECTIONAL, // Light comes from a direction
  POINT,       // Light emits in all directions
  SPOT         // Light emits in a cone
};

class LightComponent {
private:
  LightType type;
  glm::vec3 position;      // For point and spot lights
  glm::vec3 direction;     // For directional and spot lights
  glm::vec3 color;         // RGB color
  float intensity;         // Overall brightness

  // Attenuation factors (how light fades with distance)
  float constant;          // Constant factor
  float linear;            // Linear factor with distance
  float quadratic;         // Quadratic factor with distance

  // Spotlight-specific parameters
  float innerCutoff;       // Inner cone angle (in degrees)
  float outerCutoff;       // Outer cone angle (in degrees)

  bool castShadows;        // Whether this light casts shadows
  std::string name;        // Identifier for the light
public:
  LightComponent() :
    type(LightType::POINT),
    position(0.0f, 0.0f, 0.0f),
    direction(0.0f, -1.0f, 0.0f),
    color(1.0f, 1.0f, 1.0f),
    intensity(1.0f),
    constant(1.0f),
    linear(0.09f),
    quadratic(0.032f),
    innerCutoff(12.5f),
    outerCutoff(17.5f),
    castShadows(false),
    name("Light") {}

    static LightComponent CreateDirectionalLight(
      const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f),
      const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f),
      float intensity = 1.0f);
      
    static LightComponent CreatePointLight(
        const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f),
        float intensity = 1.0f,
        float constant = 1.0f,
        float linear = 0.09f,
        float quadratic = 0.032f);
        
    static LightComponent CreateSpotLight(
        const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f),
        const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f),
        float intensity = 1.0f,
        float innerCutoff = 12.5f,
        float outerCutoff = 17.5f);

    // Getters and setters
    LightType GetType() const { return type; }
    void SetType(LightType newType) { type = newType; }
    
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
    
    bool GetCastShadows() const { return castShadows; }
    void SetCastShadows(bool value) { castShadows = value; }
    
    std::string GetName() const { return name; }
    void SetName(const std::string& newName) { name = newName; }
    
    // Methods to apply this light to a shader
    void ApplyToShader(unsigned int shaderProgram, const std::string& uniformName) const;
};

#endif // LIGHTCOMPONENT_H