// 应用程序类
#ifndef kgl_app_h__
#define kgl_app_h__

#include "kgl_renderer.h"
#include "kgl_camera.h"

namespace kgl
{
	class App
	{
	public:
		enum GLProfile
		{
			ANY,
			CORE,
			COMPAT
		};
	public:
		App();
		virtual ~App();
		virtual void InitWindow(int wnd_width, int wnd_height, bool wnd_resizable, const char* wnd_title, int context_version_major = 3, int context_version_minor = 3, App::GLProfile profile = App::CORE);
		virtual void InitRenderer();
		virtual void InitScene();
		void Run();
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
		static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
		static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	protected:
		static App* s_instance_;
	protected:
		virtual void ProcessInput();
		virtual void PreRenderFrame();
		virtual void RenderFrame();
		virtual void PostRenderFrame();
		virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode);
		virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos);
		virtual void OnScrollAction(GLFWwindow* window, double xoffset, double yoffset);
	protected:
		GLFWwindow*			window_handle_;
		RendererPtr			renderer_;
		CameraPtr			main_camera_;
		std::bitset<384>	key_state_;		// 维护键盘是否按下的状态bit
	};
}

#endif // kgl_app_h__

