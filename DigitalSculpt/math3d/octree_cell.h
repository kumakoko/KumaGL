/******************************************************************************
    created:	2024/01/23
    created:	23:1:2024   14:20
    file name: 	Assets\CSCode\DigitalSculpt\OctreeCell.cs
    file path:	Assets\CSCode\DigitalSculpt
    author:		Xiong Xinke

    purpose:	�˲����Ľڵ���
******************************************************************************/
#ifndef digital_sculpt_octree_cell_h__
#define digital_sculpt_octree_cell_h__

#include <vector>
#include <cstdint>
#include "glm/vec3.hpp"
#include "misc/utils.h"

namespace DigitalSculpt
{
    class Mesh;

    class OctreeCell
    {
    private:
        OctreeCell* _parent;  // ���˲����ڵ�ĸ��ڵ�
        std::vector<OctreeCell*> _children;
        static void InitSTACK(); // ���˲����ڵ���ӽڵ��б�
        static bool _IsSTACKinit;
    public:
        int _depth = 0; // ���˲����ڵ������ĵڼ��㣬����Ǹ��ڵ㣬���ڵ�0�㣬��������
        SixElemArray _aabbLoose;
        SixElemArray _aabbSplit;
        // ������ڵ���һ��Ҷ�ӽڵ�Ļ�����ô�ͻ�洢�Ź����ڱ��ڵ�����id��Ϣ
        std::vector<std::uint32_t> _iFaces;// = nullptr;//[]; // faces (if cell is a leaf)
        std::int32_t _flag = 0; // to track deleted cell
        static std::uint32_t FLAG;
        static std::uint32_t MAX_DEPTH;
        static std::uint32_t MAX_FACES;
        static std::vector<OctreeCell*> STACK;
  
        /******************************************************************************************************************
         * Desc: 
         * Method:    OctreeCell
         * Returns:   
         * Parameter: OctreeCell * parent
         ****************************************************************************************************************/
         OctreeCell(OctreeCell* parent);

        /******************************************************************************************************************
         * Desc: ���ñ��ڵ�������飬����������գ��ٸ���ָ��������ֵ���������ӵ�_iFaces�б���
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
         void build(Mesh* mesh);

    private:
        /******************************************************************************************************************
         * Desc: Construct the leaf 
         * Method:    constructLeaf
         * Returns:   void
         * Parameter: Mesh * mesh
         ****************************************************************************************************************/
         void constructLeaf(Mesh* mesh);

        /******************************************************************************************************************
         * Desc: �����˲�����cell�ڵ㣨ע�ⲻ��Ҷ�ӽڵ㣬cell�ڵ㱾�����洢����Ϣ
         * Method:    constructChildren
         * Returns:   void
         * Parameter: Mesh * mesh
         ****************************************************************************************************************/
         void constructChildren(Mesh* mesh);

    public:
        void setAabbSplit(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);

        void setAabbLoose(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);

        /******************************************************************************************************************
         * Desc: Ͷ��һ�����ߣ����а˲����е�ĳ��cell����ȡ��cell���������face
         * Method:    collectIntersectRay
         * Returns:   DigitalSculpt::Uint32Array
         * Parameter: const glm::vec3 & vNear
         * Parameter: const glm::vec3 & eyeDir
         * Parameter: Uint32Array & collectFaces
         * Parameter: std::vector<OctreeCell * > * leavesHit
         ****************************************************************************************************************/
        Uint32Array collectIntersectRay(const glm::vec3& vNear, const glm::vec3& eyeDir, Uint32Array& collectFaces, std::vector<OctreeCell*>* leavesHit);

        /******************************************************************************************************************
         * Desc: ����һ����İ뾶�����������꣬��ȡ���ڴ����ڵ���
         * Method:    collectIntersectSphere
         * Returns:   DigitalSculpt::Uint32Array
         * Parameter: const glm::vec3 & vert
         * Parameter: float radiusSquared
         * Parameter: Uint32Array & collectFaces
         * Parameter: std::vector<OctreeCell * > * leavesHit
         ****************************************************************************************************************/
        Uint32Array collectIntersectSphere(const glm::vec3& vert, float radiusSquared, Uint32Array& collectFaces, std::vector<OctreeCell*>* leavesHit);

        /******************************************************************************************************************
         * Desc: ��˲���������һ��face������б�Ҫ�Ļ������ԶԵ��µ�cell���н�һ��ϸ��
         * Method:    addFace
         * Returns:   DigitalSculpt::OctreeCell*
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
        OctreeCell* addFace(int faceId, float bxmin, float bymin, float bzmin, float bxmax, float bymax, float bzmax, float cx, float cy, float cz);

        /******************************************************************************************************************
         * Desc: �����Ҫ�Ļ�����Ҷ�ӽڵ�ҲҪ��һ���޽�
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