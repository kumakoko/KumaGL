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
#include "kgl_primitive_tool.h"
#include "kgl_vertex_attribute.h"
#include "kgl_vertex_type.h"
#include "kgl_basic_static_mesh.h"

namespace kgl
{
    /*
    很多学习opengl的同学可能不是很清楚gl_front和gl_back的区别，下面来分析一下，
    OpenGL中用表面来表示物体，一个物体就是一组平面。光线照射在平面上会产生反射，入射线与反射线的角平分线就是法线，
    它垂直于平面。面法线有两种可能的方向，我们称面有两侧。当描述封闭物体的外表面时，法线应该从内部指向外部；而表示在物体内部时，法线应该从外部指向内部。
    用glNormal*(N_Vector);指定当前法向矢量。此后的顶点都会使用该法向，直到再次改变。
    面的法向矢量是怎么确定的呢？右手手指沿顶点顺序握拳，拇指竖立所指方向即为一个面的法向，如果面对该平面，以逆时针序指定顶点，则法向指向你。当你用这样的顺序指定平面的顶点时，OpenGL能够正确的计算出法向矢量。法矢与顶点坐标一样要经过模型视矩阵变换，这样物体坐标系内的法向矢量就可以正确的变成世界坐标系内的法向矢量，保证最终生成的图像是正确的。
    当一个顶点属于多个平面时，该顶点的法向应该取所在所有平面法向的平均值，以使表面棱角部份平滑自然的，否则会有一道黑线。
    我们把平面的法向所指一侧称为前面，另一侧称为后面，可以为两侧分别指定不同的属性，所以glMaterial*命令的第一个参数必须是GL_FRONT/GL_BACK/GL_FRONT_AND_BACK之一。
    */
    PrimitiveSPtr PrimitiveTool::BuildCube()
    {
        GLfloat vertices[] =
        {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f
        };

        VertexAttribute va_position;
        va_position.index = 0;
        va_position.normalized = GL_FALSE;
        va_position.type = GL_FLOAT;
        va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
        va_position.stride = 7 * sizeof(GLfloat); // 每个顶点的步长为 
        va_position.pointer = nullptr;

        VertexAttribute va_color;
        va_color.index = 1;
        va_color.normalized = GL_FALSE;
        va_color.type = GL_FLOAT;
        va_color.size = 4; // 一个“顶点颜色”的属性由3个分量组成
        va_color.stride = 7 * sizeof(GLfloat); // 每个位置的步长为 
        va_color.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

        std::vector<VertexAttribute> vtx_attri_array;
        vtx_attri_array.clear();
        vtx_attri_array.push_back(va_position);
        vtx_attri_array.push_back(va_color);

        PrimitiveSPtr p = std::make_shared<Primitive>();
        p->Create(GL_TRIANGLES, vertices, sizeof(vertices), 36, GL_STATIC_DRAW, vtx_attri_array);
        return p;
    }


    PrimitiveSPtr PrimitiveTool::BuildTexturedCube()
    {

        GLfloat cube_vertices[] =
        {
            // Back face
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom-right         
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
            // Front face
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, // top-left
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
            // Left face
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-right
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-left
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-right
            // Right face
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right         
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left     
            // Bottom face
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // top-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
            // Top face
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right     
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f  // bottom-left        
        };


        VertexAttribute va_position;
        va_position.index = 0;
        va_position.normalized = GL_FALSE;
        va_position.type = GL_FLOAT;
        va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
        va_position.stride = 5 * sizeof(GLfloat); // 每个顶点的步长为 
        va_position.pointer = nullptr;

        VertexAttribute va_texture;
        va_texture.index = 1;
        va_texture.normalized = GL_FALSE;
        va_texture.type = GL_FLOAT;
        va_texture.size = 2; // 一个“顶点纹理坐标”的属性由2个分量组成
        va_texture.stride = 5 * sizeof(GLfloat); // 每个位置的步长为 
        va_texture.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

        std::vector<VertexAttribute> vtx_attri_array;
        vtx_attri_array.clear();
        vtx_attri_array.push_back(va_position);
        vtx_attri_array.push_back(va_texture);

        PrimitiveSPtr p = std::make_shared<Primitive>();
        p->Create(GL_TRIANGLES, cube_vertices, sizeof(cube_vertices), 36, GL_STATIC_DRAW, vtx_attri_array);
        return p;
    }


