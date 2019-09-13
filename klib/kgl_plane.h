/**************************************************************************************************************************
Copyright(C) 2014-2018 www.xionggf.com

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
 * \file kgl_plane.h
 * \date 2018/02/12 11:42
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: 平面类
 *
 * \note
*/
#ifndef kgl_plane_h__
#define kgl_plane_h__

namespace kgl
{
    class Plane
    {
    private:
        /// <summary>
        /// 平面的面法线
        /// </summary>
        glm::vec3 normal_;

        /// <summary>
        /// 平面上的一个点
        /// </summary>
        glm::vec3 point_;

        /// <summary>
        /// 截距
        /// </summary>
        float d_;
    public:
        inline const glm::vec3& Normal() const
        {
            return normal_;
        }

        /// <summary>
        /// 类<see cref="Plane"/>的构造函数.用三个点确定一个平面
        /// </summary>
        /// <param name="v1">The v1.</param>
        /// <param name="v2">The v2.</param>
        /// <param name="v3">The v3.</param>
        Plane(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);
        
        /// <summary>
        /// 类<see cref="Plane"/>的缺省构造函数
        /// </summary>
        Plane();
        
        /// <summary>
        /// 类<see cref="Plane"/>的缺省构造函数
        /// </summary>
        ~Plane();

        /// <summary>
        /// 用三个点确定一个平面
        /// </summary>
        /// <param name="v1">The v1.</param>
        /// <param name="v2">The v2.</param>
        /// <param name="v3">The v3.</param>
        void SetThreePoints(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);

        /// <summary>
        /// 用平面上的一个点和法线确定一个平面
        /// </summary>
        /// <param name="normal">The normal.</param>
        /// <param name="point">The point.</param>
        void SetNormalAndPoint(const glm::vec3 &normal, const glm::vec3 &point);

        /// <summary>
        /// 设置平面方程ax+by+cz+d=0的四个系数a，b，c，d
        /// 其中a,b,c是平面的面法线
        /// </summary>
        /// <param name="a">a.</param>
        /// <param name="b">The b.</param>
        /// <param name="c">The c.</param>
        /// <param name="d">The d.</param>
        void SetCoefficients(float a, float b, float c, float d);

        /// <summary>
        /// 点P到本平面的距离
        /// </summary>
        /// <param name="p">The p.</param>
        /// <returns>float.</returns>
        float Distance(const glm::vec3 &p);

        void InfoPrint();
    };
}
#endif // kgl_plane_h__