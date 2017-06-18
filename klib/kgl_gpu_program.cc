// shader类

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

    // Constructor generates the shader on the fly
    void GPUProgram::CreateFromFile(const GLchar* vs_file_path, const GLchar* fs_file_path, const GLchar* gs_file_path)
    {
        std::string shader_code_string;
        const GLchar* shader_code_cstr = nullptr;
        GLuint vs_handle_, fs_handle;
        GLint success;
        GLchar info_log[512];

        if (nullptr != vs_file_path)
        {
            shader_code_string.clear();

            // 打开文件，读入代码到流中，然后载入到内存，提交编译
            std::ifstream vs_file_stream;
            vs_file_stream.open(vs_file_path);
            
            if (!vs_file_stream.is_open())
            {
                std::wstringstream wss;
                wss << L"Can not open shader file named " << StringConvertor::ANSItoUTF16LE(vs_file_path);
                throw Error(wss.str(), __FILE__, __LINE__);
            }

            std::stringstream vs_string_stream;
            vs_string_stream << vs_file_stream.rdbuf();
            vs_file_stream.close();

            shader_code_string = vs_string_stream.str();
            shader_code_cstr = shader_code_string.c_str();

            vs_handle_ = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vs_handle_, 1, &shader_code_cstr, NULL);
            glCompileShader(vs_handle_);
            glGetShaderiv(vs_handle_, GL_COMPILE_STATUS, &success);

            if (!success)
            {
                glGetShaderInfoLog(vs_handle_, 512, NULL, info_log);
                throw Error(StringConvertor::ANSItoUTF16LE(info_log), __FILE__, __LINE__);
            }

        }

        if (nullptr != fs_file_path)
        {
            shader_code_string.clear();

            std::ifstream fs_file_stream;
            fs_file_stream.open(fs_file_path);

            if (!fs_file_stream.is_open())
            {
                std::wstringstream wss;
                wss << L"Can not open shader file named " << StringConvertor::ANSItoUTF16LE(fs_file_path);
                throw Error(wss.str(), __FILE__, __LINE__);
            }

            std::stringstream fs_string_stream;
            fs_string_stream << fs_file_stream.rdbuf();
            
            fs_file_stream.close();

            shader_code_string = fs_string_stream.str();
            shader_code_cstr = shader_code_string.c_str();

            fs_handle = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fs_handle, 1, &shader_code_cstr, NULL);
            glCompileShader(fs_handle);
            glGetShaderiv(fs_handle, GL_COMPILE_STATUS, &success);

            if (!success)
            {
                glGetShaderInfoLog(fs_handle, 512, NULL, info_log);
                throw Error(StringConvertor::ANSItoUTF16LE(info_log), __FILE__, __LINE__);
            }
        }

        if (nullptr != gs_file_path)
        {

        }

        // 链接shader代码
        // GPUProgram program_handle_
        this->program_handle_ = glCreateProgram();

        if (nullptr != vs_file_path)
        {
            glAttachShader(this->program_handle_, vs_handle_);
        }

        if (nullptr != fs_file_path)
        {
            glAttachShader(this->program_handle_, fs_handle);
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
            glDeleteShader(vs_handle_);
        }

        if (nullptr != fs_file_path)
        {
            glDeleteShader(fs_handle);
        }

        if (nullptr != gs_file_path)
        {
            
        }
    }

    GPUProgram::~GPUProgram()
    {

    }

    void GPUProgram::Use() const
    {
        glUseProgram(this->program_handle_);
    }
    
    // http://blog.csdn.net/racehorse/article/details/6634830
    void GPUProgram::ApplyTexture(TextureSPtr texture, const char* uniform_var_name,GLuint slot_index)
    {
        texture->ActiveBind(slot_index);
        glUniform1i(glGetUniformLocation(program_handle_, uniform_var_name), slot_index);
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

    void GPUProgram::ApplyFloat(GLfloat float_data, const char* uniform_var_name)
    {
        GLint location = glGetUniformLocation(program_handle_, uniform_var_name);
        glUniform1f(location, float_data);
    }

    void GPUProgram::ApplyInt(GLint int_data, const char* uniform_var_name)
    {
        GLint location = glGetUniformLocation(program_handle_, uniform_var_name);
        glUniform1i(location, int_data);
    }

    void GPUProgram::ApplyVector2(const GLfloat* vector2_data_pointer, const char* uniform_var_name)
    {
        GLint location = glGetUniformLocation(program_handle_, uniform_var_name);
        glUniform2fv(location, 1,vector2_data_pointer);
    }

    void GPUProgram::ApplyVector3(const GLfloat* vector3_data_pointer, const char* uniform_var_name)
    {
        GLint location = glGetUniformLocation(program_handle_, uniform_var_name);
        glUniform3fv(location, 1, vector3_data_pointer);
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

    void GPUProgram::ApplyDirectionalLight(const DirectionalLight* light, const char* uniform_var_name)
    {
        std::string light_var_name(uniform_var_name);
        glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Ambient").c_str()), light->Ambient.r, light->Ambient.g, light->Ambient.b);
        glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Diffuse").c_str()), light->Diffuse.r, light->Diffuse.g, light->Diffuse.b);
        glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Specular").c_str()), light->Specular.r, light->Specular.g, light->Specular.b);
        // glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Shininess").c_str()), light->Position.x, light->Position.y, light->Position.z);
        glUniform3f(glGetUniformLocation(program_handle_, (light_var_name + ".Direction").c_str()), light->Direction.x, light->Direction.y, light->Direction.z);
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
}