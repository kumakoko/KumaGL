#ifndef octree_cell_h__
#define octree_cell_h__

#include "../klib/kgl_lib_pch.h"

/******************************************************************************
    created:	2024/01/23
    created:	23:1:2024   14:20
    file name: 	Assets\CSCode\DigitalSculpt\OctreeCell.cs
    file path:	Assets\CSCode\DigitalSculpt
    author:		Xiong Xinke

    purpose:	八叉树的节点类
******************************************************************************/
#ifndef digital_sculpt_octree_cell_h__
#define digital_sculpt_octree_cell_h__

#include "../klib/kgl_lib_pch.h"
#include "utils.h"

namespace DigitalSculpt
{
    class OctreeCell;
    class Mesh;

    typedef boost::intrusive_ptr<OctreeCell> OctreeCellPtr;
    typedef boost::intrusive_ptr<Mesh> MeshPtr;
    
    class OctreeCell
    {
    private:
        OctreeCellPtr _parent;  // 本八叉树节点的父节点
        std::vector<OctreeCellPtr> _children;
        static void InitSTACK(); // 本八叉树节点的子节点列表
        static bool _IsSTACKinit;
    public:
        static std::uint32_t FLAG;
        static std::uint32_t MAX_DEPTH;
        static std::uint32_t MAX_FACES;
        static std::vector<OctreeCellPtr> STACK;
    public:
        int _depth = 0; // 本八叉树节点在树的第几层，如果是根节点，则在第0层，其余类推
        SixElemArray _aabbLoose;
        SixElemArray _aabbSplit;
        // 如果本节点是一个叶子节点的话，那么就会存储着归属于本节点的面的id信息
        std::vector<std::uint32_t> _iFaces;// = nullptr;//[]; // faces (if cell is a leaf)
        std::int32_t _flag = 0; // to track deleted cell


        /******************************************************************************************************************
         * Desc:
         * Method:    OctreeCell
         * Returns:
         * Parameter: OctreeCell * parent
         ****************************************************************************************************************/
        OctreeCell(OctreeCellPtr parent);

        /******************************************************************************************************************
         * Desc: 重置本节点的面数组，将旧数据清空，再根据指定的面数值，重新添加到_iFaces列表中
         * Method:    resetNbFaces
         * Returns:   void
         * Parameter: int nbFaces
         ****************************************************************************************************************/
        void resetNbFaces(int nbFaces);

        /******************************************************************************************************************
         * Desc: Subdivide octree, aabbSplit must be already set, and aabbLoose will be expanded if it's a leaf
         * Method:    build
         * Returns:   void
         * Parameter: Mesh * mesh
         ****************************************************************************************************************/
        void build(MeshPtr mesh);

    private:
        /******************************************************************************************************************
         * Desc: Construct the leaf
         * Method:    constructLeaf
         * Returns:   void
         * Parameter: Mesh * mesh
         ****************************************************************************************************************/
        void constructLeaf(MeshPtr mesh);

        /******************************************************************************************************************
         * Desc: 构建八叉树的cell节点（注意不是叶子节点，cell节点本身不存储面信息
         * Method:    constructChildren
         * Returns:   void
         * Parameter: Mesh * mesh
         ****************************************************************************************************************/
        void constructChildren(MeshPtr mesh);

    public:
        /***************************************************

        @name: DigitalSculpt::OctreeCell::setAabbSplit
        @return: void
        @param: float xmin
        @param: float ymin
        @param: float zmin
        @param: float xmax
        @param: float ymax
        @param: float zmax
        ***************************************************/
        void setAabbSplit(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);

        /***************************************************

        @name: DigitalSculpt::OctreeCell::setAabbLoose
        @return: void
        @param: float xmin
        @param: float ymin
        @param: float zmin
        @param: float xmax
        @param: float ymax
        @param: float zmax
        ***************************************************/
        void setAabbLoose(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);

        /******************************************************************************************************************
         * Desc: 投射一根光线，击中八叉树中的某个cell，获取到cell里面的所有face
         * Method:    collectIntersectRay
         * Returns:   DigitalSculpt::Uint32Array
         * Parameter: const glm::vec3 & vNear
         * Parameter: const glm::vec3 & eyeDir
         * Parameter: Uint32Array & collectFaces
         * Parameter: std::vector<OctreeCell * > * leavesHit
         ****************************************************************************************************************/
        Uint32Array collectIntersectRay(const glm::vec3& vNear, const glm::vec3& eyeDir, Uint32Array& collectFaces, std::vector<OctreeCellPtr>* leavesHit);

        /******************************************************************************************************************
         * Desc: 给定一个球的半径和求球心坐标，获取到在此球内的面
         * Method:    collectIntersectSphere
         * Returns:   DigitalSculpt::Uint32Array
         * Parameter: const glm::vec3 & vert
         * Parameter: float radiusSquared
         * Parameter: Uint32Array & collectFaces
         * Parameter: std::vector<OctreeCell * > * leavesHit
         ****************************************************************************************************************/
        Uint32Array collectIntersectSphere(const glm::vec3& vert, float radiusSquared, Uint32Array& collectFaces, std::vector<OctreeCellPtr>* leavesHit);

        /******************************************************************************************************************
         * Desc: 向八叉树内添加一个face，如果有必要的话，可以对当下的cell进行进一步细分
         * Method:    addFace
         * Returns:   DigitalSculpt::OctreeCellPtr
         * Parameter: int faceId
         * Parameter: float bxmin
         * Parameter: float bymin
         * Parameter: float bzmin
         * Parameter: float bxmax
         * Parameter: float bymax
         * Parameter: float bzmax
         * Parameter: float cx
         * Parameter: float cy
         * Parameter: float cz
         ****************************************************************************************************************/
        OctreeCellPtr addFace(int faceId, float bxmin, float bymin, float bzmin, float bxmax, float bymax, float bzmax, float cx, float cy, float cz);

        /******************************************************************************************************************
         * Desc: 如果需要的话，对叶子节点也要进一步修建
         * Method:    pruneIfPossible
         * Returns:   void
         ****************************************************************************************************************/
        void pruneIfPossible();

        /******************************************************************************************************************
         * Desc:
         * Method:    expandsAabbLoose
         * Returns:   void
         * Parameter: float bxmin
         * Parameter: float bymin
         * Parameter: float bzmin
         * Parameter: float bxmax
         * Parameter: float bymax
         * Parameter: float bzmax
         ****************************************************************************************************************/
        void expandsAabbLoose(float bxmin, float bymin, float bzmin, float bxmax, float bymax, float bzmax);
    }; //class



} //namespace

#endif // digital_sculpt_octree_cell_h__

#endif // octree_cell_h__
