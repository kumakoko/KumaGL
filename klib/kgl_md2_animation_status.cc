#include "kgl_lib_pch.h"
#include "kgl_md2_animation_status.h"

namespace kgl
{
	void MD2AnimationStatus::set_type(const char* val)
	{
		memset(type_, 0, sizeof(char)*ANIM_TYPE_NAME_LEN);
		std::size_t len = strlen(val);
		len = len >= ANIM_TYPE_NAME_LEN ? ANIM_TYPE_NAME_LEN - 1 : len;
		//strcpy_s(type_, len, val);
		strcpy(type_, val);
	}

	bool MD2AnimationStatus::IsInvalidStatus() const
	{
		return start_frame() == -1 && end_frame() == -1 && fps() == -1;
	}

	MD2AnimationStatus::MD2AnimationStatus() :start_frame_(-1), end_frame_(-1), fps_(-1)
	{
		memset(type_, 0, sizeof(char)*ANIM_TYPE_NAME_LEN);
	}

	bool MD2AnimationStatus::IsShouldUseNextFrame() const
	{
		return current_time() - old_time() > 1.0f / static_cast<float>(fps());
	}
}