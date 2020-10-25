/**************************************************************************************************************************
Copyright(C) 2014-2019 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
#include "kgl_lib_pch.h"
#include "kgl_vertex_buffer_object.h"

namespace kgl
{	
	VertexBufferObject::VertexBufferObject()
	{
		data_uploaded_ = false;
		buffer_id_ = 0;
	}

	VertexBufferObject::~VertexBufferObject()
	{
		Delete();
	}

	void VertexBufferObject::Create(uint32_t size /*= 0*/)
	{
		glGenBuffers(1, &buffer_id_);
		data_.reserve(size);
		size_ = size;
		current_size_ = 0;
	}

	void VertexBufferObject::Delete()
	{
		if (buffer_id_ != 0)
		{
			glDeleteBuffers(1, &buffer_id_);
			data_uploaded_ = false;
			data_.clear();
			buffer_id_ = 0;
		}
	}
		
	void* VertexBufferObject::MapBufferToMemory(GLint usage)
	{
		if (!data_uploaded_)
			return nullptr;
		/*
		VBO相对于显示列表的优势在于客户端可以读取与编辑缓存对象数据，而显示列表不能这样做。
		更新VBO的最简便方法是使用glBufferData()或glBufferSubData()将新数据重新拷贝到绑
		定的VBO中。这种情况下，你的应用程序需要始终拥有一个有效的顶点数组。也就是说，你必
		须始终拥有2份顶点数据：一份在应用程序中，另一份在VBO中。

		修改缓存对象的另一个方式是将缓存对象映射到客户端内存，客户端可以使用映射缓存的指针
		更新数据。。

		注意，glMapBuffer()引起同步问题。如果GPU仍然工作于该缓存对象，glMapBuffer()将一
		直等待直到GPU结束对应缓存对象上的工作。

		为了避免等待，你可以首先使用NULL调用glBufferData()，然后再调用glMapBuffer()。这
		样，前一个数据将被丢弃且glMapBuffer()立即返回一个新分配的指针，即使GPU任然工作于
		前一个数据。

		然而，由于你丢弃了前一个数据，这种方法只有在你想更新整个数据集的时候才有效。如果你
		仅仅希望更改部分数据或读取数据，你最好不要释放先前的数据。
		*/
		void* ptrRes = glMapBuffer(buffer_type_, usage);
		return ptrRes;
	}

	void* VertexBufferObject::MapSubBufferToMemory(GLint usage, uint32_t offset, uint32_t length)
	{
		if (!data_uploaded_)return NULL;
		void* ptrRes = glMapBufferRange(buffer_type_, offset, length, usage);
		return ptrRes;
	}
	
	void VertexBufferObject::UnmapBuffer()
	{
		glUnmapBuffer(buffer_type_);
	}
	
	void VertexBufferObject::Bind(GLint buffer_type /*= GL_ARRAY_BUFFER*/)
	{
		buffer_type_ = buffer_type;
		glBindBuffer(buffer_type_, buffer_id_);
	}
	
	void VertexBufferObject::UploadDataToGPU(GLint usage_hint)
	{
		/*当缓存初始化之后，你可以使用glBufferData()将数据拷贝到缓存对象。
		
		void glBufferData(GLenum target，GLsizeiptr size, const GLvoid*  data, GLenum usage);

		第1个参数target可以为GL_ARRAY_BUFFER或GL_ELEMENT_ARRAY。
		第2个参数size为待传递数据字节数量。
		第3个参数为源数据数组指针，如data为NULL，则VBO仅仅预留给定数据大小的内存空间。
		第4个参数usage标志位VBO的另一个性能提示，它提供缓存对象将如何使用：static、dynamic或stream、与read、copy或draw。

		VBO为usage标志指定9个枚举值：

		GL_STATIC_DRAW
		GL_STATIC_READ
		GL_STATIC_COPY
		GL_DYNAMIC_DRAW
		GL_DYNAMIC_READ
		GL_DYNAMIC_COPY
		GL_STREAM_DRAW
		GL_STREAM_READ
		GL_STREAM_COPY

		static表示VBO中的数据将不会被改动（一次指定多次使用），
		dynamic表示数据将会被频繁改动（反复指定与使用），
		stream表示每帧数据都要改变（一次指定一次使用）。
		draw表示数据将被发送到GPU以待绘制（应用程序到GL），
		read表示数据将被客户端程序读取（GL到应用程序），
		copy表示数据可用于绘制与读取（GL到GL）。

		注意，仅仅draw标志对VBO有用，copy与read标志对顶点/帧缓存对象（PBO或FBO）更有意义，
		如GL_STATIC_DRAW与GL_STREAM_DRAW使用显卡内存，GL_DYNAMIC使用AGP内存。
		_READ_相关缓存更适合在系统内存或AGP内存，因为这样数据更易访问。*/
		glBufferData(buffer_type_, data_.size(), data_.data(), usage_hint);
		data_uploaded_ = true;
		data_.clear();
	}
		
	void VertexBufferObject::AddData(void* data_pointer, std::size_t data_size)
	{
		auto start_pointer = reinterpret_cast<uint8_t*>(data_pointer);
		auto end_pointer = start_pointer + static_cast<ptrdiff_t>(data_size);
		data_.insert(data_.end(), start_pointer, end_pointer);
		current_size_ += data_size;
	}
		
	void* VertexBufferObject::GetDataPointer()
	{
		return data_uploaded_ ? nullptr : (void*)data_[0];
	}
		
	GLuint VertexBufferObject::GetBufferID()
	{
		return buffer_id_;
	}
	
	std::size_t VertexBufferObject::GetCurrentSize()
	{
		return current_size_;
	}
}