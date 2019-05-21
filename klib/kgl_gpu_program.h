/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
/*!
* \file kgl_gpu_program.h
* \date 2017/12/31 13:44
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief
*
* TODO: shader类
*
* \note
*/
#ifndef kgl_gpu_program_h__
#define kgl_gpu_program_h__

#include "kgl_texture.h"
#include "kgl_material.h"
#include "kgl_light.h"

namespace kgl
{
    class GPUProgram
    {
    public:
        /// <summary>
        /// <see cref="GPUProgram"/> 类的构造函数.
        /// </summary>
        GPUProgram();

        /// <summary>
        /// <see cref="GPUProgram"/> 类的析构函数.
        /// </summary>
        ~GPUProgram();

        /// <summary>
        /// 使用本shader程序
        /// </summary>
        void Use() const;

        void Finish() const;

        /// <summary>
        /// 从指定的vertex shader , fragment shader, geometry shader中创建处一个可执行的shader程序
        /// </summary>
        /// <param name="vs_file_path">vertex shader代码文件路径名</param>
        /// <param name="fs_file_path">fragment shader代码文件路径名</param>
        /// <param name="gs_file_path">geometry shader代码文件路径名</param>
        void CreateFromFile(const GLchar* vs_file_path, const GLchar* fs_file_path, const GLchar* gs_file_path);

        /// <summary>
        /// 从指定的vertex shader代码组 , fragment shader代码组, geometry shader代码组中创建处一个可执行的shader程序
        /// </summary>
        /// <param name="vs_file_paths">The vs_file_paths.</param>
        /// <param name="fs_file_paths">The fs_file_paths.</param>
        /// <param name="gs_file_path">The gs_file_path.</param>
        /// <param name="main_shader_version">The main_shader_version.</param>
        /// <param name="minor_shader_version">The minor_shader_version.</param>
        /// <param name="tail_shader_version">The tail_shader_version.</param>
        void CreateFromFile(const std::vector<std::string>& vs_file_paths, const std::vector<std::string>& fs_file_paths, const std::vector<std::string>& gs_file_path,
            uint32_t main_shader_version = 3, uint32_t minor_shader_version = 3 , uint32_t tail_shader_version = 0);

        /// <summary>
        /// Applies the texture.
        /// </summary>
        /// <param name="texture">The texture.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        /// <param name="slot_index">The slot_index.</param>
        void ApplyTexture(TextureSPtr texture, const char* uniform_var_name, GLuint slot_index);

        /// <summary>
        /// Applies the texture.
        /// </summary>
        /// <param name="texture">The texture.</param>
        /// <param name="location">The location.</param>
        /// <param name="slot_index">The slot_index.</param>
        void ApplyTexture(TextureSPtr texture, GLint location, GLuint slot_index);

        /// <summary>
        /// 目标texture已经处于激活并绑定状态，所以直接指定texture unit slot即可
        /// </summary>
        /// <param name="uniform_var_name">着色器代码中使用的纹理sampler的名字</param>
        /// <param name="slot_index">texture unit slot</param>
        void ApplyTexture(const char* uniform_var_name, GLuint slot_index);

        /// <summary>
        /// Applies the texture.
        /// </summary>
        /// <param name="location">The location.</param>
        /// <param name="slot_index">The slot_index.</param>
        void ApplyTexture(GLint location, GLuint slot_index);

        /// <summary>
        /// Applies the texture.
        /// </summary>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        /// <param name="texture_unit_slot">The texture_unit_slot.</param>
        /// <param name="texture_id">The texture_id.</param>
        /// <param name="target">The target.</param>
        void ApplyTexture(const char* uniform_var_name, GLuint texture_unit_slot, GLuint texture_id, GLenum target = GL_TEXTURE_2D);

        /// <summary>
        /// Applies the texture.
        /// </summary>
        /// <param name="location">The location.</param>
        /// <param name="texture_unit_slot">The texture_unit_slot.</param>
        /// <param name="texture_id">The texture_id.</param>
        /// <param name="target">The target.</param>
        void ApplyTexture(GLint location, GLuint texture_unit_slot, GLuint texture_id, GLenum target = GL_TEXTURE_2D);

        /// <summary>
        /// Applies the matrix.
        /// </summary>
        /// <param name="matrix_data_pointer">存放着矩阵数据的数组首指针</param>
        /// <param name="uniform_var_name">代码中该矩阵变量的名字</param>
        /// <param name="need_transpose">传递进来的矩阵需要转置吗</param>
        void ApplyMatrix(const GLfloat* matrix_data_pointer, const char* uniform_var_name, bool need_transpose = false);

        /// <summary>
        /// Applies the matrix.
        /// </summary>
        /// <param name="matrix_data_pointer">The matrix_data_pointer.</param>
        /// <param name="location">The location.</param>
        /// <param name="need_transpose">The need_transpose.</param>
        void ApplyMatrix(const GLfloat* matrix_data_pointer, GLint location, bool need_transpose = false);

        /// <summary>
        /// Applies the float.
        /// </summary>
        /// <param name="float_data">The float_data.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplyFloat(GLfloat float_data, const char* uniform_var_name);

        /// <summary>
        /// Applies the float.
        /// </summary>
        /// <param name="float_data">The float_data.</param>
        /// <param name="location">The location.</param>
        void ApplyFloat(GLfloat float_data, GLint location);

