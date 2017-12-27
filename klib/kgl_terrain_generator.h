/*!
 * \file kgl_terrain_generator.h
 * \date 2017/11/24 13:49
 *
 * \author xiongxinke
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef kgl_terrain_generator_h__
#define kgl_terrain_generator_h__

#include "kgl_perlin_noise.h"
#include "kgl_color_generator.h"
#include "kgl_terrain.h"

namespace kgl
{
	namespace low_poly_terrain
	{
		 class TerrainGenerator
		 {
		 private:
			 PerlinNoiseSPtr perlin_noise_;
			 ColorGeneratorSPtr color_generator_;
			 CameraSPtr camera_;
		 public: 
			 /// <summary>
			 /// Initializes a new instance of the <see cref="TerrainGenerator"/> class.
			 /// </summary>
			 /// <param name="perlinNoise">The perlin noise.</param>
			 /// <param name="colourGen">The colour gen.</param>
			 TerrainGenerator(PerlinNoiseSPtr perlinNoise, ColorGeneratorSPtr colourGen,CameraSPtr camera);

			/**
			* Generates a terrain. First the heights and colours of all the vertices
			* are generated.
			*
			* @param gridSize
			*            - The number of grid squares along one side of the terrain.
			* @return The generated terrain.
			*/
			 /// <summary>
			 /// Generates the terrain.
			 /// </summary>
			 /// <param name="gridSize">Size of the grid.</param>
			 /// <returns>TerrainSPtr.</returns>
			 TerrainSPtr generateTerrain(int gridSize);

			/**
			* For use when the app closes.
			*/
			 /// <summary>
			 /// Cleans up.
			 /// </summary>
			 virtual void cleanUp() = 0;

			/**
			* Generates the terrain mesh data, loads it up to a VAO, and initializes
			* the terrain.
			*
			* @param heights
			*            - The heights of all the vertices in the terrain.
			* @param colours
			*            - The colours of all the vertices.
			* @return The new terrain.
			*/
		 protected:
			 /// <summary>
			 /// Creates the terrain.
			 /// </summary>
			 /// <param name="heights">The heights.</param>
			 /// <param name="colours">The colours.</param>
			 /// <returns>TerrainSPtr.</returns>
			 virtual TerrainSPtr createTerrain(const ublas::matrix<float>& heights, const ublas::matrix<glm::vec4>& colours) = 0;

			/**
			* Uses the perlin noise generator (which might actually not be using the
			* Perlin Noise algorithm - I'm not quite sure if it is or isn't) to
			* generate heights for all of the terrain's vertices.
			*
			* @param gridSize - The number of grid squares along one edge of the terrain.
			* @param perlinNoise - The heights generator.
			* @return All the heights for the vertices.
			*/
		 private:
			 /// <summary>
			 /// 使用perlin噪声生成器，去生成高度图，然后用高度图去生成对应的地形
			 /// </summary>
			 /// <param name="gridSize">单元地形格的大小</param>
			 /// <param name="perlinNoise">The perlin noise.</param>
			 /// <param name="heights">The heights.</param>
			 void generateHeights(int gridSize, PerlinNoiseSPtr perlinNoise, ublas::matrix<float>& heights);
		 };
	}
}

#endif // kgl_terrain_generator_h__