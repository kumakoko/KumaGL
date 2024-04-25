/******************************************************************************
    created:	2024/01/23
    created:	23:1:2024   14:20
    file name: 	Assets\CSCode\DigitalSculpt\OctreeCell.cs
    file path:	Assets\CSCode\DigitalSculpt
    author:		Xiong Xinke

    purpose:	�˲����Ľڵ���
******************************************************************************/
#include <limits>
#include <cstdint>
#include "math3d/octree_cell.h"
#include "misc/utils.h"
#include "mesh/mesh.h"

namespace DigitalSculpt
{
    std::uint32_t OctreeCell::FLAG = 0;
    std::uint32_t  OctreeCell::MAX_DEPTH = 8;
    std::uint32_t  OctreeCell::MAX_FACES = 100;
    bool OctreeCell::_IsSTACKinit = false;

    void OctreeCell::InitSTACK()
    {
        if (!_IsSTACKinit)
        {
            STACK.clear();
            STACK.resize(1 + 7 * MAX_DEPTH, nullptr);
            _IsSTACKinit = true;
        }
    }

    OctreeCell::OctreeCell(OctreeCell* parent)
    {
        _parent = parent != nullptr ? parent : nullptr; // parent
        _depth = parent != nullptr ? parent->_depth + 1 : 0; // depth of current node
        _children.clear();// children
        _iFaces.clear();// = new List<int>(); // faces (if cell is a leaf)
        _flag = 0; // to track deleted cell

        _aabbLoose[0] = std::numeric_limits<float>::infinity();
        _aabbLoose[1] = std::numeric_limits<float>::infinity();
        _aabbLoose[2] = std::numeric_limits<float>::infinity();
        _aabbLoose[3] = -std::numeric_limits<float>::infinity();
        _aabbLoose[4] = -std::numeric_limits<float>::infinity();
        _aabbLoose[5] = -std::numeric_limits<float>::infinity();

        _aabbSplit[0] = std::numeric_limits<float>::infinity();
        _aabbSplit[1] = std::numeric_limits<float>::infinity();
        _aabbSplit[2] = std::numeric_limits<float>::infinity();
        _aabbSplit[3] = -std::numeric_limits<float>::infinity();
        _aabbSplit[4] = -std::numeric_limits<float>::infinity();
        _aabbSplit[5] = -std::numeric_limits<float>::infinity();
    }

    void OctreeCell::resetNbFaces(int nbFaces)
    {
        _iFaces.clear();

        for (int i = 0; i < nbFaces; ++i)
            _iFaces.push_back(i);
    }

    void OctreeCell::build(Mesh* mesh)
    {
        int curStack = 1;
        //var stack = OctreeCell.STACK;
        STACK[0] = this;

        std::vector<OctreeCell*> leaves;// = new List<OctreeCell>();

        while (curStack > 0)
        {
            OctreeCell* cell = STACK[--curStack]; // �õ���ǰջ��������һ���ڵ㣬Ȼ��ֵ������cell
            std::uint32_t nbFaces = cell->_iFaces.size(); // ���cell���ж��ٸ���

            // �����cell����������ÿcell����������ҵ�ǰcell�����е����ֵ������������ֵ
            // �����ټ���ϸ����ȥ
            if (nbFaces > OctreeCell::MAX_FACES && cell->_depth < OctreeCell::MAX_DEPTH)
            {
                cell->constructChildren(mesh);
                //var children = cell->_children;

                // �����������乹���˸��ӽڵ�����ᣬ��cell��8���ӽڵ��¼��ջ
                // ����ֱ��ѹջ��
                for (int i = 0; i < 8; ++i)
                    STACK[curStack + i] = cell->_children[i];

                // ��¼���α�
                curStack += 8;
            }
            else if (nbFaces > 0) // ������㲻����������
            {
                leaves.push_back(cell);//leaves.push(cell);
            }
        }

        std::uint32_t nbLeaves = leaves.size();// .Count;
        for (std::uint32_t i = 0; i < nbLeaves; ++i)
            leaves[i]->constructLeaf(mesh);
    }

