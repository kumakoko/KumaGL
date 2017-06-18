// 封装了拣选模式
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
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}
}