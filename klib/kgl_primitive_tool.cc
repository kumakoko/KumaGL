// 图元辅助工具
#include "kgl_lib_pch.h"
#include "kgl_primitive_tool.h"
#include "kgl_vertex_attribute.h"

namespace kgl
{
    PrimitiveSPtr PrimitiveTool::BuildCube()
    {
        GLfloat vertices[] =
        {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,1.0f,
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
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,1.0f
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
            // 顶点          // 纹理坐标
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
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


	PrimitiveSPtr PrimitiveTool::BuildTexturedXZPlane(float y_height, float x_scale, float z_scale, int texture_repeart)
    {
        GLfloat plane_vertices[] = {
            // 坐标位置          // 纹理贴图坐标
            0.5f, 0.0f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.0f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.0f, -0.5f, 0.0f, 1.0f,

            0.5f,  0.0f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.0f, -0.5f, 0.0f, 1.0f,
            0.5f,  0.0f, -0.5f, 1.0f, 1.0f
        };
        
        // 赋值计算
        for (int i = 0; i < 6; ++i)
        {
            plane_vertices[i * 5 + 0] = plane_vertices[i * 5 + 0] * x_scale; // 调整x值
            plane_vertices[i * 5 + 1] = y_height; // 调整y值
            plane_vertices[i * 5 + 2] = plane_vertices[i * 5 + 2] * z_scale;
        }

        plane_vertices[3] = plane_vertices[3] * texture_repeart;
        plane_vertices[14] = plane_vertices[14] * texture_repeart;
        plane_vertices[18] = plane_vertices[18] * texture_repeart;
        plane_vertices[24] = plane_vertices[24] * texture_repeart;
        plane_vertices[28] = plane_vertices[28] * texture_repeart;
        plane_vertices[29] = plane_vertices[29] * texture_repeart;
        
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
        p->Create(GL_TRIANGLES, plane_vertices, sizeof(plane_vertices), 6, GL_STATIC_DRAW, vtx_attri_array);
        return p;
    }

    PrimitiveSPtr PrimitiveTool::BuildNDCTexturedRectange(float left, float right, float top, float bottom, int texture_repeart)
    {
        /*
        GLfloat quadVertices[] = {
            // Positions   // TexCoords
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f
        }; */

        GLfloat rectangle_vertices[24];
        rectangle_vertices[0] = left; rectangle_vertices[1] = top; rectangle_vertices[2] = 0.f; rectangle_vertices[3] = 1.0f * texture_repeart;
        rectangle_vertices[4] = left; rectangle_vertices[5] = bottom; rectangle_vertices[6] = 0.f; rectangle_vertices[7] = 0.f;
        rectangle_vertices[8] = right; rectangle_vertices[9] = bottom; rectangle_vertices[10] = 1.0f * texture_repeart; rectangle_vertices[11] = 0.f;

        rectangle_vertices[12] = left; rectangle_vertices[13] = top; rectangle_vertices[14] = 0.f; rectangle_vertices[15] = 1.0f * texture_repeart;
        rectangle_vertices[16] = right; rectangle_vertices[17] = bottom; rectangle_vertices[18] = 1.0f * texture_repeart; rectangle_vertices[19] = 0.f;
        rectangle_vertices[20] = right; rectangle_vertices[21] = top; rectangle_vertices[22] = 1.0f * texture_repeart; rectangle_vertices[23] = 1.0f * texture_repeart;

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
}