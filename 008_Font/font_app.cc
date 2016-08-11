// 简单的纹理应用示例


#include "font_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_string_convertor.h"

FontApp::FontApp() :font_renderer_(nullptr)
{
}

FontApp::~FontApp()
{
	KGL_SAFE_DELETE(font_renderer_);
}

void FontApp::InitScene()
{
	font_renderer_ = new kgl::FontRenderer();
	font_renderer_->CreateFontTexture("resources/font/fzss_gbk.ttf", "fzss24", 24, 512, 512);
	font_renderer_->SetCurrentFont("fzss24");

	// 打开文件，读入代码到流中，然后载入到内存，提交编译
	std::ifstream file_stream;
	file_stream.open("resources/config/font_app.txt");
	std::stringstream vs_string_stream;
	vs_string_stream << file_stream.rdbuf();
	text_ = kgl::StringConvertor::UTF8toUTF16LE(vs_string_stream.str().c_str());
	file_stream.close();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void FontApp::InitHelper()
{
	helper_shader_ = new kgl::GPUProgram;
	helper_shader_->CreateFromFile("resources/shader/font_helper_vs.glsl", "resources/shader/font_helper_fs.glsl", nullptr);

	GLfloat vertices[] =
	{
		// 位置               // 纹理映射坐标
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // 右上角
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // 右下角
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // 左下角
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f  //左上角
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
	va_position.Stride = 5 * sizeof(GLfloat); // 每个顶点的步长
	va_position.Pointer = nullptr;

	kgl::VertexAttribute va_texture_coord;
	va_texture_coord.Index = 1;
	va_texture_coord.Normalized = GL_FALSE;
	va_texture_coord.Type = GL_FLOAT;
	va_texture_coord.Size = 2; // 一个“顶点坐标”的属性由3个分量组成
	va_texture_coord.Stride = 5 * sizeof(GLfloat); // 每个顶点的步长
	va_texture_coord.Pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat)); // 纹理坐标数据在字段中的起始指针处

	std::vector<kgl::VertexAttribute> vtx_attri_array;
	vtx_attri_array.push_back(va_position);
	vtx_attri_array.push_back(va_texture_coord);

	helper_rectangle_ = new kgl::Primitive;
	helper_rectangle_->Create(GL_TRIANGLES, vertices, sizeof(vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, indices, sizeof(indices), GL_STATIC_DRAW, vtx_attri_array);

	
}

void FontApp::RenderFrame()
{
	/*
	helper_shader_->Use();
	helper_shader_->ApplyTexture(font_renderer_->font_texture_->GetTexture(), "source_texture", 0);
	helper_rectangle_->DrawIndexed();
	*/
	boost::wformat wf(text_);
	wf % glfwGetTime();
	font_renderer_->AddToRendered(wf.str(), 0, 0,
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 1.5f);
	font_renderer_->AddToRendered(L"This is more than just a game", 0, 100,
		glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 1.5f);
	font_renderer_->Draw();
}