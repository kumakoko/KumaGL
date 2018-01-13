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
#include "kgl_lib_pch.h"
#include "kgl_color_generator.h"

namespace kgl
{
	namespace low_poly_terrain
	{


		ColorGenerator::ColorGenerator(const std::vector<glm::vec4>& biome_colors, float spread)
		{
			this->biome_colors_ = biome_colors;
			this->spread_ = spread;
			this->half_spread_ = spread / 2.f;
			this->part_ = 1.f / (biome_colors.size() - 1);
		}

		void ColorGenerator::GenerateColours(const ublas::matrix<float>& heights, float amplitude, ublas::matrix<glm::vec4>& generated_color_array)
		{
			generated_color_array.resize(heights.size1(), heights.size2(), false);

			for (size_t z = 0; z < generated_color_array.size1(); z++)
			{
				for (size_t x = 0; x < generated_color_array.size2(); x++)
				{
					generated_color_array(z, x) = CalculateColour(heights(z, x), amplitude);
				}
			}
		}

		glm::vec4 ColorGenerator::CalculateColour(float height, float amplitude)
		{
			float value = (height + amplitude) / (amplitude * 2);
			value = glm::clamp((value - half_spread_) * (1.f / spread_), 0.f, 0.9999f);
			int firstBiome = (int)glm::floor(value / part_);
			float blend = (value - (firstBiome * part_)) / part_;
			// 仅对顶点颜色中的RGB分量进行插值
			glm::vec3 color_1(biome_colors_[firstBiome].r, biome_colors_[firstBiome].g, biome_colors_[firstBiome].b);
			glm::vec3 color_2(biome_colors_[firstBiome+1].r, biome_colors_[firstBiome+1].g, biome_colors_[firstBiome+1].b);
			glm::vec3 mix_factor(blend, blend, blend);
			glm::vec3 result_color = glm::mix(color_1, color_2, mix_factor);
			return glm::vec4(result_color, 1.0f);
		}
	}
}