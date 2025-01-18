/* Copyright (c) 2014-2017, ARM Limited and Contributors
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_error.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_string_convertor.h"
#include "mesh.h"

Vertex::Vertex()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    normal = glm::vec3(0.0f, 0.0f, 1.0f);
    tex = glm::vec2(0.0f, 0.0f);
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 tex)
    : position(position), normal(normal), tex(tex)
{
}

GLDrawable::GLDrawable()
{
    aabb.minpos = glm::vec4(0.0f);
    aabb.maxpos = glm::vec4(0.0f);

    GL_CHECK_SIMPLE(glGenVertexArrays(1, &vertex_array_object_));
    GL_CHECK_SIMPLE(glGenBuffers(1, &vertex_buffer_object_));
    GL_CHECK_SIMPLE(glGenBuffers(1, &index_buffer_object_));
    GL_CHECK_SIMPLE(glBindVertexArray(vertex_array_object_));
    GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_));

    static const float vertices[] = 
    {
        -1, -1,
         1, -1,
        -1,  1,
         1,  1,
    };

    static const uint16_t indices[] = { 0, 1, 2, 3, 2, 1, };

    GL_CHECK_SIMPLE(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    GL_CHECK_SIMPLE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_));
    GL_CHECK_SIMPLE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
    GL_CHECK_SIMPLE(glEnableVertexAttribArray(0));
    GL_CHECK_SIMPLE(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0));
    GL_CHECK_SIMPLE(glBindVertexArray(0));
    GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CHECK_SIMPLE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    num_elements_ = 6;
}

GLDrawable::GLDrawable(const Mesh& mesh)
{
    aabb = mesh.aabb;
    num_elements_ = mesh.ibo.size();

    GL_CHECK_SIMPLE(glGenVertexArrays(1, &vertex_array_object_));
    GL_CHECK_SIMPLE(glGenBuffers(1, &vertex_buffer_object_));
    GL_CHECK_SIMPLE(glGenBuffers(1, &index_buffer_object_));

    GL_CHECK_SIMPLE(glBindVertexArray(vertex_array_object_));

    GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_));
    GL_CHECK_SIMPLE(glBufferData(GL_ARRAY_BUFFER, mesh.vbo.size() * sizeof(Vertex), &mesh.vbo[0], GL_STATIC_DRAW));

    GL_CHECK_SIMPLE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_));
    GL_CHECK_SIMPLE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo.size() * sizeof(uint16_t), &mesh.ibo[0], GL_STATIC_DRAW));

    // Vertex position
    GL_CHECK_SIMPLE(glEnableVertexAttribArray(0));
    GL_CHECK_SIMPLE(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(offsetof(Vertex, position))));

    // Normal
    GL_CHECK_SIMPLE(glEnableVertexAttribArray(1));
    GL_CHECK_SIMPLE(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(offsetof(Vertex, normal))));

    // Tex coord
    GL_CHECK_SIMPLE(glEnableVertexAttribArray(2));
    GL_CHECK_SIMPLE(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(offsetof(Vertex, tex))));

    GL_CHECK_SIMPLE(glBindVertexArray(0));
    GL_CHECK_SIMPLE(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CHECK_SIMPLE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

GLDrawable::~GLDrawable()
{
    glDeleteVertexArrays(1, &vertex_array_object_);
    glDeleteBuffers(1, &vertex_buffer_object_);
    glDeleteBuffers(1, &index_buffer_object_);
}

const AABB& GLDrawable::get_aabb() const
{
    return aabb;
}

uint32_t GLDrawable::get_num_elements() const
{
    return num_elements_;
}

GLuint GLDrawable::get_vertex_array() const
{
    return vertex_array_object_;
}

Mesh create_sphere_mesh(float radius, glm::vec3 center, uint32_t vertices_per_circumference)
{
    Mesh mesh;

    mesh.vbo.resize(2 + vertices_per_circumference * vertices_per_circumference);

    // No idea if the tangents here are correct.

    // Bottom
    mesh.vbo[0] = Vertex(glm::vec3(0, -1, 0) * glm::vec3(radius) + center, glm::vec3(0, -1, 0), glm::vec2(0.5f, 0.0f));
    // Top
    mesh.vbo[vertices_per_circumference * vertices_per_circumference + 1] = Vertex(glm::vec3(0, +1, 0) * glm::vec3(radius) + center, glm::vec3(0, +1, 0), glm::vec2(0.5f, 1.0f));

    for (uint32_t y = 0; y < vertices_per_circumference; y++)
    {
        for (uint32_t x = 0; x < vertices_per_circumference; x++)
        {
            float pos_y = sin(glm::pi<float>() * (float(y + 1) / (vertices_per_circumference + 1) - 0.5f));
            float xz_mod = sqrt(1.0f - pos_y * pos_y);
            float radians_rot_y = 2.0f * glm::pi<float>() * x / vertices_per_circumference;
            float pos_x = xz_mod * cos(radians_rot_y);
            float pos_z = xz_mod * -sin(radians_rot_y);

            glm::vec3 normal(pos_x, pos_y, pos_z);

            mesh.vbo[y * vertices_per_circumference + x + 1] = Vertex(
                glm::vec3(radius) * normal + center,
                normal,
                glm::vec2(float(x) / vertices_per_circumference, (y + 1.0f) / (vertices_per_circumference + 1))
            );
        }
    }

    // Bottom
    for (uint32_t x = 0; x < vertices_per_circumference - 1; x++)
    {
        mesh.ibo.push_back(x + 1);
        mesh.ibo.push_back(0);
        mesh.ibo.push_back(x + 2);
    }
    mesh.ibo.push_back(vertices_per_circumference);
    mesh.ibo.push_back(0);
    mesh.ibo.push_back(1);

    // Quads
    for (uint32_t y = 0; y < vertices_per_circumference - 1; y++)
    {
        for (uint32_t x = 0; x < vertices_per_circumference - 1; x++)
        {
            uint32_t base = 1 + y * vertices_per_circumference + x;
            mesh.ibo.push_back(base);
            mesh.ibo.push_back(base + 1);
            mesh.ibo.push_back(base + vertices_per_circumference);
            mesh.ibo.push_back(base + vertices_per_circumference + 1);
            mesh.ibo.push_back(base + vertices_per_circumference);
            mesh.ibo.push_back(base + 1);
        }
        uint32_t base = 1 + y * vertices_per_circumference + (vertices_per_circumference - 1);
        mesh.ibo.push_back(base);
        mesh.ibo.push_back(base - (vertices_per_circumference - 1));
        mesh.ibo.push_back(base + vertices_per_circumference);
        mesh.ibo.push_back(base + 1);
        mesh.ibo.push_back(base + vertices_per_circumference);
        mesh.ibo.push_back(base - (vertices_per_circumference - 1));
    }

    // Top
    for (uint32_t x = 0; x < vertices_per_circumference - 1; x++)
    {
        uint32_t base = 1 + (vertices_per_circumference - 1) * vertices_per_circumference + x;
        mesh.ibo.push_back(base);
        mesh.ibo.push_back(base + 1);
        mesh.ibo.push_back(vertices_per_circumference * vertices_per_circumference + 1);
    }
    mesh.ibo.push_back(1 + (vertices_per_circumference - 1) * vertices_per_circumference + vertices_per_circumference - 1);
    mesh.ibo.push_back(1 + (vertices_per_circumference - 1) * vertices_per_circumference);
    mesh.ibo.push_back(vertices_per_circumference * vertices_per_circumference + 1);

    mesh.aabb.minpos = glm::vec4(center - glm::vec3(radius), 0.0f);
    mesh.aabb.maxpos = glm::vec4(center + glm::vec3(radius), 0.0f);

    return mesh;
}

Mesh create_box_mesh(const AABB& aabb)
{
    static const Vertex vertex_data[] = {
        Vertex(glm::vec3(-1, -1,  1), glm::vec3(0, 0,  1), glm::vec2(0, 0)), // Front
        Vertex(glm::vec3(1, -1,  1), glm::vec3(0, 0,  1), glm::vec2(1, 0)),
        Vertex(glm::vec3(-1,  1,  1), glm::vec3(0, 0,  1), glm::vec2(0, 1)),
        Vertex(glm::vec3(1,  1,  1), glm::vec3(0, 0,  1), glm::vec2(1, 1)),

        Vertex(glm::vec3(1, -1, -1), glm::vec3(0, 0, -1), glm::vec2(0, 0)), // Back
        Vertex(glm::vec3(-1, -1, -1), glm::vec3(0, 0, -1), glm::vec2(1, 0)),
        Vertex(glm::vec3(1,  1, -1), glm::vec3(0, 0, -1), glm::vec2(0, 1)),
        Vertex(glm::vec3(-1,  1, -1), glm::vec3(0, 0, -1), glm::vec2(1, 1)),

        Vertex(glm::vec3(-1, -1, -1), glm::vec3(-1, 0,  0), glm::vec2(0, 0)), // Left
        Vertex(glm::vec3(-1, -1,  1), glm::vec3(-1, 0,  0), glm::vec2(1, 0)),
        Vertex(glm::vec3(-1,  1, -1), glm::vec3(-1, 0,  0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-1,  1,  1), glm::vec3(-1, 0,  0), glm::vec2(1, 1)),

        Vertex(glm::vec3(1, -1,  1), glm::vec3(1, 0,  0), glm::vec2(0, 0)), // Right
        Vertex(glm::vec3(1, -1, -1), glm::vec3(1, 0,  0), glm::vec2(1, 0)),
        Vertex(glm::vec3(1,  1,  1), glm::vec3(1, 0,  0), glm::vec2(0, 1)),
        Vertex(glm::vec3(1,  1, -1), glm::vec3(1, 0,  0), glm::vec2(1, 1)),

        Vertex(glm::vec3(-1,  1,  1), glm::vec3(0, 1,  0), glm::vec2(0, 0)), // Top
        Vertex(glm::vec3(1,  1,  1), glm::vec3(0, 1,  0), glm::vec2(1, 0)),
        Vertex(glm::vec3(-1,  1, -1), glm::vec3(0, 1,  0), glm::vec2(0, 1)),
        Vertex(glm::vec3(1,  1, -1), glm::vec3(0, 1,  0), glm::vec2(1, 1)),

        Vertex(glm::vec3(-1, -1, -1), glm::vec3(0, -1, 0), glm::vec2(0, 0)), // Bottom
        Vertex(glm::vec3(1, -1, -1), glm::vec3(0, -1, 0), glm::vec2(1, 0)),
        Vertex(glm::vec3(-1, -1,  1), glm::vec3(0, -1, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(1, -1,  1), glm::vec3(0, -1, 0), glm::vec2(1, 1)),
    };

    static const GLushort indices[] = {
        0, 1, 2, // Front
        3, 2, 1,

        4, 5, 6, // Back
        7, 6, 5,

        8, 9, 10, // Left
        11, 10, 9,

        12, 13, 14, // Right
        15, 14, 13,

        16, 17, 18, // Top
        19, 18, 17,

        20, 21, 22, // Bottom
        23, 22, 21,
    };

    Mesh mesh;
    mesh.aabb = aabb;

    glm::vec3 half_position = glm::vec3(0.5f) * (glm::vec3(aabb.minpos) + glm::vec3(aabb.maxpos));
    glm::vec3 half_distance = glm::vec3(0.5f) * (glm::vec3(aabb.maxpos) - glm::vec3(aabb.minpos));

    for (uint32_t i = 0; i < sizeof(vertex_data) / sizeof(Vertex); i++)
    {
        const Vertex& vert = vertex_data[i];
        glm::vec3 pos = half_position + half_distance * vert.position;
        mesh.vbo.push_back(Vertex(pos, vert.normal, vert.tex));
    }

    mesh.ibo.insert(mesh.ibo.begin(), indices, indices + sizeof(indices) / sizeof(indices[0]));
    return mesh;
}