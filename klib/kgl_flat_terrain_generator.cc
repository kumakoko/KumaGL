#include "kgl_lib_pch.h"
#include "kgl_flat_terrain_generator.h"
#include "kgl_smooth_normals_generator.h"
#include "kgl_primitive.h"
#include "kgl_vertex_attribute.h"
#include "kgl_debug_tool.h"

namespace kgl
{
	namespace low_poly_terrain
	{
		FlatTerrainGenerator::FlatTerrainGenerator(PerlinNoiseSPtr perlinNoise, ColorGeneratorSPtr colourGen, CameraSPtr camera, IndicesGeneratorSPtr indicesGenerator) :
			TerrainGenerator(perlinNoise, colourGen,camera)
		{
			const char* vs_file = "resources/shader/low_poly_terrain/flat_terrain_vs.glsl";
			const char* fs_file = "resources/shader/low_poly_terrain/flat_terrain_fs.glsl";
			GPUProgramSPtr shader = std::make_shared<GPUProgram>();
			shader->CreateFromFile(vs_file, fs_file, nullptr);
			this->indicesGenerator = indicesGenerator;
			this->renderer = std::make_shared<TerrainRenderer>(shader, camera, true);
		}

		TerrainSPtr FlatTerrainGenerator::createTerrain(const ublas::matrix<float>& heights, const ublas::matrix<glm::vec4>& colours)
		{
			ublas::matrix<glm::vec3> normals;
			std::vector<int> indices;
			std::vector<float> vertices;

			SmoothNormalsGenerator::generateNormals(heights, normals);
			indicesGenerator->generateIndexBuffer(heights.size1(), indices);
			PrimitiveSPtr primitive = std::make_shared<Primitive>();

			for (int z = 0; z < heights.size1(); z++) 
			{
				for (int x = 0; x < heights.size2(); x++)
				{
					float fx = static_cast<float>(x);
					float fy = heights(z, x) / 50000.0f;
					float fz = static_cast<float>(z);

					vertices.push_back(fx);
					vertices.push_back(fy);
					vertices.push_back(fz);
					
					const glm::vec3& n = normals(z, x);
					vertices.push_back(n.x);
					vertices.push_back(n.y);
					vertices.push_back(n.z);

					const glm::vec4& c = colours(z, x);
					vertices.push_back(c.r);
					vertices.push_back(c.g);
					vertices.push_back(c.b);
					vertices.push_back(c.a);

					//DebugTool::PrintVector3("Height = ", glm::vec3(fx, fy, fz));
				}
			}

			kgl::VertexAttribute va_position;
			va_position.index = 0;
			va_position.normalized = GL_FALSE;
			va_position.type = GL_FLOAT;
			va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
			va_position.stride = 10 * sizeof(GLfloat); // 每个顶点的步长为 
			va_position.pointer = nullptr;

			kgl::VertexAttribute va_normal;
			va_normal.index = 1;
			va_normal.normalized = GL_FALSE;
			va_normal.type = GL_FLOAT;
			va_normal.size = 3; // 一个“顶点法线”的属性由3个分量组成
			va_normal.stride = 10 * sizeof(GLfloat); // 每个位置的步长为 
			va_normal.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

			kgl::VertexAttribute va_color;
			va_color.index = 2;
			va_color.normalized = GL_FALSE;
			va_color.type = GL_FLOAT;
			va_color.size = 4; // 一个“顶点颜色”的属性由3个分量组成
			va_color.stride = 10 * sizeof(GLfloat); // 每个位置的步长为 
			va_color.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat) + 3 * sizeof(GLfloat));

			std::vector<kgl::VertexAttribute> vtx_attri_array;
			vtx_attri_array.push_back(va_position);
			vtx_attri_array.push_back(va_normal);
			vtx_attri_array.push_back(va_color);

			primitive->CreateIndexed(GL_TRIANGLES, &(vertices[0]), sizeof(float)*vertices.size(), GL_STATIC_DRAW, 
				kgl::Primitive::UINT32, &(indices[0]), sizeof(int)*indices.size(), GL_STATIC_DRAW, vtx_attri_array);

			return std::make_shared<Terrain>(primitive, renderer);
		}
	}
}