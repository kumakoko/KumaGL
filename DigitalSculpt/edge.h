﻿// 定义了“边”这样子的一个数据结构
#pragma once
//#ifndef edge_h__
//#define edge_h__

#include <set>
#include <unordered_set>
#include "GL/glew.h"
#include "vertex_look_up.h"

namespace DigitalSculpt
{
    struct Edge
    {
    public:
        /******************************************************************************************************************
         * Desc:
         * Method:    Edge
         * Returns:
         ****************************************************************************************************************/
        Edge();

        /******************************************************************************************************************
         * Desc:
         * Method:    ~Edge
         * Returns:
         ****************************************************************************************************************/
        ~Edge();

        /******************************************************************************************************************
         * Desc: remove all duplicate edges that might occur during loading.
         * Method:    cleanUp
         * Returns:   void
         ****************************************************************************************************************/
        void cleanUp();

        /******************************************************************************************************************
         * Desc: 向控制台打印除本“边”的信息
         * Method:    printEdge
         * Returns:   void
         ****************************************************************************************************************/
        void printEdge(); // print the edge to console

        bool touched = false;
        std::unordered_set<GLuint> vertexEdges;
    };
}

//#endif // edge_h__