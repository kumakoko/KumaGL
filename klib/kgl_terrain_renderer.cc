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
