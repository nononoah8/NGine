#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
out vec3 fragPos;
out vec3 Normal;

void main() {
    // Apply all three transformation matrices in the correct order
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // Calculate fragment position in world space (for lighting)
    fragPos = vec3(model * vec4(aPos, 1.0));

    // Transform normals to world space using normal matrix
    // This handles non-uniform scaling correctly
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // Pass color to fragment shader
    ourColor = aColor;
}