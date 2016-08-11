// 基础图元类
#ifndef klib_primitive_h__
#define klib_primitive_h__

#include "kgl_vertex_attribute.h"

namespace kgl
{
	class Primitive
	{
	public:
		enum IndexValueValue // 顶点索引值的数据类型
		{
			INT32,
			UINT32,
			UINT16,
			INT16
		};
	public:
		//************************************
		// Method:    Primitive
		// FullName:  kgl::Primitive::Primitive
		// Access:    public 
		// Returns:   
		// Qualifier:
		//************************************
		Primitive();

		//************************************
		// Method:    ~Primitive
		// FullName:  kgl::Primitive::~Primitive
		// Access:    public 
		// Returns:   
		// Qualifier:
		//************************************
		~Primitive();

		//************************************
		// Method:    Create
		// FullName:  kgl::Primitive::Create
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: GLenum primitive_mode
		// Parameter: GLvoid * vertices
		// Parameter: GLsizeiptr vertices_byte_count
		// Parameter: GLenum vb_usage
		// Parameter: IndexValueValue index_data_type
		// Parameter: GLvoid * indices
		// Parameter: GLsizeiptr indices_byte_count
		// Parameter: GLenum ib_usage
		// Parameter: const std::vector<VertexAttribute> & vtx_attri_arraty
		//************************************
		void Create(GLenum primitive_mode, GLvoid* vertices, GLsizeiptr vertices_byte_count, GLenum vb_usage,IndexValueValue index_data_type, GLvoid* indices, GLsizeiptr indices_byte_count, GLenum ib_usage, const std::vector<VertexAttribute>& vtx_attri_arraty);
		
		// 不带索引缓冲区的圖元
		void Create(GLenum primitive_mode, GLvoid* vertices, GLsizeiptr vertices_byte_count, GLint vertices_count, GLenum vb_usage, const std::vector<VertexAttribute>& vtx_attri_arraty);

		//************************************
		// Method:    Draw
		// FullName:  kgl::Primitive::DrawIndexed
		// Access:    public 
		// Returns:   void
		// Qualifier: 绘制使用了索引缓冲区的图元
		//************************************
		void DrawIndexed();

		// 绘制未使用索引缓冲区的图元
		void Draw();

		//************************************
		// Method:    UpdateAllVertexData
		// FullName:  kgl::Primitive::UpdateAllVertexData
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: const void * src_data
		// Parameter: std::size_t src_data_byte_count
		// Parameter: bool clear_all 更新之前需要把旧的数据全部清0吗？
		//************************************
		void UpdateAllVertexData(const void* src_data, std::size_t src_data_byte_count);

		// 指定待绘制的顶点数，仅对Draw方法有效
		inline void SetDrawnVerticesCount(int32_t count)
		{
			drawn_vertices_count_ = count;
		}
	private:
		GLuint vertex_buffer_object_;
		GLuint vertex_attribute_object_;
		GLuint element_buffer_object_;

		GLenum  primitive_mode_;
		GLenum  indices_type_;
		GLsizei indices_count_;
		GLsizei vertices_count_;

		// 指定待绘制的顶点数，仅对Draw方法有效
		GLsizei drawn_vertices_count_;
	};

	typedef std::shared_ptr<Primitive> PrimitiveSPtr;
}

#endif // klib_primitive_h__