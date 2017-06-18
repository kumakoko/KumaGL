// 封装了深度测试相关的渲染状态的object
#include "kgl_lib_pch.h"
#include "kgl_render_state_depth.h"

namespace kgl
{ 
    void RenderStateDepth::Enable(bool enable)
    {
        if (enable)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
    }

	void RenderStateDepth::Enable(GLboolean enable)
	{
		if (GL_TRUE == enable)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void RenderStateDepth::SetDepthTestFunc(GLenum func, GLboolean enabled)
    {
		GLboolean used = glIsEnabled(GL_DEPTH_TEST);

		if (GL_TRUE == used)
		{
			if (enabled == GL_TRUE)
				glDepthFunc(func);
			else
				glDisable(GL_DEPTH_TEST);
		}
		else
		{
			if (enabled == GL_TRUE)
			{
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(func);
			}
		}
    }

	void RenderStateDepth::TakeSnapshotState(GLboolean* enabled, GLenum* taken_depth_func)
	{
		depth_test_enable_snapshot_ = glIsEnabled(GL_DEPTH_TEST);
		glGetIntegerv(GL_DEPTH_FUNC, 
			reinterpret_cast<GLint*>(&depth_test_func_snapshot_)); // 当前时刻取得深度测试函数值

		if (nullptr != taken_depth_func)
			*taken_depth_func = depth_test_func_snapshot_;

		if (nullptr != enabled)
			*enabled = depth_test_enable_snapshot_;
	}

	void RenderStateDepth::UseSnapshotState()
	{
		if (GL_TRUE == depth_test_enable_snapshot_)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		glDepthFunc(depth_test_func_snapshot_);
	}
}