﻿/**************************************************************************************************************************
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
#include "kgl_lib_pch.h"
#include "kgl_gpu_program.h"
#include "kgl_error.h"
#include "kgl_string_convertor.h"
#include "kgl_defines.h"

namespace kgl
{
    GPUProgram::GPUProgram()
    {
    }

    void GPUProgram::CreateFromFile(const std::vector<std::string>& vs_file_paths, const std::vector<std::string>& fs_file_paths, const std::vector<std::string>& gs_file_path,
        uint32_t main_shader_version , uint32_t minor_shader_version, uint32_t tail_shader_version)
    {
        boost::format f("#version %d%d%d core\n");
        f % main_shader_version % minor_shader_version %tail_shader_version;
        std::string version_string = f.str();
        
        std::string shader_src_code;

        std::size_t sz = vs_file_paths.size();
        if (sz > 0)
            shader_src_code.append(version_string);

        for (std::size_t t = 0; t < sz; ++t)
        {
            // 打开文件，读入代码到流中，然后载入到内存，提交编译
            std::ifstream vs_file_stream;
            vs_file_stream.open(vs_file_paths[t]);

            if (!vs_file_stream.is_open())
            {
                std::wstringstream wss;
                wss << L"Can not open shader file named " << StringConvertor::ANSItoUTF16LE(vs_file_paths[t].c_str());
                throw Error(wss.str(), __FILE__, __LINE__);
            }

            std::stringstream vs_string_stream;
            vs_string_stream << vs_file_stream.rdbuf();
            vs_file_stream.close();

            shader_src_code.append(vs_string_stream.str());
            shader_src_code.append("\n"); // 回车换行
        }

        GLint success = 0;
        GLuint vs_handle = 0;
        GLchar info_log[512];
        const char* shader_src_code_pointer = shader_src_code.c_str();

        vs_handle = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs_handle, 1, &(shader_src_code_pointer), NULL);
        glCompileShader(vs_handle);
        glGetShaderiv(vs_handle, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(vs_handle, 512, NULL, info_log);
            throw Error(StringConvertor::ANSItoUTF16LE(info_log), __FILE__, __LINE__);
        }
        
// ================================================================
        shader_src_code.clear();
        sz = fs_file_paths.size();

        if (sz > 0)
            shader_src_code.append(version_string);

        for (std::size_t t = 0; t < sz; ++t)
        {
            // 打开文件，读入代码到流中，然后载入到内存，提交编译
            std::ifstream fs_file_stream;
            fs_file_stream.open(fs_file_paths[t]);

            if (!fs_file_stream.is_open())
            {
                std::wstringstream wss;
                wss << L"Can not open shader file named " << StringConvertor::ANSItoUTF16LE(fs_file_paths[t].c_str());
                throw Error(wss.str(), __FILE__, __LINE__);
            }

            std::stringstream fs_string_stream;
            fs_string_stream << fs_file_stream.rdbuf();
            fs_file_stream.close();

            shader_src_code.append(fs_string_stream.str());
            shader_src_code.append("\n"); // 回车换行
        }

        GLuint fs_handle = 0;
        shader_src_code_pointer = shader_src_code.c_str();

        fs_handle = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs_handle, 1, &(shader_src_code_pointer), NULL);
        glCompileShader(fs_handle);
        glGetShaderiv(fs_handle, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(fs_handle, 512, NULL, info_log);
            throw Error(StringConvertor::ANSItoUTF16LE(info_log), __FILE__, __LINE__);
        }

// ================================================================

        // 链接shader代码
        // GPUProgram program_handle_
        this->program_handle_ = glCreateProgram();

        if (vs_file_paths.size() > 0)
            glAttachShader(this->program_handle_, vs_handle);
        if (fs_file_paths.size() > 0)
            glAttachShader(this->program_handle_, fs_handle);
        
        glLinkProgram(this->program_handle_);
        glGetProgramiv(this->program_handle_, GL_LINK_STATUS, &success);

        if (!success)
        {
            glGetProgramInfoLog(this->program_handle_, 512, NULL, info_log);
            throw Error(StringConvertor::ANSItoUTF16LE(info_log), __FILE__, __LINE__);
        }

        if (vs_file_paths.size() > 0)
            glDeleteShader(vs_handle);
        if (fs_file_paths.size() > 0)
            glDeleteShader(fs_handle);
    }

    std::string GPUProgram::LoadShaderFileToString(const char* shader_file_path)
    {
        // 打开文件，读入代码到流中，然后载入到内存，提交编译
        std::ifstream file_stream;
        file_stream.open(shader_file_path);

        if (!file_stream.is_open())
        {
            std::wstringstream wss;
            wss << L"Can not open shader file named " << StringConvertor::ANSItoUTF16LE(shader_file_path);
            throw Error(wss.str(), __FILE__, __LINE__);
        }

        std::stringstream shader_string_stream;
        shader_string_stream << file_stream.rdbuf();
        file_stream.close();
        return shader_string_stream.str();
    }

    GLuint GPUProgram::LoadAndCompileShader(const char* shader_file_path, GLuint shader_type)
    {
        std::string shader_code = LoadShaderFileToString(shader_file_path);
        const char* shader_string = shader_code.c_str();
        GLchar info_log[512];
        GLint is_success = 0; // 编译成功的话，返回0
        GLuint shader_handle = glCreateShader(shader_type);
        glShaderSource(shader_handle, 1, &(shader_string), nullptr);
        glCompileShader(shader_handle);
        glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &is_success);

        if (!is_success)
        {
            std::wstring log = StringConvertor::ANSItoUTF16LE(shader_file_path);
            glGetShaderInfoLog(shader_handle, 512, NULL, info_log);
            log.append(L"\n");
            log.append(StringConvertor::ANSItoUTF16LE(info_log));
            throw Error(log, __FILE__, __LINE__);
            return -1;
        }

        return shader_handle;
    }

    void GPUProgram::CreateFromFile(const GLchar* vs_file_path, const GLchar* fs_file_path, const GLchar* gs_file_path)
    {
        GLuint vs_handle = 0 , fs_handle = 0 , gs_handle = 0;
        GLint success;
        GLchar info_log[512];
        memset(info_log, 0, sizeof(GLchar) * 512);

        if (nullptr != vs_file_path)
        {
            vs_handle = LoadAndCompileShader(vs_file_path, GL_VERTEX_SHADER);
        }

        if (nullptr != fs_file_path)
        {
            fs_handle = LoadAndCompileShader(fs_file_path, GL_FRAGMENT_SHADER);
        }

        if (nullptr != gs_file_path)
        {
            gs_handle = LoadAndCompileShader(gs_file_path, GL_GEOMETRY_SHADER);
        }

        // 链接shader代码
        // GPUProgram program_handle_
        this->program_handle_ = glCreateProgram();

        if (nullptr != vs_file_path)
        {
            glAttachShader(this->program_handle_, vs_handle);
        }

        if (nullptr != fs_file_path)
        {
            glAttachShader(this->program_handle_, fs_handle);
        }

        if (nullptr != gs_file_path)
        {
            glAttachShader(this->program_handle_, gs_handle);
        }

        glLinkProgram(this->program_handle_);
        glGetProgramiv(this->program_handle_, GL_LINK_STATUS, &success);

        if (!success)
        {
            glGetProgramInfoLog(this->program_handle_, 512, NULL, info_log);
            throw Error(StringConvertor::ANSItoUTF16LE(info_log), __FILE__, __LINE__);
        }
        
        if (nullptr != vs_file_path)
        {
            glDeleteShader(vs_handle);
        }

        if (nullptr != fs_file_path)
        {
            glDeleteShader(fs_handle);
        }

        if (nullptr != gs_file_path)
        {
            glDeleteShader(gs_handle);
        }
    }

    GPUProgram::~GPUProgram()
    {
        glDeleteProgram(program_handle_);
    }

    void GPUProgram::Use() const
    {
        GL_CHECK_SIMPLE(glUseProgram(this->program_handle_));
    }
    
    void GPUProgram::Finish() const
    {
        glUseProgram(0);
    }

    // http://blog.csdn.net/racehorse/article/details/6634830
    void GPUProgram::ApplyTexture(TextureSPtr texture, const char* uniform_var_name,GLuint slot_index)
    {
        texture->ActiveBind(slot_index);
        glUniform1i(glGetUniformLocation(program_handle_, uniform_var_name), slot_index);
    }

    void GPUProgram::ApplyTexture(TextureSPtr texture, GLint location, GLuint slot_index)
    {
        texture->ActiveBind(slot_index);
        glUniform1i(location, slot_index);
    }

    void GPUProgram::ApplyTexture(GLint location, GLuint slot_index, GLuint texture_id, GLenum target)
    {
        GLuint i = glm::clamp(slot_index, static_cast<GLuint>(0), static_cast<GLuint>(8));
        GLenum texture_unit = GL_TEXTURE0 + i;
        glActiveTexture(texture_unit);
        glBindTexture(target, texture_id);
        glUniform1i(location, slot_index);
    }

    void GPUProgram::ApplyTexture(const char* uniform_var_name, GLuint texture_unit_slot, GLuint texture_id, GLenum target)
    {
        GLuint location = glGetUniformLocation(program_handle_, uniform_var_name);
        this->ApplyTexture(location, texture_unit_slot, texture_id, target);
    }

    void GPUProgram::ApplyTexture(const char* uniform_var_name, GLuint slot_index)
    {
        glUniform1i(glGetUniformLocation(program_handle_, uniform_var_name), slot_index);
    }

    void GPUProgram::ApplyTexture(GLint location, GLuint slot_index)
    {
        glUniform1i(location, slot_index);
    }

    void GPUProgram::ApplyMatrix(const GLfloat* matrix_data_pointer, const char* uniform_var_name,bool need_transpose)
    {
        GLint location = glGetUniformLocation(program_handle_, uniform_var_name);
        
        /*
        glUniformMatrix2(34)fv函数定义的可以加载4x4矩阵数据，还可以加载像2x2,3x3,3x2,2x4,4x2,3x4和4x3这样的矩阵。
        
        第一个参数是uniform变量的位置。在shader编译后使用glGetUniformLocation()获取；
        
        第二个参数指的是我们要更新的矩阵的个数，我们使用参数1来更新一个矩阵，但我们也可以使用这个函数在一次回调中更新多个矩阵
        
        第三个参数通常会使新手误解，第三个参数指的是矩阵是行优先还是列优先的。行优先指的是矩阵是从顶部开始一行一行给出的，而列
        优先是从左边一列一列给出的。C/C++中数组默认是行优先的。也就是说当构建一个二维数组时，在内存中是一行一行存储的，顶部的
        行在更低的地址区。

        OpenGL开发者通常使用一种内部矩阵布局，叫做列优先矩阵(Column-major Ordering)布局。GLM的默认布局
        就是列主序，所以并不需要置换矩阵，我们填GL_FALSE。最后一个参数是真正的矩阵数据首指针，但是GLM并不是
        把它们的矩阵储存为OpenGL所希望接受的那种形式，例如我们构造一个在xyz方向上分别平移2，3，4个单位的4x4
        平移矩阵。如果按照列优先的写法，那么这个矩阵应该是：

        | 1 0 0 2 |
        | 0 1 0 3 |
        | 0 0 1 4 |
        | 0 0 0 1 |
        
        但是GLM的matrix4x4的代码实现，并不是使用一个横4纵4的二维数组去存储数据，而是采用了“有4个列优先
        4维向量”的“数组”，假如这个数组称为vecs，也就是说，上面的矩阵：
        
        第1列：（1 0 0 0）存储在vecs[0]所对应的那个4维向量中。
        第2列：（0 1 0 0）存储在vecs[1]所对应的那个4维向量中。
        第3列：（0 0 1 0）存储在vecs[2]所对应的那个4维向量中。
        第4列：（2 3 4 1）存储在vecs[3]所对应的那个4维向量中。

        4维向量重载了[]，使用[0][1][2][3]依次取得从左到右的4维向量的4个分量，所以，如果我们从
        利用数组的索引值，以下的形式：
        
        [0][0],[0][1],[0][2],[0][3]
        [1][0],[1][1],[1][2],[1][3]
        [2][0],[2][1],[2][2],[2][3]
        [3][0],[3][1],[3][2],[3][3]

        打印出数据来的话，就会发现，GLM的矩阵，实质上是列优先的逻辑，但是却看起来像行优先的存储方式
        这也就是为什么要使用glm::value_ptr函数去处理GLM矩阵以获取数据首指针的缘故   
        */
        glUniformMatrix4fv(location, 1, need_transpose ? GL_TRUE : GL_FALSE, matrix_data_pointer);
    }

    void GPUProgram::ApplyMatrix(const GLfloat* matrix_data_pointer, GLint location, bool need_transpose)
    {
        glUniformMatrix4fv(location, 1, need_transpose ? GL_TRUE : GL_FALSE, matrix_data_pointer);
    }

    void GPUProgram::ApplyFloat(GLfloat float_data, const char* uniform_var_name)
    {
        GLint location = glGetUniformLocation(program_handle_, uniform_var_name);
        glUniform1f(location, float_data);
    }

    void GPUProgram::ApplyFloat(GLfloat float_data, GLint location)
    {
        glUniform1f(location, float_data);
    }

    void GPUProgram::ApplyFloat3(GLfloat x, GLfloat y, GLfloat z, GLint location)
    {
        glProgramUniform3f(program_handle_, location, x, y, z);
    }

    void GPUProgram::ApplyInt(GLint int_data, GLint location)
    {
        glUniform1i(location, int_data);
    }

    void GPUProgram::ApplyInt(GLint int_data, const char* uniform_var_name)
    {
        GLint location = glGetUniformLocation(program_handle_, uniform_var_name);
        glUniform1i(location, int_data);
    }

    void GPUProgram::ApplyBoolean(bool bool_data, const char* uniform_var_name)
    {
        GLint location = glGetUniformLocation(program_handle_, uniform_var_name);
        glUniform1i(location, bool_data ? 1 : 0);
    }

    void GPUProgram::ApplyBoolean(bool bool_data, GLint location)
    {
        glUniform1i(location, bool_data ? 1 : 0);
    }

    void GPUProgram::ApplyVector2(const GLfloat* vector2_data_pointer, const char* uniform_var_name)
    {
        GLint location = glGetUniformLocation(program_handle_, uniform_var_name);
        glUniform2fv(location, 1,vector2_data_pointer);
    }

    void GPUProgram::ApplyVector2(const GLfloat* vector2_data_pointer, GLint location)
    {
        glUniform2fv(location, 1, vector2_data_pointer);
    }

    void GPUProgram::ApplyVector3(const GLfloat* vector3_data_pointer, const char* uniform_var_name)
    {
        GLint location = glGetUniformLocation(program_handle_, uniform_var_name);
        glUniform3fv(location, 1, vector3_data_pointer);
    }

    void GPUProgram::ApplyVector3(const GLfloat* vector3_data_pointer, GLint location)
    {
        glUniform3fv(location, 1, vector3_data_pointer);
    }

    void GPUProgram::ApplyVector4(const GLfloat* vector4_data_pointer, GLint location)
    {
        glUniform4fv(location, 1, vector4_data_pointer);
    }

    void GPUProgram::ApplyVector4(const GLfloat* vector4_data_pointer, const char* uniform_var_name)
    {
        GLint location = glGetUniformLocation(program_handle_, uniform_var_name);
        glUniform4fv(location, 1, vector4_data_pointer);
    }

    void GPUProgram::ApplyMaterial(const Material* material, const char* uniform_var_name)
    {
        std::string material_var_name(uniform_var_name);
        glUniform3f(glGetUniformLocation(program_handle_, (material_var_name + ".Ambient").c_str()), material->Ambient.r, material->Ambient.g, material->Ambient.b);
        glUniform3f(glGetUniformLocation(program_handle_, (material_var_name + ".Diffuse").c_str()), material->Diffuse.r, material->Diffuse.g, material->Diffuse.b);
        glUniform3f(glGetUniformLocation(program_handle_, (material_var_name + ".Specular").c_str()), material->Specular.r, material->Specular.g, material->Specular.b);
        glUniform1f(glGetUniformLocation(program_handle_, (material_var_name + ".Shininess").c_str()),material->Shininess);
    }

    void GPUProgram::ApplyMaterial(const Material* material, const MaterialUniformLocation& location)
    {
        glUniform3f(location.Ambient, material->Ambient.r, material->Ambient.g, material->Ambient.b);
        glUniform3f(location.Diffuse, material->Diffuse.r, material->Diffuse.g, material->Diffuse.b);
        glUniform3f(location.Specular, material->Specular.r, material->Specular.g, material->Specular.b);
        glUniform1f(location.Shininess, material->Shininess);
    }

    MaterialUniformLocation GPUProgram::GetMaterialUniformLocation(const char* uniform_var_name) const
    {
        MaterialUniformLocation location;
        std::string material_var_name(uniform_var_name);
        location.Ambient = GetUniformLocation((material_var_name + ".Ambient").c_str());
        location.Diffuse = GetUniformLocation((material_var_name + ".Diffuse").c_str());
        location.Specular = GetUniformLocation( (material_var_name + ".Specular").c_str());
        location.Shininess = GetUniformLocation((material_var_name + ".Shininess").c_str());
        return location;
    }

    void GPUProgram::ApplyDirectionalLight(const DirectionalLight* light, const char* uniform_var_name)
    {
        std::string light_var_name(uniform_var_name);
        glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Ambient").c_str()), light->Ambient.r, light->Ambient.g, light->Ambient.b);
        glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Diffuse").c_str()), light->Diffuse.r, light->Diffuse.g, light->Diffuse.b);
        glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Specular").c_str()), light->Specular.r, light->Specular.g, light->Specular.b);
        glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Direction").c_str()), light->Direction.x, light->Direction.y, light->Direction.z);
    }

    void ApplyDirectionalLight(const DirectionalLight* light, const DirLightUniformLocation& location)
    {
        glUniform3f(location.Ambient, light->Ambient.r, light->Ambient.g, light->Ambient.b);
        glUniform3f(location.Diffuse, light->Diffuse.r, light->Diffuse.g, light->Diffuse.b);
        glUniform3f(location.Specular, light->Specular.r, light->Specular.g, light->Specular.b);
        glUniform3f(location.Direction, light->Direction.x, light->Direction.y, light->Direction.z);
    }

    DirLightUniformLocation GPUProgram::GetDirLightUniformLocation(const char* uniform_var_name) const
    {
        std::string light_var_name(uniform_var_name);
        DirLightUniformLocation location;
        location.Ambient = GetUniformLocation((light_var_name + ".Ambient").c_str());
        location.Diffuse = GetUniformLocation((light_var_name + ".Diffuse").c_str());
        location.Specular = GetUniformLocation((light_var_name + ".Specular").c_str());
        location.Direction = GetUniformLocation((light_var_name + ".Direction").c_str());
        return location;
    }

    PointLightUniformLocation GPUProgram::GetPointLightUniformLocation(const char* uniform_var_name) const
    {
        std::string light_var_name(uniform_var_name);
        PointLightUniformLocation location;
        location.Ambient = GetUniformLocation((light_var_name + ".Ambient").c_str());
        location.Diffuse = GetUniformLocation((light_var_name + ".Diffuse").c_str());
        location.Specular = GetUniformLocation((light_var_name + ".Specular").c_str());
        location.Position = GetUniformLocation((light_var_name + ".Position").c_str());
        location.AttenuationConstant = GetUniformLocation((light_var_name + ".AttenuationConstant").c_str());
        location.AttenuationLinear = GetUniformLocation((light_var_name + ".AttenuationLinear").c_str());
        location.AttenuationExp = GetUniformLocation((light_var_name + ".AttenuationExp").c_str());
        return location;
    }

    void GPUProgram::ApplyPointLight(const PointLight* light, const char* uniform_var_name)
    {
        std::string light_var_name(uniform_var_name);
        glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Ambient").c_str()), light->Ambient.r, light->Ambient.g, light->Ambient.b);
        glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Diffuse").c_str()), light->Diffuse.r, light->Diffuse.g, light->Diffuse.b);
        glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Specular").c_str()), light->Specular.r, light->Specular.g, light->Specular.b);
        glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Position").c_str()), light->Position.x, light->Position.y, light->Position.z);
        glUniform1f(glGetUniformLocation(program_handle_, (light_var_name + ".AttenuationConstant").c_str()), light->AttenuationConstant);
        glUniform1f(glGetUniformLocation(program_handle_, (light_var_name + ".AttenuationLinear").c_str()), light->AttenuationLinear);
        glUniform1f(glGetUniformLocation(program_handle_, (light_var_name + ".AttenuationExp").c_str()), light->AttenuationExp);
    }

    void GPUProgram::ApplyPointLight(const PointLight* light, const PointLightUniformLocation& location)
    {
        glUniform3f(location.Ambient, light->Ambient.r, light->Ambient.g, light->Ambient.b);
        glUniform3f(location.Diffuse, light->Diffuse.r, light->Diffuse.g, light->Diffuse.b);
        glUniform3f(location.Specular, light->Specular.r, light->Specular.g, light->Specular.b);
        glUniform3f(location.Position, light->Position.x, light->Position.y, light->Position.z);
        glUniform1f(location.AttenuationConstant, light->AttenuationConstant);
        glUniform1f(location.AttenuationLinear, light->AttenuationLinear);
        glUniform1f(location.AttenuationExp, light->AttenuationExp);
    }

    void GPUProgram::ApplySpotLight(const SpotLight* light, const char* uniform_var_name)
    {
        std::string light_var_name(uniform_var_name);
        glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Ambient").c_str()), light->Ambient.r, light->Ambient.g, light->Ambient.b);
        glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Diffuse").c_str()), light->Diffuse.r, light->Diffuse.g, light->Diffuse.b);
        glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Specular").c_str()), light->Specular.r, light->Specular.g, light->Specular.b);
        glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Position").c_str()), light->Position.x, light->Position.y, light->Position.z);
        glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Direction").c_str()), light->Direction.x, light->Direction.y, light->Direction.z);
        glUniform4f(glGetUniformLocation(program_handle_, (light_var_name + ".CutOffAndAttenuation").c_str()), light->CutOffAndAttenuation.x, light->CutOffAndAttenuation.y, light->CutOffAndAttenuation.z, light->CutOffAndAttenuation.w);
        glUniform1f(glGetUniformLocation(program_handle_, (light_var_name + ".Exponent").c_str()), light->Exponent);
    }

    void GPUProgram::ApplySpotLight(const SpotLight* light, const SpotLightUniformLocation& location)
    {
        glUniform3f(location.Ambient, light->Ambient.r, light->Ambient.g, light->Ambient.b);
        glUniform3f(location.Diffuse, light->Diffuse.r, light->Diffuse.g, light->Diffuse.b);
        glUniform3f(location.Specular, light->Specular.r, light->Specular.g, light->Specular.b);
        glUniform3f(location.Position, light->Position.x, light->Position.y, light->Position.z);
        glUniform3f(location.Direction, light->Direction.x, light->Direction.y, light->Direction.z);
        glUniform4f(location.CutOffAndAttenuation, light->CutOffAndAttenuation.x, light->CutOffAndAttenuation.y, light->CutOffAndAttenuation.z, light->CutOffAndAttenuation.w);
        glUniform1f(location.Exponent, light->Exponent);
    }

    SpotLightUniformLocation GPUProgram::GetSpotLightUniformLocation(const char* uniform_var_name) const
    {
        std::string light_var_name(uniform_var_name);
        SpotLightUniformLocation location;
        location.Ambient = GetUniformLocation((light_var_name + ".Ambient").c_str());
        location.Diffuse = GetUniformLocation((light_var_name + ".Diffuse").c_str());
        location.Specular = GetUniformLocation((light_var_name + ".Specular").c_str());
        location.Position = GetUniformLocation((light_var_name + ".Position").c_str());
        location.Direction = GetUniformLocation((light_var_name + ".Direction").c_str());
        location.CutOffAndAttenuation = GetUniformLocation((light_var_name + ".CutOffAndAttenuation").c_str());
        location.Exponent = GetUniformLocation((light_var_name + ".Exponent").c_str());
        return location;
    }

    GLint GPUProgram::GetUniformLocation(const char* uniform_var_name) const
    {
        GLint location = glGetUniformLocation(program_handle_, uniform_var_name);

        if (-1 == location)
        {
            GLenum error_code = glGetError();

            if (GL_INVALID_VALUE == error_code)
            {
                std::wstringstream wss;
                wss << L"Error! GetUniformLocaiton by name: \n"
                    << StringConvertor::ANSItoUTF16LE(uniform_var_name) << std::endl
                    << L" is failed!\n\n"
                    << L"Error code: GL_INVALID_VALUE\n"
                    << L"It means the SHADER PROGRAME OBJECT whose ID = "
                    << program_handle_
                    << L" is not a value generated by OpenGL!";
                throw Error(wss.str(), __FILE__, __LINE__);
            }
            else if (GL_INVALID_OPERATION == location)
            {
                std::wstringstream wss;
                wss << L"Error! GetUniformLocaiton by name: \n"
                    << StringConvertor::ANSItoUTF16LE(uniform_var_name) << std::endl
                    << L" is failed!\n\n"
                    << L"Error code: GL_INVALID_OPERATION\n"
                    << L"It means the SHADER PROGRAME OBJECT whose ID = "
                    << program_handle_
                    << L" is not a is not a program object OR has not been successfully linked!";
                throw Error(wss.str(), __FILE__, __LINE__);
            }
        }
        
        return location;
    }

    void GPUProgram::CreateTransformFeedbackShaderFromFile(const GLchar* vs_file_path, const GLchar* gs_file_path, const std::vector<const char*>& varings)
    {
        if (vs_file_path == nullptr || gs_file_path == nullptr)
        {

        }

        GLint success;
        GLchar info_log[512];
        GLuint vs_handle = LoadAndCompileShader(LoadShaderFileToString(vs_file_path).c_str(), GL_VERTEX_SHADER);
        GLuint gs_handle = LoadAndCompileShader(LoadShaderFileToString(gs_file_path).c_str(), GL_GEOMETRY_SHADER);

        // 创建程序
        this->program_handle_ = glCreateProgram();
        glAttachShader(this->program_handle_, vs_handle);
        glAttachShader(this->program_handle_, gs_handle);

        for (std::size_t i = 0; i < varings.size(); ++i)
        {
            /*第一个参数对应shader程序的program id，
            第二个参数指示有多少个feed back变量需要与feed back对象引出接口
            第三个参数varyings则向函数提供需要引出接口的变量的名称，
            第四个阐述最后一个参数用于管理变量存在于feed back缓存中的模式，一种是多个变量在feed back缓存中交替出现，另一种是为每一个变量关联一个对应的feed back缓存对象。
            需要注意的是feed back与program是对应的，当发生program切换的同时伴随feed back队列的变换。！*/
            glTransformFeedbackVaryings(program_handle_, varings.size(), varings.data(), GL_INTERLEAVED_ATTRIBS);
        }

        glLinkProgram(this->program_handle_);
        glGetProgramiv(this->program_handle_, GL_LINK_STATUS, &success);

        if (!success)
        {
            glGetProgramInfoLog(this->program_handle_, 512, NULL, info_log);
            throw Error(StringConvertor::ANSItoUTF16LE(info_log), __FILE__, __LINE__);
        }

        glDeleteShader(vs_handle);
        glDeleteShader(gs_handle);
    }
}