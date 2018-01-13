/*!
 * \file kgl_perlin_noise.h
 * \date 2017/11/22 20:04
 *
 * \author xiongxinke
 * Contact: sun_of_lover@sina.com
 *
 * \brief
 *
 * TODO: long description
 *
 * \note
 */
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
#ifndef kgl_perlin_noise_h__
#define kgl_perlin_noise_h__

namespace kgl
{
	namespace low_poly_terrain
	{
		class PerlinNoise
		{
		private:
			/// <summary>
			/// 用来获取得噪声值的随机数种子，该值是由另一个随机数发生器生成
			/// </summary>
			int seed_;

			/// <summary>
			/// 粗糙程度
			/// </summary>
			float roughness_;

			/// <summary>
			/// 阶数
			/// </summary>
			int octaves_;

			/// <summary>
			/// 振动幅度
			/// </summary>
			float amplitude_;

		public:
			/// <summary>
			/// <see cref="PerlinNoise"/> 类的构造函数
			/// </summary>
			/// <param name="seed">获取得噪声值的随机数种子/param>
			/// <param name="octaves">粗糙程度</param>
			/// <param name="amplitude">振动幅度/param>
			/// <param name="roughness">The roughness.</param>
			PerlinNoise(int seed, int octaves, float amplitude, float roughness);

			/// <summary>
			/// <see cref="PerlinNoise"/> 类的构造函�?
			/// </summary>
			/// <param name="octaves">The octaves.</param>
			/// <param name="amplitude">振动幅度/param>
			/// <param name="roughness">粗糙程度</param>
			PerlinNoise(int octaves, float amplitude, float roughness);

			/// <summary>
			/// 获取得噪声值的随机数种子
			/// </summary>
			/// <returns>获取得噪声值的随机数种子/returns>
			inline int GetSeed() const
			{
				return seed_;
			}

			/// <summary>
			/// 获取振动幅度
			/// </summary>
			/// <returns>振动幅度/returns>
			inline float GetAmplitude() const
			{
				return amplitude_;
			}

			/// <summary>
			/// 得到给定坐标点对应的perlin噪声
			/// </summary>
			/// <param name="x">The x.</param>
			/// <param name="y">The y.</param>
			/// <returns>float.</returns>
			float GetPerlinNoise(int x, int y);

		private:
			/// <summary>
			/// Gets the smooth noise.
			/// </summary>
			/// <param name="x">The x.</param>
			/// <param name="y">The y.</param>
			/// <returns>float.</returns>
			float GetSmoothNoise(int x, int y);

			/// <summary>
			/// Gets the noise.
			/// </summary>
			/// <param name="x">The x.</param>
			/// <param name="y">The y.</param>
			/// <returns>float.</returns>
			float GetNoise(int x, int y);

			/// <summary>
			/// Gets the interpolated noise.
			/// </summary>
			/// <param name="x">The x.</param>
			/// <param name="y">The y.</param>
			/// <returns>float.</returns>
			float GetInterpolatedNoise(float x, float y);

			/// <summary>
			/// Interpolates the specified a.
			/// </summary>
			/// <param name="a">a.</param>
			/// <param name="b">The b.</param>
			/// <param name="blend">The blend.</param>
			/// <returns>float.</returns>
			float Interpolate(float a, float b, float blend);

		};

		typedef std::shared_ptr<PerlinNoise> PerlinNoiseSPtr;
	}
}

#endif // kgl_perlin_noise_h__
