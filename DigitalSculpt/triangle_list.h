// 三角形列表
#ifndef triangle_list_h__
#define triangle_list_h__

#include <vector>
#include "indexed_triangle.h"
#include "indices.h"

namespace DigitalSculpt
{
    class TriangleList
    {
    public:
        TriangleList();
        ~TriangleList();

        void printTriangleListToConsole() ; // accesses each triangle and prints it's indices to console - debugging only.

        const int triangleCountAsindiceCount() const; // number of triangles * 3 == number of indices used to render
        const int triangleMemorySize() const;         // returns the memory size of triangles allocated
        const int totalTriangles() const;             // a wrapper for triangles.size() as triangle.size() is ambiguous to read.
        std::vector<IndexedTriangle> triangles;
    };

}

#endif // triangle_list_h__