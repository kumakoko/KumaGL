// 演示在各个坐标空间中进行变换的示例程序的fragment shader
#version 330 core

in vec2 fragment_tex_coord_1;
in vec4 fragment_color; 
out vec4 final_color; //  最终的片元颜色

uniform sampler2D source_texture_1; // 作用在片元上的第一层纹理映射坐标

void main()
{
	final_color = texture(source_texture_1, fragment_tex_coord_1);
}