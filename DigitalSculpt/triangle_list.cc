// 三角形列表
#include "spdlog/spdlog.h"
#include "triangle_list.h"

namespace DigitalSculpt
{
    TriangleList::TriangleList()
    {
    }

    TriangleList::~TriangleList()
    {
    }

    const int TriangleList::triangleCountAsindiceCount() const
    {
        return triangles.size() * 3;
    }

    const int TriangleList::triangleMemorySize() const
    {
        return triangles.size() * (sizeof(IndexedTriangle));
    }

    const int TriangleList::totalTriangles() const
    {
        return triangles.size();
    }

    void TriangleList::printTriangleListToConsole()
    {
        for (auto& tri : triangles)
        {
            tri.print();
        }

        spdlog::info("\n");
    }
}