    kgl::PrimitiveSPtr PrimitiveTool::BuildNDCTexturedRectange(float left /*= -1.0f*/, float right /*= 1.0f*/, float top /*= 1.0f*/, float bottom /*= -1.0f*/, int texture_repeat /*= 1*/)
    {
        /*
        GLfloat quadVertices[] = {

        // Positions   // TexCoords
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f
        1.0f, -1.0f, 1.0f, 0.0f,

        }; */

        GLfloat rectangle_vertices[24];
        rectangle_vertices[0] = left; rectangle_vertices[1] = top; rectangle_vertices[2] = 0.f; rectangle_vertices[3] = 1.0f * texture_repeat;
        rectangle_vertices[4] = left; rectangle_vertices[5] = bottom; rectangle_vertices[6] = 0.f; rectangle_vertices[7] = 0.f;
        rectangle_vertices[8] = right; rectangle_vertices[9] = bottom; rectangle_vertices[10] = 1.0f * texture_repeat; rectangle_vertices[11] = 0.f;

        rectangle_vertices[12] = left; rectangle_vertices[13] = top; rectangle_vertices[14] = 0.f; rectangle_vertices[15] = 1.0f * texture_repeat;
        rectangle_vertices[16] = right; rectangle_vertices[17] = bottom; rectangle_vertices[18] = 1.0f * texture_repeat; rectangle_vertices[19] = 0.f;
        rectangle_vertices[20] = right; rectangle_vertices[21] = top; rectangle_vertices[22] = 1.0f * texture_repeat; rectangle_vertices[23] = 1.0f * texture_repeat;

        VertexAttribute va_position;
        va_position.index = 0;
        va_position.normalized = GL_FALSE;
        va_position.type = GL_FLOAT;
        va_position.size = 2; // 一个“顶点位置”的属性由3个分量组成
        va_position.stride = 4 * sizeof(GLfloat); // 每个顶点的步长为 
        va_position.pointer = nullptr;

        VertexAttribute va_texture;
        va_texture.index = 1;
        va_texture.normalized = GL_FALSE;
        va_texture.type = GL_FLOAT;
        va_texture.size = 2; // 一个“顶点纹理坐标”的属性由2个分量组成
        va_texture.stride = 4 * sizeof(GLfloat); // 每个位置的步长为 
        va_texture.pointer = reinterpret_cast<GLvoid*> (2 * sizeof(GLfloat));

        std::vector<VertexAttribute> vtx_attri_array;
        vtx_attri_array.clear();
        vtx_attri_array.push_back(va_position);
        vtx_attri_array.push_back(va_texture);

        PrimitiveSPtr p = std::make_shared<Primitive>();
        p->Create(GL_TRIANGLES, rectangle_vertices, sizeof(rectangle_vertices), 6, GL_STATIC_DRAW, vtx_attri_array);
        return p;
    }

