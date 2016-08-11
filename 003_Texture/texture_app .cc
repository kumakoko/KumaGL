// 简单的纹理应用示例


#include "texture_app.h"
#include "../klib/kgl_defines.h"

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

void TextureApp::InitScene()
{
	gpu_program_ = new kgl::GPUProgram;
	gpu_program_->CreateFromFile("resources/shader/003_texture_vs.glsl", "resources/shader/003_texture_fs.glsl", nullptr);

	kgl::TextureParams texture_param;
	texture_param.WrapSMode = GL_REPEAT;
	texture_param.WrapTMode = GL_REPEAT;
	texture_param.MagFilterMode = GL_LINEAR;
	texture_param.MinFilterMode = GL_LINEAR;
	texture_param.InternalFormat = GL_RGB;
	texture_param.SrcImgPixelComponentType = GL_UNSIGNED_BYTE;
	texture_param.SrcImgFormat = GL_RGB;
	texture_param.LoadChannel = SOIL_LOAD_RGB;
	texture_param.UseMipmap = false;

	texture_1_ = std::make_shared<kgl::SourceTexture>();
	texture_1_->CreateFromFile("resources/image/coordinate.bmp", texture_param);

	texture_2_ = std::make_shared<kgl::SourceTexture>();
	texture_2_->CreateFromFile("resources/image/braynzar.jpg", texture_param);

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
	va_position.Index = 0;
	va_position.Normalized = GL_FALSE;
	va_position.Type = GL_FLOAT;
	va_position.Size = 3; // 一个“顶点位置”的属性由3个分量组成
	va_position.Stride = 8 * sizeof(GLfloat); // 每个顶点的步长
	va_position.Pointer = nullptr;

	kgl::VertexAttribute va_color;
	va_color.Index = 1;
	va_color.Normalized = GL_FALSE;
	va_color.Type = GL_FLOAT;
	va_color.Size = 3; // 一个“顶点颜色”的属性由3个分量组成
	va_color.Stride = 8 * sizeof(GLfloat); //每个顶点的步长
	va_color.Pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

	kgl::VertexAttribute va_texture_coord;
	va_texture_coord.Index = 2;
	va_texture_coord.Normalized = GL_FALSE;
	va_texture_coord.Type = GL_FLOAT;
	va_texture_coord.Size = 2; // 一个“顶点坐标”的属性由3个分量组成
	va_texture_coord.Stride = 8 * sizeof(GLfloat); // 每个顶点的步长
	va_texture_coord.Pointer = reinterpret_cast<GLvoid*> (6 * sizeof(GLfloat)); // 纹理坐标数据在字段中的起始指针处

	std::vector<kgl::VertexAttribute> vtx_attri_array;
	vtx_attri_array.push_back(va_position);
	vtx_attri_array.push_back(va_color);
	vtx_attri_array.push_back(va_texture_coord);

	rectangle_primitive_ = new kgl::Primitive;
	rectangle_primitive_->Create(GL_TRIANGLES, vertices, sizeof(vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, indices, sizeof(indices), GL_STATIC_DRAW, vtx_attri_array);
}

void TextureApp::RenderFrame()
{
	gpu_program_->Use();
	gpu_program_->ApplyTexture(texture_1_, "source_texture_1", 0);
	gpu_program_->ApplyTexture(texture_2_, "source_texture_2", 1);
	rectangle_primitive_->DrawIndexed();
}