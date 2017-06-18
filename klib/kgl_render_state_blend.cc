// 封装了透明混合相关的渲染状态的object
#include "kgl_lib_pch.h"
#include "kgl_render_state_blend.h"

namespace kgl
{
    RenderStateBlend::RenderStateBlend()
    {

    }

    void RenderStateBlend::Enable(bool enable)
    {
        if (enable)
        {
            glEnable(GL_BLEND);
        }
        else
        {
            glDisable(GL_BLEND);
        }
    }

    void RenderStateBlend::SetBlendFunction(BlendFunc src_func, BlendFunc dst_func)
    {
        glBlendFunc(src_func, dst_func);
    }
}