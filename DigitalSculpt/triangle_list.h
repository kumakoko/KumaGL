// 三角形列表
#pragma once

#include <vector>
#include "indexed_triangle.h"
#include "indices.h"

namespace DigitalSculpt
{
    class TriangleList
    {
    public:
        /******************************************************************************************************************
         * Desc: 
         * Method:    TriangleList
         * Returns:   
         ****************************************************************************************************************/
         TriangleList();

        /******************************************************************************************************************
         * Desc: 
         * Method:    ~TriangleList
         * Returns:   
         ****************************************************************************************************************/
         ~TriangleList();

        /******************************************************************************************************************
         * Desc: 向控制台打印出每一个三角形的信息，打印出组成三角形的每一个顶点索引
         * Method:    printTriangleListToConsole
         * Returns:   void
         ****************************************************************************************************************/
        void printTriangleListToConsole();

        /******************************************************************************************************************
         * Desc: 获取本三角形列表中，有多少个顶点索引组成三角形，三角形数目为N的话，则顶点索引数为Nx3
         * Method:    triangleCountAsindiceCount
         * Returns:   const int
         ****************************************************************************************************************/
        const int triangleCountAsindiceCount() const;

        /******************************************************************************************************************
         * Desc: 返回这些三角形信息占据了多少字节的内存
         * Method:    triangleMemorySize
         * Returns:   const int
         ****************************************************************************************************************/
        const int triangleMemorySize() const;
        
        /******************************************************************************************************************
         * Desc: 返回三角形的个数
         * Method:    totalTriangles
         * Returns:   const int
         ****************************************************************************************************************/
        const int totalTriangles() const;
        
        std::vector<IndexedTriangle> triangles;
    };
}