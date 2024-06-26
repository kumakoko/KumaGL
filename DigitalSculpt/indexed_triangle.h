// 三角形
#pragma once

#include <vector>
#include "GL/glew.h"
#include "indices.h"

namespace DigitalSculpt
{
    class IndexedTriangle
    {
    public:
        /******************************************************************************************************************
         * Desc:
         * Method:    IndexedTriangle
         * Returns:
         ****************************************************************************************************************/
        IndexedTriangle();

        /******************************************************************************************************************
         * Desc:
         * Method:    IndexedTriangle
         * Returns:
         * Parameter: GLuint v0
         * Parameter: GLuint v1
         * Parameter: GLuint v2
         ****************************************************************************************************************/
        IndexedTriangle(GLuint v0, GLuint v1, GLuint v2);

        /******************************************************************************************************************
         * Desc:
         * Method:    ~IndexedTriangle
         * Returns:
         ****************************************************************************************************************/
        ~IndexedTriangle();

        /******************************************************************************************************************
         * Desc: 获取到组成本三角形的第pos个顶点索引
         * Method:    operator[]
         * Returns:   GLuint&
         * Parameter: const int pos 组成本三角形的第pos个顶点索引，pos的取值范围是0，1，2
         ****************************************************************************************************************/
        GLuint& operator[](const int pos);

        /******************************************************************************************************************
         * Desc:获取到组成本三角形的第pos个顶点索引
         * Method:    operator[]
         * Returns:   const GLuint&
         * Parameter: const int pos 组成本三角形的第pos个顶点索引，pos的取值范围是0，1，2
         ****************************************************************************************************************/
        const GLuint& operator[](const int pos) const;

        /******************************************************************************************************************
         * Desc: 依序在log中打印出组成本三角形的三个顶点索引值
         * Method:    print
         * Returns:   void
         ****************************************************************************************************************/
        void print();

        /******************************************************************************************************************
         * Desc: 把组成本三角形的三个顶点索引值，每一个值都减去1
         * Method:    delevel
         * Returns:   void
         ****************************************************************************************************************/
        void delevel();

        /******************************************************************************************************************
         * Desc: 指定一个oldIndex值，如果组成本三角形的顶点的索引值有等于oldIndex的，则替换成newIndex值
         * Method:    swapVertexIndex
         * Returns:   void
         * Parameter: int oldIndex
         * Parameter: int newIndex
         ****************************************************************************************************************/
        void swapVertexIndex(int oldIndex, int newIndex);

        /// <summary>
        /// 每三个顶点索引构成一个三角形
        /// </summary>
        GLuint indice[3]; // the actual key components - these values are fed straight into opengl to draw meshes by indice
    };
}