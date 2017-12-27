// 延迟渲染中的渲染几何信息阶段的vertex shader
#version 330                                                                        
                                                                                    
layout(location = 0) in vec3 Position;                                             
layout(location = 1) in vec2 TexCoord;                                             
layout(location = 2) in vec3 Normal;                                               

uniform mat4 u_wvp_matrix;		// world-view-projection matrix
uniform mat4 u_world_matrix;	// world matrix
                                        
out vec2 TexCoord0;		// 纹理映射坐标
out vec3 Normal0;		//经过变换后的法线坐标，将会                                                                   
out vec3 WorldPos0;                                                                 

void main()
{       
    gl_Position    = u_wvp_matrix * vec4(Position, 1.0);
    TexCoord0      = TexCoord;                  
    Normal0        = (u_world_matrix * vec4(Normal, 0.0)).xyz;   
    WorldPos0      = (u_world_matrix * vec4(Position, 1.0)).xyz;
}