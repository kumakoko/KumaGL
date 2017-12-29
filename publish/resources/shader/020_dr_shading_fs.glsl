#version 330 core
out vec4 FragColor;

in vec2 v2f_texture_coord_1;

uniform sampler2D u_pos_in_gbuffer;
uniform sampler2D u_normal_in_gbuffer;
uniform sampler2D u_albedo_in_gbuffer;
uniform vec3 u_view_pos;

struct PointLight
{
    vec3 Ambient;               // 点光源提供给环境光计算的颜色
    vec3 Diffuse;               // 点光源提供给漫反射计算的颜色
    vec3 Specular;              // 点光源提供给镜面反射计算的颜色
    vec3 Position;              // 点光源在世界空间中的坐标位置
    float AttenuationConstant;  // 常数衰减值
    float AttenuationLinear;    // 线性衰减值
    float AttenuationExp;       // 指数衰减值
};

const int NR_LIGHTS = 30;

uniform PointLight u_point_lights[NR_LIGHTS];

vec4 PhongPointLight()
{
// 从G-buffer的四个缓冲区中取得片元的颜色，法线和漫反射颜色
    vec3 FragPos = texture(u_pos_in_gbuffer, v2f_texture_coord_1).rgb;
    vec3 Normal = texture(u_normal_in_gbuffer, v2f_texture_coord_1).rgb;
    vec3 Diffuse = texture(u_albedo_in_gbuffer, v2f_texture_coord_1).rgb;
    float Specular = texture(u_albedo_in_gbuffer, v2f_texture_coord_1).a;
    
    // 计算
    vec3 lighting = vec3(0.0,0.0,0.0);
    vec3 viewDir  = normalize(u_view_pos - FragPos);
    
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        // 全局环境光由灯光的ambient分量提供
        vec3 ambient_component = u_point_lights[i].Ambient;
        
        // 漫反射部分由灯光的diffuse分量和diffuse纹理贴图提供
        vec3 light_dir = normalize(u_point_lights[i].Position - FragPos);
        vec3 diffuse_component = max(dot(Normal, light_dir), 0.0) * Diffuse * u_point_lights[i].Diffuse;

        // 镜面分量
        vec3 half_way_dir = normalize(light_dir + viewDir);  
        float spec = pow(max(dot(Normal, half_way_dir), 0.0), 16.0);
        vec3 specular_component = u_point_lights[i].Specular * spec * Specular;


        // 计算衰减值
        float distance = length(u_point_lights[i].Position - FragPos);
        float attenuation = 1.0 / (u_point_lights[i].AttenuationConstant + 
                                   u_point_lights[i].AttenuationLinear * distance +
                                   u_point_lights[i].AttenuationExp * distance * distance);
        diffuse_component *= attenuation;
        specular_component *= attenuation;
        ambient_component *= attenuation;
        lighting += diffuse_component + specular_component + ambient_component;
    }
    
    return vec4(lighting, 1.0);
}

void main()
{
	FragColor = PhongPointLight();
}