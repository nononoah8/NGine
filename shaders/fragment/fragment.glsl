#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec3 fragPos;
in vec3 Normal;

// Light types (must match the C++ enum)
const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;
const int SPOT_LIGHT = 2;

// Light struct matching your LightComponent class
struct Light {
    int type;               // Light type (directional, point, spot)
    vec3 position;          // Position (for point/spot lights)
    vec3 direction;         // Direction (for directional/spot lights)
    vec3 color;             // Base color
    float intensity;        // Brightness multiplier
    
    // Attenuation properties
    float constant;
    float linear;
    float quadratic;
    
    // Spotlight properties
    float innerCutoff;      // Cosine of inner angle
    float outerCutoff;      // Cosine of outer angle
};

// Material properties
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// Uniforms
uniform Light light;        // Currently active light
uniform Material material;  // Material properties
uniform vec3 viewPos;       // Camera position

// Calculate lighting for directional light
// vec3 CalcDirectionalLight(Light light, vec3 normal, vec3 viewDir) {

// }

// Calculate lighting for point light with attenuation
// vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {

// }

// Calculate lighting for spotlight
// vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {

// }


void main() {


    // Output to screen
    FragColor = vec4(ourColor, 1.0);
}