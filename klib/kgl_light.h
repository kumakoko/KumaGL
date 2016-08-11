// 灯光类定义
#ifndef kgl_light_h__
#define kgl_light_h__

namespace kgl
{
	struct Light 
	{
		glm::vec3 Position;
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
	};
}
#endif // kgl_light_h__