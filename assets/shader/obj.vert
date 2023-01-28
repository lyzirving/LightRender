// specify the minimum version of opengl glsl, if current version is lower than this, the shader will not complie
#version 330 core

precision mediump float;
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex;

uniform mat4 u_mtModel;
uniform mat4 u_mtView;
uniform mat4 u_mtPrj;

out vec2 v_tex;

void main() {
    v_tex = a_tex;
    gl_Position = u_mtPrj * u_mtView * u_mtModel * vec4(a_pos, 1.0);
}