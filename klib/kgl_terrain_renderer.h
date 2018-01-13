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
			GPUProgramSPtr shader_;
			bool has_indices_;
			CameraSPtr camera_;
		public:
			/// <summary>
			/// 创建一个<see cref="TerrainRenderer" />类实例对象
			/// </summary>
			/// <param name="shader">The shader.</param>
			/// <param name="camera">The camera.</param>
			/// <param name="usesIndices">Indicates whether the terrain will be rendered with an index buffer</param>
			TerrainRenderer(GPUProgramSPtr shader, CameraSPtr camera, bool uses_indices);

			/// <summary>
			/// Finalizes an instance of the <see cref="TerrainRenderer"/> class.
			/// </summary>
			~TerrainRenderer();

			/// <summary>
			///  Renders a terrain to the screen. If the terrain has an index buffer the
			/// </summary>
			/// <param name="terrain">将要绘制的地形对象</param>
			/// <param name="light">The light being used to iluminate the terrain.</param>
			void Render(Terrain* terrain, const LowPolyTerrainLight& light);
			
			/// <summary>
			/// Used when the program closes. Deletes the shader program.
			/// </summary>
			void cleanUp();
		private:
			/// <summary>
			/// Prepares the specified terrain.
			/// </summary>
			/// <param name="terrain">The terrain.</param>
			/// <param name="light">The light.</param>
			void Prepare(Terrain* terrain, const LowPolyTerrainLight& light);

			/// <summary>
			/// End the rendering process by unbinding the VAO and stopping the shader program.
			/// </summary>
			/// <param name="terrain">The terrain.</param>
			void Finish(Terrain* terrain);
		};

		typedef std::shared_ptr<TerrainRenderer> TerrainRendererSPtr;
	}
}

#endif // kgl_terrain_renderer_h__