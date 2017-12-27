// 天空盒渲染器的FS
#version 330

in vec3 texture_coord_0; // 从VS输出，传递给FS的顶点纹理映射坐标
out vec4 fragment_color; // 最终输出的片元颜色
uniform samplerCube skybox_cubemap_texture; // 立方体纹理采样器

void main()
{
   fragment_color = texture(skybox_cubemap_texture, texture_coord_0);
}