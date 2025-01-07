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
#include "kgl_lib_pch.h"
#include "kgl_defines.h"
#include "kgl_string_convertor.h"
#include "kgl_compute_shader_program.h"

namespace kgl
{
    ComputeShaderProgram::ComputeShaderProgram() :program_(0)
    {

    }

    ComputeShaderProgram::~ComputeShaderProgram()
    {
        Destroy();
    }

    void ComputeShaderProgram::Use()
    {
        if (program_ != 0)
        {
            //GL_CHECK_SIMPLE(glUseProgram(program_));
            glUseProgram(program_);
        }
    }

    void ComputeShaderProgram::Destroy()
    {
        if (program_ != 0)
        {
            GL_CHECK_SIMPLE(glDeleteProgram(program_));
            program_ = 0;
        }
    }

    void ComputeShaderProgram::CreateFromSingleFile(const char* cs_source)
    {
        std::string shader_src_code;

        std::ifstream cs_file_stream; // 打开文件，读入代码到流中，然后载入到内存，提交编译
        cs_file_stream.open(cs_source);

        if (!cs_file_stream.is_open())
        {
            std::wstringstream wss;
            wss << L"Can not open shader file named " << StringConvertor::ANSItoUTF16LE(cs_source);
            throw Error(wss.str(), __FILE__, __LINE__);
        }

        std::stringstream cs_string_stream;
        cs_string_stream << cs_file_stream.rdbuf();
        cs_file_stream.close();
        shader_src_code = cs_string_stream.str();

       

        GLuint cs = common_compile(GL_COMPUTE_SHADER, shader_src_code.c_str());

        if (cs == 0)
        {
            //GL_CHECK_SIMPLE(glDeleteProgram(program_));
            return;
        }

        GL_CHECK_SIMPLE(program_ = glCreateProgram());

        // 绑定compute shader句柄和gpu program句柄
        GL_CHECK_SIMPLE(glAttachShader(program_, cs));

        GL_CHECK_SIMPLE(glLinkProgram(program_));

        GL_CHECK_SIMPLE(glDeleteShader(cs)); // 连接成功后可以删除shader本身

        if (!check_program(program_))
        {
            throw Error(L"Failed to link program");
        }
    }

    GLuint ComputeShaderProgram::common_compile(GLenum type, const char* source)
    {
        GL_CHECK_SIMPLE(GLuint shader = glCreateShader(type));
        GL_CHECK_SIMPLE(glShaderSource(shader, 1, &source, NULL));
        GL_CHECK_SIMPLE(glCompileShader(shader));

        GLint status;
        GL_CHECK_SIMPLE(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));

        if (status == GL_FALSE)
        {
            GLint len;
            GLsizei out_len;

            GL_CHECK_SIMPLE(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len));
            std::vector<char> buf(len);
            GL_CHECK_SIMPLE(glGetShaderInfoLog(shader, len, &out_len, &buf[0]));
            //LOGI("Shader log:\n%s", &buf[0]);

            GL_CHECK_SIMPLE(glDeleteShader(shader));
            return 0;
        }

        return shader;
    }

    bool ComputeShaderProgram::check_program(GLuint prog)
    {
        GLint status;
        GL_CHECK_SIMPLE(glGetProgramiv(prog, GL_LINK_STATUS, &status));

        if (status == GL_FALSE)
        {
            GLint len;
            GLsizei out_len;

            GL_CHECK_SIMPLE(glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len));

            std::vector<char> buf(len);

            GL_CHECK_SIMPLE(glGetProgramInfoLog(prog, len, &out_len, &buf[0]));
            // LOGI("Program log:\n%s", &buf[0]);

            GL_CHECK_SIMPLE(glDeleteProgram(prog));
            return false;
        }

        return true;
    }

    void ComputeShaderProgram::ApplyUint(GLuint uint_data, GLint location)
    {
        GL_CHECK_SIMPLE(glProgramUniform1ui(program_, location, uint_data));
    }

    void ComputeShaderProgram::ApplyFloat(GLfloat float_data, GLint location)
    {
        GL_CHECK_SIMPLE(glProgramUniform1f(program_, location, float_data));
    }

    void ComputeShaderProgram::Dispatch(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z)
    {
        GL_CHECK_SIMPLE(glDispatchCompute(num_groups_x, num_groups_y, num_groups_z));
    }
}