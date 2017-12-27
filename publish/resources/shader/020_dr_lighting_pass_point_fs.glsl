// 点光源的光照计算的fragment shader
// 光照计算用到的一些预定义结构体
// 表面材质，描述对光的反射能力
#version 330

struct LightingMaterial 
{
    vec3  Ambient;		// 对环境光的反射能力
    vec3  Diffuse;		// 对漫反射部分的反射能力
    vec3  Specular;		// 对镜面高光的反射能力
    float Shininess;	// 镜面高光的高亮系数
}; 

// 点光源的描述
struct PointLight
{
	vec3 Ambient;		
	vec3 Diffuse;
	vec3 Specular;
	vec3 Position;				// 点光源在世界空间中的坐标位置
	float AttenuationConstant;	// 常数衰减值
	float AttenuationLinear;	// 线性衰减值
	float AttenuationExp;		// 指数衰减值
};

uniform sampler2D u_position_in_map; 	// 存储在G-buffer中的片元的position
uniform sampler2D u_color_in_map;    	// 存储在G-buffer中的片元的漫反射纹理颜色
uniform sampler2D u_normal_in_map;   	// 存储在G-buffer中的片元的法线
uniform PointLight u_point_light; 		// 从CPU传入的点光源的属性值
uniform vec3 u_view_pos_in_world;		// 摄像机在世界空间中的坐标
uniform vec2 u_screen_size;				// 屏幕的高宽
  
// 基于Phong光照模型的逐片元光照函数
// 根据传递进来的片元的在世界中的位置坐标和法线值，计算光照
vec4 PhongPointLighting
(
vec3 fragment_pos_in_world,    // fragment_pos_in_world 片元在世界空间中的坐标
vec3 fragment_normal_in_world, // fragment_normal_in_world 片元法线在世界空间中的坐标
vec3 view_pos_in_world,        // view_pos_in_world 摄像机在世界空间中的坐标
PointLight point_light,        // point_light 点光源结构体，在lighting_struct.glsl中定义
LightingMaterial light_mat
)
{
	vec3 light_dir = fragment_pos_in_world - point_light.Position; // 得到从点光源到片元在世界空间中的连线
	float dist = length(light_dir); // 得到连线的长度
	light_dir = normalize(light_dir); // 单位化点光源到片元位置的连线方向向量
	
	// 计算全局环境光
    vec3 ambient = point_light.Ambient * light_mat.Ambient;

    // 计算漫反射部分的光照 
    float diff = max(dot(fragment_normal_in_world, light_dir), 0.0);
    vec3 diffuse = point_light.Diffuse * (diff * light_mat.Diffuse);
    
    // 计算镜面反射部分的光照
    vec3 view_dir = normalize(view_pos_in_world - fragment_pos_in_world);
    vec3 reflect_dir = reflect(-light_dir, fragment_normal_in_world);  
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), light_mat.Shininess);
    vec3 specular = point_light.Specular * (spec * light_mat.Specular);  

    vec4 result_color = vec4((ambient + diffuse + specular), 1.0f);
	
	// 计算衰减值
    float attenuation = point_light.AttenuationConstant +
          point_light.AttenuationLinear * dist + point_light.AttenuationExp * dist * dist;
		  
    return result_color / attenuation;
}

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / u_screen_size;
}

out vec4 fragment_color;

void main()
{

    vec2 TexCoord = CalcTexCoord();
	
	// 从G-buffer纹理中取出片元所对应的world position
    vec3 fragment_world_pos = texture(u_position_in_map, TexCoord).xyz;
	
	// 取出片元的材质颜色
    vec3 Color = texture(u_color_in_map, TexCoord).xyz;
	
	// 取出片元的法线并单位化
    vec3 fragment_normal = texture(u_normal_in_map, TexCoord).xyz;
    fragment_normal = normalize(fragment_normal);
		
	LightingMaterial light_material;
	light_material.Ambient = vec3(1.0,1.0,1.0);	// 对环境光的反射能力
    light_material.Diffuse = vec3(1.0,1.0,1.0);	// 对漫反射部分的反射能力
    light_material.Specular = vec3(1.0,1.0,1.0);// 对镜面高光的反射能力
    light_material.Shininess = 12.0;				// 高亮系数
	/*
	// 传递片元的world position和normal进去计算光照
    fragment_color = PhongPointLighting(
	fragment_world_pos,  // 片元在世界空间中的坐标
	fragment_normal,     // 片元法线在世界空间中的坐标
	u_view_pos_in_world, // 摄像机在世界空间中的坐标
	u_point_light,		 // 点光源结构体
	light_material);	 // 光照材质
	*/
	fragment_color = vec4(0.45,0.32,0.89,1.0);
}
