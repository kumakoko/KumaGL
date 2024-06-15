#pragma once

#include "GL/glew.h"
#include "mesh_stats.h"

namespace DigitalSculpt
{
    struct point_hash
    {
        std::size_t operator()(const GLuint &p) const
        {
            return std::hash<int>()(p);
        }
    };

    struct point_equal
    {
        bool operator()(const GLuint& p1, const GLuint& p2) const
        {
            return p1 == p2;
        }
    };
}