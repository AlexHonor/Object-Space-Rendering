#version 150

uniform mat4 u_projection;
uniform mat4 u_model;
uniform mat4 u_view;

in vec3 a_position;
in vec3 a_color;
in vec3 a_normal;
in vec3 a_texcoord1;
in vec3 a_texcoord2;

out vec3 v_color;

void main() {
    v_color = a_color;

    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}