        /// <summary>
        /// Applies the vector2.
        /// </summary>
        /// <param name="vector2_data_pointer">The vector2_data_pointer.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplyVector2(const GLfloat* vector2_data_pointer, const char* uniform_var_name);

        /// <summary>
        /// Applies the vector2.
        /// </summary>
        /// <param name="vector2_data_pointer">The vector2_data_pointer.</param>
        /// <param name="location">The location.</param>
        void ApplyVector2(const GLfloat* vector2_data_pointer, GLint location);

        /// <summary>
        /// Applies the vector3.
        /// </summary>
        /// <param name="vector3_data_pointer">The vector3_data_pointer.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplyVector3(const GLfloat* vector3_data_pointer, const char* uniform_var_name);

        /// <summary>
        /// Applies the vector3.
        /// </summary>
        /// <param name="vector3_data_pointer">The vector3_data_pointer.</param>
        /// <param name="location">The location.</param>
        void ApplyVector3(const GLfloat* vector3_data_pointer, GLint location);

        /// <summary>
        /// Applies the vector4.
        /// </summary>
        /// <param name="vector4_data_pointer">The vector4_data_pointer.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplyVector4(const GLfloat* vector4_data_pointer, const char* uniform_var_name);

        /// <summary>
        /// Applies the vector4.
        /// </summary>
        /// <param name="vector4_data_pointer">The vector4_data_pointer.</param>
        /// <param name="location">The location.</param>
        void ApplyVector4(const GLfloat* vector4_data_pointer, GLint location);

        /// <summary>
        /// Applies the int.
        /// </summary>
        /// <param name="int_data">The int_data.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplyInt(GLint int_data, const char* uniform_var_name);

        /// <summary>
        /// Applies the int.
        /// </summary>
        /// <param name="int_data">The int_data.</param>
        /// <param name="location">The location.</param>
        void ApplyInt(GLint int_data, GLint location);

        /// <summary>
        /// 要使用此方法去设置shader中的材质，则shader代码务必使用material.glsl中定义的Material结构
        /// </summary>
        /// <param name="material">The material.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplyMaterial(const Material* material, const char* uniform_var_name);

        /// <summary>
        /// Applies the material.
        /// </summary>
        /// <param name="material">The material.</param>
        /// <param name="location">The location.</param>
        void ApplyMaterial(const Material* material, const MaterialUniformLocation& location);

        /// <summary>
        /// 获取到给定的材质变量所对应的uniform location
        /// </summary>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        /// <returns>kgl.MaterialUniformLocation.</returns>
        MaterialUniformLocation GetMaterialUniformLocation(const char* uniform_var_name) const;

        /// <summary>
        /// Applies the directional light.
        /// </summary>
        /// <param name="light">The light.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplyDirectionalLight(const DirectionalLight* light, const char* uniform_var_name);

        /// <summary>
        /// Applies the directional light.
        /// </summary>
        /// <param name="light">The light.</param>
        /// <param name="location">The location.</param>
        void ApplyDirectionalLight(const DirectionalLight* light, const DirLightUniformLocation& location);

        /// <summary>
        /// Gets the dir light uniform location.
        /// </summary>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        /// <returns>kgl.DirLightUniformLocation.</returns>
        DirLightUniformLocation GetDirLightUniformLocation(const char* uniform_var_name) const;

        /// <summary>
        /// Applies the point light.
        /// </summary>
        /// <param name="light">The light.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplyPointLight(const PointLight* light, const char* uniform_var_name);

        /// <summary>
        /// Applies the point light.
        /// </summary>
        /// <param name="light">The light.</param>
        /// <param name="location">The location.</param>
        void ApplyPointLight(const PointLight* light, const PointLightUniformLocation& location);

        /// <summary>
        /// Gets the point light uniform location.
        /// </summary>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        /// <returns>kgl.PointLightUniformLocation.</returns>
        PointLightUniformLocation GetPointLightUniformLocation(const char* uniform_var_name) const;

        /// <summary>
        /// Applies the spot light.
        /// </summary>
        /// <param name="light">The light.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplySpotLight(const SpotLight* light, const char* uniform_var_name);

        /// <summary>
        /// Applies the spot light.
        /// </summary>
        /// <param name="light">The light.</param>
        /// <param name="location">The location.</param>
        void ApplySpotLight(const SpotLight* light, const SpotLightUniformLocation& location);

        /// <summary>
        /// Gets the spot light uniform location.
        /// </summary>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        /// <returns>kgl.SpotLightUniformLocation.</returns>
        SpotLightUniformLocation GetSpotLightUniformLocation(const char* uniform_var_name) const;

        /// <summary>
        /// 根据指定的uniform变量名，获取到该变量对应的locaton值
        /// </summary>
        /// <param name="uniform_var_name">niform变量名</param>
        /// <returns>该变量对应的locaton值</returns>
        GLint GetUniformLocation(const char* uniform_var_name) const;
protected:
        /// <summary>
        /// shader代码对象的句柄
        /// </summary>
        GLuint program_handle_;
    };

    typedef std::shared_ptr<GPUProgram> GPUProgramSPtr;
}
#endif // kgl_gpu_program_h__