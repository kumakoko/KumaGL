// 调试工具
#ifndef kgl_debug_tool_h__
#define kgl_debug_tool_h__

namespace kgl
{
	class DebugTool
	{
	public:
		/// <summary>
		/// 在IDE控制台中打印调试信息
		/// </summary>
		/// <param name="message">待打印出来的调试信息.</param>
		static void OutputDebugMessage(const char* message);

		static void PrintMatrix(const char* matrix_name, const glm::mat4& matrix);

		static void PrintMatrix(const char* matrix_name, const aiMatrix4x4& matrix);

		static void PrintGLMMatrix(const char* matrix_name, const glm::mat4& matrix, bool column_major = true);
	};
}
#endif // kgl_debug_tool_h__