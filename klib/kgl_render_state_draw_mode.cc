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
// 封装绘制模式
#include "kgl_lib_pch.h"
#include "kgl_render_state_draw_mode.h"

namespace kgl
{
    void RenderStateDrawMode::SetCurrentDrawMode(DrawMode current_mode)
    {
        if (DM_POINT == current_mode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            current_draw_mode_ = current_mode;
        }
        else if (DM_LINE == current_mode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            current_draw_mode_ = current_mode;
        }
        else if (DM_FILL == current_mode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            current_draw_mode_ = current_mode;
        }
    }
}