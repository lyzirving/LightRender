#version 330 core
precision mediump float;
layout (location = 0) in vec3 a_pos;

out vec3 i_pixel;

void main() {
    gl_Position = vec4(a_pos, 1.0);
    i_pixel = a_pos;
}