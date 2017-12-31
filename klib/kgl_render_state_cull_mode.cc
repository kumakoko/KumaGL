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
#include "kgl_render_state_cull_mode.h"

namespace kgl
{
    RenderStateCullMode::RenderStateCullMode()
    {

    }

    RenderStateCullMode::RenderStateCullMode(GLboolean enable, GLint cull_mode) :
        enable_(enable), cull_mode_(cull_mode)
    {

    }

    void RenderStateCullMode::TakeSnapshotState(RenderStateCullMode& rs_cull_mode)
    {
        GLboolean enabled_snapshot = glIsEnabled(GL_CULL_FACE); // 取得当前时刻是否启用面拣选
        GLint cull_mode_snapshot;
        glGetIntegerv(GL_CULL_FACE_MODE, &cull_mode_snapshot); // 当前时刻取得面拣选模式
        rs_cull_mode.SetEnable(enabled_snapshot);
        rs_cull_mode.SetCullMode(cull_mode_snapshot);
    }

    void RenderStateCullMode::Use()
    {
        if (GL_TRUE == enable_)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(cull_mode_);
            glFrontFace(front_face_mode_);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
    }
}