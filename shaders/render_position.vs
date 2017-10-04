#version 150

uniform mat4 u_projection;
uniform mat4 u_model;
uniform mat4 u_view;

in vec3 a_position;
in vec3 a_color;
in vec3 a_normal;
in vec2 a_texcoord1;
in vec2 a_texcoord2;

out vec3 v_color;
out vec2 v_texcoord1;

void main() {
    v_color = a_position;
    v_texcoord1 = a_texcoord1 * 2.0 - vec2(1.0f, 1.0f);

    gl_Position = vec4(v_texcoord1, 0.0f, 1.0);
}