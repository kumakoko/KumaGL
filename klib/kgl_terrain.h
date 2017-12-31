/*!
 * \file kgl_terrain.h
 * \date 2017/11/24 15:32
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
#ifndef kgl_terrain_h__
#define kgl_terrain_h__


#include "kgl_light.h"
#include "kgl_primitive.h"
#include "kgl_terrain_renderer.h"

namespace kgl
{
	namespace low_poly_terrain
	{
		class Terrain
		{
		private:
			friend class TerrainRenderer;

			/// <summary>
			/// The vertex_count_
			/// </summary>
			int vertex_count_;

			/// <summary>
			/// The renderer_
			/// </summary>
			TerrainRendererSPtr renderer_;

			/// <summary>
			/// The primitive_
			/// </summary>
			PrimitiveSPtr		primitive_;
		public:
			/// <summary>
			/// Initializes a new instance of the <see cref="Terrain" /> class.
			/// </summary>
			/// <param name="primitive">The primitive.</param>
			/// <param name="renderer">The renderer.</param>
			Terrain(PrimitiveSPtr primitive, TerrainRendererSPtr renderer);

			/// <summary>
			/// Finalizes an instance of the <see cref="Terrain"/> class.
			/// </summary>
			~Terrain();

			/// <summary>
			/// Gets the vertex count.
			/// </summary>
			/// <returns>int.</returns>
			int getVertexCount() const;

			/// <summary>
			/// Renders the specified light.
			/// </summary>
			/// <param name="light">The light.</param>
			void Render(const LowPolyTerrainLight& light);
		};

		typedef std::shared_ptr<Terrain> TerrainSPtr;
	}
}

#endif // kgl_terrain_h__