    PrimitiveSPtr PrimitiveTool::BuildPNTBT1Square(unsigned int quality, bool is_clock_wise)
    {
        // 正方形为25个，则顶点数为36个，三角形为50个，如上图
        int num_vertices = (quality + 1) * (quality + 1);
        int num_triangles = quality * quality * 2;

        // 因为使用三角形列表的方式进行渲染，所以索引值的个数为三角形的个数的三倍
        int num_indices = num_triangles * 3;
        std::vector<GLuint> indices;
        indices.resize(num_indices);

        // 写入三角形索引
        for (unsigned int row = 0; row < quality; row++)
        {
            for (unsigned int col = 0; col < quality; col++)
            {
                int base_index = 6 * (row * quality + col);

                indices[base_index + 0] = (row + 0) * (quality + 1) + col + 0;// 每个小正方形左上角的顶点               
                indices[base_index + 1] = (row + 1) * (quality + 1) + col + 1;// 每个小正方形右下角的顶点           
                indices[base_index + 2] = (row + 1) * (quality + 1) + col + 0;// 每个小正方形左下角的顶点           
                indices[base_index + 3] = (row + 0) * (quality + 1) + col + 0;// 每个小正方形左上角的顶点           
                indices[base_index + 4] = (row + 0) * (quality + 1) + col + 1;// 每个小正方形右上角的顶点                   
                indices[base_index + 5] = (row + 1) * (quality + 1) + col + 1;// 每个小正方形右下角的顶点
            }
        }

        std::vector<VertexPNTBT1> vertices;

        //计算顶点的坐标值
        for (unsigned int row = 0; row < quality + 1; row++)
        {
            for (unsigned int col = 0; col < quality + 1; col++)
            {
                VertexPNTBT1 v;
                // 按照行优先的原则，计算每行从左到右的顶点坐标。，以正方形的中心点为本图元的模型空间的坐标原点
                int index = row * (quality + 1) + col;

                // 顶点坐标
                v.position.x = -1.0f / 2 + col*1.0f / quality;
                v.position.y = 1.0f / 2 - row*1.0f / quality;
                v.position.z = 0.0f;

                // 纹理坐标
                v.texture_coord_1.x = static_cast<float>(col) / static_cast<float>(quality);
                v.texture_coord_1.y = static_cast<float>(row) / static_cast<float>(quality);

                // 法线
                v.normal.x = v.normal.y = 0.0f;
                v.normal.z = is_clock_wise ? 1.0f : -1.0f;

                vertices.push_back(v);
            }
        }

        // 计算每个顶点的副法线和切线
        for (int i = 0; i < num_indices; i += 3)
        {
            GLuint vtx_idx_0 = indices[i + 0];
            GLuint vtx_idx_1 = indices[i + 1];
            GLuint vtx_idx_2 = indices[i + 2];

            // 拿到组成三角形的三个顶点的坐标
            glm::vec3& v0 = vertices[vtx_idx_0].position;
            glm::vec3& v1 = vertices[vtx_idx_1].position;
            glm::vec3& v2 = vertices[vtx_idx_2].position;

            // 拿到组成三角形的三个顶点的纹理贴图坐标
            glm::vec2& uv0 = vertices[vtx_idx_0].texture_coord_1;
            glm::vec2& uv1 = vertices[vtx_idx_1].texture_coord_1;
            glm::vec2& uv2 = vertices[vtx_idx_2].texture_coord_1;

            // 拿到组成三角形两条边 v0->v1，v0->v2
            glm::vec3 delta_pos_1 = v1 - v0;
            glm::vec3 delta_pos_2 = v2 - v0;

            // 拿到组成三角形的纹理映射空间的两个变现
            glm::vec2 delta_uv_1 = uv1 - uv0;
            glm::vec2 delta_uv_2 = uv2 - uv0;

            // 计算切线和副法线：
            float r = 1.0f / (delta_uv_1.x * delta_uv_2.y - delta_uv_1.y * delta_uv_2.x);
            glm::vec3 tangent = (delta_pos_1 * delta_uv_2.y - delta_pos_2 * delta_uv_1.y)*r;
            glm::vec3 bitangent = (delta_pos_2 * delta_uv_1.x - delta_pos_1 * delta_uv_2.x)*r;

            vertices[vtx_idx_0].tangent =
                vertices[vtx_idx_1].tangent =
                vertices[vtx_idx_2].tangent = tangent;

            vertices[vtx_idx_0].binormal =
                vertices[vtx_idx_1].binormal =
                vertices[vtx_idx_2].binormal = bitangent;
        }

        GLsizei stride = sizeof(VertexPNTBT1);

        VertexAttribute va_position; // 顶点位置
        va_position.index = 0;
        va_position.normalized = GL_FALSE;
        va_position.type = GL_FLOAT;
        va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
        va_position.stride = stride; // 每个顶点的步长为sizeof(VertexPNTBT1)
        va_position.pointer = nullptr;

        VertexAttribute va_normal; // 顶点法线坐标
        va_normal.index = 1;
        va_normal.normalized = GL_FALSE;
        va_normal.type = GL_FLOAT;
        va_normal.size = 3; // 一个“顶点法线纹理坐标”的属性由3个分量组成
        va_normal.stride = stride; //  每个顶点的步长为sizeof(VertexPNTBT1)
        va_normal.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

        VertexAttribute va_tangent; // 顶点切线坐标
        va_tangent.index = 2;
        va_tangent.normalized = GL_TRUE;
        va_tangent.type = GL_FLOAT;
        va_tangent.size = 3; // 一个“顶点的切线”的属性由3个分量组成
        va_tangent.stride = stride; // 每个顶点的步长为sizeof(VertexPNTBT1)
        va_tangent.pointer = reinterpret_cast<GLvoid*> (6 * sizeof(GLfloat));

        VertexAttribute va_binormal; // 顶点副法线坐标
        va_binormal.index = 3;
        va_binormal.normalized = GL_FALSE;
        va_binormal.type = GL_FLOAT;
        va_binormal.size = 3; // 一个“顶点的副法线”的属性由3个分量组成
        va_binormal.stride = stride; // 每个顶点的步长为sizeof(VertexPNTBT1)
        va_binormal.pointer = reinterpret_cast<GLvoid*> (9 * sizeof(GLfloat));

        VertexAttribute va_tex_coord_1; // 顶点的第一层纹理坐标
        va_tex_coord_1.index = 4;
        va_tex_coord_1.normalized = GL_FALSE;
        va_tex_coord_1.type = GL_FLOAT;
        va_tex_coord_1.size = 2; // 一个“顶点的第一层纹理坐标”的属性由2个分量组成
        va_tex_coord_1.stride = stride; // 每个顶点的步长为sizeof(VertexPNTBT1)
        va_tex_coord_1.pointer = reinterpret_cast<GLvoid*> (12 * sizeof(GLfloat));

        std::vector<VertexAttribute> vtx_attri_array;
        vtx_attri_array.clear();
        vtx_attri_array.push_back(va_position);
        vtx_attri_array.push_back(va_normal);
        vtx_attri_array.push_back(va_tangent);
        vtx_attri_array.push_back(va_binormal);
        vtx_attri_array.push_back(va_tex_coord_1);

        PrimitiveSPtr p = std::make_shared<Primitive>();

        p->CreateIndexed(
            GL_TRIANGLES,
            reinterpret_cast<GLvoid*>(&(vertices[0])),
            sizeof(VertexPNTBT1)*vertices.size(),
            GL_STATIC_DRAW,
            kgl::Primitive::UINT32, // 顶点索引的类型
            reinterpret_cast<GLvoid*>(&(indices[0])),
            sizeof(GLuint)*indices.size(),
            GL_STATIC_DRAW, vtx_attri_array);

        return p;
    }

