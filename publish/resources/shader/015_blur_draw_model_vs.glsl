// 光照计算

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 fs_normal;
out vec3 fs_frag_pos_world;

uniform mat4 local_transform_matrix;
uniform mat4 world_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	mat4 mw = world_matrix * local_transform_matrix;
    gl_Position = projection_matrix * view_matrix * mw  * vec4(position, 1.0f);
    fs_frag_pos_world = vec3(mw * vec4(position, 1.0f));
    fs_normal = mat3(transpose(inverse(mw))) * normal;  
} 