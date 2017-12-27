#include "kgl_lib_pch.h"
#include "kgl_terrain.h"
#include "kgl_terrain_renderer.h"

namespace kgl
{
	namespace low_poly_terrain
	{
		Terrain::~Terrain()
		{
			renderer_.reset();
		}

		Terrain::Terrain(PrimitiveSPtr primitive, TerrainRendererSPtr renderer)
		{
			primitive_ = primitive;
			renderer_ = renderer;
		}

		int Terrain::getVertexCount() const
		{
			return primitive_->GetVerticesCount();
		}

		void Terrain::Render(const LowPolyTerrainLight& light)
		{
			renderer_->render(this, light);
		}
	}
}