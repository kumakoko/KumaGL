/*!
 * \file triangle_octant_key_pair_list.h
 * \date 2024/06/15 17:36
 *
 * \author  kumakoko
 * Contact: user@company.com
 *
 * \brief
 *
 * TODO: 定义了三角形索引值-八叉树元（octant）映射关系的key pair
 *
 * \note
*/
#pragma once

#include <vector>
#include "triangle_octant_key_pair.h"
#include "mesh_stats.h"

namespace DigitalSculpt
{
    class TriangleOctantKeyPairList : public MeshStats
    {
    public:
        TriangleOctantKeyPairList()
        {
        }

        ~TriangleOctantKeyPairList()
        {
        }

        void loadTriangleOctantKeyPairList()
        {
            const int triCount = totalTriangles();

            triangleToOctantList.reserve(triCount);

            for (int i = 0; i < triCount; i++)
            {
                triangleToOctantList.emplace_back(TriangleOctantKeyPair(i, -1));
            }
        }   

    public:
        std::vector<TriangleOctantKeyPair> triangleToOctantList;
    };
}