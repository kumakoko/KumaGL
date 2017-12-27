#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec4 in_colour;

flat out vec3 pass_colour;//The "flat" qualifier stops the colour from being interpolated over the triangles.

uniform vec3 light_direction;
uniform vec3 light_color;
uniform vec2 light_bias;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

//simple diffuse lighting

vec3 calculateLighting()
{
    vec3 normal = in_normal.xyz * 2.0 - 1.0;//required just because of the format the normals were stored in (0 - 1)
    float brightness = max(dot(-light_direction, normal), 0.0);
    return (light_color * light_bias.x) + (brightness * light_color * light_bias.y);
}

void main()
{
    gl_Position = projection_matrix * view_matrix * vec4(in_position, 1.0);
    vec3 lighting = calculateLighting();
    pass_colour = in_colour.rgb * lighting;
}