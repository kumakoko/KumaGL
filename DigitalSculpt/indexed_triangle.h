// 三角形

#ifndef indexed_triangle_h__
#define indexed_triangle_h__

#include <vector>
#include "indices.h"

namespace DigitalSculpt
{
    struct IndexedTriangle
    {
    public:
        IndexedTriangle();
        IndexedTriangle(GLuint v0, GLuint v1, GLuint v2);
        ~IndexedTriangle();
        GLuint& operator[](const int pos); // look up operator to directly access the underlying component of the indices
        const GLuint& operator[](const int pos) const;
        void print();      // print the 3 indices in order to console

        /// <summary>
        /// 每三个顶点索引构成一个三角形
        /// </summary>
        GLuint indice[3]; // the actual key components - these values are fed straight into opengl to draw meshes by indice
        void delevel();
        void swapVertexIndex(int oldIndex, int newIndex) ; // swap out a vertex with a new one.
    };
} // namespace IndexedTriangleDefinition

#endif // indexed_triangle_h__
