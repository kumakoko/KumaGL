#ifndef vertex_id_hashing_h__
#define vertex_id_hashing_h__

#include "GL/glew.h"
#include "mesh_stats.h"

namespace VertexIDHashingDefinition
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

#endif // vertex_id_hashing_h__