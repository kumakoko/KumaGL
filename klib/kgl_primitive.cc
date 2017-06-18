// 基础图元类
#include "kgl_lib_pch.h"
#include "kgl_primitive.h"
#include "kgl_defines.h"

namespace kgl
{
    Primitive::Primitive() :vertex_buffer_object_(0), vertex_attribute_object_(0), element_buffer_object_(0),
    primitive_mode_(GL_TRIANGLES), vertices_count_(0)
    {
    }

    Primitive::~Primitive()
    {
        if (vertex_attribute_object_ != 0)
            glDeleteVertexArrays(1, &vertex_attribute_object_);

        if (vertex_buffer_object_ != 0)
            glDeleteBuffers(1, &vertex_buffer_object_);

        if (element_buffer_object_ != 0)
            glDeleteBuffers(1, &element_buffer_object_);
    }

    /*
    http://blog.sina.com.cn/s/blog_7cfb366d010158he.html
    http ://www.tuicool.com/articles/bU3yYn
    http ://blog.csdn.net/myarrow/article/details/7743338
    */
    void Primitive::Create(GLenum primitive_mode, GLvoid* vertices, GLsizeiptr vertices_byte_count, GLenum vb_usage,
        IndexValueValue index_data_type, GLvoid* indices, GLsizeiptr indices_byte_count, GLenum ib_usage, const std::vector<VertexAttribute>& vtx_attri_arraty)
    {
        primitive_mode_ = primitive_mode;

        GLsizei vertex_array_count = 1;
        GLsizei vbo_count = 1;
        GLsizei ebo_count = 1;

        // 创建一个vertex array object，并马上绑定
        glGenVertexArrays(vertex_array_count, &vertex_attribute_object_);
        glBindVertexArray(vertex_attribute_object_);

        // 创建一个vertex buffer object，并绑定，绑定完毕之后
        // 马上对vertex_buffer_object_进行数据填充
        glGenBuffers(vbo_count, &vertex_buffer_object_);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
        glBufferData(GL_ARRAY_BUFFER, vertices_byte_count, vertices, vb_usage);

        // 创建一个element buffer object，并绑定，绑定element_buffer_object_之后，
        // 马上对element_buffer_object_进行数据填充
        glGenBuffers(ebo_count, &element_buffer_object_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_byte_count, indices, ib_usage);

        for (std::size_t i = 0; i < vtx_attri_arraty.size(); ++i)
        {
            const VertexAttribute& va = vtx_attri_arraty[i];
            glVertexAttribPointer(
                va.Index,//0, 
                va.Size,//3,
                va.Type,//GL_FLOAT,
                va.Normalized,//GL_FALSE,
                va.Stride,//3 * sizeof(GLfloat),
                va.Pointer//(GLvoid*)0
                );
            glEnableVertexAttribArray(i);
        }


        glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered vertex_buffer_object_ as the currently bound vertex buffer object so afterwards we can safely unbind
        glBindVertexArray(0); // Unbind vertex_attribute_object_ (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the element_buffer_object_, keep it bound to this vertex_attribute_object_

		switch (index_data_type)
		{
		case Primitive::UINT32:
			indices_count_ = static_cast<GLsizei>(indices_byte_count) / sizeof(GLuint);
			indices_type_ = GL_UNSIGNED_INT;
			break;
		case Primitive::INT32:
			indices_count_ = static_cast<GLsizei>(indices_byte_count) / sizeof(GLint);
			indices_type_ = GL_INT;
			break;
		case Primitive::UINT16:
			indices_count_ = static_cast<GLsizei>(indices_byte_count) / sizeof(GLushort);
			indices_type_ = GL_UNSIGNED_SHORT;
			break;
		case Primitive::INT16:
			indices_count_ = static_cast<GLsizei>(indices_byte_count) / sizeof(GLshort);
			indices_type_ = GL_SHORT;
			break;
		}
	}

	// 不带索引缓冲区的圖元
	void Primitive::Create(GLenum primitive_mode, GLvoid* vertices, GLsizeiptr vertices_byte_count, GLint vertices_count,GLenum vb_usage, const std::vector<VertexAttribute>& vtx_attri_arraty)
	{
		primitive_mode_ = primitive_mode;

		GLsizei vertex_array_count = 1;
		GLsizei vbo_count = 1;

		// 创建一个vertex array object，并马上绑定
		glGenVertexArrays(vertex_array_count, &vertex_attribute_object_);
		glBindVertexArray(vertex_attribute_object_);

		// 创建一个vertex buffer object，并绑定，绑定完毕之后
		// 马上对vertex_buffer_object_进行数据填充
		glGenBuffers(vbo_count, &vertex_buffer_object_);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
		glBufferData(GL_ARRAY_BUFFER, vertices_byte_count, vertices, vb_usage);

		for (std::size_t i = 0; i < vtx_attri_arraty.size(); ++i)
		{
			const VertexAttribute& va = vtx_attri_arraty[i];
			glVertexAttribPointer(
				va.Index,//0, 
				va.Size,//3,
				va.Type,//GL_FLOAT,
				va.Normalized,//GL_FALSE,
				va.Stride,//3 * sizeof(GLfloat),
				va.Pointer//(GLvoid*)0
				);
			glEnableVertexAttribArray(i);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0); 
		glBindVertexArray(0);

		vertices_count_ = vertices_count;
		drawn_vertices_count_ = vertices_count;
	}
		

	void Primitive::DrawIndexed()
	{
		glBindVertexArray(vertex_attribute_object_);
		glDrawElements(primitive_mode_, indices_count_, indices_type_, 0);
		glBindVertexArray(0);
	}

	void Primitive::Draw()
	{
		glBindVertexArray(vertex_attribute_object_);
		// glDrawArrays(GL_TRIANGLES, 0, drawn_vertices_count_);
		glDrawArrays(primitive_mode_, 0, drawn_vertices_count_);
		glBindVertexArray(0);
	}

	void Primitive::Draw(GLint first_slot,GLsizei drawn_vertices_count)
	{
		glBindVertexArray(vertex_attribute_object_);
		glDrawArrays(primitive_mode_, first_slot, drawn_vertices_count);
		glBindVertexArray(0);
	}

	// 参考文档： https://www.opengl.org/wiki/Vertex_Specification_Best_Practices#Dynamic_VBO
	void Primitive::UpdateAllVertexData(const void* src_data, std::size_t src_data_byte_count)
	{
		// std::vector<std::string> error_desc_array;
		// std::vector<GLenum> error_code_array;
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
		//THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);
		GLvoid* buffer_pointer = glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
		memcpy(buffer_pointer, src_data, src_data_byte_count);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}