    PrimitiveSPtr PrimitiveTool::BuildSquare(unsigned int quality, bool is_clock_wise, const glm::vec4& vertex_color, const glm::vec2& textureOffset, const glm::vec2& textureScale)
    {
        // 正方形为25个，则顶点数为36个，三角形为50个，如上图
        int num_vertices = (quality + 1) * (quality + 1);
        int num_triangles = quality * quality * 2;

        // 因为使用三角形列表的方式进行渲染，所以索引值的个数为三角形的个数的三倍
        int num_indices = num_triangles * 3;
        std::vector<GLuint> indices;
        indices.resize(num_indices);

        // 写入三角形索引
        for (unsigned int row = 0; row < quality; row++)
        {
            for (unsigned int col = 0; col < quality; col++)
            {
                int baseIndex = 6 * (row * quality + col);

                indices[baseIndex + 0] = (row + 0) * (quality + 1) + col + 0;// 每个小正方形左上角的顶点                
                indices[baseIndex + 1] = (row + 1) * (quality + 1) + col + 1;// 每个小正方形右下角的顶点            
                indices[baseIndex + 2] = (row + 1) * (quality + 1) + col + 0;// 每个小正方形左下角的顶点            
                indices[baseIndex + 3] = (row + 0) * (quality + 1) + col + 0;// 每个小正方形左上角的顶点            
                indices[baseIndex + 4] = (row + 0) * (quality + 1) + col + 1;// 每个小正方形右上角的顶点                    
                indices[baseIndex + 5] = (row + 1) * (quality + 1) + col + 1;// 每个小正方形右下角的顶点
            }
        }

        std::vector<float> vertices;

        //计算顶点的坐标值
        for (unsigned int row = 0; row < quality + 1; row++)
        {
            for (unsigned int col = 0; col < quality + 1; col++)
            {
                // 按照行优先的原则，计算每行从左到右的顶点坐标。，以正方形的中心点为本图元的模型空间的坐标原点
                int index = row * (quality + 1) + col;

                float x = -1.0f / 2 + col*1.0f / quality;
                float y = 1.0f / 2 - row*1.0f / quality;

                // 顶点坐标
                vertices.push_back(x); // position x
                vertices.push_back(y); // position y
                vertices.push_back(0.0f); // position z

                // 纹理坐标
                float s = textureOffset.x + textureScale.x * col / quality;
                float t = textureOffset.y + textureScale.y * row / quality;
                vertices.push_back(s);
                vertices.push_back(t);

                // 法线
                vertices.push_back(0.0f);
                vertices.push_back(is_clock_wise ? 1.0f : -1.0f);
                vertices.push_back(0.0f);


                // 顶点颜色
                vertices.push_back(vertex_color.r);
                vertices.push_back(vertex_color.g);
                vertices.push_back(vertex_color.b);
                vertices.push_back(vertex_color.a);
            }
        }

        GLsizei stride = 12 * sizeof(GLfloat);

        VertexAttribute va_position;
        va_position.index = 0;
        va_position.normalized = GL_FALSE;
        va_position.type = GL_FLOAT;
        va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
        va_position.stride = stride; // 每个顶点的步长为 
        va_position.pointer = nullptr;

        VertexAttribute va_texture;
        va_texture.index = 1;
        va_texture.normalized = GL_FALSE;
        va_texture.type = GL_FLOAT;
        va_texture.size = 2; // 一个“顶点纹理坐标”的属性由2个分量组成
        va_texture.stride = stride; // 每个位置的步长为 
        va_texture.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

        VertexAttribute va_normal;
        va_normal.index = 2;
        va_normal.normalized = GL_TRUE;
        va_normal.type = GL_FLOAT;
        va_normal.size = 3; // 一个“法线”的属性由3个分量组成
        va_normal.stride = stride; // 每个顶点的步长为 
        va_normal.pointer = reinterpret_cast<GLvoid*> (5 * sizeof(GLfloat));

        VertexAttribute va_color;
        va_color.index = 3;
        va_color.normalized = GL_FALSE;
        va_color.type = GL_FLOAT;
        va_color.size = 4; // 一个“顶点颜色”的属性由2个分量组成
        va_color.stride = stride; // 每个位置的步长为 
        va_color.pointer = reinterpret_cast<GLvoid*> (8 * sizeof(GLfloat));

        std::vector<VertexAttribute> vtx_attri_array;
        vtx_attri_array.clear();
        vtx_attri_array.push_back(va_position);
        vtx_attri_array.push_back(va_texture);
        vtx_attri_array.push_back(va_normal);
        vtx_attri_array.push_back(va_color);

        PrimitiveSPtr p = std::make_shared<Primitive>();

        p->CreateIndexed(
            GL_TRIANGLES,
            reinterpret_cast<GLvoid*>(&(vertices[0])),
            sizeof(float)*vertices.size(),
            GL_STATIC_DRAW,
            kgl::Primitive::UINT32, // 顶点索引的类型
            reinterpret_cast<GLvoid*>(&(indices[0])),//&p_DeviceIBMem[0],
            sizeof(GLuint)*indices.size(),
            GL_STATIC_DRAW, vtx_attri_array);

        return p;
    }

