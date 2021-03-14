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
* \file kgl_math_tools.h
* \date 2017/12/31 13:45
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief
*
* TODO: 一系列的数学工具函数
*
* \note
*/
#ifndef kgl_math_tools_h__
#define kgl_math_tools_h__

namespace kgl
{
    class MathTools
    {
    public:
        static const double PERSISTENCE;
        static const int OCTAVES_COUNT;
    public:
        /// <summary>
        /// Interpolates the cube.
        /// </summary>
        /// <param name="v0">The v0.</param>
        /// <param name="v1">The v1.</param>
        /// <param name="v2">The v2.</param>
        /// <param name="v3">The v3.</param>
        /// <param name="x">The x.</param>
        /// <returns>T.</returns>
        template<typename T>
        static T interpolateCube(T v0, T v1, T v2, T v3, T x)
        {
            T p = (v3 - v2) - (v0 - v1);
            T q = (v0 - v1) - p;
            T r = v2 - v0;
            T s = v1;
            return p * pow(x, static_cast<T>(3)) + q * pow(x, static_cast<T>(2)) + r * x + s;
        }

        /// <summary>
        /// Interpolates the cos.
        /// </summary>
        /// <param name="a">a.</param>
        /// <param name="b">The b.</param>
        /// <param name="x">The x.</param>
        /// <returns>T.</returns>
        template<typename T>
        static T InterpolateCos(T a, T b, T x)
        {
            double ft = x * 3.1415927;
            double f = (1 - cos(ft)) * 0.5;
            return a*(1 - f) + b*f;
        }

        /// <summary>
        /// 噪声函数
        /// </summary>
        /// <param name="x">The x.</param>
        /// <param name="y">The y.</param>
        /// <returns>double.</returns>
        static double Noise(int x, int y)
        {
            long n = x + y * 57;
            n = (n << 13) ^ n;
            return (1.0 - ((n * (n*n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
        }

        static double Noise(float x, float y)
        {
            int ix = static_cast<int>(x);
            int iy = static_cast<int>(y);
            return Noise(ix, iy);
        }

        /// <summary>
        /// 平滑噪声
        /// </summary>
        /// <param name="x">The x.</param>
        /// <param name="y">The y.</param>
        /// <returns>返回平滑生成的噪声值</returns>
        static double SmoothedNoise(float x, float y);

        /// <summary>
        /// Interpolateds the noise.
        /// </summary>
        /// <param name="x">The x.</param>
        /// <param name="y">The y.</param>
        /// <returns>double.</returns>
        static double InterpolatedNoise(float x, float y);

        /// <summary>
        /// Perlins the noise2 d.
        /// </summary>
        /// <param name="x">The x.</param>
        /// <param name="y">The y.</param>
        /// <returns>double.</returns>
        static double PerlinNoise2D(float x, float y);

        /// <summary>
        /// Gausses the specified x.
        /// </summary>
        /// <param name="x">The x.</param>
        /// <param name="y">The y.</param>
        /// <param name="offset">The offset.</param>
        /// <returns>double.</returns>
        static double Gauss(float x, float y, float offset);

        /// <summary>
        /// 根据平移量，旋转量，缩放量，构建一个变换矩阵Creates the transformation matrix.
        /// </summary>
        /// <param name="translation">平移量</param>
        /// <param name="rotation">旋转量</param>
        /// <param name="scale">缩放量</param>
        /// <returns>返回一个构造出来的变换矩阵</returns>
        static glm::mat4 CreateTransformationMatrix(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);

        /// <summary>
        /// Barries the centric.
        /// </summary>
        /// <param name="p1">The p1.</param>
        /// <param name="p2">The p2.</param>
        /// <param name="p3">The p3.</param>
        /// <param name="pos">The position.</param>
        /// <returns>float.</returns>
        static float BarryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos);

		template<typename T>
		inline static T Lerp(T a, T b, T f)
		{
			return a + f * (b - a);
		}
    };
}

#endif // kgl_math_tools_h__