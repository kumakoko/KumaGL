#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "v3d.h"
#include "octree.h"

namespace DigitalSculpt
{
    enum DrawMode
    {
        AllOctants = 0,
        LeafOctants = 1,
        AllIntersectedOctants = 2,
        IntersectedLeafOctants = 3,
    };

    enum DepthColorMode
    {
        NoDepthColor = 0,
        EnableDepthColor = 1,
        EnableColorWithTriangles = 2,
        OctantColor = 3
    };

    enum DepthColor
    {
        White = 0,
        Red = 1,
        Cyan = 2,
        Green = 3,
        Yellow = 4,
        Magenta = 5,
        Blue = 6,
        Pink = 7,
        Orange = 8,
        Purple = 9
    };

    struct OctreeWireframe
    {
        std::vector<GLuint> octreeIndices;
        std::vector<V3D> octreeVertices;
        bool shouldDraw = false;
    };

    class OctreeVisualization : public Octree
    {
    public:
        void visualizeOctree(DepthColorMode = EnableDepthColor);
        void generateOctantWireframe(int octantID);
        void bindOctreeWireframe();
        void refreshOctreeWireFrame();
        void drawOctreeWireframe();
        void colorTrianglePerOctant();
        void toggleOctreeVisualization();

        DrawMode drawMode = LeafOctants;
        DepthColorMode depthColorMode = EnableDepthColor;
        OctreeWireframe octreeWireframe;
        GraphicsBufferObject gboOctreeWireframe;
        bool shouldBind = true;


        glm::vec4 DepthColors[20] = 
        {
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), // White
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), // Red
            glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), // Cyan
            glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // Green
            glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), // Yellow
            glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), // Magenta
            glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // Blue
            glm::vec4(1.0f, 0.5f, 0.5f, 1.0f), // Pink
            glm::vec4(1.0f, 0.5f, 0.0f, 1.0f), // Orange
            glm::vec4(0.5f, 0.5f, 1.0f, 1.0f), // Purple
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), // White
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), // Red
            glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), // Cyan
            glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // Green
            glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), // Yellow
            glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), // Magenta
            glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // Blue
            glm::vec4(1.0f, 0.5f, 0.5f, 1.0f), // Pink
            glm::vec4(1.0f, 0.5f, 0.0f, 1.0f), // Orange
            glm::vec4(0.5f, 0.5f, 1.0f, 1.0f)  // Purple
        };
    };

    extern const std::vector<GLuint> octantWireframeBase;
}