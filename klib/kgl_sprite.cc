// 精灵类
#include "kgl_lib_pch.h"
#include "kgl_sprite.h"

namespace kgl
{
	Sprite::Sprite(GPUProgramSPtr shader)
	{
		this->shader_ = shader;
		this->initRenderData();
	}

	Sprite::~Sprite()
	{
		glDeleteVertexArrays(1, &this->quadVAO);
	}

	void Sprite::Draw(SourceTextureSPtr sprite_texture, glm::vec2 sprite_position, glm::vec2 sprite_size /*= glm::vec2(10, 10)*/, GLfloat sprite_rotation /*= 0.0f*/, glm::vec4 sprite_color /*= glm::vec4(1.0f)*/)
	{
		// 启用本shader
		shader_->Use();

		glm::mat4 sprite_model_matrix;
		sprite_model_matrix = glm::translate(sprite_model_matrix, glm::vec3(sprite_position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)
		sprite_model_matrix = glm::translate(sprite_model_matrix, glm::vec3(0.5f * sprite_size.x, 0.5f * sprite_size.y, 0.0f)); // Move origin of rotation to center of quad
		sprite_model_matrix = glm::rotate(sprite_model_matrix, sprite_rotation, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
		sprite_model_matrix = glm::translate(sprite_model_matrix, glm::vec3(-0.5f * sprite_size.x, -0.5f * sprite_size.y, 0.0f)); // Move origin back
		sprite_model_matrix = glm::scale(sprite_model_matrix, glm::vec3(sprite_size, 1.0f)); // Last scale

		shader_->ApplyMatrix(glm::value_ptr(sprite_model_matrix), "sprite_model_matrix");
		shader_->ApplyVector4(glm::value_ptr(sprite_color), "sprite_color");
		shader_->ApplyTexture(sprite_texture,"sprite_texture",0);

		rectangle_primitive_->DrawIndexed();
	}

	void Sprite::initRenderData()
	{
		GLfloat vertices[] =
		{
			// 位置             // 颜色
			0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// 右上角
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
	}
}