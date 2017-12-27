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

