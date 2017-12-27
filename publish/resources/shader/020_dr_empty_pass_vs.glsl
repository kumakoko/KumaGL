#version 330

layout (location = 0) in vec3 Position; 

uniform mat4 u_wvp_matrix;

void main()
{          
    gl_Position = u_wvp_matrix * vec4(Position, 1.0);
}
