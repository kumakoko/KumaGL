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
#include "kgl_render_state_depth.h"

namespace kgl
{ 
    RenderStateDepth::RenderStateDepth()
    {

    }

    RenderStateDepth::RenderStateDepth(GLboolean enable, GLenum test_func):
    enable_(enable), depth_test_func_(test_func)
    {

    }

    void RenderStateDepth::Use()
    {
        if (GL_TRUE == enable_)
        {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(depth_test_func_);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
    }

    void RenderStateDepth::TakeSnapshotState(RenderStateDepth& rs_depth)
    {
        GLboolean enabled_snapshot = glIsEnabled(GL_DEPTH_TEST); // 取得当前时刻是否启用的深度测试
        GLenum test_func_snapshot;
        glGetIntegerv(GL_DEPTH_FUNC, reinterpret_cast<GLint*>(&test_func_snapshot)); // 当前时刻取得深度测试函数值
        rs_depth.SetEnable(enabled_snapshot);
        rs_depth.SetDepthTestFunc(test_func_snapshot);
    }

    const char* RenderStateDepth::GetTestFuncName(GLuint func)
    {
        switch (func)
        {
        case GL_LESS:return "GL_LESS";
        case GL_EQUAL:return "GL_EQUAL";
        case GL_LEQUAL:return "GL_LEQUAL";
        case GL_GREATER:return "GL_GREATER";
        case GL_NOTEQUAL:return "GL_NOTEQUAL";
        case GL_GEQUAL:return "GL_GEQUAL";
        case GL_ALWAYS:return "GL_ALWAYS";
        default:return "Unknown Depth Test Function";
        }
    }
}