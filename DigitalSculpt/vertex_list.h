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
        VertexList();
        ~VertexList();

        const V3D& getVertex(int index) const;
        V3D& getVertex(int index); // access element reference by index

        void setVertex(const V3D& element, int index); // set element by index

        void colorDataUniformly(const glm::vec4& uniformColor);

        const int verticesMemorySize(); // returns the memory size of vertices allocate within the vertex list

        int verticeCount(); // returns the count of vertices in the vertex list

        std::vector<V3D> vertices; //

        std::vector<glm::vec3> normalList;

        const V3D* getVertexArray(); // get the underlying array component.

        std::vector<int> getTrianglesFromVertices(const std::vector<GLuint>& vertexIDs); // Get all triangles that contain any of the given vertices
    };
}
