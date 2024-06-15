#ifndef octant_h__
#define octant_h__

#include <concurrent_vector.h>
#include <mutex>
#include <unordered_set>

#include "glm/glm.hpp"
#include "mesh_stats.h"
#include "triangle_octant_key_pair_list.h"

namespace DigitalSculpt
{
    //typedef int ChildArray;
    //typedef int OctantParent;

    //struct Octant;

    // ^ This is forward declare to allow for this below typedef which is used in Octant::subdivide

  //  typedef std::vector<Octant> OctantList; // list of octants
    // typedef unordered_set<OctantIndex> OctantIndexList; // list of octant IDs

    extern const int NoOctantParentSet;
    extern const int NoOctantChildSet;
    extern const int NonExistentOctantIndex;

    enum OctantState
    {
        OctantEmptyInternal = 0,    // 本八叉树元是一个空的非叶节点
        OctantNotEmptyInternal = 1, // 本八叉树元是一个非空的非叶节点 
        OctantLeaf = 2              // 本八叉树元是一个叶节点
    };

    enum OctantPosition
    {
        BottomBackLeft = 0,
        BottomFrontLeft = 1,
        TopBackLeft = 2,
        TopFrontLeft = 3,
        BottomBackRight = 4,
        BottomFrontRight = 5,
        TopBackRight = 6,
        TopFrontRight = 7
    };

    extern const std::vector<glm::vec3> octantPositionVectors;

    struct Octant
    {
        Octant();
        
        /// <summary>
        /// 分配給本八叉树元的
        /// </summary>
        int octantIndex; 

        /// <summary>
        /// 指向存储着本八叉树元内的三角形索引的集合的指针
        /// </summary>
        std::shared_ptr<std::unordered_set<int>> triangleIDs;

        /// <summary>
        /// 本八叉树元的父节点的id
        /// </summary>
        int parent = NoOctantParentSet;

        /// <summary>
        /// 本八叉树元的8个子节点的ID列表
        /// </summary>
        std::vector<int> children = 
        {    
            NoOctantChildSet,
            NoOctantChildSet,
            NoOctantChildSet,
            NoOctantChildSet,
            NoOctantChildSet,
            NoOctantChildSet,
            NoOctantChildSet,
            NoOctantChildSet
        };

        /// <summary>
        /// 本八叉树元的半长宽高
        /// </summary>
        float octantHalfSize;

        /// <summary>
        /// 本八叉树元的中心点位置
        /// </summary>
        glm::vec3 octantCenter;                      

        /// <summary>
        /// 本八叉树元的状态（种类）
        /// </summary>
        OctantState octantState = OctantLeaf;

        /// <summary>
        /// debug用的标志变量
        /// </summary>
        bool intersected = false;

        /// <summary>
        /// 
        /// </summary>
        glm::vec4 color = glm::vec4(0.5, 0.5, 0.5, 1);
    };

    typedef Octant& OctantReference;

} // namespace OctantDefinition

#endif // octant_h__