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
    v_texcoord1 = a_position.xy / 2.0f + 0.5f;

    gl_Position = vec4(a_position, 1.0);
}