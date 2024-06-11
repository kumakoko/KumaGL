// 顶点列表

#include "v3d.h"
#include "indices.h"
#include <unordered_set>
#include <vector>

namespace DigitalSculpt
{
    class VertexList
    {
    public:
        /******************************************************************************************************************
         * Desc:
         * Method:    VertexList
         * Returns:
         ****************************************************************************************************************/
        VertexList();

        /******************************************************************************************************************
         * Desc:
         * Method:    ~VertexList
         * Returns:
         ****************************************************************************************************************/
        ~VertexList();

        /******************************************************************************************************************
         * Desc:
         * Method:    getVertex
         * Returns:   const DigitalSculpt::V3D&
         * Parameter: int index
         ****************************************************************************************************************/
        const V3D& getVertex(int index) const;

        /******************************************************************************************************************
         * Desc:
         * Method:    getVertex
         * Returns:   DigitalSculpt::V3D&
         * Parameter: int index
         ****************************************************************************************************************/
        V3D& getVertex(int index); // access element reference by index

        /******************************************************************************************************************
         * Desc:
         * Method:    setVertex
         * Returns:   void
         * Parameter: const V3D & element
         * Parameter: int index
         ****************************************************************************************************************/
        void setVertex(const V3D& element, int index); // set element by index

        /******************************************************************************************************************
         * Desc:
         * Method:    colorDataUniformly
         * Returns:   void
         * Parameter: const glm::vec4 & uniformColor
         ****************************************************************************************************************/
        void colorDataUniformly(const glm::vec4& uniformColor);

        /******************************************************************************************************************
         * Desc:
         * Method:    verticesMemorySize
         * Returns:   const int
         ****************************************************************************************************************/
        const int verticesMemorySize(); // returns the memory size of vertices allocate within the vertex list

        /******************************************************************************************************************
         * Desc:
         * Method:    verticeCount
         * Returns:   int
         ****************************************************************************************************************/
        int verticeCount(); // returns the count of vertices in the vertex list

        /******************************************************************************************************************
         * Desc:
         * Method:    getVertexArray
         * Returns:   const DigitalSculpt::V3D*
         ****************************************************************************************************************/
        const V3D* getVertexArray(); // get the underlying array component.

        /******************************************************************************************************************
         * Desc:
         * Method:    getTrianglesFromVertices
         * Returns:   std::vector<int>
         * Parameter: const std::vector<GLuint> & vertexIDs
         ****************************************************************************************************************/
        std::vector<int> getTrianglesFromVertices(const std::vector<GLuint>& vertexIDs); // Get all triangles that contain any of the given vertices

        std::vector<V3D> vertices; //

        std::vector<glm::vec3> normalList;
    };
}
