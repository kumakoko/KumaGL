/*!
 * \file kgl_terrain_renderer.h
 * \date 2017/11/24 15:43
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

#ifndef kgl_terrain_renderer_h__
#define kgl_terrain_renderer_h__

#include "kgl_gpu_program.h"
#include "kgl_camera.h"

namespace kgl
{
	namespace low_poly_terrain
	{
		class Terrain;

		class TerrainRenderer
		{

		private:
			GPUProgramSPtr shader;
			bool hasIndices;
			CameraSPtr camera;

			/**
			* @param shader
			*            - The shader program used for rendering this terrain.
			* @param usesIndices
			*            - Indicates whether the terrain will be rendered with an index
			*            buffer or not.
			*/
		public:
			/// <summary>
			/// Initializes a new instance of the <see cref="TerrainRenderer" /> class.
			/// </summary>
			/// <param name="shader">The shader.</param>
			/// <param name="camera">The camera.</param>
			/// <param name="usesIndices">The uses indices.</param>
			TerrainRenderer(GPUProgramSPtr shader, CameraSPtr camera, bool usesIndices);

			/// <summary>
			/// Finalizes an instance of the <see cref="TerrainRenderer"/> class.
			/// </summary>
			~TerrainRenderer();

			/**
			* Renders a terrain to the screen. If the terrain has an index buffer the
			* glDrawElements is used. Otherwise glDrawArrays is used.
			*
			* @param terrain
			*            - The terrain to be rendered.
			* @param camera
			*            - The camera being used for rendering the terrain.
			* @param light
			*            - The light being used to iluminate the terrain.
			*/
			/// <summary>
			/// Renders the specified terrain.
			/// </summary>
			/// <param name="terrain">将要绘制的地形对象</param>
			/// <param name="light">The light.</param>
			void render(Terrain* terrain, const LowPolyTerrainLight& light);
			

			/**
			* Used when the program closes. Deletes the shader program.
			*/
			/// <summary>
			/// Cleans up.
			/// </summary>
			void cleanUp();

			/**
			* Starts the shader program and loads up any necessary uniform variables.
			*
			* @param terrain
			*            - The terrain to be rendered.
			* @param camera
			*            - The camera being used to render the scene.
			* @param light
			*            - The light in the scene.
			*/
		private:
			/// <summary>
			/// Prepares the specified terrain.
			/// </summary>
			/// <param name="terrain">The terrain.</param>
			/// <param name="light">The light.</param>
			void prepare(Terrain* terrain, const LowPolyTerrainLight& light);

			/**
			* End the rendering process by unbinding the VAO and stopping the shader
			* program.
			*
			* @param terrain
			*/
			/// <summary>
			/// Finishes the specified terrain.
			/// </summary>
			/// <param name="terrain">The terrain.</param>
			void finish(Terrain* terrain);
		};

		typedef std::shared_ptr<TerrainRenderer> TerrainRendererSPtr;
	}
}

#endif // kgl_terrain_renderer_h__