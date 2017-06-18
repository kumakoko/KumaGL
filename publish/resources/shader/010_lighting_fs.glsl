// 光照计算
// #version 330 core 因为这里要include其他的代码，所以不用在这里指定版本了

in vec3 fs_normal;
in vec3 fs_frag_pos_world;  
uniform vec3                view_pos;           // 视点（摄像机）的位置，基于世界坐标
uniform LightingMaterial    material;           // 物体表面材质，在lighting_struct.glsl文件中定义
uniform DirectionalLight    directional_light;  // 从外部传入的有向平行光，在lighting_struct.glsl文件中定义
uniform PointLight			point_light;
uniform SpotLight			spot_light;

out vec4 color;

void main()
{
    // 传递进来的有向平行光执行光照计算
    color = PhongDirectionalLighting(fs_frag_pos_world,fs_normal,view_pos,directional_light,material);
	color += PhongPointLighting(fs_frag_pos_world,fs_normal,view_pos,point_light,material);
	color += PhongSpotLighting(fs_frag_pos_world,fs_normal,view_pos,spot_light,material);
}