// 图元辅助工具
#ifndef kgl_primitive_tool_h__
#define kgl_primitive_tool_h__

#include "kgl_primitive.h"

namespace kgl
{
    class PrimitiveTool
    {
	public:
		/// <summary>
		/// 生成一个只带长宽高为1和顶点RBGA颜色的一个方盒
		/// </summary>
		/// <returns>回图元对象的智能指针</returns>
		static PrimitiveSPtr BuildCube();

		/// <summary>
		/// Builds the textured cube.
		/// </summary>
		/// <returns>回图元对象的智能指针</returns>
		static PrimitiveSPtr BuildTexturedCube();


		/// <summary>
		/// 生成一个带一层纹理坐标的平面，本平面在初始生成是和XZ平面平行+
		/// </summary>
		/// <param name="y_height">y轴方向上的高度值</param>
		/// <param name="x_scale">x轴方向上的缩放值.</param>
		/// <param name="z_scale">z轴方向上的缩放值</param>
		/// <param name="texture_repeart">T貼圖的重複次數</param>
		/// <returns>返回图元对象的智能指针</returns>
		static PrimitiveSPtr  BuildTexturedXZPlane(float y_height = 0.0f, float x_scale = 1.0f, float z_scale = 1.0f, int texture_repeart = 1);


		/// <summary>
		/// 创建一个基于NDC坐标系的，带一层纹理的矩形
		/// </summary>
		/// <param name="left">矩形的最左边的x坐标</param>
		/// <param name="right">矩形的最右边的x坐标</param>
		/// <param name="top">矩形的最上边的y坐标</param>
		/// <param name="bottom">矩形的最下边的y坐标</param>
		/// <param name="texture_repeart">貼圖的重複次數</param>
		/// <returns>返回图元对象的智能指针</returns>
		static PrimitiveSPtr BuildNDCTexturedRectange(float left = -1.0f, float right = 1.0f, float top = 1.0f, float bottom = -1.0f, int texture_repeart = 1);
    };
}
#endif // kgl_primitive_tool_h__