// 使用法线贴图，实现凹凸效果的顶点着色器
// #version 330 core

out vec4 FragColor;

in VSOutput 
{
    vec3 fragment_pos_in_world;
    vec2 fragment_tex_coord_1;
    vec3 light_pos_in_tangent_space;
    vec3 camera_pos_in_tangent_space;
    vec3 fragment_pos_in_tangent_space;
	
	vec3 specular;
	float attenuation_constant;	// 常数衰减值
	float attenuation_linear;	// 线性衰减值
	float attenuation_exp;		// 指数衰减值
} fs_in;

uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform float shininess;       // 镜面反射所使用的辉光值 32.0f


void main()
{           
	// 从法线贴图中，取得本片元对应的法线值，因为从图片中取得
	// 的法线值是RGB值，其取值范围是[0,1]。而转换到切线空间后
	// 取值范围是[-1,1]。所以要做一个映射关系的转换。
    vec3 normal = texture(normal_map, fs_in.fragment_tex_coord_1).rgb;
    normal = normalize(normal * 2.0 - 1.0);
   
    // 从漫反射贴图中，取得片元的对应的漫反射颜色值
    vec3 color = texture(diffuse_map, fs_in.fragment_tex_coord_1).rgb;

	vec3 light_line = fs_in.light_pos_in_tangent_space - fs_in.fragment_pos_in_tangent_space;
	
	// 计算点光源的衰减值
	float dist = length( light_line );  
    float attenuation = 1.0 / (fs_in.attenuation_constant + fs_in.attenuation_linear * dist + fs_in.attenuation_exp * dist * dist );  
	
	// 环境光
	vec3 ambient = color * attenuation;
	
	// 利用法线，对输入的光线进行干扰，算出本片元实际的漫反射颜色
    vec3 light_dir = normalize(light_line);
    float diff = max(dot(light_dir, normal), 0.0);
	// 漫反射所需要的入射光值和反射材质值，已经计算并存储好放在漫反射纹理中了，
	// 所以不需要用到光源中的漫反射分量去计算了
    vec3 diffuse = diff * color * attenuation;
   
    // 利用法线和观察点，对输入的光线进行干扰，算出本片元实际的镜面反射反射颜色
    vec3 view_dir = normalize(fs_in.camera_pos_in_tangent_space - fs_in.fragment_pos_in_tangent_space);
    vec3 reflect_dir = reflect(-light_dir, normal);
    vec3 halfway_dir = normalize(light_dir + view_dir);  
    float spec = pow(max(dot(normal, halfway_dir), 0.0), shininess);
    vec3 specular =  fs_in.specular * spec * attenuation;
    FragColor = vec4(1,0,0,1);//vec4(ambient + diffuse + specular, 1.0);
}