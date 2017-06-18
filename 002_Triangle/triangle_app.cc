// 展示一个简单的三角形
#include "triangle_app.h"
#include "../klib/kgl_defines.h"

TriangleApp::TriangleApp() :gpu_program_(nullptr), rectangle_primitive_(nullptr),
triangle_primitive_1_(nullptr), triangle_primitive_2_(nullptr), 
triangle_primitive_3_(nullptr), triangle_primitive_4_(nullptr)
{
}

TriangleApp::~TriangleApp()
{
	KGL_SAFE_DELETE(gpu_program_);
	KGL_SAFE_DELETE(rectangle_primitive_);
	KGL_SAFE_DELETE(triangle_primitive_3_);
	KGL_SAFE_DELETE(triangle_primitive_2_);
	KGL_SAFE_DELETE(triangle_primitive_3_);
	KGL_SAFE_DELETE(triangle_primitive_4_);
}

void TriangleApp::InitModel()
{
	gpu_program_ = new kgl::GPUProgram;
	gpu_program_->CreateFromFile("resources/shader/002_triangle_vs.glsl", "resources/shader/002_triangle_fs.glsl", nullptr);
	
	GLfloat vertices[] = 
	{
		// 位置             // 颜色
		0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,// 右上角
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 右下角
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // 左下角
		-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f   // 左上角 
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
	va_position.stride = 6 * sizeof(GLfloat); // 每个顶点的步长为 
	va_position.pointer = nullptr;
	
	kgl::VertexAttribute va_color;
	va_color.index = 1;
	va_color.normalized = GL_FALSE;
	va_color.type = GL_FLOAT;
	va_color.size = 3; // 一个“顶点颜色”的属性由3个分量组成
	va_color.stride = 6 * sizeof(GLfloat); // 每个位置的步长为 
	va_color.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

	std::vector<kgl::VertexAttribute> vtx_attri_array;
	vtx_attri_array.push_back(va_position);
	vtx_attri_array.push_back(va_color);

	rectangle_primitive_ = new kgl::Primitive;
	rectangle_primitive_->Create(GL_TRIANGLES, vertices, sizeof(vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, indices, sizeof(indices), GL_STATIC_DRAW, vtx_attri_array);


	GLfloat triangle_vertices[] =
	{
		// 位置             // 颜色
		-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,// 右下角
		-0.6f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 右下角
		-1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 1.0f,  // 左下角
	};

	GLuint triangle_indices[] =
	{
		0, 1, 2,
	};

	va_position.index = 0;
	va_position.normalized = GL_FALSE;
	va_position.type = GL_FLOAT;
	va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
	va_position.stride = 6 * sizeof(GLfloat); // 每个顶点的步长为 
	va_position.pointer = nullptr;

	va_color.index = 1;
	va_color.normalized = GL_FALSE;
	va_color.type = GL_FLOAT;
	va_color.size = 3; // 一个“顶点颜色”的属性由3个分量组成
	va_color.stride = 6 * sizeof(GLfloat); // 每个位置的步长为 
	va_color.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));


	vtx_attri_array.clear();
	vtx_attri_array.push_back(va_position);
	vtx_attri_array.push_back(va_color);

	triangle_primitive_1_ = new kgl::Primitive;
	triangle_primitive_1_->Create(GL_TRIANGLES, triangle_vertices, sizeof(triangle_vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, triangle_indices, sizeof(triangle_indices), GL_STATIC_DRAW, vtx_attri_array);

	//=====================================================================================

	GLfloat triangle_2_vertices[] =
	{
		// 位置             // 颜色
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,// 右下角
		0.6f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 右下角
		1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 1.0f,  // 左下角
	};

	va_position.index = 0;
	va_position.normalized = GL_FALSE;
	va_position.type = GL_FLOAT;
	va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
	va_position.stride = 6 * sizeof(GLfloat); // 每个顶点的步长为 
	va_position.pointer = nullptr;

	va_color.index = 1;
	va_color.normalized = GL_FALSE;
	va_color.type = GL_FLOAT;
	va_color.size = 3; // 一个“顶点颜色”的属性由3个分量组成
	va_color.stride = 6 * sizeof(GLfloat); // 每个位置的步长为 
	va_color.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));


	vtx_attri_array.clear();
	vtx_attri_array.push_back(va_position);
	vtx_attri_array.push_back(va_color);

	triangle_primitive_2_ = new kgl::Primitive;
	triangle_primitive_2_->Create(GL_TRIANGLES, triangle_2_vertices, sizeof(triangle_2_vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, triangle_indices, sizeof(triangle_indices), GL_STATIC_DRAW, vtx_attri_array);

	//= == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==

	GLfloat triangle_3_vertices[] =
	{
		// 位置             // 颜色
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,// 右下角
		0.6f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 右下角
		1.0f, 0.6f, 0.0f, 0.0f, 0.0f, 1.0f,  // 左下角
	};

	va_position.index = 0;
	va_position.normalized = GL_FALSE;
	va_position.type = GL_FLOAT;
	va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
	va_position.stride = 6 * sizeof(GLfloat); // 每个顶点的步长为 
	va_position.pointer = nullptr;

	va_color.index = 1;
	va_color.normalized = GL_FALSE;
	va_color.type = GL_FLOAT;
	va_color.size = 3; // 一个“顶点颜色”的属性由3个分量组成
	va_color.stride = 6 * sizeof(GLfloat); // 每个位置的步长为 
	va_color.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

	vtx_attri_array.clear();
	vtx_attri_array.push_back(va_position);
	vtx_attri_array.push_back(va_color);

	triangle_primitive_3_ = new kgl::Primitive;
	triangle_primitive_3_->Create(GL_TRIANGLES, triangle_3_vertices, sizeof(triangle_3_vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, triangle_indices, sizeof(triangle_indices), GL_STATIC_DRAW, vtx_attri_array);

	//= == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==

	GLfloat triangle_4_vertices[] =
	{
		// 位置             // 颜色
		-1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,// 右下角
		-0.6f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 右下角
		-1.0f, 0.6f, 0.0f, 0.0f, 0.0f, 1.0f,  // 左下角
	};

	va_position.index = 0;
	va_position.normalized = GL_FALSE;
	va_position.type = GL_FLOAT;
	va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
	va_position.stride = 6 * sizeof(GLfloat); // 每个顶点的步长为 
	va_position.pointer = nullptr;

	va_color.index = 1;
	va_color.normalized = GL_FALSE;
	va_color.type = GL_FLOAT;
	va_color.size = 3; // 一个“顶点颜色”的属性由3个分量组成
	va_color.stride = 6 * sizeof(GLfloat); // 每个位置的步长为 
	va_color.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

	vtx_attri_array.clear();
	vtx_attri_array.push_back(va_position);
	vtx_attri_array.push_back(va_color);

	triangle_primitive_4_ = new kgl::Primitive;
	triangle_primitive_4_->Create(GL_TRIANGLES, triangle_4_vertices, sizeof(triangle_4_vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, triangle_indices, sizeof(triangle_indices), GL_STATIC_DRAW, vtx_attri_array);

}

void TriangleApp::RenderFrame()
{
	gpu_program_->Use();
	rectangle_primitive_->DrawIndexed();
	triangle_primitive_1_->DrawIndexed();
	triangle_primitive_2_->DrawIndexed();
	triangle_primitive_3_->DrawIndexed();
	triangle_primitive_4_->DrawIndexed();
}