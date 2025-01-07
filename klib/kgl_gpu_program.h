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
        /*********************************************************
        GPUProgram类的构造函数
        *********************************************************/
        GPUProgram();

        /*********************************************************
        GPUProgram类的析构函数
        *********************************************************/
        ~GPUProgram();

        /*********************************************************
        使用本shader程序
        *********************************************************/
        void Use() const;

        /*********************************************************
        结束本shader程序
        *********************************************************/
        void Finish() const;

        /*********************************************************
        从指定的vertex shader , fragment shader, geometry shader中
        创建处一个可执行的shader程序
        @param const GLchar * vs_file_path vertex shader 代码文件路径名
        @param const GLchar * fs_file_path fragment shader 代码文件路径名
        @param const GLchar * gs_file_path geometry shader 代码文件路径名
        *********************************************************/
        void CreateFromFile(const GLchar* vs_file_path, const GLchar* fs_file_path, const GLchar* gs_file_path);

        /*********************************************************

        @param  const GLchar * vs_file_path
        @param  const GLchar * gs_file_path
        @param  const std::vector<const char * > & varings
        @return void
        *********************************************************/
        void CreateTransformFeedbackShaderFromFile(const GLchar* vs_file_path, const GLchar* gs_file_path, const std::vector<const char*>& varings);

        /*********************************************************
        从指定的vertex shader 代码组 , fragment shader 代码组,
        geometry shader 代码组中创建处一个可执行的shader程序
        @param  const std::vector<std::string> & vs_file_paths
        @param  const std::vector<std::string> & fs_file_paths
        @param  const std::vector<std::string> & gs_file_path
        @param  uint32_t main_shader_version
        @param  uint32_t minor_shader_version
        @param  uint32_t tail_shader_version
        @return void
        *********************************************************/
        void CreateFromFile(const std::vector<std::string>& vs_file_paths, const std::vector<std::string>& fs_file_paths, const std::vector<std::string>& gs_file_path,
            uint32_t main_shader_version = 3, uint32_t minor_shader_version = 3, uint32_t tail_shader_version = 0);

        /*********************************************************
        Applies the texture.
        @param TextureSPtr texture
        @param const char * uniform_var_name
        @param GLuint slot_index
        *********************************************************/
        void ApplyTexture(TextureSPtr texture, const char* uniform_var_name, GLuint slot_index);

        /*********************************************************

        @param  TextureSPtr texture
        @param  GLint location
        @param  GLuint slot_index
        *********************************************************/
        void ApplyTexture(TextureSPtr texture, GLint location, GLuint slot_index);

        /*********************************************************
        目标texture已经处于激活并绑定状态，所以直接指定texture unit slot 即可
        @param const char * uniform_var_name 色器代码中使用的纹理sampler的名字
        @param GLuint slot_index texture unit slot
        *********************************************************/
        void ApplyTexture(const char* uniform_var_name, GLuint slot_index);

        /*********************************************************

        @param  GLint location
        @param  GLuint slot_index
        @return void
        *********************************************************/
        void ApplyTexture(GLint location, GLuint slot_index);

        /*********************************************************
        绑定一个纹理到shader的uniform变量中
        @param  const char * uniform_var_name uniform变量名
        @param  GLuint texture_unit_slot texture用的slot，比如GL_TEXTURE0，GL_TEXTURE1这些
        @param  GLuint texture_id texture 的id
        @param  GLenum target
        *********************************************************/
        void ApplyTexture(const char* uniform_var_name, GLuint texture_unit_slot, GLuint texture_id, GLenum target = GL_TEXTURE_2D);

        /*********************************************************

        @param  GLint location
        @param  GLuint texture_unit_slot
        @param  GLuint texture_id
        @param  GLenum target
        @return void
        *********************************************************/
        void ApplyTexture(GLint location, GLuint texture_unit_slot, GLuint texture_id, GLenum target = GL_TEXTURE_2D);

        /*********************************************************

        @param  const GLfloat * matrix_data_pointer 存放着矩阵数据的数组首指针
        @param  const char * uniform_var_name 代码中该矩阵变量的名字
        @param  bool need_transpose 传递进来的矩阵需要转置吗
        @return void
        *********************************************************/
        void ApplyMatrix(const GLfloat* matrix_data_pointer, const char* uniform_var_name, bool need_transpose = false);

        /*********************************************************

        @param  const GLfloat * matrix_data_pointer
        @param  GLint location
        @param  bool need_transpose
        @return void
        *********************************************************/
        void ApplyMatrix(const GLfloat* matrix_data_pointer, GLint location, bool need_transpose = false);

        /*********************************************************

        @param  GLfloat float_data
        @param  const char * uniform_var_name
        @return void
        *********************************************************/
        void ApplyFloat(GLfloat float_data, const char* uniform_var_name);

        /*********************************************************

        @param  GLfloat float_data
        @param  GLint location
        @return void
        *********************************************************/
        void ApplyFloat(GLfloat float_data, GLint location);

        /*********************************************************

        @param  GLfloat x
        @param  GLfloat y
        @param  GLfloat z
        @param  GLint location
        @return void
        *********************************************************/
        void ApplyFloat3(GLfloat x, GLfloat y, GLfloat z, GLint location);

        /*********************************************************

        @param  const GLfloat * vector2_data_pointer
        @param  const char * uniform_var_name
        @return void
        *********************************************************/
        void ApplyVector2(const GLfloat* vector2_data_pointer, const char* uniform_var_name);

        /*********************************************************

        @param  const GLfloat * vector2_data_pointer
        @param  GLint location
        @return void
        *********************************************************/
        void ApplyVector2(const GLfloat* vector2_data_pointer, GLint location);

        /*********************************************************

        @param  const GLfloat * vector3_data_pointer
        @param  const char * uniform_var_name
        @return void
        *********************************************************/
        void ApplyVector3(const GLfloat* vector3_data_pointer, const char* uniform_var_name);

        /*********************************************************

        @param  const GLfloat * vector3_data_pointer
        @param  GLint location
        @return void
        *********************************************************/
        void ApplyVector3(const GLfloat* vector3_data_pointer, GLint location);

        /*********************************************************

        @param  const GLfloat * vector4_data_pointer
        @param  const char * uniform_var_name
        @return void
        *********************************************************/
        void ApplyVector4(const GLfloat* vector4_data_pointer, const char* uniform_var_name);

        /*********************************************************

        @param  const GLfloat * vector4_data_pointer
        @param  GLint location
        @return void
        *********************************************************/
        void ApplyVector4(const GLfloat* vector4_data_pointer, GLint location);

        /*********************************************************

        @param  GLint int_data
        @param  const char * uniform_var_name
        @return void
        *********************************************************/
        void ApplyInt(GLint int_data, const char* uniform_var_name);

        /*********************************************************

        @param  GLint int_data
        @param  GLint location
        @return void
        *********************************************************/
        void ApplyInt(GLint int_data, GLint location);

        /*********************************************************

        @param  bool bool_data
        @param  const char * uniform_var_name
        *********************************************************/
        void ApplyBoolean(bool bool_data, const char* uniform_var_name);

        /*********************************************************

        @param  bool bool_data
        @param  GLint location
        *********************************************************/
        void ApplyBoolean(bool bool_data, GLint location);

        /*********************************************************
        要使用此方法去设置shader中的材质，则shader代码务必使用material.glsl中定义的Material结构
        @param  const Material * material 待设置的材质结构体
        @param  const char * uniform_var_name uniform变量在shader代码中的名字
        *********************************************************/
        void ApplyMaterial(const Material* material, const char* uniform_var_name);

        /*********************************************************

        @param  const Material * material
        @param  const MaterialUniformLocation & location
        @return void
        *********************************************************/
        void ApplyMaterial(const Material* material, const MaterialUniformLocation& location);

        /*********************************************************
        获取到给定的材质变量所对应的uniform location
        @param  const char * uniform_var_name
        @return kgl::MaterialUniformLocation
        *********************************************************/
        MaterialUniformLocation GetMaterialUniformLocation(const char* uniform_var_name) const;

        /*********************************************************

        @param  const DirectionalLight * light
        @param  const char * uniform_var_name
        @return void
        *********************************************************/
        void ApplyDirectionalLight(const DirectionalLight* light, const char* uniform_var_name);

        /*********************************************************

        @param  const DirectionalLight * light
        @param  const DirLightUniformLocation & location
        @return void
        *********************************************************/
        void ApplyDirectionalLight(const DirectionalLight* light, const DirLightUniformLocation& location);

        /*********************************************************

        @param  const char * uniform_var_name
        @return kgl::DirLightUniformLocation
        *********************************************************/
        DirLightUniformLocation GetDirLightUniformLocation(const char* uniform_var_name) const;

        /*********************************************************

        @param  const PointLight * light
        @param  const char * uniform_var_name
        @return void
        *********************************************************/
        void ApplyPointLight(const PointLight* light, const char* uniform_var_name);

        /*********************************************************

        @param  const PointLight * light
        @param  const PointLightUniformLocation & location
        @return void
        *********************************************************/
        void ApplyPointLight(const PointLight* light, const PointLightUniformLocation& location);

        /*********************************************************

        @param  const char * uniform_var_name
        @return kgl::PointLightUniformLocation
        *********************************************************/
        PointLightUniformLocation GetPointLightUniformLocation(const char* uniform_var_name) const;

        /*********************************************************

        @param  const SpotLight * light
        @param  const char * uniform_var_name
        @return void
        *********************************************************/
        void ApplySpotLight(const SpotLight* light, const char* uniform_var_name);

        /*********************************************************

        @param  const SpotLight * light
        @param  const SpotLightUniformLocation & location
        @return void
        *********************************************************/
        void ApplySpotLight(const SpotLight* light, const SpotLightUniformLocation& location);

        /*********************************************************

        @param  const char * uniform_var_name
        @return kgl::SpotLightUniformLocation
        *********************************************************/
        SpotLightUniformLocation GetSpotLightUniformLocation(const char* uniform_var_name) const;

        /*********************************************************
        根据指定的uniform变量名，获取到该变量对应的locaton值
        @param  const char * uniform_var_name uniform变量名
        @return GLint 该变量对应的locaton值
        *********************************************************/
        GLint GetUniformLocation(const char* uniform_var_name) const;
    protected:
        /*********************************************************
         从shader代码字符串中装载并编译（仅编译，未链接）着色器，成功的话返回着色器handle
        @param  const char* shader_file_path  shader代码文件名
        @param  GLuint shader_type 着色器类型，取值为GL_VERTEX_SHADER，GL_FRAGMENT_SHADER，GL_GEOMETRY_SHADER的其中一种
        @return GLuint 着色器handle，失败的话返回-1
        *********************************************************/
        GLuint LoadAndCompileShader(const char* shader_file_path, GLuint shader_type);

        /*********************************************************
        把一个shader文件载入到一个字符串中
        @param  const char * shader_file_path
        @return std::string
        *********************************************************/
        std::string LoadShaderFileToString(const char* shader_file_path);
    protected:
        /// <summary>
        /// shader代码对象的句柄
        /// </summary>
        GLuint program_handle_;
    };

    typedef std::shared_ptr<GPUProgram> GPUProgramSPtr;
}
#endif // kgl_gpu_program_h__