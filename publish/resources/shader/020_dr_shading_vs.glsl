#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 v2f_texture_coord_1;

void main()
{
    v2f_texture_coord_1 = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}