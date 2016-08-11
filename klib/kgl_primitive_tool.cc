// 图元辅助工具
#include "kgl_lib_pch.h"
#include "kgl_primitive_tool.h"
#include "kgl_vertex_attribute.h"

namespace kgl
{
	PrimitiveSPtr PrimitiveTool::BuildCube()
	{
		GLfloat vertices[] =
		{
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f,

			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,

			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f,

			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f,

			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,1.0f
		};

		VertexAttribute va_position;
		va_position.Index = 0;
		va_position.Normalized = GL_FALSE;
		va_position.Type = GL_FLOAT;
		va_position.Size = 3; // 一个“顶点位置”的属性由3个分量组成
		va_position.Stride = 7 * sizeof(GLfloat); // 每个顶点的步长为 
		va_position.Pointer = nullptr;

		VertexAttribute va_color;
		va_color.Index = 1;
		va_color.Normalized = GL_FALSE;
		va_color.Type = GL_FLOAT;
		va_color.Size = 4; // 一个“顶点颜色”的属性由3个分量组成
		va_color.Stride = 7 * sizeof(GLfloat); // 每个位置的步长为 
		va_color.Pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

		std::vector<VertexAttribute> vtx_attri_array;
		vtx_attri_array.clear();
		vtx_attri_array.push_back(va_position);
		vtx_attri_array.push_back(va_color);

		PrimitiveSPtr p = std::make_shared<Primitive>();
		p->Create(GL_TRIANGLES, vertices, sizeof(vertices), 36, GL_STATIC_DRAW, vtx_attri_array);
		return p;
	}
}