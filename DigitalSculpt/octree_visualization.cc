#include "GL/glew.h"
#include "octree_visualization.h"
#include "open_gl_wrapper.h"
    
namespace DigitalSculpt
{
    const std::vector<GLuint> octantWireframeBase = {
        // Bottom
        BottomBackLeft, /*  */ BottomFrontLeft,  //
        BottomFrontLeft, /* */ BottomFrontRight, //
        BottomFrontRight, /**/ BottomBackRight,  //
        BottomBackRight, /* */ BottomBackLeft,   //

        // Top
        TopBackLeft, /*     */ TopFrontLeft,  //
        TopFrontLeft, /*    */ TopFrontRight, //
        TopFrontRight, /*   */ TopBackRight,  //
        TopBackRight, /*    */ TopBackLeft,   //

        // Left
        BottomBackLeft, /*  */ TopBackLeft,     //
        TopBackLeft, /*     */ TopFrontLeft,    //
        TopFrontLeft, /*    */ BottomFrontLeft, //
        BottomFrontLeft, /* */ BottomBackLeft,  //

        // Right
        BottomBackRight, /* */ TopBackRight,     //
        TopBackRight, /*    */ TopFrontRight,    //
        TopFrontRight, /*   */ BottomFrontRight, //
        BottomFrontRight, /**/ BottomBackRight,  //
    };

    void OctreeVisualization::toggleOctreeVisualization() 
    {
        octreeWireframe.shouldDraw = !octreeWireframe.shouldDraw;
        if (shouldBind) {
            bindOctreeWireframe();
            shouldBind = false;
        }
        if (octreeWireframe.shouldDraw) {
            visualizeOctree();
        }
    }

    void OctreeVisualization::visualizeOctree(DepthColorMode depthColorMode)
    {
        this->depthColorMode = depthColorMode;

        // octreeWireframe.shouldDraw = true;
        octreeWireframe.octreeVertices.clear();
        octreeWireframe.octreeIndices.clear();

        octreeWireframe.octreeVertices.reserve(octants.size() * 8);    // Every octant has 8 vertices
        octreeWireframe.octreeIndices.reserve(octants.size() * 8 * 2); // Every vertex is repeated twice

        for(auto& octant : octants)
        {
            generateOctantWireframe(octant.octantIndex);
        }

        colorTrianglePerOctant();
        refreshOctreeWireFrame();
        refresh();
    }

    void OctreeVisualization::generateOctantWireframe(int octantID)
    {
        Octant& octant = octants[octantID];

        if ((octant.triangleIDs->size() == 0 && (drawMode == LeafOctants || drawMode == IntersectedLeafOctants)) ||
            (!octant.intersected && (drawMode == IntersectedLeafOctants || drawMode == AllIntersectedOctants)))
        {
            return;
        }

        int currentIndex = (int)octreeWireframe.octreeVertices.size();
        int curr = octantID;
        int octantDepth = 0;

        while (curr != root)
        {
            curr = octants[curr].parent;
            octantDepth++;
        }

        for (int i = 0; i < 8; i++)
        {
            V3D vertex = V3D(octant.octantCenter + octant.octantHalfSize * octantPositionVectors[(OctantPosition)i]);

            if (depthColorMode == EnableDepthColor)
            {
                vertex.color = DepthColors[(DepthColor)octantDepth];
            }
            else if (depthColorMode == OctantColor)
            {
                vertex.color = octant.color;
            }
            octreeWireframe.octreeVertices.emplace_back(vertex);
        }

        //std::vector<GLuint> octantWireframe = octantWireframeBase;

        for(auto& index : octantWireframeBase)
        {
            octreeWireframe.octreeIndices.emplace_back(currentIndex + index);
        }
    }

    void OctreeVisualization::bindOctreeWireframe()
    {
        glGenVertexArrays(1, &gboOctreeWireframe.vao);
        glGenBuffers(1, &gboOctreeWireframe.vbo);
        glGenBuffers(1, &gboOctreeWireframe.ebo);

        refreshOctreeWireFrame();
    }

    void OctreeVisualization::refreshOctreeWireFrame()
    {
        glBindVertexArray(gboOctreeWireframe.vao);

        // binding vertices
        glBindBuffer(GL_ARRAY_BUFFER, gboOctreeWireframe.vbo);
        glBufferData(GL_ARRAY_BUFFER, octreeWireframe.octreeVertices.size() * sizeof(V3D), octreeWireframe.octreeVertices.data(), GL_STATIC_DRAW);

        // binding indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gboOctreeWireframe.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, octreeWireframe.octreeIndices.size() * sizeof(GLuint), static_cast<void*>(octreeWireframe.octreeIndices.data()), GL_STATIC_DRAW);

        // enable position data
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, OpenGLVertexAttributes::SizeOfVertex, (void*)0);
        // enable color data
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, OpenGLVertexAttributes::SizeOfVertex, (void*)offsetof(V3D, V3D::color)); // color data reserves 4 slots.
    }

    void OctreeVisualization::drawOctreeWireframe()
    {
        if (octreeWireframe.shouldDraw)
        {
            refreshOctreeWireFrame();
            GridShader.use(); // want it independent of wireframe
            gboOctreeWireframe.bindVAO();
            glDrawElements(GL_LINES, (GLsizei)octreeWireframe.octreeIndices.size(), GL_UNSIGNED_INT, NULL);
            GL::unbindActiveVAO();
        }
    }

    // random_device rd;
    // mt19937 e2(rd());
    // normal_distribution<> dist(50, 100);

    void OctreeVisualization::colorTrianglePerOctant()
    {
        glm::vec4 black(0.0f, 0.0f, 0.0f, 0.0f);
        colorDataUniformly(black);

        for(auto& octant :octants)
        {
            glm::vec4 color = octant.color;

            for(auto& triangle : *(octant.triangleIDs))
            {
                for (int i = 0; i < 3; i++)
                {
                    V3D& vertex = vertices[triangles[triangle][i]];
                    vertex.color = vertex.color == black ? color : 0.5f * (vertex.color + color);
                }
            }
        }
    }
}