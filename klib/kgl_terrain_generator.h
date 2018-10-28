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
             /// <summary
             /// >初始化一个<see cref="TerrainGenerator"/>类实例对象.
             /// </summary>
             /// <param name="perlinNoise">The perlin noise.</param>
             /// <param name="colourGen">The colour gen.</param>
             TerrainGenerator(PerlinNoiseSPtr perlinNoise, ColorGeneratorSPtr colourGen,CameraSPtr camera);

             /// <summary>
             /// Generates a terrain. First the heights and colours of all the vertices are generated.
             /// </summary>
             /// <param name="gridSize">The number of grid squares along one side of the terrain.</param>
             /// <returns>The generated terrain</returns>
             TerrainSPtr GenerateTerrain(int gridSize);

             /// <summary>
             /// Cleans up.
             /// </summary>
             virtual void CleanUp() = 0;

         protected:
             /// <summary>
             /// Generates the terrain mesh data, loads it up to a VAO, and initializes the terrain.
             /// </summary>
             /// <param name="heights">The heights of all the vertices in the terrain.</param>
             /// <param name="colours">The colours of all the vertices.</param>
             /// <returns>The new terrain</returns>
             virtual TerrainSPtr CreateTerrain(const ublas::matrix<float>& heights, const ublas::matrix<glm::vec4>& colours) = 0;

         private:
             /// <summary>
             /// 使用perlin噪声生成器，去生成高度图，然后用高度图去生成对应的地形
             /// </summary>
             /// <param name="gridSize">单元地形格的大小</param>
             /// <param name="perlinNoise">The perlin noise.</param>
             /// <param name="heights">The heights.</param>
             void GenerateHeights(int gridSize, PerlinNoiseSPtr perlinNoise, ublas::matrix<float>& heights);
         };
    }
}

#endif // kgl_terrain_generator_h__