#version 330 core
layout (location = 0) in vec2 vertex;               // 顶点坐标
layout (location = 1) in vec2 input_tex_coord;      // 顶点纹理映射坐标

out vec2 output_tex_coords;                 // 输出到片元着色器的纹理坐标

uniform mat4 sprite_model_matrix;                  // 模型的世界坐标
uniform mat4 sprite_project_matrix;             // 模型的投影坐标

void main()
{
    output_tex_coords = vec2(input_tex_coord.x, 1.0 - input_tex_coord.y);
    gl_Position = sprite_project_matrix * sprite_model_matrix * vec4(vertex, 0.0, 1.0);
}