// 深度测试示例程序的的fragment shader

#version 330 core

in vec2 fragment_tex_coord;
in vec4 fragment_color; 
out vec4 final_color;

uniform sampler2D source_texture_1;

float LinearizeDepth(float depth)
{
    float near = 0.1;
    float far = 100.0;
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near) / (far + near - z * (far - near));
}

void main()
{
	final_color = texture(source_texture_1, fragment_tex_coord);
}