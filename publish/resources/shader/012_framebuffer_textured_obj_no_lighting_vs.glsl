// 不带光照的，只使用了一层纹理的vertex shader

#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 input_tex_coords;

out vec2 fragment_tex_coord;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0f);
    fragment_tex_coord = input_tex_coords;
}