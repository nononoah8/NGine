#version 330 core

out vec4 FragColor;

// uniform vairbale to pass in the time
uniform int iTime;
uniform vec2 iResolution;

void main() {
    // Normalized pixel coordinates (from 0 to 1)
    vec2 fragCoord = gl_FragCoord.xy; // Built-in variable for fragment coordinates
    vec2 uv = fragCoord / iResolution;

    float scaledTime = float(iTime) * 0.001;

    // Time-varying pixel color
    vec3 col = 0.5 + 0.5 * cos(scaledTime + uv.xyx + vec3(0, 2, 4));

    // Output to screen
    FragColor = vec4(col, 1.0);
}