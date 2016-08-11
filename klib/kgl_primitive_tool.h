// 图元辅助工具
#ifndef kgl_primitive_tool_h__
#define kgl_primitive_tool_h__

#include "kgl_primitive.h"

namespace kgl
{
	class PrimitiveTool
	{
	public:
		//************************************
		// Method:    BuildCube
		// FullName:  kgl::PrimitiveTool::BuildCube
		// Access:    public static 
		// Returns:   kgl::PrimitiveSPtr
		// Qualifier: 生成一个只带长宽高为1和顶点RBGA颜色的一个方盒
		//************************************
		static PrimitiveSPtr BuildCube();
	};
}
#endif // kgl_primitive_tool_h__