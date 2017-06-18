// 封装了深度测试相关的渲染状态的object
#ifndef kgl_render_state_depth_h__
#define kgl_render_state_depth_h__

namespace kgl
{
    enum class DepthTestFunct
    {
        DTF_ALWAYS = GL_ALWAYS,     // The depth test always passes.
        DTF_NEVER = GL_NEVER,       // The depth test never passes.
        DTF_LESS = GL_LESS,         // Passes if the fragment's depth value is less than the stored depth value.
        DTF_EQUA = GL_EQUAL,        // Passes if the fragment's depth value is equal to the stored depth value.
        DTF_LEQUA = GL_LEQUAL,      // Passes if the fragment's depth value is less than or equal to the stored depth value.
        DTF_GREATER = GL_GREATER,   // Passes if the fragment's depth value is greater than the stored depth value.
        DTF_NOTEQUAL = GL_NOTEQUAL, // Passes if the fragment's depth value is not equal to the stored depth value.
        DTF_GEQUA = GL_GEQUAL       // Passes if the fragment's depth value is greater than or equal to the stored depth value.
    };

    class RenderStateDepth
    {
    private:
		GLenum depth_test_func_snapshot_;		// 当前时刻取得深度测试函数值
		GLboolean depth_test_enable_snapshot_;	// 当前时刻取得的是否启用深度测试的布尔值
    public:
        //************************************
        // Method:    Enable
        // FullName:  kgl::RenderStateDepth::Enable
        // Access:    public 
        // Returns:   void
        // Details:   启用或者关闭深度测试
        // Parameter: bool enable
        //************************************
        void Enable(bool enable);

		//************************************
		// Method:    Enable
		// FullName:  kgl::RenderStateDepth::Enable
		// Access:    public 
		// Returns:   void
		// Details:   启用或者关闭深度测试
		// Parameter: GLboolean enable
		//************************************
		void Enable(GLboolean enable);

        //************************************
        // Method:    SetDepthTestFunc
        // FullName:  kgl::RenderStateDepth::SetDepthTestFunc
        // Access:    public 
        // Returns:   void
        // Details:   设置深度测试函数
        // Parameter: GLenum func 启用深度状态的话，设置深度测试函数
		// Parameter: GLboolean enabled 是否启用
        //************************************
		void SetDepthTestFunc(GLenum func, GLboolean enabled = GL_TRUE);

		//************************************
		// Method:    TakeSnapshotState
		// FullName:  kgl::RenderStateDepth::TakeSnapshotState
		// Access:    public 
		// Returns:   void
		// Qualifier: 保存调用本函数时的深度测试是否已经启用，以及对应的深度测试函数
		// Parameter: GLboolean * enabled  如果不为nullptr，则返回调用本函数时的是否启用深度测试状态的布尔值
		// Parameter: GLenum * taken_depth_func 如果不为nullptr，则返回调用本函数时，深度测试函数的值
		//************************************
		void TakeSnapshotState(GLboolean* enabled = nullptr, GLenum* taken_depth_func = nullptr);

		//************************************
		// Method:    UseSnapshotState
		// FullName:  kgl::RenderStateDepth::UseSnapshotState
		// Access:    public 
		// Returns:   void
		// Details: 使用先前保存的状态值
		//************************************
		void UseSnapshotState();
    };
}
#endif // kgl_render_state_depth_h__