    // unsigned int numSlices;  //沿着Y轴球的切片数
    // unsigned int numSegments;//每一切片的段数
    // unsigned int numTriangles;//Number of triangles used 
    kgl::PrimitiveSPtr PrimitiveTool::BuildSphere(uint32_t quality, bool is_ccw_cull, const glm::vec4& vertex_color /*= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)*/, const glm::vec2& texture_offset /*= glm::vec2(0.0f, 0.0f)*/, const glm::vec2& texture_scale /*= glm::vec2(1.0f, 1.0f)*/)
    {
        uint32_t num_slices = quality + 2;  //沿着Y轴球的切片数 切片数为quality+2， 
        uint32_t num_segments = quality + 3; // 每一切片的段数

        //每个切片分成的段，也就是说，每个切片由几个四边形（每个四边形由
        //两个三角形组成）组成，在渲染的时候，实际的segment是qulity+2
        uint32_t num_vertices = num_slices * num_segments;
        uint32_t num_triangles = 2 * (num_slices - 1) * (num_segments - 1); //三角形的数目，由上面的注解可以得知这个数量的来由
        uint32_t num_indices = num_triangles * 3; //因为使用的是三角形列表的方式来渲染，所以索引数为三角形数×3

        std::vector<uint32_t> indices;
        indices.resize(num_indices);

        //填充索引数
        for (uint32_t slice = 0; slice < (num_slices - 1); slice++)
        {
            for (uint32_t segment = 0; segment < (num_segments - 1); segment++)
            {
                uint32_t sliceBase = slice * num_segments;
                uint32_t segIndex = sliceBase + segment;
                uint32_t baseIndex = (slice * num_slices + segment) * 6;

                //递归填充某slice的某segment的两个三角形的六个顶点索引

                indices[baseIndex + 0] = segIndex;
                indices[baseIndex + 1] = segIndex + num_segments;
                indices[baseIndex + 2] = sliceBase + segment + 1;
                indices[baseIndex + 3] = sliceBase + segment + 1;
                indices[baseIndex + 4] = segIndex + num_segments;
                indices[baseIndex + 5] = segIndex + num_segments + 1;
            }
        }

        std::vector<float> vertices;

        for (unsigned int slice = 0; slice < num_slices; slice++)
        {
            float heightAngle = ((float)slice / (num_slices - 1)) * glm::pi<float>();
            float y = 0.5f * 1.0f * cosf(heightAngle);
            float radius = (float)fabs(0.5f*sinf(heightAngle));

            for (unsigned int segment = 0; segment < num_segments; segment++)
            {
                float angle = ((float)segment / num_slices) * 2 * glm::pi<float>();

                float x = radius * 1.0f * sinf(angle);
                float z = -radius * 1.0f * cosf(angle);

                unsigned int point = slice*num_segments + segment;
                // vertexMemory[point].position =D3DXVECTOR3(x, y, z);

                // 写入顶点的位置坐标
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                // 纹理坐标
                float s = texture_offset.x + texture_scale.x * (1.0f - (float)segment / (num_slices - 1));
                float t = texture_offset.y + texture_scale.y * ((float)slice / (num_slices - 1));
                vertices.push_back(s);
                vertices.push_back(t);

                glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));

                if (is_ccw_cull) // 逆时针拣选的话从看到外的内表面
                {
                    vertices.push_back(normal.x);
                    vertices.push_back(normal.y);
                    vertices.push_back(normal.z);
                }
                else // 顺时针拣选的话从看到球的内表面
                {
                    vertices.push_back(-normal.x);
                    vertices.push_back(-normal.y);
                    vertices.push_back(-normal.z);
                }

                // 顶点颜色
                vertices.push_back(vertex_color.r);
                vertices.push_back(vertex_color.g);
                vertices.push_back(vertex_color.b);
                vertices.push_back(vertex_color.a);
            }
        }

        GLsizei stride = 12 * sizeof(GLfloat);

        VertexAttribute va_position;
        va_position.index = 0;
        va_position.normalized = GL_FALSE;
        va_position.type = GL_FLOAT;
        va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
        va_position.stride = stride; // 每个顶点的步长为 
        va_position.pointer = nullptr;

        VertexAttribute va_texture;
        va_texture.index = 1;
        va_texture.normalized = GL_FALSE;
        va_texture.type = GL_FLOAT;
        va_texture.size = 2; // 一个“顶点纹理坐标”的属性由2个分量组成
        va_texture.stride = stride; // 每个位置的步长为 
        va_texture.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

        VertexAttribute va_normal;
        va_normal.index = 2;
        va_normal.normalized = GL_TRUE;
        va_normal.type = GL_FLOAT;
        va_normal.size = 3; // 一个“法线”的属性由3个分量组成
        va_normal.stride = stride; // 每个顶点的步长为 
        va_normal.pointer = reinterpret_cast<GLvoid*> (5 * sizeof(GLfloat));

        VertexAttribute va_color;
        va_color.index = 3;
        va_color.normalized = GL_FALSE;
        va_color.type = GL_FLOAT;
        va_color.size = 4; // 一个“顶点颜色”的属性由2个分量组成
        va_color.stride = stride; // 每个位置的步长为 
        va_color.pointer = reinterpret_cast<GLvoid*> (8 * sizeof(GLfloat));

        std::vector<VertexAttribute> vtx_attri_array;
        vtx_attri_array.clear();
        vtx_attri_array.push_back(va_position);
        vtx_attri_array.push_back(va_texture);
        vtx_attri_array.push_back(va_normal);
        vtx_attri_array.push_back(va_color);

        PrimitiveSPtr p = std::make_shared<Primitive>();

        p->CreateIndexed(
            GL_TRIANGLES,
            reinterpret_cast<GLvoid*>(&(vertices[0])),
            sizeof(float)*vertices.size(),
            GL_STATIC_DRAW,
            kgl::Primitive::UINT32, // 顶点索引的类型
            reinterpret_cast<GLvoid*>(&(indices[0])),//&p_DeviceIBMem[0],
            sizeof(GLuint)*indices.size(),
            GL_STATIC_DRAW, vtx_attri_array);

        return p;
    }


    PrimitiveSPtr PrimitiveTool::BuildNDCTextureNormalCube()
    {
        float vertices[] =
        {
            // back face
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
            1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom-right
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
            -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            // right face
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
            1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
            1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
            1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f  // bottom-left        
        };

        GLsizei stride = 8 * sizeof(GLfloat);

        VertexAttribute va_position;
        va_position.index = 0;
        va_position.normalized = GL_FALSE;
        va_position.type = GL_FLOAT;
        va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
        va_position.stride = stride; // 每个顶点的步长为 
        va_position.pointer = nullptr;

        VertexAttribute va_normal;
        va_normal.index = 1;
        va_normal.normalized = GL_TRUE;
        va_normal.type = GL_FLOAT;
        va_normal.size = 3; // 一个“法线”的属性由3个分量组成
        va_normal.stride = stride; // 每个顶点的步长为 
        va_normal.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

        VertexAttribute va_texture;
        va_texture.index = 2;
        va_texture.normalized = GL_FALSE;
        va_texture.type = GL_FLOAT;
        va_texture.size = 2; // 一个“顶点纹理坐标”的属性由2个分量组成
        va_texture.stride = stride; // 每个位置的步长为 
        va_texture.pointer = reinterpret_cast<GLvoid*> (6 * sizeof(GLfloat));

        std::vector<VertexAttribute> vtx_attri_array;
        vtx_attri_array.clear();
        vtx_attri_array.push_back(va_position);
        vtx_attri_array.push_back(va_normal);
        vtx_attri_array.push_back(va_texture);

        PrimitiveSPtr p = std::make_shared<Primitive>();
        GLenum primitive_mode = GL_TRIANGLES;
        GLsizeiptr vertices_byte_count = sizeof(float) * 36 * 8;
        GLint vertices_count = 36;
        GLenum vb_usage = GL_STATIC_DRAW;
        p->Create(GL_TRIANGLES, vertices, vertices_byte_count, vertices_count, vb_usage, vtx_attri_array);
        return p;
    }

    kgl::PrimitiveSPtr PrimitiveTool::BuildNormalTexturedXZPlane(float size, float y_offset)
    {
        float vertices[] =
        {
            // 位置坐标             // 法线             // 第一层纹理坐标
            size, y_offset, size, 0.0f, 1.0f, 0.0f, size, 0.0f,
            -size, y_offset, size, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -size, y_offset, -size, 0.0f, 1.0f, 0.0f, 0.0f, size,

            size, y_offset, size, 0.0f, 1.0f, 0.0f, size, 0.0f,
            -size, y_offset, -size, 0.0f, 1.0f, 0.0f, 0.0f, size,
            size, y_offset, -size, 0.0f, 1.0f, 0.0f, size, size
        };

        GLsizei stride = 8 * sizeof(GLfloat);

        VertexAttribute va_position;
        va_position.index = 0;
        va_position.normalized = GL_FALSE;
        va_position.type = GL_FLOAT;
        va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
        va_position.stride = stride; // 每个顶点的步长为 
        va_position.pointer = nullptr;

        VertexAttribute va_normal;
        va_normal.index = 1;
        va_normal.normalized = GL_TRUE;
        va_normal.type = GL_FLOAT;
        va_normal.size = 3; // 一个“法线”的属性由3个分量组成
        va_normal.stride = stride; // 每个顶点的步长为 
        va_normal.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

        VertexAttribute va_texture;
        va_texture.index = 2;
        va_texture.normalized = GL_FALSE;
        va_texture.type = GL_FLOAT;
        va_texture.size = 2; // 一个“顶点纹理坐标”的属性由2个分量组成
        va_texture.stride = stride; // 每个位置的步长为 
        va_texture.pointer = reinterpret_cast<GLvoid*> (6 * sizeof(GLfloat));

        std::vector<VertexAttribute> vtx_attri_array;
        vtx_attri_array.clear();
        vtx_attri_array.push_back(va_position);
        vtx_attri_array.push_back(va_normal);
        vtx_attri_array.push_back(va_texture);

        PrimitiveSPtr p = std::make_shared<Primitive>();
        GLenum primitive_mode = GL_TRIANGLES;
        GLsizeiptr vertices_byte_count = sizeof(float) * 6 * 8;
        GLint vertices_count = 6;
        GLenum vb_usage = GL_STATIC_DRAW;
        p->Create(GL_TRIANGLES, vertices, vertices_byte_count, vertices_count, vb_usage, vtx_attri_array);
        return p;
    }

    PrimitiveSPtr  PrimitiveTool::BuildNormalTexturedRectangle(unsigned int num_verts_along_x, unsigned int num_verts_along_y, float mesh_length_along_x, float mesh_length_along_y)
    {
        struct V3_N3_T2
        {
            float x, y, z;
            float nx, ny, nz;
            float tu, tv;
        };

        unsigned int mesh_vetex_count = (num_verts_along_x - 1) * (num_verts_along_y - 1) * 6;
        // Compute position deltas for moving down the X, and Z axis during mesh creation
        const float dX = (1.0f / (num_verts_along_x - 1));
        const float dZ = -(1.0f / (num_verts_along_y - 1));

        // Compute tex-coord deltas for moving down the X, and Z axis during mesh creation
        const float dTU = 1.0f / (num_verts_along_x - 1);
        const float dTV = 1.0f / (num_verts_along_y - 1);

        int i = 0;
        int x = 0;
        int y = 0;

        std::vector<V3_N3_T2> mesh_vertices;
        mesh_vertices.resize(mesh_vetex_count);

        // These are all the same...
        for (i = 0; i < mesh_vetex_count; ++i)
        {
            // Mesh tessellation occurs in the X,Z plane, so Y is always zero
            mesh_vertices[i].z = 0.0f;
            mesh_vertices[i].nx = 0.0f;
            mesh_vertices[i].ny = 0.0f;
            mesh_vertices[i].nz = -1.0f;
        }

        //
        // Create all the vertex points required by the mesh...
        //
        // Note: Mesh tessellation occurs in the X,Z plane.
        //

        // For each row of our mesh...
        for (y = 0, i = 0; y < (num_verts_along_y - 1); ++y)
        {
            // Fill the row with quads which are composed of two triangles each...
            for (x = 0; x < (num_verts_along_x - 1); ++x)
            {
                // First triangle of the current quad
                //   ___ 2
                //  |  /|
                //  |/__|
                // 0     1

                // 0
                mesh_vertices[i].x = mesh_length_along_x * x * dX;
                mesh_vertices[i].y = mesh_length_along_y * y * dZ;
                mesh_vertices[i].tu = x * dTU;
                mesh_vertices[i].tv = y * dTV;
                ++i;

                // 1
                mesh_vertices[i].x = mesh_length_along_x * (x + 1.0f) * dX;
                mesh_vertices[i].y = mesh_length_along_y * y * dZ;
                mesh_vertices[i].tu = (x + 1.0f) * dTU;
                mesh_vertices[i].tv = y * dTV;
                ++i;

                // 2
                mesh_vertices[i].x = mesh_length_along_x * (x + 1.0f) * dX;
                mesh_vertices[i].y = mesh_length_along_y * (y + 1.0f) * dZ;
                mesh_vertices[i].tu = (x + 1.0f) * dTU;
                mesh_vertices[i].tv = (y + 1.0f) * dTV;
                ++i;

                // Second triangle of the current quad
                // 2 ___ 1
                //  |  /|
                //  |/__|
                // 0

                // 0
                mesh_vertices[i].x = mesh_length_along_x * x * dX;
                mesh_vertices[i].y = mesh_length_along_y * y * dZ;
                mesh_vertices[i].tu = x * dTU;
                mesh_vertices[i].tv = y * dTV;
                ++i;

                // 1
                mesh_vertices[i].x = mesh_length_along_x * (x + 1.0f) * dX;
                mesh_vertices[i].y = mesh_length_along_y * (y + 1.0f) * dZ;
                mesh_vertices[i].tu = (x + 1.0f) * dTU;
                mesh_vertices[i].tv = (y + 1.0f) * dTV;
                ++i;

                // 2
                mesh_vertices[i].x = mesh_length_along_x * x * dX;
                mesh_vertices[i].y = mesh_length_along_y * (y + 1.0f) * dZ;
                mesh_vertices[i].tu = x * dTU;
                mesh_vertices[i].tv = (y + 1.0f) * dTV;
                ++i;
            }
        }


        GLsizei stride = 8 * sizeof(GLfloat);

        VertexAttribute va_position;
        va_position.index = 0;
        va_position.normalized = GL_FALSE;
        va_position.type = GL_FLOAT;
        va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
        va_position.stride = stride; // 每个顶点的步长为 
        va_position.pointer = nullptr;

        VertexAttribute va_normal;
        va_normal.index = 1;
        va_normal.normalized = GL_TRUE;
        va_normal.type = GL_FLOAT;
        va_normal.size = 3; // 一个“法线”的属性由3个分量组成
        va_normal.stride = stride; // 每个顶点的步长为 
        va_normal.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

        VertexAttribute va_texture;
        va_texture.index = 2;
        va_texture.normalized = GL_FALSE;
        va_texture.type = GL_FLOAT;
        va_texture.size = 2; // 一个“顶点纹理坐标”的属性由2个分量组成
        va_texture.stride = stride; // 每个位置的步长为 
        va_texture.pointer = reinterpret_cast<GLvoid*> (6 * sizeof(GLfloat));

        std::vector<VertexAttribute> vtx_attri_array;
        vtx_attri_array.clear();
        vtx_attri_array.push_back(va_position);
        vtx_attri_array.push_back(va_normal);
        vtx_attri_array.push_back(va_texture);

        PrimitiveSPtr p = std::make_shared<Primitive>();
        GLenum primitive_mode = GL_TRIANGLES;
        GLsizeiptr vertices_byte_count = sizeof(float) * mesh_vetex_count * 8;
        GLint vertices_count = mesh_vetex_count;
        GLenum vb_usage = GL_STATIC_DRAW;
        GLvoid* vertices_datas_pointer = &(mesh_vertices[0]);
        p->Create(GL_TRIANGLES, vertices_datas_pointer, vertices_byte_count, vertices_count, vb_usage, vtx_attri_array);
        return p;
    }
}