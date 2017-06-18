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