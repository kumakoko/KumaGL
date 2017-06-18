// 材质定义
#ifndef kgl_material_h__
#define kgl_material_h__

namespace kgl
{
    struct Material 
    {
        glm::vec3   Ambient;
        glm::vec3   Diffuse;
        glm::vec3   Specular;
        float       Shininess;
    };
}
#endif // kgl_material_h__