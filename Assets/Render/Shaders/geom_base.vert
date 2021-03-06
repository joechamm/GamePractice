#version 300 es

precision mediump float;

uniform mat4 MVP;

layout(location = 0) in vec3 vert;

void main() {
    gl_Position = MVP * vec4(vert, 1.f);
}