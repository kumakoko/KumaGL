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
#ifndef kgl_vertex_buffer_object_h__
#define kgl_vertex_buffer_object_h__

namespace kgl
{
	class VertexBufferObject
	{
	public:
		/*********************************************************
		创建一个VBO的对象
		@param  uint32_t size       
		*********************************************************/
		void Create(uint32_t size = 0);

		/*********************************************************
		删除掉VBO相关的缓冲区     
		*********************************************************/
		void Delete();

		/*********************************************************
		把数据缓冲区区域映射向一片内存
		@param          GLint usage 映射的方式，类似于GL_READ_ONLY，GL_WRITE_ONLY
		@return         返回指向该片数据在内存的首指针
		*********************************************************/
		void* MapBufferToMemory(GLint usage);

		/*********************************************************
		把部分数据缓冲区区域映射向一片内存
		@param          GLint usage  映射的方式，类似于GL_READ_ONLY，GL_WRITE_ONLY
		@param          uint32_t offset 部分区域的首地址与整个区域首地址的偏移值
		@param          uint32_t length 部分区域的长度
		@return         返回指向该片数据在内存的首指针     
		*********************************************************/
		void* MapSubBufferToMemory(GLint usage, uint32_t offset, uint32_t length);

		/*********************************************************
		解除对缓冲区的映射      
		*********************************************************/
		void UnmapBuffer();

		/*********************************************************
		绑定VBO
		@param  GLint buffer_type         
		*********************************************************/
		void Bind(GLint buffer_type = GL_ARRAY_BUFFER);
		
		/*********************************************************
		把顶点数据uploda到GPU
		@param  int usage_hint GL_STATIC_DRAW, GL_DYNAMIC_DRAW等一系列的值       
		*********************************************************/
		void UploadDataToGPU(GLint usage_hint);

		/*********************************************************
		加入任意长度到当前VBO的缓冲区
		@param  void * data_pointer 待添加的数据的首指针
		@param  std::size_t data_size 待添加数据的字节数       
		*********************************************************/
		void AddData(void* data_pointer, std::size_t data_size);

		/*********************************************************
		获取到当前数据缓冲区首指针
		@return 当前数据缓冲区首指针        
		*********************************************************/
		void* GetDataPointer();
		
		/*********************************************************
		获取到当前VBO的id值
		@return  当前VBO的id值       
		*********************************************************/
		GLuint GetBufferID();

		/*********************************************************
		获取到当前已经添加到缓冲区中的数据长度
		@return 当前已经添加到缓冲区中的数据长度      
		*********************************************************/
		std::size_t GetCurrentSize();

		VertexBufferObject();

		~VertexBufferObject();

	private:
		GLuint buffer_id_;
		std::size_t size_;
		std::size_t current_size_;
		int buffer_type_;
		bool data_uploaded_;
		std::vector<uint8_t> data_;	
	};

	typedef std::shared_ptr<VertexBufferObject> VBOSPtr;
}

#endif // kgl_vertex_buffer_object_h__