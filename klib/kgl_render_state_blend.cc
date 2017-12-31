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
#include "kgl_lib_pch.h"
#include "kgl_render_state_blend.h"

namespace kgl
{
    RenderStateBlend::RenderStateBlend()
    {

    }

    RenderStateBlend::RenderStateBlend(GLboolean enable, GLenum src_factor, GLenum dst_factor) :
    enable_(enable),
    src_factor_(src_factor),
    dst_factor_(dst_factor)
    {

    }

    void RenderStateBlend::SetEnable(GLboolean enable)
    {
        enable_ = enable;
    }

    void RenderStateBlend::SetBlendFunction(GLenum src_factor, GLenum dst_factor)
    {
        src_factor_ = src_factor;
        dst_factor_ = dst_factor;
    }

    void RenderStateBlend::Use()
    {
        if (enable_)
        {
            glEnable(GL_BLEND);
            glBlendFunc(src_factor_, dst_factor_);
        }
        else
        {
            glDisable(GL_BLEND);
        }
    }

    void RenderStateBlend::TakeSnapshotState(RenderStateBlend& blend_mode)
    {
        GLboolean enabled_snapshot = glIsEnabled(GL_BLEND); // 取得当前时刻是否启用混合
        GLint src_mode;
        GLint dst_mode;
        glGetIntegerv(GL_BLEND_SRC,&src_mode);
        glGetIntegerv(GL_BLEND_DST,&dst_mode);
        blend_mode.SetEnable(enabled_snapshot);
        blend_mode.SetBlendFunction(src_mode,dst_mode);
    }
}