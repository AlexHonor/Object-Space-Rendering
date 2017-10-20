#version 150

uniform sampler2D t_position;
uniform sampler2D t_texcoord;
uniform sampler2D t_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat3 u_normal;

uniform sampler2D t_diffuse;
in vec3 v_color;
in vec2 v_texcoord1;

out vec4 out_color;

vec4 GetWorldPosition(sampler2D t_pos, vec2 tex_coord) {
    return u_model * u_view * texture(t_pos, tex_coord);
}

vec4 GetWorldNormal(sampler2D t_pos, vec2 tex_coord) {
    return u_model * u_view * texture(t_pos, tex_coord);
}

void main() {
    vec4 frag_position = GetWorldPosition(t_position, v_texcoord1);
    
    vec4 light_position = vec4(1, 2, -2, 0);

    vec4 dl = frag_position - light_position;

    float atten = 1.0f / (1.0f + dot(dl,dl));

    vec4 color = vec4(u_normal * texture(t_normal, v_texcoord1).xyz, 0.0f);

    out_color = vec4(0.5, 0.5, 0.5, 0) + 0.5 * color;
}