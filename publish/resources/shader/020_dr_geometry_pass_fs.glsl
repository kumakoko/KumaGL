#version 330
                                                                        
in vec2 TexCoord0;                                                                  
in vec3 Normal0;                                                                    
in vec3 WorldPos0;                                                                  

layout (location = 0) out vec3 DiffuseOut;  
layout (location = 1) out vec3 WorldPosOut;
layout (location = 2) out vec3 NormalOut;     
layout (location = 3) out vec3 TexCoordOut;    
										
uniform sampler2D u_color_map;
out vec4 fragment_color;
											
void main()									
{											
	WorldPosOut     = WorldPos0;					
	DiffuseOut      = texture(u_color_map, TexCoord0).xyz;	
	NormalOut       = normalize(Normal0);					
	TexCoordOut     = vec3(TexCoord0, 0.0);	
}