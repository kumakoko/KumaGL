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
			/// 用来获取得噪声值的随机数种子，该值是由另一个随机数发生器生�?
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
			/// 振动幅�?
			/// </summary>
			float amplitude_;

		public:
			/// <summary>
			/// <see cref="PerlinNoise"/> 类的构造函�?
			/// </summary>
			/// <param name="seed">获取得噪声值的随机数种子/param>
			/// <param name="octaves">粗糙程度</param>
			/// <param name="amplitude">振动幅�?/param>
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
			/// 获取得噪声值的随机数种�?
			/// </summary>
			/// <returns>获取得噪声值的随机数种�?/returns>
			inline int getSeed() const
			{
				return seed_;
			}

			/// <summary>
			/// 获取振动幅�?
			/// </summary>
			/// <returns>振动幅�?/returns>
			inline float getAmplitude() const
			{
				return amplitude_;
			}

			/// <summary>
			/// 得到给定坐标点对应的perlin噪声�?
			/// </summary>
			/// <param name="x">The x.</param>
			/// <param name="y">The y.</param>
			/// <returns>float.</returns>
			float getPerlinNoise(int x, int y);

		private:
			/// <summary>
			/// Gets the smooth noise.
			/// </summary>
			/// <param name="x">The x.</param>
			/// <param name="y">The y.</param>
			/// <returns>float.</returns>
			float getSmoothNoise(int x, int y);

			/// <summary>
			/// Gets the noise.
			/// </summary>
			/// <param name="x">The x.</param>
			/// <param name="y">The y.</param>
			/// <returns>float.</returns>
			float getNoise(int x, int y);

			/// <summary>
			/// Gets the interpolated noise.
			/// </summary>
			/// <param name="x">The x.</param>
			/// <param name="y">The y.</param>
			/// <returns>float.</returns>
			float getInterpolatedNoise(float x, float y);

			/// <summary>
			/// Interpolates the specified a.
			/// </summary>
			/// <param name="a">a.</param>
			/// <param name="b">The b.</param>
			/// <param name="blend">The blend.</param>
			/// <returns>float.</returns>
			float interpolate(float a, float b, float blend);

		};

		typedef std::shared_ptr<PerlinNoise> PerlinNoiseSPtr;
	}
}

#endif // kgl_perlin_noise_h__
