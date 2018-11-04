/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

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
/*!
* \file kgl_primitive.h
* \date 2017/12/31 13:45
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief
*
* TODO: 基础图元类
*
* \note
*/
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
        /// 更新缓冲区中的所有顶点数据
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

        /// <summary>
        /// 获取顶点数量
        /// </summary>
        /// <returns>GLsizei.</returns>
        inline GLsizei GetVerticesCount() const
        {
            return vertices_count_;
        }

    private:
        /// <summary>
        /// The vertex_buffer_object_
        /// </summary>
        GLuint vertex_buffer_object_;

        /// <summary>
        /// The vertex_attribute_object_
        /// </summary>
        GLuint vertex_attribute_object_;

        /// <summary>
        /// The element_buffer_object_
        /// </summary>
        GLuint element_buffer_object_;

        /// <summary>
        /// The primitive_mode_
        /// </summary>
        GLenum  primitive_mode_;

        /// <summary>
        /// The indices_type_
        /// </summary>
        GLenum  indices_type_;

        /// <summary>
        /// The indices_count_
        /// </summary>
        GLsizei indices_count_;

        /// <summary>
        /// 缓冲区中的顶点数
        /// </summary>
        GLsizei vertices_count_;

        /// <summary>
        /// 指定待绘制的顶点数，仅对Draw方法有效
        /// </summary>
        GLsizei drawn_vertices_count_;
    };

    typedef std::shared_ptr<Primitive> PrimitiveSPtr;
}

#endif // klib_primitive_h__