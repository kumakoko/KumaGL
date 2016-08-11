// shader类
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
		GPUProgram();
		~GPUProgram();

		//************************************
		// Method:    Use
		// FullName:  kgl::GPUProgram::Use
		// Access:    public 
		// Returns:   void
		// Qualifier: const
		//************************************
		void Use() const;

		//************************************
		// Method:    CreateFromFile
		// FullName:  kgl::GPUProgram::CreateFromFile
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: const GLchar * vs_file_path
		// Parameter: const GLchar * fs_file_path
		// Parameter: const GLchar * gs_file_path
		//************************************
		void CreateFromFile(const GLchar* vs_file_path, const GLchar* fs_file_path, const GLchar* gs_file_path);

		//************************************
		// Method:    CreateFromFile
		// FullName:  kgl::GPUProgram::CreateFromFile
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: const std::vector<std::string> & vs_file_paths
		// Parameter: const std::vector<std::string> fs_file_paths
		// Parameter: const std::vector<std::string> & gs_file_path
		//************************************
		void CreateFromFile(const std::vector<std::string>& vs_file_paths, const std::vector<std::string> fs_file_paths, const std::vector<std::string>& gs_file_path,
			uint32_t main_shader_version = 3, uint32_t minor_shader_version = 3 , uint32_t tail_shader_version = 0);

		//************************************
		// Method:    ApplyTexture
		// FullName:  kgl::GPUProgram::ApplyTexture
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: TextureSPtr texture
		// Parameter: const char * uniform_var_name
		// Parameter: GLuint slot_index
		//************************************
		void ApplyTexture(TextureSPtr texture, const char* uniform_var_name, GLuint slot_index);

		//************************************
		// Method:    ApplyMatrix
		// FullName:  kgl::GPUProgram::ApplyMatrix
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: const GLfloat * matrix_data_pointer  存放着矩阵数据的数组首指针
		// Parameter: const char * uniform_var_name shader代码中该矩阵变量的名字
		// Parameter: bool need_transpose 传递进来的矩阵需要转置吗
		//************************************
		void ApplyMatrix(const GLfloat* matrix_data_pointer, const char* uniform_var_name, bool need_transpose = false);

		//************************************
		// Method:    ApplyFloat
		// FullName:  kgl::GPUProgram::ApplyFloat
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: GLfloat float_data
		// Parameter: const char * uniform_var_name
		//************************************
		void ApplyFloat(GLfloat float_data, const char* uniform_var_name);

		//************************************
		// Method:    ApplyVector2
		// FullName:  kgl::GPUProgram::ApplyVector2
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: const GLfloat * vector2_data_pointer
		// Parameter: const char * uniform_var_name
		//************************************
		void ApplyVector2(const GLfloat* vector2_data_pointer, const char* uniform_var_name);

		//************************************
		// Method:    ApplyVector3
		// FullName:  kgl::GPUProgram::ApplyVector3
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: const GLfloat * vector3_data_pointer
		// Parameter: const char * uniform_var_name
		//************************************
		void ApplyVector3(const GLfloat* vector3_data_pointer, const char* uniform_var_name);

		//************************************
		// Method:    ApplyVector4
		// FullName:  kgl::GPUProgram::ApplyVector4
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: const GLfloat * vector4_data_pointer
		// Parameter: const char * uniform_var_name
		//************************************
		void ApplyVector4(const GLfloat* vector4_data_pointer, const char* uniform_var_name);

		//************************************
		// Method:    ApplyInt
		// FullName:  kgl::GPUProgram::ApplyInt
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: GLint int_data
		// Parameter: const char * uniform_var_name
		//************************************
		void ApplyInt(GLint int_data, const char* uniform_var_name);

		//************************************
		// Method:    ApplyMaterial
		// FullName:  kgl::GPUProgram::ApplyMaterial
		// Access:    public 
		// Returns:   void
		// Qualifier: 要使用此方法去设置shader中的材质，则shader代码务必使用material.glsl中
		//            定义的Material结构
		// Parameter: const Material * material
		// Parameter: const char * uniform_var_name
		//************************************
		void ApplyMaterial(const Material* material, const char* uniform_var_name);

		//************************************
		// Method:    ApplyLight
		// FullName:  kgl::GPUProgram::ApplyLight
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: const Light * light
		// Parameter: const char * uniform_var_name
		//************************************
		void ApplyLight(const Light* light, const char* uniform_var_name);
	protected:
		GLuint program_handle_;
	};

	typedef std::shared_ptr<GPUProgram> GPUProgramSPtr;
}
#endif // kgl_gpu_program_h__