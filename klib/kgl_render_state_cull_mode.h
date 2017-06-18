#ifndef kgl_render_state_cull_mode_h__
#define kgl_render_state_cull_mode_h__

// 封装了拣选模式的类

namespace kgl
{
    class RenderStateCullMode
    {
    public:
        RenderStateCullMode();
        void Enable(bool enable);

		//************************************
		// Method:    SaveCurrentMode
		// FullName:  kgl::RenderStateCullMode::TakeSnapshotMode
		// Access:    public 
		// Returns:   void
		// Detail:	  保存一个当前的模式的快照，其实就是保存执行
		//            本方法时的函数拣选状态
		//************************************
		void TakeSnapshotMode();

		//************************************
		// Method:    UseSnapshotMode
		// FullName:  kgl::RenderStateCullMode::UseSnapshotMode
		// Access:    public 
		// Returns:   void
		// Detail:    使用先前保存的拣选模式的快照
		//************************************
		void UseSnapshotMode();

		void SetCullMode(GLenum func, GLboolean enabled = GL_TRUE);
	private:
		GLint taken_mode_snapshot_ = GL_FRONT;
    };
}

#endif // kgl_render_state_cull_mode_h__