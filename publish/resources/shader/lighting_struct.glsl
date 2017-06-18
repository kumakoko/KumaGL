// 光照计算用到的一些预定义结构体

// 表面材质，描述对光的反射能力

struct LightingMaterial 
{
    vec3  Ambient;		// 对环境光的反射能力
    vec3  Diffuse;		// 对漫反射部分的反射能力
    vec3  Specular;		// 对镜面高光的反射能力
    float Shininess;	// 镜面高光的高亮系数
}; 

// 有向平行光的描述
struct DirectionalLight 
{
    vec3 Direction;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
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

// 探照灯的描述
struct SpotLight
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	vec3 Position;				// 点光源在世界空间中的坐标位置
	vec3 Direction;				// 探照灯的照射方向
	vec4 CutOffAndAttenuation; 	// 4个分量分别是cut off ，常数衰减值，线性衰减值，指数衰减值
	float Exponent;
};