// 动态改变顶点缓冲区数据的应用示例
#include "dynamic_vertex_buffer_app.h"
#include "../klib/kgl_defines.h"

const int VERTEX_DATA_COUNT = 24;

DynamicVertexBufferApp::DynamicVertexBufferApp() :gpu_program_(nullptr), rectangle_primitive_(nullptr)
{
	GLfloat vertices[VERTEX_DATA_COUNT] =
	{
		// 位置             // 颜色
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// 右上角
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 右下角
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // 左下角
		-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f   // 左上角 
	};

	vertices_data_ = new GLfloat[VERTEX_DATA_COUNT];
	memcpy(vertices_data_, vertices, sizeof(GLfloat) * VERTEX_DATA_COUNT);
}

DynamicVertexBufferApp::~DynamicVertexBufferApp()
{
	KGL_SAFE_DELETE_ARRAY(vertices_data_);
	KGL_SAFE_DELETE(gpu_program_);
	KGL_SAFE_DELETE(rectangle_primitive_);
	texture_1_.reset();
	texture_2_.reset();
}

void DynamicVertexBufferApp::InitScene()
{
	gpu_program_ = new kgl::GPUProgram;
	gpu_program_->CreateFromFile("resources/shader/002_triangle_vs.glsl", "resources/shader/002_triangle_fs.glsl", nullptr);

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
	rectangle_primitive_->CreateIndexed(GL_TRIANGLES, vertices_data_, sizeof(GLfloat)* VERTEX_DATA_COUNT, GL_DYNAMIC_DRAW, kgl::Primitive::UINT32, indices, sizeof(indices), GL_DYNAMIC_DRAW, vtx_attri_array);
}

void DynamicVertexBufferApp::RenderFrame()
{
	gpu_program_->Use();

	float current_time = glfwGetTime();

	// 2秒更新一次顶点缓冲区数据
	if (current_time - last_update_vb_time_ > 0.1f)
	{
		last_update_vb_time_ = current_time;

		vertices_data_[3] = vertices_data_[3] + 0.01f > 1.0f ? 0.0f : vertices_data_[3] + 0.01f;
		vertices_data_[4] = vertices_data_[4] - 0.01f < 0.0f ? 1.0f : vertices_data_[4] - 0.01f;
		vertices_data_[5] = vertices_data_[5] + 0.01f > 1.0f ? 0.0f : vertices_data_[5] + 0.01f;

		vertices_data_[9] = vertices_data_[9] + 0.01f > 1.0f ? 0.0f : vertices_data_[9] + 0.01f;
		vertices_data_[10] = vertices_data_[10] + 0.01f > 1.0f ? 0.0f : vertices_data_[10] + 0.01f;
		vertices_data_[11] = vertices_data_[11] - 0.01f < 0.0f ? 1.0f : vertices_data_[11] - 0.01f;

		vertices_data_[15] = vertices_data_[15] + 0.01f > 1.0f ? 0.0f : vertices_data_[15] + 0.01f;
		vertices_data_[16] = vertices_data_[16] + 0.01f < 0.0f ? 1.0f : vertices_data_[16] - 0.01f;
		vertices_data_[17] = vertices_data_[17] + 0.01f > 1.0f ? 0.0f : vertices_data_[17] + 0.01f;

		vertices_data_[21] = vertices_data_[21] - 0.01f < 0.0f ? 1.0f : vertices_data_[21] - 0.01f;
		vertices_data_[22] = vertices_data_[22] + 0.01f > 1.0f ? 0.0f : vertices_data_[22] + 0.01f;
		vertices_data_[23] = vertices_data_[23] - 0.01f < 0.0f ? 1.0f : vertices_data_[23] - 0.01f;

		rectangle_primitive_->UpdateAllVertexData(vertices_data_, sizeof(GLfloat) * VERTEX_DATA_COUNT);
	}
	
	
	rectangle_primitive_->DrawIndexed();
}