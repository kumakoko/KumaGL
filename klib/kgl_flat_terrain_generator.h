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

			//private static final MyFile VERTEX_SHADER = new MyFile("flatTerrain", "terrainVertex.glsl");
			//private static final MyFile FRAGMENT_SHADER = new MyFile("flatTerrain", "terrainFragment.glsl");

		 private:
			 static const int VERTEX_SIZE_BYTES = 12 + 4 + 4;// position + normal
			// + colour

			IndicesGeneratorSPtr indicesGenerator;
			TerrainRendererSPtr renderer;

			/**
			* Sets up the generator and initializes the renderer which is going to be
			* used to render terrains that use this generator.
			*
			* @param perlinNoise
			*            - The generator used to generate the vertex heights.
			* @param colourGen
			*            - The generator used to generate the colours of the vertices.
			* @param indicesGenerator
			*            - The generator used to generate the index buffer for the
			*            terrain which connects the vertices into triangles.
			*/
		 public :
			 FlatTerrainGenerator(PerlinNoiseSPtr perlinNoise, ColorGeneratorSPtr colourGen, CameraSPtr camera, IndicesGeneratorSPtr indicesGenerator);
		
			 virtual TerrainSPtr createTerrain(const ublas::matrix<float>& heights, const ublas::matrix<glm::vec4>& colours) override;
				

				virtual void cleanUp() override 
				{
				renderer->cleanUp();
			}

			/**
			* @param heights
			*            - The heights of all the vertices in the terrain.
			* @param normals
			*            - The smooth normal vectors for every vertex of the terrain.
			* @param colours
			*            - The colour of every vertex.
			* @return The interleaved vertex data for all the vertices in this terrain.
			*         Each vertex has a position, normal, and colour. The vertex data
			*         is already in byte format, ready to be stored into a VBO.
			*/
				/*
			private static byte[] getMeshData(float[][] heights, glm::vec3[][] normals, Colour[][] colours) {
				int byteSize = VERTEX_SIZE_BYTES * heights.length * heights[0].length;
				ByteBuffer buffer = ByteBuffer.allocate(byteSize).order(ByteOrder.nativeOrder());
				for (int z = 0; z < heights.length; z++) {
					for (int x = 0; x < heights[z].length; x++) {
						DataStoring.packVertexData(x, heights[z][x], z, normals[z][x], colours[z][x], buffer);
					}
				}
				return buffer.array();
			}*/

		};
	}
}

#endif // kgl_flat_terrain_generator_h__