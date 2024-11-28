#version 330 core

out vec4 FragColor;

uniform vec2 center;
uniform float scale;  
uniform int iter;     
uniform vec2 resolution;

void main() {
    vec2 z0 = vec2(
        center.x + (gl_FragCoord.x / resolution.x - 0.5) * scale,
        center.y + (gl_FragCoord.y / resolution.y - 0.5) * scale * (resolution.y / resolution.x) // Adjust for aspect ratio
    );

    vec2 z = vec2(0.0, 0.0);
    int i;
    for (i = 0; i < iter; ++i) {
        if (dot(z, z) > 4.0) break; // Escape condition: z^2 > 4
        z = vec2(
            z.x * z.x - z.y * z.y + z0.x,
            2.0 * z.x * z.y + z0.y
        );
    }

    float color = float(i) / float(iter);
    FragColor = vec4(vec3(color), 1.0); // Grayscale gradient
}