    void OctreeCell::constructLeaf(Mesh* mesh)
    {
        /* try
         {*/
        std::uint32_t nbFaces = _iFaces.size();

        float bxmin = std::numeric_limits<float>::infinity();//Infinity;
        float bymin = std::numeric_limits<float>::infinity();//Infinity;
        float bzmin = std::numeric_limits<float>::infinity();//Infinity;
        float bxmax = -std::numeric_limits<float>::infinity();//-Infinity;
        float bymax = -std::numeric_limits<float>::infinity();//-Infinity;
        float bzmax = -std::numeric_limits<float>::infinity();//-Infinity;

        const Float32Array& faceBoxes = mesh->getFaceBoxes();
        Uint32Array& facePosInLeaf = mesh->getFacePosInLeaf();
        std::vector<OctreeCell*>& faceLeaf = mesh->getFaceLeaf(); // ��ȡҶ�ӽڵ㱾��

        std::uint32_t faceBoxesCount = faceBoxes.size();
        std::uint32_t facePosInLeafCount = facePosInLeaf.size();
        std::uint32_t faceLeafCount = faceLeaf.size();

        // ������Ҷ�ӽڵ���������AABB��Ȼ�����һ������AABB��ס���Ҷ�ӽڵ�
        for (int i = 0; i < nbFaces; ++i)
        {
            // ��face id������ȡ��
            int id = _iFaces[i];

            if (id >= 0 && id < faceLeafCount)
            {
                faceLeaf[id] = this;
            }
            else
            {
                //ms_Logger.Warn($"OctreeCell.constructLeaf  id = {id},faceLeafCount = {faceLeafCount}");
                continue;
            }

            if (id >= 0 && id < facePosInLeafCount)
            {
                facePosInLeaf[id] = i; // ��¼�±�Ҷ�ӽڵ��������face������ֵ

            }
            else
            {
                //ms_Logger.Warn($"OctreeCell.constructLeaf  id = {id},facePosInLeafCount = {facePosInLeafCount}");
                continue;
            }

            id *= 6; // ÿһ��face��Ӧһ��faceBox����ÿ��faceBox������Ԫ�ع��ɣ����Է���ʱ��6��6������Ծʽ����

            float xmin = faceBoxes[id + 0];
            float ymin = faceBoxes[id + 1];
            float zmin = faceBoxes[id + 2];
            float xmax = faceBoxes[id + 3];
            float ymax = faceBoxes[id + 4];
            float zmax = faceBoxes[id + 5];

            if (xmin < bxmin) bxmin = xmin;
            if (xmax > bxmax) bxmax = xmax;
            if (ymin < bymin) bymin = ymin;
            if (ymax > bymax) bymax = ymax;
            if (zmin < bzmin) bzmin = zmin;
            if (zmax > bzmax) bzmax = zmax;
        }

        this->expandsAabbLoose(bxmin, bymin, bzmin, bxmax, bymax, bzmax);
        /*}*/
        /*catch (Exception e)
        {
            ms_Logger.Warn(ExceptionFormatter.Format(e));
        }*/
    }

