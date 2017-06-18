// 封装了深度测试相关的渲染状态的object
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
}