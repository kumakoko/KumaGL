// 简单的纹理应用示例


#include "texture_app.h"
#include "../klib/kgl_defines.h"

#if defined(__APPLE__) && defined(__MACH__)
#include "../third_lib/soil/src/SOIL.h"
#endif

TextureApp::TextureApp() :gpu_program_(nullptr), rectangle_primitive_(nullptr)
{
}

TextureApp::~TextureApp()
{
	KGL_SAFE_DELETE(gpu_program_);
	KGL_SAFE_DELETE(rectangle_primitive_);
	texture_1_.reset();
	texture_2_.reset();
}

void TextureApp::InitModel()
{
    const GLchar* vs_file_path = nullptr;
    const GLchar* fs_file_path = nullptr;
    std::string texture_1_name;
    std::string texture_2_name;
#if defined(WIN32) || defined(_WIN32)
    vs_file_path = "resources/shader/003_texture_vs.glsl";
    fs_file_path = "resources/shader/003_texture_fs.glsl";
    texture_1_name = "resources/image/coordinate.bmp";
    texture_2_name = "resources/image/braynzar.jpg";
#elif defined(__APPLE__) && defined(__MACH__)
    vs_file_path = "/Users/xiongxinke/Desktop/SvnChina/kgl/publish/resources/shader/003_texture_vs.glsl";
    fs_file_path = "/Users/xiongxinke/Desktop/SvnChina/kgl/publish/resources/shader/003_texture_fs.glsl";
    texture_1_name = "/Users/xiongxinke/Desktop/SvnChina/kgl/publish/resources/image/coordinate.bmp";
    texture_2_name = "/Users/xiongxinke/Desktop/SvnChina/kgl/publish/resources/image/braynzar.jpg";
#endif
    
	gpu_program_ = new kgl::GPUProgram;
	gpu_program_->CreateFromFile(vs_file_path, fs_file_path, nullptr);

	kgl::TextureParams texture_param;
	texture_param.wrap_s_mode = GL_REPEAT;
	texture_param.wrap_t_mode = GL_REPEAT;
	texture_param.mag_filter_mode = GL_LINEAR;
	texture_param.min_filter_mode = GL_LINEAR;
	texture_param.internal_format = GL_RGB;
	texture_param.src_img_px_component_type = GL_UNSIGNED_BYTE;
	texture_param.src_img_format = GL_RGB;
	texture_param.used_mipmap = false;

	texture_1_ = std::make_shared<kgl::SourceTexture>();
	texture_1_->CreateFromFile(texture_1_name, texture_param);

	texture_2_ = std::make_shared<kgl::SourceTexture>();
	texture_2_->CreateFromFile(texture_2_name, texture_param);

	GLfloat vertices[] =
	{
		// 位置          // 颜色           // 纹理映射坐标
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右上角
		1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下角
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下角
		-1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  //左上角
	};

	GLuint indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	kgl::VertexAttribute va_position;
	va_position.index = 0;
	va_position.normalized = GL_FALSE;
	va_position.type = GL_FLOAT;
	va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
	va_position.stride = 8 * sizeof(GLfloat); // 每个顶点的步长
	va_position.pointer = nullptr;

	kgl::VertexAttribute va_color;
	va_color.index = 1;
	va_color.normalized = GL_FALSE;
	va_color.type = GL_FLOAT;
	va_color.size = 3; // 一个“顶点颜色”的属性由3个分量组成
	va_color.stride = 8 * sizeof(GLfloat); //每个顶点的步长
	va_color.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

	kgl::VertexAttribute va_texture_coord;
	va_texture_coord.index = 2;
	va_texture_coord.normalized = GL_FALSE;
	va_texture_coord.type = GL_FLOAT;
	va_texture_coord.size = 2; // 一个“顶点坐标”的属性由3个分量组成
	va_texture_coord.stride = 8 * sizeof(GLfloat); // 每个顶点的步长
	va_texture_coord.pointer = reinterpret_cast<GLvoid*> (6 * sizeof(GLfloat)); // 纹理坐标数据在字段中的起始指针处

	std::vector<kgl::VertexAttribute> vtx_attri_array;
	vtx_attri_array.push_back(va_position);
	vtx_attri_array.push_back(va_color);
	vtx_attri_array.push_back(va_texture_coord);

	rectangle_primitive_ = new kgl::Primitive;
	rectangle_primitive_->CreateIndexed(GL_TRIANGLES, vertices, sizeof(vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, indices, sizeof(indices), GL_STATIC_DRAW, vtx_attri_array);
}

void TextureApp::RenderFrame()
{
	gpu_program_->Use();
	gpu_program_->ApplyTexture(texture_1_, "source_texture_1", 0);
	gpu_program_->ApplyTexture(texture_2_, "source_texture_2", 1);
	rectangle_primitive_->DrawIndexed();
}