    void OctreeCell::constructChildren(Mesh* mesh)
    {
        // �õ��������зָ��õ�AABB
        //float[] split = this._aabbSplit;
        float xmin = _aabbSplit[0];
        float ymin = _aabbSplit[1];
        float zmin = _aabbSplit[2];
        float xmax = _aabbSplit[3];
        float ymax = _aabbSplit[4];
        float zmax = _aabbSplit[5];

        // AABB��������İ볤
        float dX = (xmax - xmin) * 0.5f;
        float dY = (ymax - ymin) * 0.5f;
        float dZ = (zmax - zmin) * 0.5f;

        // AABB���ĵ�
        float xcen = (xmax + xmin) * 0.5f;
        float ycen = (ymax + ymin) * 0.5f;
        float zcen = (zmax + zmin) * 0.5f;

        /* �˲����ӽڵ������˳��


                    7           6

                4           5

                    3           2

                0           1
        y
        |  z
        |/
        ----x
        */

        OctreeCell* child0 = new OctreeCell(this); // ���º�
        OctreeCell* child1 = new OctreeCell(this); // ���º�
        OctreeCell* child2 = new OctreeCell(this); // ����ǰ
        OctreeCell* child3 = new OctreeCell(this); // ����ǰ
        OctreeCell* child4 = new OctreeCell(this); // ���Ϻ�
        OctreeCell* child5 = new OctreeCell(this); // ���Ϻ�
        OctreeCell* child6 = new OctreeCell(this); // ����ǰ
        OctreeCell* child7 = new OctreeCell(this); // ����ǰ

        // �õ�ģ����ÿ��������ĵ�
        const Float32Array& faceCenters = mesh->getFaceCenters();
        std::uint32_t nbFaces = _iFaces.size();

        // ����ÿһ����
        for (int i = 0; i < nbFaces; ++i)
        {
            // _iFaces�Ǵ洢��face id������
            // ��ÿһ��face id���ֱ����0��1��2֮�󣬿��Դ�mesh��
            // ���е�������ȡ���е��������������
            std::int32_t iFace = _iFaces[i];
            std::int32_t id = iFace * 3;
            float cx = faceCenters[id + 0];
            float cy = faceCenters[id + 1];
            float cz = faceCenters[id + 2];

            if (cx > xcen)
            {
                if (cy > ycen)
                {
                    if (cz > zcen)
                        child6->_iFaces.push_back(iFace);
                    else
                        child5->_iFaces.push_back(iFace);
                }
                else
                {
                    if (cz > zcen)
                        child2->_iFaces.push_back(iFace);
                    else
                        child1->_iFaces.push_back(iFace);
                }
            }
            else
            {
                if (cy > ycen)
                {
                    if (cz > zcen)
                        child7->_iFaces.push_back(iFace);
                    else
                        child4->_iFaces.push_back(iFace);
                }
                else
                {
                    if (cz > zcen)
                        child3->_iFaces.push_back(iFace);
                    else
                        child0->_iFaces.push_back(iFace);
                }
            }
        }

        // �ѸղŻ�Ϊ�õİ˸���AABB�񣬵�����˸��ӽڵ����AABB�񣨼�split AABB�����ú���
        child0->setAabbSplit(xmin, ymin, zmin, xcen, ycen, zcen);
        child1->setAabbSplit(xmin + dX, ymin, zmin, xcen + dX, ycen, zcen);
        child2->setAabbSplit(xcen, ycen - dY, zcen, xmax, ymax - dY, zmax);
        child3->setAabbSplit(xmin, ymin, zmin + dZ, xcen, ycen, zcen + dZ);
        child4->setAabbSplit(xmin, ymin + dY, zmin, xcen, ycen + dY, zcen);
        child5->setAabbSplit(xcen, ycen, zcen - dZ, xmax, ymax, zmax - dZ);
        child6->setAabbSplit(xcen, ycen, zcen, xmax, ymax, zmax);
        child7->setAabbSplit(xcen - dX, ycen, zcen, xmax - dX, ymax, zmax);

        // ���һ�����б�Ȼ�����Щ�µ��ӽڵ����δ���
        _children.clear();
        _children.push_back(child0);
        _children.push_back(child1);
        _children.push_back(child2);
        _children.push_back(child3);
        _children.push_back(child4);
        _children.push_back(child5);
        _children.push_back(child6);
        _children.push_back(child7);

        _iFaces.clear();// ��������ڵ������������ݣ���Ϊ�Ѿ�ϸ�ֵ���һ���ڵ��ˣ����ڵ���������ò�����
    }

    void OctreeCell::setAabbSplit(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax)
    {
        _aabbSplit[0] = xmin;
        _aabbSplit[1] = ymin;
        _aabbSplit[2] = zmin;
        _aabbSplit[3] = xmax;
        _aabbSplit[4] = ymax;
        _aabbSplit[5] = zmax;
    }

    void OctreeCell::setAabbLoose(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax)
    {
        _aabbLoose[0] = xmin;
        _aabbLoose[1] = ymin;
        _aabbLoose[2] = zmin;
        _aabbLoose[3] = xmax;
        _aabbLoose[4] = ymax;
        _aabbLoose[5] = zmax;
    }

