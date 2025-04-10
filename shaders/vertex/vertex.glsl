#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;       // Added view matrix
uniform mat4 projection; // Added projection matrix

out vec3 ourColor;
out vec3 fragPos;  // For lighting calculations

void main() {
    // Apply all three transformation matrices in the correct order
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // Calculate fragment position in world space (for lighting)
    fragPos = vec3(model * vec4(aPos, 1.0));
    
    // Pass color to fragment shader
    ourColor = aColor;
}