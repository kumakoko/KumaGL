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
		/// <summary>
		/// Initializes a new instance of the <see cref="GPUProgram"/> class.
		/// </summary>
		GPUProgram();

		/// <summary>
		/// Finalizes an instance of the <see cref="GPUProgram"/> class.
		/// </summary>
		~GPUProgram();

		/// <summary>
		/// Uses this instance.
		/// </summary>
		void Use() const;

		/// <summary>
		/// Creates from file.
		/// </summary>
		/// <param name="vs_file_path">The vs_file_path.</param>
		/// <param name="fs_file_path">The fs_file_path.</param>
		/// <param name="gs_file_path">The gs_file_path.</param>
		void CreateFromFile(const GLchar* vs_file_path, const GLchar* fs_file_path, const GLchar* gs_file_path);

		/// <summary>
		/// Creates from file.
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
		/// Applies the matrix.
		/// </summary>
		/// <param name="matrix_data_pointer">存放着矩阵数据的数组首指针</param>
		/// <param name="uniform_var_name">代码中该矩阵变量的名字</param>
		/// <param name="need_transpose">传递进来的矩阵需要转置吗</param>
		void ApplyMatrix(const GLfloat* matrix_data_pointer, const char* uniform_var_name, bool need_transpose = false);

		/// <summary>
		/// Applies the float.
		/// </summary>
		/// <param name="float_data">The float_data.</param>
		/// <param name="uniform_var_name">The uniform_var_name.</param>
		void ApplyFloat(GLfloat float_data, const char* uniform_var_name);

		/// <summary>
		/// Applies the vector2.
		/// </summary>
		/// <param name="vector2_data_pointer">The vector2_data_pointer.</param>
		/// <param name="uniform_var_name">The uniform_var_name.</param>
		void ApplyVector2(const GLfloat* vector2_data_pointer, const char* uniform_var_name);

		/// <summary>
		/// Applies the vector3.
		/// </summary>
		/// <param name="vector3_data_pointer">The vector3_data_pointer.</param>
		/// <param name="uniform_var_name">The uniform_var_name.</param>
		void ApplyVector3(const GLfloat* vector3_data_pointer, const char* uniform_var_name);

		/// <summary>
		/// Applies the vector4.
		/// </summary>
		/// <param name="vector4_data_pointer">The vector4_data_pointer.</param>
		/// <param name="uniform_var_name">The uniform_var_name.</param>
		void ApplyVector4(const GLfloat* vector4_data_pointer, const char* uniform_var_name);

		/// <summary>
		/// Applies the int.
		/// </summary>
		/// <param name="int_data">The int_data.</param>
		/// <param name="uniform_var_name">The uniform_var_name.</param>
		void ApplyInt(GLint int_data, const char* uniform_var_name);

		/// <summary>
		/// 要使用此方法去设置shader中的材质，则shader代码务必使用material.glsl中定义的Material结构
		/// </summary>
		/// <param name="material">The material.</param>
		/// <param name="uniform_var_name">The uniform_var_name.</param>
		void ApplyMaterial(const Material* material, const char* uniform_var_name);

		/// <summary>
		/// Applies the directional light.
		/// </summary>
		/// <param name="light">The light.</param>
		/// <param name="uniform_var_name">The uniform_var_name.</param>
		void ApplyDirectionalLight(const DirectionalLight* light, const char* uniform_var_name);

		/// <summary>
		/// Applies the point light.
		/// </summary>
		/// <param name="light">The light.</param>
		/// <param name="uniform_var_name">The uniform_var_name.</param>
		void ApplyPointLight(const PointLight* light, const char* uniform_var_name);

		/// <summary>
		/// Applies the spot light.
		/// </summary>
		/// <param name="light">The light.</param>
		/// <param name="uniform_var_name">The uniform_var_name.</param>
		void ApplySpotLight(const SpotLight* light, const char* uniform_var_name);
    protected:
		/// <summary>
		/// The program_handle_
		/// </summary>
		GLuint program_handle_;
    };

    typedef std::shared_ptr<GPUProgram> GPUProgramSPtr;
}
#endif // kgl_gpu_program_h__