    Uint32Array OctreeCell::collectIntersectRay(const glm::vec3& vNear, const glm::vec3& eyeDir, Uint32Array& collectFaces, std::vector<OctreeCell*>* leavesHit)
    {
        int acc = 0;
        STACK[0] = this;
        int curStack = 1;

        while (curStack > 0)
        {
            OctreeCell* cell = STACK[--curStack];
            //float[] loose = cell->_aabbLoose;
            float t1 = (cell->_aabbLoose[0] - vNear.x) / eyeDir.x;
            float t2 = (cell->_aabbLoose[3] - vNear.x) / eyeDir.x;
            float t3 = (cell->_aabbLoose[1] - vNear.y) / eyeDir.y;
            float t4 = (cell->_aabbLoose[4] - vNear.y) / eyeDir.y;
            float t5 = (cell->_aabbLoose[2] - vNear.z) / eyeDir.z;
            float t6 = (cell->_aabbLoose[5] - vNear.z) / eyeDir.z;
            float tmin = std::max(std::min(t1, t2), std::min(t3, t4), std::min(t5, t6));
            float tmax = std::min(std::max(t1, t2), std::max(t3, t4), std::max(t5, t6));

            if (tmax < 0 || tmin > tmax) // no intersection
                continue;

            //var children = cell->_children;

            if (cell->_children.size() == 8)
            {
                for (std::uint32_t i = 0; i < 8; ++i)
                    STACK[curStack + i] = cell->_children[i];

                curStack += 8;
            }
            else
            {
                if (leavesHit != nullptr)
                    leavesHit->push_back(cell);

                Utils::set(collectFaces, cell->_iFaces);//collectFaces.set(cell->_iFaces, acc);
                acc += cell->_iFaces.size();
            }
        }

        return Utils::subarray(collectFaces, 0, acc);//return collectFaces.subarray(0, acc);//return new Uint32Array(collectFaces.subarray(0, acc));
    }

    Uint32Array OctreeCell::collectIntersectSphere(const glm::vec3& vert, float radiusSquared, Uint32Array& collectFaces, std::vector<OctreeCell*>* leavesHit)
    {
        /* float vx = vert[0];
         float vy = vert[1];
         float vz = vert[2];*/
        int acc = 0;
        STACK[0] = this;
        int curStack = 1;

        // ��δ�STACK�б�������������Ҷ�ӽڵ��
        while (curStack > 0)
        {
            OctreeCell* cell = STACK[--curStack];

            float dx, dy, dz;

            if (cell->_aabbLoose[0] > vert.x)
                dx = cell->_aabbLoose[0] - vert.x;
            else if (cell->_aabbLoose[3] < vert.x)
                dx = cell->_aabbLoose[3] - vert.x;
            else
                dx = 0.0f;

            if (cell->_aabbLoose[1] > vert.y)
                dy = cell->_aabbLoose[1] - vert.y;
            else if (cell->_aabbLoose[4] < vert.y)
                dy = cell->_aabbLoose[4] - vert.y;
            else
                dy = 0.0f;

            if (cell->_aabbLoose[2] > vert.z)
                dz = cell->_aabbLoose[2] - vert.z;
            else if (cell->_aabbLoose[5] < vert.z)
                dz = cell->_aabbLoose[5] - vert.z;
            else
                dz = 0.0f;

            if ((dx * dx + dy * dy + dz * dz) > radiusSquared) // no intersection
                continue;

            if (cell->_children.size() == 8)
            {
                for (std::uint32_t i = 0; i < 8; ++i)
                    STACK[curStack + i] = cell->_children[i];

                curStack += 8;
            }
            else
            {
                if (leavesHit != nullptr)
                    leavesHit->push_back(cell);


                //var iFaces = cell->_iFaces;
                //collectFaces.set(iFaces, acc);
                //acc += iFaces.Count;
                Utils::set(collectFaces, cell->_iFaces, acc);
                acc += cell->_iFaces.size();
            }
        }

        return Utils::subarray(collectFaces, 0, acc);//collectFaces.subarray(0, acc);//return new Uint32Array(collectFaces.subarray(0, acc));
    }

