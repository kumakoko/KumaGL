/*!
 * \file kgl_render_state_draw_mode.h
 * \date 2017/11/27 17:48
 *
 * \author xiongxinke
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: 封装绘制模式
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
#ifndef kgl_render_state_draw_mode_h__
#define kgl_render_state_draw_mode_h__

namespace kgl
{
    enum DrawMode
    {
        DM_POINT = GL_POINT,
        DM_LINE = GL_LINE,
        DM_FILL = GL_FILL
    };

    class RenderStateDrawMode
    {
    private:
        DrawMode current_draw_mode_;
    public:
        void SetCurrentDrawMode(DrawMode current_mode);

        inline DrawMode GetCurrentDrawMode()
        {
            return current_draw_mode_;
        }
    };
}

#endif // kgl_render_state_draw_mode_h__