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
        /// <summary>
        /// Initializes a new instance of the <see cref="Primitive"/> class.
        /// </summary>
        Primitive();

        /// <summary>
        /// Finalizes an instance of the <see cref="Primitive"/> class.
        /// </summary>
        ~Primitive();

        /// <summary>
        /// Creates the specified primitive_mode.
        /// </summary>
        /// <param name="primitive_mode">The primitive_mode.</param>
        /// <param name="vertices">The vertices.</param>
        /// <param name="vertices_byte_count">The vertices_byte_count.</param>
        /// <param name="vb_usage">The vb_usage.</param>
        /// <param name="index_data_type">The index_data_type.</param>
        /// <param name="indices">The indices.</param>
        /// <param name="indices_byte_count">The indices_byte_count.</param>
        /// <param name="ib_usage">The ib_usage.</param>
        /// <param name="vtx_attri_arraty">The vtx_attri_arraty.</param>
        void CreateIndexed(GLenum primitive_mode, GLvoid* vertices, GLsizeiptr vertices_byte_count, GLenum vb_usage, IndexValueValue index_data_type, GLvoid* indices, GLsizeiptr indices_byte_count, GLenum ib_usage, const std::vector<VertexAttribute>& vtx_attri_arraty);
        
        /// <summary>
        /// 创建不带索引缓冲区的圖元
        /// </summary>
        /// <param name="primitive_mode">The primitive_mode.</param>
        /// <param name="vertices">The vertices.</param>
        /// <param name="vertices_byte_count">The vertices_byte_count.</param>
        /// <param name="vertices_count">The vertices_count.</param>
        /// <param name="vb_usage">The vb_usage.</param>
        /// <param name="vtx_attri_arraty">The vtx_attri_arraty.</param>
        void Create(GLenum primitive_mode, GLvoid* vertices, GLsizeiptr vertices_byte_count, GLint vertices_count, GLenum vb_usage, const std::vector<VertexAttribute>& vtx_attri_arraty);

        /// <summary>
        /// 绘制使用了索引缓冲区的图元
        /// </summary>
        void DrawIndexed();

        /// <summary>
        /// 绘制未使用索引缓冲区的图元
        /// </summary>
        void Draw();

        /// <summary>
        ///  绘制未使用索引缓冲区的图元，并且指定待绘制的起始slot和长度
        /// </summary>
        /// <param name="first_slot">The first_slot.</param>
        /// <param name="drawn_vertices_count">The drawn_vertices_count.</param>
        void Draw(GLint first_slot, GLsizei drawn_vertices_count);

        /// <summary>
        /// Updates all vertex data.
        /// </summary>
        /// <param name="src_data">The src_data.</param>
        /// <param name="src_data_byte_count">The src_data_byte_count.</param>
        void UpdateAllVertexData(const void* src_data, std::size_t src_data_byte_count);

        /// <summary>
        /// 指定待绘制的顶点数，仅对Draw方法有效
        /// </summary>
        /// <param name="count">The count.</param>
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