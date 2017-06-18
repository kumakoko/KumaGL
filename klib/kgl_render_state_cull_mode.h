#ifndef kgl_render_state_cull_mode_h__
#define kgl_render_state_cull_mode_h__

/*
在三维空间中，一个多边形虽然有两个面，但我们无法看见背面的那些多边形，而一些多边形虽然是正面的，但被其他多边形所遮挡。如果将无法看见的多边形和可见的多边形同等对待，无疑会降低我们处理图形的效率。在这种时候，可以将不必要的面剔除。
首先，使用glEnable(GL_CULL_FACE); 来启动剔除功能（使用glDisable(GL_CULL_FACE)可以关闭之）
然后，使用glCullFace来进行剔除。
glCullFace的参数可以是GL_FRONT，GL_BACK或者GL_FRONT_AND_BACK，分别表示剔除正面、剔除反面、剔除正反两面的多边形。
注意：剔除功能只影响多边形，而对点和直线无影响。例如，使用glCullFace(GL_FRONT_AND_BACK)后，所有的多边形都将被剔除，所以看见的就只有点和直线。
*/

namespace kgl
{
    class RenderStateCullMode
    {
    public:
		/// <summary>
		/// Initializes a new instance of the <see cref="RenderStateCullMode"/> class.
		/// </summary>
		RenderStateCullMode();

		RenderStateCullMode(GLboolean enable, GLint cull_mode);

		/// <summary>
		/// 设置本状态是否启用面拣选
		/// </summary>
		/// <param name="enable">GL_TRUE为启用，GL_FALSE为禁用.</param>
		inline void SetEnable(GLboolean enable)
		{
			enable_ = enable;
		}
		
		/// <summary>
		/// 获取本状态是否启用面拣选
		/// </summary>
		/// <returns>GLboolean.</returns>
		inline GLboolean GetEnable() const
		{
			return enable_;
		}

		/// <summary>
		/// 保存一个当前的模式的快照，其实就是保存执行本方法时的函数拣选状态
		/// </summary>
		void TakeSnapshotMode();

		/// <summary>
		/// 使用
		/// </summary>
		void Use();

		/// <summary>
		/// Sets the cull mode.
		/// </summary>
		/// <param name="mode">The function.</param>
		inline void SetCullMode(GLenum mode)
		{
			cull_mode_ = mode;
		}
		
		/// <summary>
		/// Gets the cull mode.
		/// </summary>
		/// <returns>GLint.</returns>
		inline GLint GetCullMode() const
		{
			return cull_mode_;
		}

		static void TakeSnapshotState(RenderStateCullMode& rs_cull_mode);

	private:
		GLboolean enable_ = GL_TRUE;
		GLint cull_mode_ = GL_FRONT;
    };
}

#endif // kgl_render_state_cull_mode_h__