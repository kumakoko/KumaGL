// 灯光类定义
#ifndef kgl_light_h__
#define kgl_light_h__

namespace kgl
{
    struct DirectionalLight 
    {
        glm::vec3 Direction;
        glm::vec3 Ambient;
        glm::vec3 Diffuse;
        glm::vec3 Specular;
    };

	struct DirLightUniformLocation
	{
		GLint Direction;
		GLint Ambient;
		GLint Diffuse;
		GLint Specular;
	};

    struct PointLight
    {
        glm::vec3 Ambient;
        glm::vec3 Diffuse;
        glm::vec3 Specular;
        glm::vec3 Position;         // 点光源在世界空间中的坐标位置
        float AttenuationConstant;  // 常数衰减值
        float AttenuationLinear;    // 线性衰减值
        float AttenuationExp;       // 指数衰减值
    };

	struct PointLightUniformLocation
	{
		GLint Ambient;
		GLint Diffuse;
		GLint Specular;
		GLint Position;         // 点光源在世界空间中的坐标位置
		GLint AttenuationConstant;  // 常数衰减值
		GLint AttenuationLinear;    // 线性衰减值
		GLint AttenuationExp;       // 指数衰减值
	};

    struct SpotLight
    {
        glm::vec3 Ambient;
        glm::vec3 Diffuse;
        glm::vec3 Specular;
        glm::vec3 Position;         // 点光源在世界空间中的坐标位置
        glm::vec3 Direction;        // 探照灯的照射方向
        glm::vec4 CutOffAndAttenuation; // 4个分量分别是cut off ，常数衰减值，线性衰减值，指数衰减值
        float     Exponent;
    };

	struct SpotLightUniformLocation
	{
		GLint Ambient;
		GLint Diffuse;
		GLint Specular;
		GLint Position;         // 点光源在世界空间中的坐标位置
		GLint Direction;        // 探照灯的照射方向
		GLint CutOffAndAttenuation; // 4个分量分别是cut off ，常数衰减值，线性衰减值，指数衰减值
		GLint Exponent;
	};

	
	/// <summary>
	/// 用来渲染low polygon terrain的简单灯光
	/// </summary>
	struct LowPolyTerrainLight
	{
		glm::vec3 Direction;
		glm::vec3 Color;
		glm::vec2 Bias;
	};
}
#endif // kgl_light_h__