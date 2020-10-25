/**************************************************************************************************************************
Copyright(C) 2014-2019 www.xionggf.com

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
#include "kgl_math_tools.h"
#include "kgl_wfx_terrain.h"
#include "kgl_debug_tool.h"

namespace kgl
{
	namespace water_effect
	{
		const GLfloat SIZE = 800.0f;
		const GLfloat MAX_HEIGHT = 40.0f;
		const GLuint MAX_PIXEL_COLOR = 256 * 256 * 256;

		Terrain::Terrain(GLfloat x, GLfloat z, LoaderSPtr loader, TerrainTexturePack texturePack, TerrainTexture blendMap, const std::string& heightMap)
			: x_(x * SIZE), z_(z * SIZE), loader_(loader), texturePack_(texturePack), blendMap_(blendMap)
		{
			model_ = generateTerrain(loader_, heightMap);
		}

		float Terrain::getHeightOfTerrain(float worldX, float worldZ) const
		{
			float terrainX = worldX - x_;
			float terrainZ = worldZ - z_;
			float gridSquareSize = SIZE / (heights_.size() - 1);
			int gridX = static_cast<int>(terrainX / gridSquareSize);
			int gridZ = static_cast<int>(terrainZ / gridSquareSize);
			if (gridX < 0 || gridX + 1 >= (int)heights_.size() || gridZ < 0 || gridZ + 1 >= (int)heights_.size()) {
				return 0.0f;
			}
			float xCoord = fmod(terrainX, gridSquareSize) / gridSquareSize;
			float zCoord = fmod(terrainZ, gridSquareSize) / gridSquareSize;
			float height;
			if (xCoord <= (1 - zCoord)) {
				height = MathTools::BarryCentric(
					glm::vec3(0, heights_[gridX][gridZ], 0),
					glm::vec3(1, heights_[gridX + 1][gridZ], 0),
					glm::vec3(0, heights_[gridX][gridZ + 1], 1),
					glm::vec2(xCoord, zCoord));
			}
			else {
				height = MathTools::BarryCentric(
					glm::vec3(1, heights_[gridX + 1][gridZ], 0),
					glm::vec3(1, heights_[gridX + 1][gridZ + 1], 1),
					glm::vec3(0, heights_[gridX][gridZ + 1], 1),
					glm::vec2(xCoord, zCoord));
			}
			return height;
		}

		RawModel Terrain::generateTerrain(LoaderSPtr loader, const std::string& heightMap)
		{
			stb::Image image{ heightMap, 4 };
			int vertex_count = image.height();
			heights_.resize(vertex_count);
			for (auto& h : heights_)
				h.resize(vertex_count);

			int count = vertex_count * vertex_count;
			auto vertices = std::vector<GLfloat>(count * 3);
			auto normals = std::vector<GLfloat>(count * 3);
			auto textureCoords = std::vector<GLfloat>(count * 2);
			auto indices = std::vector<GLuint>(6 * (vertex_count - 1)*(vertex_count - 1));
			int vertexPointer = 0;

			for (int i = 0; i < vertex_count; i++)
			{
				for (int j = 0; j < vertex_count; j++)
				{
					vertices[vertexPointer * 3] = (float)j / ((float)vertex_count - 1) * SIZE;
					float height = getHeight(j, i, image);
					heights_[j][i] = height;
					vertices[vertexPointer * 3 + 1] = height;
					vertices[vertexPointer * 3 + 2] = (float)i / ((float)vertex_count - 1) * SIZE;

					auto normal = calculateNormal(j, i, image);
					normals[vertexPointer * 3] = normal.x;
					normals[vertexPointer * 3 + 1] = normal.y;
					normals[vertexPointer * 3 + 2] = normal.z;
					textureCoords[vertexPointer * 2] = (float)j / ((float)vertex_count - 1);
					textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)vertex_count - 1);

					/*boost::format f1("vertices[%d][%d]");
					f1 % j % i;
					DebugTool::PrintVector3(f1.str().c_str(), glm::vec3(
					vertices[vertexPointer * 3 + 0],
					vertices[vertexPointer * 3 + 1],
					vertices[vertexPointer * 3 + 2]));

					boost::format f2("normal[%d][%d]");
					f2 % j % i;
					DebugTool::PrintVector3(f2.str().c_str(), glm::vec3(
					normals[vertexPointer * 3 + 0],
					normals[vertexPointer * 3 + 1],
					normals[vertexPointer * 3 + 2]));*/

					vertexPointer++;
				}
			}
			int pointer = 0;
			for (int gz = 0; gz < vertex_count - 1; gz++){
				for (int gx = 0; gx < vertex_count - 1; gx++){
					int topLeft = (gz*vertex_count) + gx;
					int topRight = topLeft + 1;
					int bottomLeft = ((gz + 1)*vertex_count) + gx;
					int bottomRight = bottomLeft + 1;
					indices[pointer++] = topLeft;
					indices[pointer++] = bottomLeft;
					indices[pointer++] = topRight;
					indices[pointer++] = topRight;
					indices[pointer++] = bottomLeft;
					indices[pointer++] = bottomRight;
				}
			}

			return loader->loadToVao(vertices, textureCoords, normals, indices);
		}

		glm::vec3 Terrain::calculateNormal(unsigned int x, unsigned int y, const stb::Image& image)
		{
			float heightL = getHeight(x - 1, y, image);
			float heightR = getHeight(x + 1, y, image);
			float heightD = getHeight(x, y - 1, image);
			float heightU = getHeight(x, y + 1, image);
			return glm::normalize(glm::vec3(heightL - heightR, 2.0f, heightD - heightU));
		}

		GLfloat Terrain::getHeight(unsigned int x, unsigned int z, const stb::Image& image)
		{
			if (x < 0 || x > static_cast<uint32_t>(image.height()) ||
				z < 0 || z >  static_cast<uint32_t>(image.height()))
			{
				return 0;
			}

			uint32_t val = image.get_rgb(x, z);

			GLfloat height = val;
			height /= (MAX_PIXEL_COLOR * 0.5f);
			height -= 1.0;
			height *= MAX_HEIGHT;
			return height;
		}

	}
}