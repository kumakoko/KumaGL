// 封装了透明混合相关的渲染状态的object
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