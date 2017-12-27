/*!
 * \file kgl_render_state_draw_mode.h
 * \date 2017/11/27 17:48
 *
 * \author xiongxinke
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: 封装绘制模式
 *
 * \note
*/

#ifndef kgl_render_state_draw_mode_h__
#define kgl_render_state_draw_mode_h__

namespace kgl
{
	enum DrawMode
	{
		DM_POINT = GL_POINT,
		DM_LINE = GL_LINE,
		DM_FILL = GL_FILL
	};

	class RenderStateDrawMode
	{
	private:
		DrawMode current_draw_mode_;
	public:
		void SetCurrentDrawMode(DrawMode current_mode);

		inline DrawMode GetCurrentDrawMode()
		{
			return current_draw_mode_;
		}
	};
}

#endif // kgl_render_state_draw_mode_h__