#ifndef kgl_glm_ext_h__
#define kgl_glm_ext_h__

namespace kgl
{
    inline glm::vec3 vec_project(const glm::vec4& vec)
    {
        return glm::vec3(vec.x,vec.y,vec.z) / glm::vec3(vec.w);
    }
}

#endif // kgl_glm_ext_h__
