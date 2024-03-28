#ifndef sculpt_digital_glm_extension_h__
#define sculpt_digital_glm_extension_h__

#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/ext/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale
#include "glm/ext/matrix_clip_space.hpp" // glm::perspective
#include "glm/ext/scalar_constants.hpp" // glm::pi
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/gtc/matrix_inverse.hpp" // 引入逆转置功能

namespace DigitalSculpt
{
    class GlmExtension
    {
    public:
        /******************************************************************************************************************
         * Desc: 在glMatrix库中，quat.rotateX函数用于对四元数进行围绕X轴的旋转。在GLM（OpenGL Mathematics）库中，完成相似操作的函数是
                 glm::rotate，但它通常用于矩阵。针对四元数进行特定轴的旋转，GLM提供了一种更直接的方法，即通过使用glm::rotate函数结合四元数
                 创建函数来实现。为了仅围绕X轴旋转，你可以先使用glm::angleAxis创建一个表示该旋转的四元数，然后将这个旋转四元数乘以你要旋转的原
                 始四元数。
         * Method:    rotateX
         * Returns:   glm::quat&
         * Parameter: glm::quat & target
         * Parameter: const glm::quat & src
         * Parameter: const float rotateXAngle 要旋转的角，单位是弧度
         ****************************************************************************************************************/
        static glm::quat& rotateX(glm::quat& target, const glm::quat& src, const float rotateXAngle)
        {
            glm::quat qX = glm::angleAxis(rotateXAngle, glm::vec3(1.0f, 0.0f, 0.0f));
            target = qX * src;
            return target;
        }

        /***************************************************
        
        @name: DigitalSculpt::GlmExtension::rotateY
        @return: glm::quat&
        @param: glm::quat & target
        @param: const glm::quat & src
        @param: const float rotateYAngle
        ***************************************************/
        static glm::quat& rotateY(glm::quat& target, const glm::quat& src, const float rotateYAngle)
        {
            glm::quat qY = glm::angleAxis(rotateYAngle, glm::vec3(0.0f, 1.0f, 0.0f));
            target = qY * src;
            return target;
        }

        /***************************************************
        
        @name: DigitalSculpt::GlmExtension::negate
        @return: glm::vec3&
        @param: glm::vec3 & target
        @param: const glm::vec3 & src
        ***************************************************/
        static glm::vec3& negate(glm::vec3& target, const glm::vec3& src)
        {
            target.x = -src.x;
            target.y = -src.y;
            target.z = -src.z;
            return target;
        }

        /***************************************************
        
        @name: DigitalSculpt::GlmExtension::set
        @return: glm::vec3&
        @param: glm::vec3 & out
        @param: float x
        @param: float y
        @param: float z
        ***************************************************/
        static glm::vec3& set(glm::vec3& out, float x, float y, float z)
        {
            out.x = x;
            out.y = y;
            out.z = z;
            return out;
        }

        /***************************************************
        
        @name: DigitalSculpt::GlmExtension::scaleAndAdd
        @return: glm::vec3&
        @param: glm::vec3 & out
        @param: const glm::vec3 & a
        @param: const glm::vec3 & b
        @param: float scale
        ***************************************************/
        static glm::vec3& scaleAndAdd(glm::vec3& out, const glm::vec3& a, const glm::vec3& b, float scale)
        {
            out[0] = a[0] + b[0] * scale;
            out[1] = a[1] + b[1] * scale;
            out[2] = a[2] + b[2] * scale;
            return out;
        }

        /*webgl的glMatrix库中的mat3.normalFromMat4函数，对应于glm库中的什么函数

       ChatGPT:
       在WebGL中使用的glMatrix库和C++中使用的GLM（OpenGL Mathematics）库都提供了
       丰富的数学运算功能，特别是在图形编程中经常用到的矩阵和向量运算。glMatrix库中的
       mat3.normalFromMat4函数用于从一个4x4的模型视图矩阵中提取出一个用于变换法线的
       3x3矩阵。这个过程通常涉及到计算原4x4矩阵的逆转置矩阵，但仅针对左上角的3x3部分，
       因为这部分负责旋转和缩放，这对法线变换是必要的。

       在GLM库中，进行相似操作的函数是glm::inverseTranspose。这个函数可以接受一个4x4
       的矩阵，并返回其逆转置矩阵。但是，为了获得与glMatrix中mat3.normalFromMat4相同
       的结果，你可能需要手动提取返回矩阵的左上角3x3部分。这是因为GLM处理的是完整的4x4矩
       阵，而不是自动抽取3x3部分。

       具体而言，你可以先使用glm::inverseTranspose对4x4矩阵进行逆转置，然后提取3x3部分
       以应用于法线变换。代码示例如下：

       #include <glm/glm.hpp>
       #include <glm/gtc/matrix_inverse.hpp> // 引入逆转置功能

       glm::mat4 modelViewMatrix = ...; // 你的4x4模型视图矩阵
       glm::mat4 invTransModelViewMatrix = glm::inverseTranspose(modelViewMatrix);

       // 现在，如果你需要3x3的法线变换矩阵，可以这样获取：
       glm::mat3 normalMatrix = glm::mat3(invTransModelViewMatrix);
       */
        static glm::mat3& normalFromMat4(glm::mat3& m3, const glm::mat4& m4)
        {
            glm::mat4 invTransModelViewMatrix = glm::inverseTranspose(m4);
            m3 = glm::mat3(invTransModelViewMatrix);
            return m3;
        }
    };
}


#endif // glm_extension_h__
