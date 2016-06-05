#version 330 core

layout(location = 0) in vec4 position;
uniform float scale;

void main() {
    gl_Position = vec4(scale * position.x, scale * position.y, position.z, 1.0);
}
