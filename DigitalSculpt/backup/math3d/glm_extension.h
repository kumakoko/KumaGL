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
#include "glm/gtc/matrix_inverse.hpp" // ������ת�ù���

namespace DigitalSculpt
{
    class GlmExtension
    {
    public:
        /******************************************************************************************************************
         * Desc: ��glMatrix���У�quat.rotateX�������ڶ���Ԫ������Χ��X�����ת����GLM��OpenGL Mathematics�����У�������Ʋ����ĺ�����
                 glm::rotate������ͨ�����ھ��������Ԫ�������ض������ת��GLM�ṩ��һ�ָ�ֱ�ӵķ�������ͨ��ʹ��glm::rotate���������Ԫ��
                 ����������ʵ�֡�Ϊ�˽�Χ��X����ת���������ʹ��glm::angleAxis����һ����ʾ����ת����Ԫ����Ȼ�������ת��Ԫ��������Ҫ��ת��ԭ
                 ʼ��Ԫ����
         * Method:    rotateX
         * Returns:   glm::quat&
         * Parameter: glm::quat & target
         * Parameter: const glm::quat & src
         * Parameter: const float rotateXAngle Ҫ��ת�Ľǣ���λ�ǻ���
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

        /*webgl��glMatrix���е�mat3.normalFromMat4��������Ӧ��glm���е�ʲô����

       ChatGPT:
       ��WebGL��ʹ�õ�glMatrix���C++��ʹ�õ�GLM��OpenGL Mathematics���ⶼ�ṩ��
       �ḻ����ѧ���㹦�ܣ��ر�����ͼ�α���о����õ��ľ�����������㡣glMatrix���е�
       mat3.normalFromMat4�������ڴ�һ��4x4��ģ����ͼ��������ȡ��һ�����ڱ任���ߵ�
       3x3�����������ͨ���漰������ԭ4x4�������ת�þ��󣬵���������Ͻǵ�3x3���֣�
       ��Ϊ�ⲿ�ָ�����ת�����ţ���Է��߱任�Ǳ�Ҫ�ġ�

       ��GLM���У��������Ʋ����ĺ�����glm::inverseTranspose������������Խ���һ��4x4
       �ľ��󣬲���������ת�þ��󡣵��ǣ�Ϊ�˻����glMatrix��mat3.normalFromMat4��ͬ
       �Ľ�����������Ҫ�ֶ���ȡ���ؾ�������Ͻ�3x3���֡�������ΪGLM�������������4x4��
       �󣬶������Զ���ȡ3x3���֡�

       ������ԣ��������ʹ��glm::inverseTranspose��4x4���������ת�ã�Ȼ����ȡ3x3����
       ��Ӧ���ڷ��߱任������ʾ�����£�

       #include <glm/glm.hpp>
       #include <glm/gtc/matrix_inverse.hpp> // ������ת�ù���

       glm::mat4 modelViewMatrix = ...; // ���4x4ģ����ͼ����
       glm::mat4 invTransModelViewMatrix = glm::inverseTranspose(modelViewMatrix);

       // ���ڣ��������Ҫ3x3�ķ��߱任���󣬿���������ȡ��
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
