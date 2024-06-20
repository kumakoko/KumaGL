#pragma once

#include <vector>
#include <unordered_set>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "octree_visualization.h"

namespace DigitalSculpt
{
    extern const float InfiniteCreaseAngle;
    // #define PI 3.141592f
    extern const float TWO_PI;

    class SubdivisionSurface : public OctreeVisualization
    {
    public:
        /***************************************************
        
        @name: DigitalSculpt::SubdivisionSurface::loopSubdivision
        @return: void
        @param: int level
        @param: bool rebuildRefresh
        ***************************************************/
        void loopSubdivision(int level = 1, bool rebuildRefresh = true);

        /***************************************************
        
        @name: DigitalSculpt::SubdivisionSurface::getEdgeMidpoint
        @return: glm::vec3
        @param: GLuint v1
        @param: GLuint v2
        ***************************************************/
        inline glm::vec3 getEdgeMidpoint(GLuint v1, GLuint v2);

        /***************************************************
        
        @name: DigitalSculpt::SubdivisionSurface::subdivisionTest
        @return: void
        ***************************************************/
        void subdivisionTest();

        /***************************************************
        
        @name: DigitalSculpt::SubdivisionSurface::getBeta
        @return: float
        @param: int nEdges
        ***************************************************/
        float getBeta(int nEdges);

        /***************************************************
        
        @name: DigitalSculpt::SubdivisionSurface::sumNeighbors
        @return: glm::vec3
        @param: unordered_set<GLuint> neighbors
        ***************************************************/
        glm::vec3 sumNeighbors(std::unordered_set<GLuint> neighbors);

        /***************************************************
        
        @name: DigitalSculpt::SubdivisionSurface::isVertexOnCrease
        @return: bool
        @param: GLuint vertexID
        ***************************************************/
        bool isVertexOnCrease(GLuint vertexID);

        /***************************************************
        
        @name: DigitalSculpt::SubdivisionSurface::getCreaseAngleOfNewVertex
        @return: float
        @param: GLuint vertexID
        ***************************************************/
        float getCreaseAngleOfNewVertex(GLuint vertexID);

        /***************************************************
        
        @name: DigitalSculpt::SubdivisionSurface::simpleSubdivision4to1
        @return: void
        @param: int level
        @param: bool octreeRebuild
        @param: bool refreshDisplay
        @param: bool isLoop
        ***************************************************/
        void simpleSubdivision4to1(int level = 1, bool octreeRebuild = true, bool refreshDisplay = true, bool isLoop = false);

        /***************************************************
        
        @name: DigitalSculpt::SubdivisionSurface::flipEdge
        @return: void
        @param: GLuint v1
        @param: GLuint v2
        ***************************************************/
        void flipEdge(GLuint v1, GLuint v2);

        int vertexOffset;
        float angleLimit = 1.5708f; // Radians, 90 degrees

        // vector<unordered_map<char, int>> vertexABCDs;
        std::vector<std::vector<GLuint>> vertexABCDs;
    };

}