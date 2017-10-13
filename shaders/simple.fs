#version 150

uniform sampler2D t_diffuse;
in vec3 v_color;
in vec2 v_texcoord1;

out vec4 out_color;

void main() {
    out_color = texture(t_diffuse, v_texcoord1);
}