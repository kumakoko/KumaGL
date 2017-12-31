/*!
 * \file kgl_color_generator.h
 *
 * \author xiongxinke
 * \date 
 *
 * 根据高度图信息，生成地形网格中的每一个顶点的颜色
 * 颜色生成器将会通过顶点的高度值对颜色进行插值生成
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
#ifndef kgl_color_generator_h__
#define kgl_color_generator_h__

#include "kgl_defines.h"

namespace kgl
{
	namespace low_poly_terrain
	{
		class ColorGenerator
		{
		private:
			/// <summary>
			/// The spread_
			/// </summary>
			float spread_;

			/// <summary>
			/// The half_spread_
			/// </summary>
			float half_spread_;

			/// <summary>
			/// The biome_colors_
			/// </summary>
			std::vector<glm::vec4> biome_colors_;

			/// <summary>
			/// The part_
			/// </summary>
			float part_;

			/**
			 * @param biomeColours
			 *            - The preset colours that will be interpolated over the
			 *            terrain. The first colours in this array will be used for the
			 *            lowest parts of the terrain, and the last colours in this
			 *            array will be used for the highest. All the other colours will
			 *            be spread out linearly inbetween.
			 * @param spread
			 *            - This indicates how much of the possible altitude range the
			 *            colours should be spread over. If this is too high the extreme
			 *            colours won't be used as there won't be any terrain vertices
			 *            high or low enough (the heights generator doesn't usually fill
			 *            the whole altitude range).
			 */
		public:
			/// <summary>
			/// Initializes a new instance of the <see cref="ColorGenerator" /> class.
			/// </summary>
			/// <param name="biome_colors">预设的颜色将会通过地形数据生成。.本参数数组中的第一个
			///                            颜色将会用地形中高度最低的那部分，最后的颜色值就用在
			///                            地形中最高的那部分。数组中的其他的分量则是在中间进行线
			///                            性铺展。</param>
			/// <param name="spread">The spread.</param>
			ColorGenerator(const std::vector<glm::vec4>& biome_colors, float spread);

			/**
			 * Calculates the colour for every vertex of the terrain, by linearly
			 * interpolating between the biome colours depending on the vertex's height.
			 *
			 * @param heights
			 *            -The heights of all the vertices in the terrain.
			 * @param amplitude
			 *            - The amplitude range of the terrain that was used in the
			 *            heights generation. Maximum possible height is
			 *            {@code altitude} and minimum possible is {@code -altitude}.
			 * @return The colours of all the vertices in the terrain, in a grid.
			 */

			/// <summary>
			/// Generates the colours.
			/// </summary>
			/// <param name="heights">The heights.</param>
			/// <param name="amplitude">The amplitude.</param>
			/// <param name="generated_color_array">The generated_color_array.</param>
			void GenerateColours(const ublas::matrix<float>& heights, float amplitude, ublas::matrix<glm::vec4>& generated_color_array);

			/**Determines the colour of the vertex based on the provided height.
			 * @param height - Height of the vertex.
			 * @param amplitude - The maximum height that a vertex can be (
			 * @return
			 */
		private:
			/// <summary>
			/// Calculates the colour.
			/// </summary>
			/// <param name="height">The height.</param>
			/// <param name="amplitude">The amplitude.</param>
			/// <returns>glm.vec4.</returns>
			glm::vec4 CalculateColour(float height, float amplitude);
		};

		typedef std::shared_ptr<ColorGenerator> ColorGeneratorSPtr;
	}
}

#endif // kgl_color_generator_h__