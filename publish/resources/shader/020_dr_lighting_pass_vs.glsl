// 延迟渲染的光照阶段的vertex shader
// 包括point light ，directional light
// 的光照都是使用同一个vertex shader
#version 330                                                                        

layout (location = 0) in vec3 Position; 

uniform mat4 u_wvp_matrix; // world-view-projection 矩阵

void main()
{          
    gl_Position = u_wvp_matrix * vec4(Position, 1.0);
}
