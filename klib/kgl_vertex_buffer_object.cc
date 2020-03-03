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