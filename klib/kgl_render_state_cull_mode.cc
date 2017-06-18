// 封装了拣选模式
#include "kgl_lib_pch.h"
#include "kgl_render_state_cull_mode.h"

namespace kgl
{
    RenderStateCullMode::RenderStateCullMode()
    {

    }

    void RenderStateCullMode::Enable(bool enable)
    {
        if (enable)
        {
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
    }

	void RenderStateCullMode::TakeSnapshotMode()
	{
		glGetIntegerv(GL_CULL_FACE_MODE, &taken_mode_snapshot_);
	}

	void RenderStateCullMode::UseSnapshotMode()
	{
		glCullFace(taken_mode_snapshot_);
	}

	void RenderStateCullMode::SetCullMode(GLenum func, GLboolean enabled)
	{
		GLboolean used = glIsEnabled(GL_CULL_FACE);

		if (GL_TRUE == used)
		{
			if (enabled == GL_TRUE)
				glCullFace(func);
			else
				glDisable(GL_CULL_FACE);
		}
		else
		{
			if (enabled == GL_TRUE)
			{
				glEnable(GL_CULL_FACE);
				glCullFace(func);
			}
		}
	}
}