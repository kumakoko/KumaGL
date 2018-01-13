/*!
 * \file kgl_flat_terrain_generator.h
 * \date 2017/11/24 20:45
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
#ifndef kgl_flat_terrain_generator_h__
#define kgl_flat_terrain_generator_h__

#include "kgl_terrain_generator.h"
#include "kgl_terrain_renderer.h"
#include "kgl_indices_generator.h"

namespace kgl
{
	namespace low_poly_terrain
	{
		class FlatTerrainGenerator : public TerrainGenerator
		{
		private:
			static const int VERTEX_SIZE_BYTES = 12 + 4 + 4;// position + normal
			IndicesGeneratorSPtr indices_generator_;
			TerrainRendererSPtr renderer_;
		public:
			/// <summary>
			/// Initializes a new instance of the <see cref="FlatTerrainGenerator"/> 类
			/// </summary>
			/// <param name="perlin_noise">The generator used to generate the vertex heights</param>
			/// <param name="color_gen">The generator used to generate the colours of the vertices</param>
			/// <param name="camera">The camera.</param>
			/// <param name="indices_generator">The generator used to generate the index buffer for the
			///                                terrain which connects the vertices into triangles</param>
			FlatTerrainGenerator(PerlinNoiseSPtr perlin_noise, ColorGeneratorSPtr color_gen, CameraSPtr camera, IndicesGeneratorSPtr indices_generator);

			virtual TerrainSPtr CreateTerrain(const ublas::matrix<float>& heights, const ublas::matrix<glm::vec4>& colours) override;

			virtual void CleanUp() override;
		};
	}
}

#endif // kgl_flat_terrain_generator_h__