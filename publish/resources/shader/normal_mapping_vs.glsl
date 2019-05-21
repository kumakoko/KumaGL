// 使用法线贴图，实现凹凸效果的顶点着色器

layout (location = 0) in vec3 vertex_position;     	// 顶点位置坐标，基于世界空间
layout (location = 1) in vec3 vertex_normal; 		// 顶点法线
layout (location = 2) in vec3 vertex_tangent; 		// 顶点切线
layout (location = 3) in vec3 vertex_binormal; 		// 顶点副法线
layout (location = 4) in vec2 vertex_tex_coord_1; 	// 顶点第一层纹理映射坐标

out VSOutput 
{
    vec3 fragment_pos_in_world;          // 片元在世界空间中的中的位置
    vec2 fragment_tex_coord_1;			 // 片元的第一层纹理贴图坐标
    vec3 light_pos_in_tangent_space;     // 光源位置从世界空间变换到切线空间后的值
    vec3 camera_pos_in_tangent_space;    // 摄像机位置从世界空间变换到切线空间后的值
    vec3 fragment_pos_in_tangent_space;  // 片元位置从世界空间变换到切线空间后的值
	
	vec3 specular;
	float attenuation_constant;	// 常数衰减值
	float attenuation_linear;	// 线性衰减值
	float attenuation_exp;		// 指数衰减值
}vs_out;

uniform mat4 projection_matrix;			 // 摄像机的投影矩阵
uniform mat4 view_matrix;				 // 摄像机的观察矩阵
uniform mat4 world_matrix;				 // 顶点的世界变换矩阵
uniform vec3 camera_world_position;		 // 摄像机在世界坐标中的位置
uniform PointLight point_light;			 // 点光源

void main()
{
    vs_out.fragment_pos_in_world = vec3(world_matrix * vec4(vertex_position, 1.0));   
    vs_out.fragment_tex_coord_1 = vertex_tex_coord_1;
    
	// 把法线从局部空间变换到世界空间，其变换矩阵为世界矩阵的逆转置矩阵
    mat3 normal_matrix = transpose(inverse(mat3(world_matrix)));
	
	// 得到转换到世界空间中的顶点的法线和切线，并规格化
    vec3 tangent_in_world = normalize(normal_matrix * vertex_tangent);
    vec3 normal_in_world = normalize(normal_matrix * vertex_normal);
    
	// 得到和世界空间中的法线及切线都垂直的副法线
	tangent_in_world = normalize(tangent_in_world - dot(tangent_in_world, normal_in_world) * normal_in_world);
    vec3 binormal_in_world = cross(normal_in_world, tangent_in_world);
    
	// 法线，切线，副法线相互垂直，所以以这三个向量为基，构造一个切线空间，并得到变换矩阵
	// 将世界空间中的光源位置，摄像机位置，还有顶点（片元）的位置
    mat3 tbn_space = transpose(mat3(tangent_in_world, binormal_in_world, normal_in_world));
    vs_out.light_pos_in_tangent_space = tbn_space * point_light.Position; // light_world_position;
    vs_out.camera_pos_in_tangent_space  = tbn_space * camera_world_position;
    vs_out.fragment_pos_in_tangent_space  = tbn_space * vs_out.fragment_pos_in_world;
	vs_out.specular = point_light.Specular;
	vs_out.attenuation_constant = point_light.AttenuationConstant;	// 常数衰减值
	vs_out.attenuation_linear = point_light.AttenuationLinear;	// 线性衰减值
	vs_out.attenuation_exp = point_light.AttenuationExp;		// 指数衰减值
    
	// 得到经过wvp变换后的片元坐标
    gl_Position = projection_matrix * view_matrix * world_matrix * vec4(vertex_position, 1.0);
}