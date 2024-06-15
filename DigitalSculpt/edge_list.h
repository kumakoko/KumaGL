#pragma once
//#ifndef edge_list_h__
//#define edge_list_h__

#include <unordered_set>
#include <vector>
#include <utility>

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "vertex_look_up.h"
#include "edge.h"

namespace DigitalSculpt
{
    class EdgeList : public VertexLookUp
    {
    public:
        EdgeList();
        ~EdgeList();

        Edge& getEdge(GLuint key);
        const Edge& getEdge(GLuint key) const;

        void generateEdges();

        void cleanUpEdges();

        void printAllEdges();

        const int edgesTotal() const ;

        glm::vec3 averageAt(GLuint key) ;

        glm::vec4 colorAverageAt(GLuint key) ;

        std::vector<GLuint> getEdgeTriangles(const std::pair<GLuint, GLuint>& edge) const;
        std::vector<GLuint> getEdgeTriangles(GLuint v1, GLuint v2) const;

        std::vector<Edge> edges;



    };
}

//#endif // edge_list_h__

