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
#ifndef kgl_compute_shader_program_h__
#define kgl_compute_shader_program_h__

namespace kgl
{
    class ComputeShaderProgram
    {
    public:
        ComputeShaderProgram();

        ~ComputeShaderProgram();

        /**************************************************************************************

        @name: kgl::ComputeShaderProgram::CreateFromSingleFile
        @return: void
        @param: const char * cs_source
        *************************************************************************************/
        void CreateFromSingleFile(const char* cs_source);

        /**************************************************************************************

        @name: kgl::ComputeShaderProgram::Use
        @return: void
        *************************************************************************************/
        void Use();

        /**************************************************************************************

        @name: kgl::ComputeShaderProgram::Destroy
        @return: void
        *************************************************************************************/
        void Destroy();

        /**************************************************************************************

        @name: kgl::ComputeShaderProgram::ApplyUint
        @return: void
        @param: GLuint uint_data
        @param: GLint location
        *************************************************************************************/
        void ApplyUint(GLuint uint_data, GLint location);

        /**************************************************************************************
        
        @name: kgl::ComputeShaderProgram::ApplyFloat
        @return: void
        @param: GLfloat float_data
        @param: GLint location
        *************************************************************************************/
        void ApplyFloat(GLfloat float_data, GLint location);

        static void Dispatch(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
    private:
        GLuint common_compile(GLenum type, const char* source);
        bool check_program(GLuint prog);
    private:
        GLuint program_;
    };

}

#endif // kgl_compute_shader_program_h__