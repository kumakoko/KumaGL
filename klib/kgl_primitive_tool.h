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
        /// 创建一个带有一层纹理映射坐标的，边长为一个单位大小的立方体
        /// 本立方体的頂點是以逆时针方向排序的
        /// </summary>
        /// <returns>回图元对象的智能指针</returns>
        static PrimitiveSPtr BuildTexturedCube();


        /// <summary>
        /// 生成一个带一层纹理坐标的平面，本平面在初始生成是和XZ平面平行。
        /// 本平面也是使用顺时针进行排列
        /// </summary>
        /// <param name="y_height">y轴方向上的高度值</param>
        /// <param name="x_scale">x轴方向上的缩放值.</param>
        /// <param name="z_scale">z轴方向上的缩放值</param>
        /// <param name="texture_repeart">T貼圖的重複次數</param>
        /// <returns>返回图元对象的智能指针</returns>
        static PrimitiveSPtr BuildTexturedXZPlane(float y_height = 0.0f, float x_scale = 1.0f, float z_scale = 1.0f, int texture_repeart = 1);
        
        /// <summary>
        /// 创建一个基于NDC坐标系的，带一层纹理的矩形
        /// </summary>
        /// <param name="left">矩形的最左边的x坐标</param>
        /// <param name="right">矩形的最右边的x坐标</param>
        /// <param name="top">矩形的最上边的y坐标</param>
        /// <param name="bottom">矩形的最下边的y坐标</param>
        /// <param name="texture_repeart">貼圖的重複次數</param>
        /// <returns>返回图元对象的智能指针</returns>
        static PrimitiveSPtr BuildNDCTexturedRectange(float left = -1.0f, float right = 1.0f, float top = 1.0f, float bottom = -1.0f, int texture_repeat = 1);

        /// <summary>
        /// Builds the square.
        /// </summary>
        /// <param name="quality">The quality.</param>
        /// <param name="is_clock_wise">The is_clock_wise.</param>
        /// <param name="vertex_color">The vertex_color.</param>
        /// <param name="textureOffset">The texture offset.</param>
        /// <param name="textureScale">The texture scale.</param>
        /// <returns>PrimitiveSPtr.</returns>
        static PrimitiveSPtr BuildSquare(unsigned int quality, bool is_clock_wise, const glm::vec4& vertex_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
            const glm::vec2& textureOffset = glm::vec2(0.0f, 0.0f), const glm::vec2& textureScale = glm::vec2(1.0f, 1.0f));

        /// <summary>
        /// 创建一个带有位置坐标，纹理映射坐标，法线值，顶点颜色值的球体
        /// </summary>
        /// <param name="quality">球体的精度</param>
        /// <param name="is_ccw_cull">是否使用逆时针旋转.</param>
        /// <param name="vertex_color">The vertex_color.</param>
        /// <param name="textureOffset">The texture offset.</param>
        /// <param name="textureScale">The texture scale.</param>
        /// <returns>PrimitiveSPtr.</returns>
        static PrimitiveSPtr BuildSphere(uint32_t quality, bool is_ccw_cull, const glm::vec4& vertex_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec2& texture_offset = glm::vec2(0.0f, 0.0f), const glm::vec2& texture_scale = glm::vec2(1.0f, 1.0f));

        /// <summary>
        /// 创建一个带有位置坐标，法线，切线，副法线，第一层纹理映射坐标的XY平面上的正方体面
        /// </summary>
        /// <param name="quality">The quality.</param>
        /// <param name="is_clock_wise">The is_clock_wise.</param>
        /// <returns>PrimitiveSPtr.</returns>
        static PrimitiveSPtr BuildPNTBT1Square(unsigned int quality, bool is_clock_wise);
    };
}
#endif // kgl_primitive_tool_h__