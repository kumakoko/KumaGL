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
#include "kgl_terrain_generator.h"

namespace kgl
{
	namespace low_poly_terrain
	{
		TerrainGenerator::TerrainGenerator(PerlinNoiseSPtr perlinNoise, ColorGeneratorSPtr colourGen, CameraSPtr camera)
		{
			this->perlin_noise_ = perlinNoise;
			this->color_generator_ = colourGen;
			this->camera_;
		}

		TerrainSPtr TerrainGenerator::generateTerrain(int gridSize)
		{
			ublas::matrix<float> heights;
			ublas::matrix<glm::vec4> colours;
			generateHeights(gridSize, perlin_noise_,heights);
			color_generator_->GenerateColours(heights, perlin_noise_->getAmplitude(), colours);
			return createTerrain(heights, colours);
		}

		void TerrainGenerator::generateHeights(int gridSize, PerlinNoiseSPtr perlinNoise, ublas::matrix<float>& heights)
		{
			heights.resize(gridSize + 1, gridSize + 1, false);

			for (int z = 0; z < heights.size1(); z++)
			{
				for (int x = 0; x < heights.size2(); x++)
				{
					heights(z, x) = perlinNoise->getPerlinNoise(x, z);
				}
			}
		}
	}
}