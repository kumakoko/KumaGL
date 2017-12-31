// 字体图元
/*!
 * \file kgl_font_primitive.h
 * \date 2017/12/31 13:38
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
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
#ifndef kgl_font_primitive_h__
#define kgl_font_primitive_h__

#include "kgl_primitive.h"
#include "kgl_vertex_attribute.h"
#include "kgl_font_texture.h"

namespace kgl
{
    class FontPrimitive
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="FontPrimitive"/> class.
        /// </summary>
        /// <param name="char_count">The char_count.</param>
        FontPrimitive(int32_t char_count);

        /// <summary>
        /// Finalizes an instance of the <see cref="FontPrimitive"/> class.
        /// </summary>
        ~FontPrimitive();

        /// <summary>
        /// Creates this instance.
        /// </summary>
        void Create();

        /// <summary>
        /// Draws this instance.
        /// </summary>
        void Draw();

        /// <summary>
        /// Sets the use.
        /// </summary>
        /// <param name="used">The used.</param>
        inline void SetUse(bool used)
        {
            used_ = used;
        }

        /// <summary>
        /// Determines whether this instance is used.
        /// </summary>
        /// <returns>bool.</returns>
        inline bool IsUsed() const
        {
            return used_;
        }

        /// <summary>
        /// Actuals the vertices count.
        /// </summary>
        /// <returns>std.int32_t.</returns>
        inline std::int32_t ActualVerticesCount() const
        {
            return actual_vertices_count_;
        }

        /// <summary>
        /// Updates the data.
        /// </summary>
        /// <param name="src_data">The src_data.</param>
        /// <param name="src_data_byte_count">The src_data_byte_count.</param>
        /// <param name="drawn_vertices_count">The drawn_vertices_count.</param>
        void UpdateData(const void* src_data, std::size_t src_data_byte_count, std::int32_t drawn_vertices_count);

        /// <summary>
        /// Gets the font texture.
        /// </summary>
        /// <returns>kgl.FontTexture *.</returns>
        inline FontTexture* GetFontTexture()
        {
            return texture_;
        }

        /// <summary>
        /// Sets the font texture.
        /// </summary>
        /// <param name="t">The t.</param>
        inline void SetFontTexture(FontTexture* t)
        {
            texture_ = t;
        }

    private:
        static std::vector<VertexAttribute> s_vertex_attribute_array;

        static bool s_init_attribute;

        /// <summary>
        /// The primitive_
        /// </summary>
        Primitive* primitive_;
        /// <summary>
        /// The texture_
        /// </summary>
        FontTexture* texture_;

        /// <summary>
        /// The used_
        /// </summary>
        bool used_;

        /// <summary>
        /// 实际的顶点数
        /// </summary>
        std::int32_t actual_vertices_count_;
    };

    struct FontPrimitiveGreater
    {
        bool operator() (const FontPrimitive* p1, const FontPrimitive* p2) const
        {
            if (p1->ActualVerticesCount() > p2->ActualVerticesCount())
                return true;

            return false;
        }
    };
}
#endif // kgl_font_primitive_h__