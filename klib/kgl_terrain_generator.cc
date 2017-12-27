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