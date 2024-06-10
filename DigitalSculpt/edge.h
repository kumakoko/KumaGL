#ifndef edge_h__
#define edge_h__

#include <set>
#include <unordered_set>
#include "GL/glew.h"
#include "vertex_look_up.h"

namespace DigitalSculpt
{
    struct Edge
    {
    public:
        Edge();
        ~Edge();
        void cleanUp(); // remove all duplicate edges that might occur during loading.
        void printEdge(); // print the edge to console

        bool touched = false;
        std::unordered_set<GLuint> vertexEdges;
    };
}

#endif // edge_h__
