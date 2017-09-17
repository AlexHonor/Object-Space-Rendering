#version 150

in vec3 v_color;
in vec2 v_texcoord1;

out vec4 out_color;

void main() {
    out_color = v_color;
}