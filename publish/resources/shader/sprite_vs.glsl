// 用来绘制精灵的顶点着色器

#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texture_coords;

out vec2 fragment_texture_coords; // 输出给片元着色器的纹理坐标

uniform mat4 sprite_model_matrix;
uniform mat4 projection_matrix;

void main()
{
    fragment_texture_coords = texture_coords;
    gl_Position = projection_matrix * sprite_model_matrix * vec4(position.xy, 0.0, 1.0);
}