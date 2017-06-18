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

        static PrimitiveSPtr BuildTexturedCube();

        //************************************
        // Method:    BuildTexturedPlane
        // FullName:  kgl::PrimitiveTool::BuildTexturedPlane
        // Access:    public static 
        // Returns:   kgl::PrimitiveSPtr
        // Qualifier: 生成一个带一层纹理坐标的平面，本平面在初始生成是和XZ平面平行+
        // Parameter: float y_height y轴方向上的高度值
        // Parameter: float x_scale x轴方向上的缩放值
        // Parameter: float z_scale x轴方向上的缩放值
        // Parameter: int texture_repeart 貼圖的重複次數
        //************************************
        static PrimitiveSPtr  BuildTexturedXZPlane(float y_height = 0.0f, float x_scale = 1.0f, float z_scale = 1.0f, int texture_repeart = 1);


        //************************************
        // Method:    BuildNDCTexturedRectange
        // FullName:  kgl::PrimitiveTool::BuildNDCTexturedRectange
        // Access:    public static 
        // Returns:   kgl::PrimitiveSPtr
        // Qualifier: 创建一个基于NDC坐标系的，带一层纹理的矩形
        // Parameter: float left    矩形的最左边的x坐标
        // Parameter: float right   矩形的最右边的x坐标
        // Parameter: float top     矩形的最上边的y坐标
        // Parameter: float bottom  矩形的最下边的y坐标
        // Parameter: int texture_repeart
        //************************************
        static PrimitiveSPtr BuildNDCTexturedRectange(float left = -1.0f, float right = 1.0f, float top = 1.0f, float bottom = -1.0f, int texture_repeart = 1);
    };
}
#endif // kgl_primitive_tool_h__