    OctreeCell* OctreeCell::addFace(int faceId, float bxmin, float bymin, float bzmin, float bxmax, float bymax, float bzmax, float cx, float cy, float cz)
    {
        STACK[0] = this;
        int curStack = 1;

        while (curStack > 0)
        {
            OctreeCell* cell = STACK[--curStack];

            if (cx <= cell->_aabbSplit[0])
                continue;

            if (cy <= cell->_aabbSplit[1])
                continue;

            if (cz <= cell->_aabbSplit[2])
                continue;

            if (cx > cell->_aabbSplit[3])
                continue;

            if (cy > cell->_aabbSplit[4])
                continue;

            if (cz > cell->_aabbSplit[5])
                continue;

            // expands cell aabb loose with aabb face
            if (bxmin < cell->_aabbLoose[0])
                cell->_aabbLoose[0] = bxmin;

            if (bymin < cell->_aabbLoose[1])
                cell->_aabbLoose[1] = bymin;

            if (bzmin < cell->_aabbLoose[2])
                cell->_aabbLoose[2] = bzmin;

            if (bxmax > cell->_aabbLoose[3])
                cell->_aabbLoose[3] = bxmax;

            if (bymax > cell->_aabbLoose[4])
                cell->_aabbLoose[4] = bymax;

            if (bzmax > cell->_aabbLoose[5])
                cell->_aabbLoose[5] = bzmax;

            if (cell->_children.size() == 8)
            {
                for (std::uint32_t i = 0; i < 8; ++i)
                    STACK[curStack + i] = cell->_children[i];

                curStack += 8;
                return nullptr;
            }
            else
            {
                cell->_iFaces.push_back(faceId);//cell->_iFaces.push(faceId);
                return cell;
            }
        }

        return nullptr;
    }

    void OctreeCell::pruneIfPossible()
    {
        OctreeCell* cell = this;

        while (cell->_parent != nullptr)
        {
            //var parent = cell->_parent;
            //var children = parent._children;
            // means that the current cell has already pruned
            // ������ڵ��и��ڵ㣬�����������ĸ��ڵ㣬������children
            // ��û���ˣ�������һ��Ľڵ㣬ȫ������leaveҶ�ӽڵ㣬û��
            // cell�ڵ��ˣ��Ѿ��������ٲ����
            if (cell->_parent->_children.size() == 0)
                return;

            // ��
            for (int i = 0; i < 8; ++i)
            {
                OctreeCell* child = cell->_parent->_children[i];
                if (child->_iFaces.size() > 0 || child->_children.size() == 8)
                {
                    return;
                }
            }

            cell->_parent->_children.clear();//children.length = 0;
            cell = cell->_parent;
        }

    }

    /** Expand aabb loose */
    void OctreeCell::expandsAabbLoose(float bxmin, float bymin, float bzmin, float bxmax, float bymax, float bzmax)
    {
        OctreeCell* parent = this;

        while (parent != nullptr)
        {
            bool proceed = false;

            if (bxmin < parent->_aabbLoose[0])
            {
                parent->_aabbLoose[0] = bxmin;
                proceed = true;
            }

            if (bymin < parent->_aabbLoose[1])
            {
                parent->_aabbLoose[1] = bymin;
                proceed = true;
            }

            if (bzmin < parent->_aabbLoose[2])
            {
                parent->_aabbLoose[2] = bzmin;
                proceed = true;
            }

            if (bxmax > parent->_aabbLoose[3])
            {
                parent->_aabbLoose[3] = bxmax;
                proceed = true;
            }

            if (bymax > parent->_aabbLoose[4])
            {
                parent->_aabbLoose[4] = bymax;
                proceed = true;
            }

            if (bzmax > parent->_aabbLoose[5])
            {
                parent->_aabbLoose[5] = bzmax;
                proceed = true;
            }

            parent = proceed ? parent->_parent : nullptr;
        }
    }


} //namespace