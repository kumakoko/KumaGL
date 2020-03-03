/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
/*!
 * \file kgl_app.h
 * \date 2017/12/31 13:34
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief
 *
 * TODO: 应用程序类的基类
 *
 * \note
 */

#ifndef kgl_app_h__
#define kgl_app_h__

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
		/// <summary>
		/// 类<see cref="App"/>的构造函数
		/// </summary>
		App();

		/// <summary>
		/// 类 <see cref="App"/>的析构函数
		/// </summary>
		virtual ~App();

		/*********************************************************
		初始化主窗口

		@param  int32_t wnd_width 窗口的宽度
		@param  int32_t wnd_height 窗口的高度
		@param  bool wnd_resizable 窗口可以调整大小吗
		@param  const char * wnd_title 窗口的标题
		@param  int32_t context_version_major GL设备上下文的主版本
		@param  int32_t context_version_minor GL设备上下文的次版本
		@param  App::GLProfile profile GL的profile
		*********************************************************/
		virtual void InitWindow(int32_t wnd_width, int32_t wnd_height, bool wnd_resizable, const char* wnd_title, int32_t context_version_major = 3, int32_t context_version_minor = 3, App::GLProfile profile = App::CORE);

		/*********************************************************
		初始化OpenGL渲染器
		*********************************************************/
		virtual void InitRenderer();

		/*********************************************************
		初始化GUI系统

		@param  bool use_gui 是否使用ImGui系统
		@param  bool install_callbacks 是否安装对键盘鼠标等外设信息的响应回调
		@param  bool install_key_callback 是否响应对键盘输入并且安装回调
		@param  bool install_scroll_callback 是否响应对鼠标滚轮的输入并且安装回调
		@param  bool install_char_callback 是否响应对字符信息的输入并且安装回调
		@param  bool install_mouse_btn_callback 是否响应对鼠标按键的输入并且安装回调
		*********************************************************/
		void InitGuiSystem(bool use_gui = true, bool install_callbacks = true, bool install_key_callback = true, bool install_scroll_callback = true,
			bool install_char_callback = true, bool install_mouse_btn_callback = true);

		/*********************************************************
		初始化demo场景
		*********************************************************/
		virtual void InitScene();

		/*********************************************************
		应用程序运行的主循环
		*********************************************************/
		void Run();

		/*********************************************************
		对键盘事件的回调的入口函数

		@param  GLFWwindow * window
		@param  int key
		@param  int scancode
		@param  int action
		@param  int mode
		*********************************************************/
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

		/*********************************************************
		对鼠标事件的回调的入口函数

		@param  GLFWwindow * window
		@param  double xpos
		@param  double ypos
		*********************************************************/
		static void MouseCallback(GLFWwindow* window, double xpos, double ypos);

		/// <summary>
		/// Scrolls the callback.
		/// </summary>
		/// <param name="window">The window.</param>
		/// <param name="xoffset">The xoffset.</param>
		/// <param name="yoffset">The yoffset.</param>
		static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		/// <summary>
		/// Sizes the changed callback.
		/// </summary>
		/// <param name="window">The window.</param>
		/// <param name="width">The width.</param>
		/// <param name="height">The height.</param>
		static void SizeChangedCallback(GLFWwindow* window, int width, int height);

		/*********************************************************
		对 点击鼠标时的事件的处理的入口函数
		@param  GLFWwindow * window 接受消息的window窗口实例指针
		@param  int received_event 窗口接受到的事件
		@param  int button 鼠标上的按键
		@param  int action 按键动作，要么是GLFW_PRESS，要么是GLFW_RELEASE       
		*********************************************************/
		static void MouseButtonCallback(GLFWwindow* window, int received_event, int button, int action);

		/*********************************************************
		对GLFW错误消息的处理的入口函数
		@param  int error_code
		@param  const char * err_str
		@return 
		@see function  
		@note           
		@attention      
		@bug            
		@warning        
		*********************************************************/
		static void GLFWErrorCallback(int error_code, const char* err_str);

		/// <summary>
		/// Characters the callback.
		/// </summary>
		/// <param name="window">The window.</param>
		/// <param name="x">The x.</param>
		static void CharCallback(GLFWwindow *window, unsigned int x);
	protected:
		static App* s_instance_;
	protected:
		/*********************************************************
		初始化场景中使用到的材质结构
		*********************************************************/
		virtual void InitMaterials();

		/*********************************************************
		初始化场景中的模型
		*********************************************************/
		virtual void InitModel();

		/*********************************************************
		初始化场景中的各个shader
		*********************************************************/
		virtual void InitShaders();

		/*********************************************************
		初始化场景中的各个动态实时光源
		*********************************************************/
		virtual void InitLights();

		/*********************************************************
		初始化场景的主摄像机
		*********************************************************/
		virtual void InitMainCamera();

		/*********************************************************
		初始化字体库
		*********************************************************/
		virtual void InitFont();

		/*********************************************************
		对外设输入事件的处理
		*********************************************************/
		virtual void ProcessInput();

		virtual void PreRenderFrame();

		/*********************************************************
		渲染每一帧
		*********************************************************/
		virtual void RenderScene();

		/*********************************************************
		 渲染GUI系统
		 *********************************************************/
		virtual void RenderGUI();

		void PreRenderGUI();

		void PostRenderGUI();

		virtual void PostRenderFrame();

		/*********************************************************
		对键盘消息的处理回调函数，如果需要，每个App类的子类可重载实现之
		@param  GLFWwindow * window
		@param  int key
		@param  int scancode
		@param  int action
		@param  int mode
		*********************************************************/
		virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode);

		/*********************************************************
		对输入鼠标消息的处理回调函数，如果需要，每个App类的子类可重载实现之
		@param  GLFWwindow * window
		@param  double xpos
		@param  double ypos
		*********************************************************/
		virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos);

		/*********************************************************
		对输入鼠标滚轮消息的处理回调函数，如果需要，每个App类的子类可重载实现之
		@param  GLFWwindow * window
		@param  double xoffset
		@param  double yoffset
		*********************************************************/
		virtual void OnScrollAction(GLFWwindow* window, double xoffset, double yoffset);

		/*********************************************************
		对输入窗口的大小发生变化的处理回调函数，如果需要，每个App类的子类可重载实现之
		@param  GLFWwindow * window
		@param  int width
		@param  int height
		*********************************************************/
		virtual void OnSizeChangedAction(GLFWwindow* window, int width, int height);

		/*********************************************************
		对输入字符消息的处理回调函数，如果需要，每个App类的子类可重载实现之
		@param  GLFWwindow * window
		@param  unsigned int x
		*********************************************************/
		virtual void OnCharAction(GLFWwindow *window, unsigned int x);

		/*********************************************************
		对输入GLFW框架的错误信息的处理回调函数，如果需要，每个App类的子类可重载实现之
		@param  int error_code
		@param  const char * err_str
		*********************************************************/
		virtual void OnGLFWErrorCallback(int error_code, const char* err_str);

		/*********************************************************
		对输入鼠标按键消息的处理回调函数，如果需要，每个App类的子类可重载实现之
		@param  GLFWwindow * window
		@param  int received_event
		@param  int button
		@param  int action
		*********************************************************/
		virtual void OnMouseButtonAction(GLFWwindow* window, int received_event, int button, int action);

		/*********************************************************
		根据GL错误枚举值，获取到该GL错误值的描述字符串
		@param  GLenum err 待打印出描述字符串的错误枚举值
		@return 返回GL错误值的描述字符串
		*********************************************************/
		const char* GetGLErrorDescription(GLenum err);

		void CheckGLError();
	protected:
		/** * @brief GLFW窗口句柄 */
		GLFWwindow* window_handle_ = nullptr;

		/** * 场景中所用到的主摄像机 */
		CameraSPtr main_camera_;

		/** * @brief 维护键盘是否按下的状态bit */
		std::bitset<384> key_state_;

		/** * @brief 窗口的宽度 */
		int32_t window_width_ = 0;

		/** * @brief 窗口的高度 */
		int32_t window_height_ = 0;

		/** * @brief GL的profile  */
		GLProfile gl_profile_ = GLProfile::ANY;

		/** * @brief 是否使用Dear IMGUI系统 */
		bool use_gui_;
	};
}

#endif // kgl_app_h__

