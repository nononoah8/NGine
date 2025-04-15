#version 330 core

// Maximum number of lights
#define MAX_LIGHTS 8

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
uniform int numLights;
uniform Light lights[MAX_LIGHTS];

// Calculate lighting for directional light
vec3 CalcDirectionalLight(Light light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);


    // Combine results
    vec3 ambient = light.color * material.ambient * light.intensity;
    vec3 diffuse = light.color * diff * material.diffuse * light.intensity;
    vec3 specular = light.color * spec * material.specular * light.intensity;
    
    return ambient + diffuse + specular;
}

// Calculate lighting for point light with attenuation
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // Vector from fragment to light
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Attenuation (light falloff with distance)
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                           light.quadratic * (distance * distance));
    
    // Combine results
    vec3 ambient = light.color * material.ambient * light.intensity;
    vec3 diffuse = light.color * diff * material.diffuse * light.intensity;
    vec3 specular = light.color * spec * material.specular * light.intensity;
    
    // Apply distance attenuation to all components
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return ambient + diffuse + specular;
}

// Calculate lighting for spotlight
// vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {

// }


void main() {
    // Properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 totalLighting = vec3(0.0);

    // Process each active light
    for(int i = 0; i < numLights && i < MAX_LIGHTS; i++) {
        vec3 lightResult = vec3(0.0);
        
        // Determine which lighting calculation to use based on light type
        if(lights[i].type == DIRECTIONAL_LIGHT) {
            lightResult = CalcDirectionalLight(lights[i], norm, viewDir);
        }
        else if(lights[i].type == POINT_LIGHT) {
            lightResult = CalcPointLight(lights[i], norm, fragPos, viewDir);
        }
        else if(lights[i].type == SPOT_LIGHT) {
            // Once implemented: lightResult = CalcSpotLight(lights[i], norm, fragPos, viewDir);
        }

        totalLighting += lightResult;
    }

    // Combine lighting with base color
    vec3 result = totalLighting * ourColor;

    // Output to screen
    FragColor = vec4(result, 1.0);
    // FragColor = vec4(ourColor, 1.0);
}