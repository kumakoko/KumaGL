#include "kgl_lib_pch.h"
#include "kgl_terrain.h"
#include "kgl_terrain_renderer.h"

namespace kgl
{
	namespace low_poly_terrain
	{

		TerrainRenderer::TerrainRenderer(GPUProgramSPtr shader, CameraSPtr camera, bool usesIndices)
		{
			this->shader = shader;
			this->camera = camera;
			this->hasIndices = usesIndices;
		}

		TerrainRenderer::~TerrainRenderer()
		{
			this->shader.reset();
		}

		void TerrainRenderer::render(Terrain* terrain, const LowPolyTerrainLight& light)
		{
			prepare(terrain, light);
			terrain->primitive_->DrawIndexed();
		}

		void TerrainRenderer::cleanUp()
		{
			//shader.cleanUp();
		}

		void TerrainRenderer::prepare(Terrain* terrain, const LowPolyTerrainLight& light)
		{
			shader->Use();
			shader->ApplyVector3(glm::value_ptr(light.Direction), "light_direction");
			shader->ApplyVector3(glm::value_ptr(light.Color), "light_color");
			shader->ApplyVector2(glm::value_ptr(light.Bias), "light_bias");
			shader->ApplyMatrix(glm::value_ptr(camera->GetViewMatrix()), "view_matrix");
			shader->ApplyMatrix(glm::value_ptr(camera->GetProjectionMatrix()), "projection_matrix");
		}

		/**
		* End the rendering process by unbinding the VAO and stopping the shader
		* program.
		*
		* @param terrain
		*/
		void TerrainRenderer::finish(Terrain* terrain)
		{
			/*
			terrain.getVao().unbind();
			shader.stop();
			*/
		}
	}
}
