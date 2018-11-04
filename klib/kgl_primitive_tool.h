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
* \file kgl_primitive_tool.h
* \date 2017/12/31 13:45
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief
*
* TODO: 图元辅助工具
*
* \note
*/
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
        /// 创建一个带有一层纹理映射坐标的，有法线的，和XZ平面平行的平面
        /// </summary>
        /// <param name="size">平面的大小</param>
        /// <param name="y_offset">和Y轴原点的偏移量</param>
        /// <returns>回图元对象的智能指针</returns>
        static PrimitiveSPtr BuildNormalTexturedXZPlane(float size, float y_offset);

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

        static PrimitiveSPtr BuildNDCTextureNormalCube();
    };
}
#endif // kgl_primitive_tool_h__