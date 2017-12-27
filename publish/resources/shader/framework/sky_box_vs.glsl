// 天空盒渲染器的VS
#version 330

layout (location = 0) in vec3 position; // 顶点position

out vec3 texture_coord_0; // 从VS输出，将要传递给FS的顶点纹理映射坐标

uniform mat4 world_matrix;          // 世界坐标
uniform mat4 view_matrix;           // 观察坐标
uniform mat4 projection_matrix; // 投影坐标

void main()
{
    vec4 wvp_position = projection_matrix * view_matrix * world_matrix * vec4(position, 1.0f);
    
    // 将输入的顶点position使用WVP矩阵进行变换，但传给FS的位置向量中的Z分量改成了W分量，在VS阶段之后，rasterizer
    // 将获得gl_Position向量，并进行透视除，以完成投影变换(所谓透视除即是将各分量除以W分量)。将Z分量设置成W分量的值
    // 可以保证透视除后位置向量最终的Z分量值为1.0。Z分量为1意味着永远处于Z轴最远处，在Z-Test中相对于其他的物体而言，
    // skybox将永远处于劣势，因此skybox就总是作为其他物体的背景
    gl_Position = wvp_position.xyww;
   
    // 使用skybox自身的局部坐标系中，顶点的原始坐标来作为3D纹理坐标。？因为对cubemap纹理进行采样的时候是从中心发射
    // 一个向量到立方体盒子或者球面上的，因此盒子表面上点的坐标恰好就是纹理坐标。顶点着色器将物体自身坐标系中的顶点坐
    // 标作为纹理坐标创给片段着色器(立方体是有8个顶点的，球体会有更多)，然后光栅器会在顶点之间进行插值，得到每个像素
    // 的位置，从而就可以利用每个像素的位置进行采样了。
    texture_coord_0 =  position; // 用输出的位置坐标作为待取得的纹理映射坐标
}

