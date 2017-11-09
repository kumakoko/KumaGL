// 展示简单的显示纹理贴图的shader
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 input_tex_coord;

out vec4 fragment_color;
out vec2 fragment_tex_coord;

void main()
{
    gl_Position = vec4(position, 1.0f);
    fragment_color = vec4(color,1.0f);
    // fragment_tex_coord = vec2(input_tex_coord.x, 1.0 - input_tex_coord.y);
    fragment_tex_coord = vec2(input_tex_coord.x, input_tex_coord.y);
}