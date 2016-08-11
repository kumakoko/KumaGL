// 光照demo
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    gl_Position = projection_matrix * view_matrix *  model_matrix * vec4(position, 1.0f);
    FragPos = vec3(model_matrix * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(model_matrix))) * normal;  
} 