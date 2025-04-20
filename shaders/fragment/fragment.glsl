#version 330 core

// Maximum number of lights
#define MAX_LIGHTS 8

out vec4 FragColor;

in vec3 ourColor;
in vec3 fragPos;
in vec3 Normal;
in vec2 TexCoords;

// Light types (must match the C++ enum)
const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;
const int SPOT_LIGHT = 2;

// Light struct matching your LightComponent class
struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    
    // Attenuation properties
    float constant;
    float linear;
    float quadratic;
    
    // Spotlight properties
    float innerCutoff;
    float outerCutoff;
};

// Material properties
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    // Texture samplers
    sampler2D diffuseMap;
    sampler2D specularMap;
    bool useTexture;
};

// Uniforms
uniform Light light;
uniform Material material;
uniform vec3 viewPos;
uniform int numLights;
uniform Light lights[MAX_LIGHTS];
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

// Calculate lighting for directional light
vec3 CalcDirectionalLight(Light light, vec3 normal, vec3 viewDir, vec3 diffuseValue, vec3 specularValue) {
    vec3 lightDir = normalize(-light.direction);

    // Diffuse shading
    float NdotL = dot(normal, lightDir);
    if (NdotL <= 0.0) {
        // Surface is facing away from light - only apply ambient
        // return light.color * diffuseValue * light.intensity * 0.1;
        return light.color * material.ambient * light.intensity;
    }

    float diff = NdotL;

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Combine results
    vec3 ambient = light.color * material.ambient * light.intensity;
    vec3 diffuse = light.color * diff * diffuseValue * light.intensity;
    vec3 specular = light.color * spec * specularValue * light.intensity;
    
    return ambient + diffuse + specular;
}

// Calculate lighting for point light with attenuation
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseValue, vec3 specularValue) {    
    // Vector from fragment to light
    vec3 lightDir = normalize(light.position - fragPos);

    // Check if surface faces away from light (backside)
    float NdotL = dot(normal, lightDir);
    if (NdotL <= 0.0) {
        // Surface is facing away from light - only apply ambient
        // return light.color * diffuseValue * light.intensity * 0.1;
        return light.color * material.ambient * light.intensity;
    }

    // Rest of your lighting calculation for surfaces facing the light
    float diff = NdotL;

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Attenuation (light falloff with distance)
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Combine results
    vec3 ambient = light.color * material.ambient * light.intensity;
    vec3 diffuse = light.color * diff * diffuseValue * light.intensity;
    vec3 specular = light.color * spec * specularValue * light.intensity;
    
    // Apply distance attenuation to all components
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return ambient + diffuse + specular;
}

// Calculate lighting for spotlight
vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseValue, vec3 specularValue) {
    // Vector from fragment to light
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Check if surface faces away from light (backface)
    float NdotL = dot(normal, lightDir);
    if (NdotL <= 0.0) {
        // Surface is facing away from light - only apply ambient
        // return light.color * diffuseValue * light.intensity * 0.1;
        return light.color * material.ambient * light.intensity;
    }

    // Diffuse shading
    float diff = NdotL;

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Attenuation (light falloff with distance)
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Spotlight effect (soft edges)
    // innerCutoff and outerCutoff are stored as cosines for efficiency
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    // Combine results
    vec3 ambient = light.color * material.ambient * light.intensity;
    vec3 diffuse = light.color * diff * diffuseValue * light.intensity;
    vec3 specular = light.color * spec * specularValue * light.intensity;

    // Apply distance attenuation and spotlight intensity
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}


void main() {
    // Properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 diffuseValue;
    vec3 specularValue;
    bool useDiffuseTex = false;

    // First check if we should use the standalone texture_diffuse1
    if (textureSize(texture_diffuse1, 0).x > 1) {
        // Valid texture in texture_diffuse1, use it
        diffuseValue = vec3(texture(texture_diffuse1, TexCoords));
        
        // Check if we have a valid specular texture too
        if (textureSize(texture_specular1, 0).x > 1) {
            // Use the specular map if available
            specularValue = vec3(texture(texture_specular1, TexCoords));
        } else {
            // Fall back to default specular
            specularValue = vec3(0.5);
        }
        
        useDiffuseTex = true;
    } else if (material.useTexture) {
        // Use texture values from material
        diffuseValue = vec3(texture(material.diffuseMap, TexCoords));
        specularValue = vec3(texture(material.specularMap, TexCoords));
    } else {
        // Use material properties
        diffuseValue = material.diffuse;
        specularValue = material.specular;
    }

    vec3 totalLighting = vec3(0.0);

    // Process each active light
    for(int i = 0; i < numLights && i < MAX_LIGHTS; i++) {
        vec3 lightResult = vec3(0.0);
        
        // Determine which lighting calculation to use based on light type
        if(lights[i].type == DIRECTIONAL_LIGHT) {
            lightResult = CalcDirectionalLight(lights[i], norm, viewDir, diffuseValue, specularValue);
        }
        else if(lights[i].type == POINT_LIGHT) {
            lightResult = CalcPointLight(lights[i], norm, fragPos, viewDir, diffuseValue, specularValue);
        }
        else if(lights[i].type == SPOT_LIGHT) {
            lightResult = CalcSpotLight(lights[i], norm, fragPos, viewDir, diffuseValue, specularValue);
        }

        totalLighting += lightResult;
    }

    // Combine lighting with base color
    // vec3 baseColor = material.useTexture ? vec3(1.0) : ourColor;
    vec3 baseColor;
    if (useDiffuseTex) {
        // For textured objects, blend with object color
        baseColor = diffuseValue * ourColor;
    } else {
        // For non-textured objects, just use object color
        baseColor = ourColor;
    }
    vec3 result = totalLighting * baseColor;

    result = result / (result + vec3(1.0));

    // Output to screen
    FragColor = vec4(result, 1.0);
}