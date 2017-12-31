// 文字渲染器
/*!
 * \file kgl_font_renderer.h
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
#ifndef kgl_font_renderer_h__
#define kgl_font_renderer_h__

#include "kgl_primitive.h"
#include "kgl_vertex_attribute.h"
#include "kgl_gpu_program.h"
#include "kgl_font_texture.h"
#include "kgl_font_primitive.h"
#include "kgl_render_state_depth.h"
#include "kgl_render_state_blend.h"
#include "kgl_render_state_cull_mode.h"
#include "kgl_render_state_draw_mode.h"
#include "dp/kgl_singleton.h"

namespace kgl
{
    class FontRenderer
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="FontRenderer"/> class.
        /// </summary>
        FontRenderer();

        /// <summary>
        /// Finalizes an instance of the <see cref="FontRenderer"/> class.
        /// </summary>
        ~FontRenderer();
        
        /// <summary>
        /// Initializes this instance.
        /// </summary>
        void Initialize();

        /// <summary>
        /// Adds to rendered.
        /// </summary>
        /// <param name="text">The text.</param>
        /// <param name="start_x">The start_x.</param>
        /// <param name="start_y">The start_y.</param>
        /// <param name="left_top_color">The left_top_color.</param>
        /// <param name="left_bottom_color">The left_bottom_color.</param>
        /// <param name="right_top_color">The right_top_color.</param>
        /// <param name="right_bottom_color">The right_bottom_color.</param>
        /// <param name="scale">The scale.</param>
        void AddToRendered(const std::wstring& text, int32_t start_x, int32_t start_y,
            const glm::vec4& left_top_color = glm::vec4(1.0f,1.0f,1.0f,1.0f),
            const glm::vec4& left_bottom_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 
            const glm::vec4& right_top_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 
            const glm::vec4& right_bottom_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
            float scale = 1.0f);

        /// <summary>
        /// Adds to rendered.
        /// </summary>
        /// <param name="text">The text.</param>
        /// <param name="start_x">The start_x.</param>
        /// <param name="start_y">The start_y.</param>
        /// <param name="color">The color.</param>
        /// <param name="scale">The scale.</param>
        void AddToRendered(const std::wstring& text, int32_t start_x, int32_t start_y,
            const glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
            float scale = 1.0f);

        /// <summary>
        /// Draws this instance.
        /// </summary>
        void Draw();

        /// <summary>
        /// Creates the font texture.
        /// </summary>
        /// <param name="font_file_name">The font_file_name.</param>
        /// <param name="texture_name">The texture_name.</param>
        /// <param name="font_size">The font_size.</param>
        /// <param name="texture_width">The texture_width.</param>
        /// <param name="texture_height">The texture_height.</param>
        void CreateFontTexture(const char* font_file_name, const char* texture_name, int32_t font_size, uint32_t texture_width, uint32_t texture_height);
        
        /// <summary>
        /// 指定当前使用的字体+字号名称的纹理图
        /// </summary>
        /// <param name="texture_name">字模所处的纹理贴图的名字.</param>
        void SetCurrentFont(const std::string& texture_name);
    private:
        /// <summary>
        /// Screens to NDC.
        /// </summary>
        /// <param name="x">The x.</param>
        /// <param name="y">The y.</param>
        /// <param name="ndc_x">The NDC_X.</param>
        /// <param name="ndc_y">The ndc_y.</param>
        void ScreenToNDC(int32_t x, int32_t y, float* ndc_x, float* ndc_y);

        /// <summary>
        /// 从未使用的文字图元集里面返回一个图元
        /// </summary>
        /// <param name="char_count">The char_count.</param>
        /// <returns>kgl.FontPrimitive *.</returns>
        FontPrimitive* GetUnusedFontPrimitive(int32_t char_count);
    protected:
        /// <summary>
        /// The font_shader_
        /// </summary>
        GPUProgram* font_shader_;

        /// <summary>
        /// The view_port_width_
        /// </summary>
        float view_port_width_;

        /// <summary>
        /// The view_port_height_
        /// </summary>
        float view_port_height_;

        /// <summary>
        /// The unused_primitive_set_
        /// </summary>
        std::set<FontPrimitive*, FontPrimitiveGreater>  unused_primitive_set_;

        /// <summary>
        /// The used_primitive_array_
        /// </summary>
        std::vector<FontPrimitive*>                     used_primitive_array_;

        /// <summary>
        ///字体名-字体纹理映射表
        /// </summary>
        std::map<std::string, FontTexture*>              font_texture_map_;

        /// <summary>
        /// The current_font_texture_
        /// </summary>
        FontTexture*                                    current_font_texture_;

        /// <summary>
        /// 绘制文字时用到的深度缓冲区状态对象
        /// </summary>
        RenderStateDepth rs_depth_not_enabled_;

		/// <summary>
		/// The rs_cull_draw_
		/// </summary>
		RenderStateCullMode rs_cull_draw_;

        /// <summary>
        /// 绘制文字时用到的混合状态对象
        /// </summary>
        RenderStateBlend rs_blend_;

		/// <summary>
		/// 绘制文字时用到的填充状态
		/// </summary>
		RenderStateDrawMode draw_mode_;
    };

    typedef dp::Singleton<FontRenderer> KFontRenderer;
}
#endif // kgl_font_renderer_h__