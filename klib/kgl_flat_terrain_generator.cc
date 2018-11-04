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
#include "kgl_flat_terrain_generator.h"
#include "kgl_smooth_normals_generator.h"
#include "kgl_primitive.h"
#include "kgl_vertex_attribute.h"
#include "kgl_debug_tool.h"

namespace kgl
{
    namespace low_poly_terrain
    {
        FlatTerrainGenerator::FlatTerrainGenerator(PerlinNoiseSPtr perlin_noise, ColorGeneratorSPtr color_gen, CameraSPtr camera, IndicesGeneratorSPtr indices_generator) :
            TerrainGenerator(perlin_noise, color_gen, camera)
        {
            const char* vs_file = "resources/shader/low_poly_terrain/flat_terrain_vs.glsl";
            const char* fs_file = "resources/shader/low_poly_terrain/flat_terrain_fs.glsl";
            GPUProgramSPtr shader = std::make_shared<GPUProgram>();
            shader->CreateFromFile(vs_file, fs_file, nullptr);
            this->indices_generator_ = indices_generator;
            this->renderer_ = std::make_shared<TerrainRenderer>(shader, camera, true);
        }

        kgl::low_poly_terrain::TerrainSPtr FlatTerrainGenerator::CreateTerrain(const ublas::matrix<float>& heights, const ublas::matrix<glm::vec4>& colours)
        {
            ublas::matrix<glm::vec3> normals;
            std::vector<int> indices;
            std::vector<float> vertices;

            SmoothNormalsGenerator::GenerateNormals(heights, normals);
            indices_generator_->GenerateIndexBuffer(heights.size1(), indices);
            PrimitiveSPtr primitive = std::make_shared<Primitive>();

            for (std::size_t z = 0; z < heights.size1(); z++)
            {
                for (std::size_t x = 0; x < heights.size2(); x++)
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

            return std::make_shared<Terrain>(primitive, renderer_);
        }

        void FlatTerrainGenerator::CleanUp()
        {
            renderer_->cleanUp();
        }
    }
}