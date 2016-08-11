// 基于逐顶点光照的顶点着色器
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 vs_output_normal;
out vec3 vs_out_frag_pos;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view *  world * vec4(position, 1.0f);
    vs_out_frag_pos = vec3(world * vec4(position, 1.0f));
    vs_output_normal = mat3(transpose(inverse(world))) * vs_output_normal;  
} 