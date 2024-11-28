#version 330

layout(location = 0) in vec4 vertex_position;
layout(location = 1) in vec4 vertex_color;

uniform float theta;

out vec4 frag_color;

void main() {
    float y =  sin(vertex_position.x * 10.0 + theta) * 0.5;

    gl_Position = vec4(vertex_position.x, y, vertex_position.z, 1.0);

    frag_color = vertex_color;
}