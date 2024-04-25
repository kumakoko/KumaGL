/******************************************************************************
    created:	2024/01/23
    created:	23:1:2024   14:20
    file name: 	Assets\CSCode\DigitalSculpt\OctreeCell.cs
    file path:	Assets\CSCode\DigitalSculpt
    author:		Xiong Xinke

    purpose:	八叉树的节点类
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
            OctreeCell* cell = STACK[--curStack]; // 拿到当前栈顶再往下一个节点，然后赋值给变量cell
            std::uint32_t nbFaces = cell->_iFaces.size(); // 检查cell中有多少个面

            // 如果该cell的面数大于每cell最大面数，且当前cell在书中的深度值还低于最大深度值
            // 可以再继续细分下去
            if (nbFaces > OctreeCell::MAX_FACES && cell->_depth < OctreeCell::MAX_DEPTH)
            {
                cell->constructChildren(mesh);
                //var children = cell->_children;

                // 调用上面的语句构建八个子节点完毕轴，把cell的8个子节点记录入栈
                // 就是直接压栈到
                for (int i = 0; i < 8; ++i)
                    STACK[curStack + i] = cell->_children[i];

                // 记录下游标
                curStack += 8;
            }
            else if (nbFaces > 0) // 如果满足不了上述条件
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
        std::vector<OctreeCell*>& faceLeaf = mesh->getFaceLeaf(); // 获取叶子节点本身

        std::uint32_t faceBoxesCount = faceBoxes.size();
        std::uint32_t facePosInLeafCount = facePosInLeaf.size();
        std::uint32_t faceLeafCount = faceLeaf.size();

        // 遍历该叶子节点的所有面的AABB，然后算出一个最大的AABB包住这个叶子节点
        for (int i = 0; i < nbFaces; ++i)
        {
            // 从face id数组中取出
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
                facePosInLeaf[id] = i; // 记录下本叶子节点所管理的face的索引值

            }
            else
            {
                //ms_Logger.Warn($"OctreeCell.constructLeaf  id = {id},facePosInLeafCount = {facePosInLeafCount}");
                continue;
            }

            id *= 6; // 每一个face对应一个faceBox，而每个faceBox是六个元素构成，所以访问时是6个6个地跳跃式访问

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
        // 拿到用来进行分割用的AABB
        //float[] split = this._aabbSplit;
        float xmin = _aabbSplit[0];
        float ymin = _aabbSplit[1];
        float zmin = _aabbSplit[2];
        float xmax = _aabbSplit[3];
        float ymax = _aabbSplit[4];
        float zmax = _aabbSplit[5];

        // AABB的三个轴的半长
        float dX = (xmax - xmin) * 0.5f;
        float dY = (ymax - ymin) * 0.5f;
        float dZ = (zmax - zmin) * 0.5f;

        // AABB中心点
        float xcen = (xmax + xmin) * 0.5f;
        float ycen = (ymax + ymin) * 0.5f;
        float zcen = (zmax + zmin) * 0.5f;

        /* 八叉树子节点的排列顺序：


                    7           6

                4           5

                    3           2

                0           1
        y
        |  z
        |/
        ----x
        */

        OctreeCell* child0 = new OctreeCell(this); // 左下后
        OctreeCell* child1 = new OctreeCell(this); // 右下后
        OctreeCell* child2 = new OctreeCell(this); // 右下前
        OctreeCell* child3 = new OctreeCell(this); // 左下前
        OctreeCell* child4 = new OctreeCell(this); // 左上后
        OctreeCell* child5 = new OctreeCell(this); // 右上后
        OctreeCell* child6 = new OctreeCell(this); // 右上前
        OctreeCell* child7 = new OctreeCell(this); // 左上前

        // 拿到模型中每个面的中心点
        const Float32Array& faceCenters = mesh->getFaceCenters();
        std::uint32_t nbFaces = _iFaces.size();

        // 遍历每一个面
        for (int i = 0; i < nbFaces; ++i)
        {
            // _iFaces是存储着face id的数组
            // 而每一个face id，分别加上0，1，2之后，可以从mesh的
            // 面中点数组中取出中点坐标的三个分量
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

        // 把刚才划为好的八个子AABB格，当作这八个子节点的总AABB格（即split AABB）设置好来
        child0->setAabbSplit(xmin, ymin, zmin, xcen, ycen, zcen);
        child1->setAabbSplit(xmin + dX, ymin, zmin, xcen + dX, ycen, zcen);
        child2->setAabbSplit(xcen, ycen - dY, zcen, xmax, ymax - dY, zmax);
        child3->setAabbSplit(xmin, ymin, zmin + dZ, xcen, ycen, zcen + dZ);
        child4->setAabbSplit(xmin, ymin + dY, zmin, xcen, ycen + dY, zcen);
        child5->setAabbSplit(xcen, ycen, zcen - dZ, xmax, ymax, zmax - dZ);
        child6->setAabbSplit(xcen, ycen, zcen, xmax, ymax, zmax);
        child7->setAabbSplit(xcen - dX, ycen, zcen, xmax - dX, ymax, zmax);

        // 清空一下子列表，然后把这些新的子节点依次存入
        _children.clear();
        _children.push_back(child0);
        _children.push_back(child1);
        _children.push_back(child2);
        _children.push_back(child3);
        _children.push_back(child4);
        _children.push_back(child5);
        _children.push_back(child6);
        _children.push_back(child7);

        _iFaces.clear();// 清理掉本节点的面数组的内容，因为已经细分到下一个节点了，本节点的面数组用不上了
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

        // 逐次从STACK中遍历，当遍历到叶子节点后，
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
            // 如果本节点有父节点，即它不是树的根节点，但它的children
            // 都没有了，即它下一层的节点，全部都是leave叶子节点，没有
            // cell节点了，已经不可以再拆分了
            if (cell->_parent->_children.size() == 0)
                return;

            // 本
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