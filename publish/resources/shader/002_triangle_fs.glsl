#version 330 core

in vec3 output_color;
out vec4 color;

void main()
{
    color = vec4(output_color,1.0f);
}