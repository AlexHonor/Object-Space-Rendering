#version 150

in vec3 v_color;
in vec2 v_texcoord1;

out vec4 frag_color;

void main() {
    frag_color = vec4(v_color, 1.0f);
}