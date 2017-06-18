// 通用的基于片元着色器的光照计算函数
// 要使用本shader，务必在载入本shader之前载入了lighting_struct.glsl

// 基于phone光照模型的有向平行光的光照计算函数
vec4 PhongDirectionalLighting
(
vec3 fragment_pos_in_world,   // 片元在世界空间中的坐标
vec3 fragment_normal_in_world,// 片元法线在世界空间中的坐标
vec3 view_pos_in_world,       // 摄像机在世界空间中的坐标
DirectionalLight dir_light,
LightingMaterial light_mat
)
{
    vec3 norm = normalize(fragment_normal_in_world);
     
    // 计算全局环境光
    vec3 ambient = dir_light.Ambient * light_mat.Ambient;

    // 计算漫反射光照 
    vec3 light_dir = normalize(dir_light.Direction);// normalize(dir_light.Position - fragment_pos_in_world);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = dir_light.Diffuse * (diff * light_mat.Diffuse);
    
    // 计算镜面高光光照
    vec3 view_dir = normalize(view_pos_in_world - fragment_pos_in_world);
    vec3 reflect_dir = reflect(-light_dir, norm);  
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), light_mat.Shininess);
    vec3 specular = dir_light.Specular * (spec * light_mat.Specular);  

    return vec4((ambient + diffuse + specular), 1.0f);
}

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

    // 计算漫反射光照 
    float diff = max(dot(fragment_normal_in_world, light_dir), 0.0);
    vec3 diffuse = point_light.Diffuse * (diff * light_mat.Diffuse);
    
    // 计算镜面高光光照
    vec3 view_dir = normalize(view_pos_in_world - fragment_pos_in_world);
    vec3 reflect_dir = reflect(-light_dir, fragment_normal_in_world);  
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), light_mat.Shininess);
    vec3 specular = point_light.Specular * (spec * light_mat.Specular);  

    vec4 result_color = vec4((ambient + diffuse + specular), 1.0f);
	
	// 计算衰减值
    float attenuation = point_light.AttenuationConstant +
          point_light.AttenuationLinear * dist + point_light.AttenuationExp * dist * dist;
		  
    // return result_color / attenuation;
	return result_color;
}

vec4 PhongSpotLighting
(
vec3 fragment_pos_in_world,    // fragment_pos_in_world 片元在世界空间中的坐标
vec3 fragment_normal_in_world, // fragment_normal_in_world 片元法线在世界空间中的坐标
vec3 view_pos_in_world,        // view_pos_in_world 摄像机在世界空间中的坐标
SpotLight spot_light,          // spot_light 探照灯光源结构体，在lighting_struct.glsl中定义
LightingMaterial light_mat
)
{
	vec3 frag_to_eyes = normalize(view_pos_in_world - fragment_pos_in_world); 
	vec3 eyes_to_light = view_pos_in_world - spot_light.Position;
	float dist_eyes_to_light = length(eyes_to_light);
	eyes_to_light = normalize(eyes_to_light);
	
	// 计算衰减值
	// y constant attenuation
	// z linear attenuation
	// w quadratic attenuation
	float att = 1.0/(spot_light.CutOffAndAttenuation.y + spot_light.CutOffAndAttenuation.z * dist_eyes_to_light + spot_light.CutOffAndAttenuation.w * dist_eyes_to_light * dist_eyes_to_light);
	
	float dot_spot = dot(-eyes_to_light, normalize(spot_light.Direction));
    float cos_cutoff = cos(spot_light.CutOffAndAttenuation.x * 3.1415926/180.0);
	
	float spot_att = 0;
    if (dot_spot < cos_cutoff)
        spot_att = 0;
    else
        spot_att = pow(dot_spot, spot_light.Exponent); 
		
	att *= spot_att;
	
	vec3 h = normalize(eyes_to_light + frag_to_eyes);
    
    float dot_n_e = max(0, dot(fragment_normal_in_world, eyes_to_light));
    float dot_n_h = max(0, dot(fragment_normal_in_world, h));
    
    vec3 amb = spot_light.Ambient * light_mat.Ambient * att;
    vec3 diff = spot_light.Diffuse * light_mat.Diffuse * dot_n_e * att;
    vec3 spec = spot_light.Specular * light_mat.Specular * pow(dot_n_h, light_mat.Shininess) * att;
	
	return vec4( amb + diff + spec , 1.0);
}