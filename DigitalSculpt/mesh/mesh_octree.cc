#include <utility>
#include <iterator>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "mesh.h"
#include "misc/array_1d.h"
#include "misc/array_2d.h"
#include "math3d/octree_cell.h"


namespace DigitalSculpt
{
    Uint32Array Mesh::intersectRay(const glm::vec3& vNear, const glm::vec3& eyeDir, bool collectLeaves)
    {
        std::uint32_t nbFaces = getNbFaces();
        Uint32Array collectFaces(nbFaces * 4, 0);// = new Uint32Array(Utils.getMemory(nbFaces * 4), 0, nbFaces);
        return _meshData->_octree->collectIntersectRay(vNear, eyeDir, collectFaces, collectLeaves ? &(_meshData->_leavesToUpdate) : nullptr);
    }

    /** Return faces inside a sphere */
    Uint32Array Mesh::intersectSphere(const glm::vec3& vert, float radiusSquared, bool collectLeaves)
    {
        std::uint32_t nbFaces = getNbFaces();
        Uint32Array collectFaces(nbFaces * 4, 0);//var collectFaces = new Uint32Array(Utils.getMemory(nbFaces * 4), 0, nbFaces);
        return _meshData->_octree->collectIntersectSphere(vert, radiusSquared, collectFaces, collectLeaves ? &(_meshData->_leavesToUpdate) : nullptr);
    }


    /**
     * Update Octree
     * For each faces we check if its position inside the octree has changed
     * if so... we mark this face and we remove it from its former cells
     * We push back the marked faces into the octree
     */
    void Mesh::updateOctree(const Uint32Array& iFaces)
    {
        if (iFaces.size() > 0)
            updateOctreeAdd(updateOctreeRemove(iFaces));
        else
            computeOctree();
    }

    SixElemArray Mesh::computeAabb()
    {
        std::uint32_t  nbVertices = getNbVertices();
        const Float32Array& vAr = getVertices();
        float xmin = std::numeric_limits<float>::infinity();//Infinity;
        float ymin = std::numeric_limits<float>::infinity();//Infinity;
        float zmin = std::numeric_limits<float>::infinity();//Infinity;
        float xmax = -std::numeric_limits<float>::infinity();//-Infinity;
        float ymax = -std::numeric_limits<float>::infinity();//-Infinity;
        float zmax = -std::numeric_limits<float>::infinity();//-Infinity;

        for (std::uint32_t i = 0; i < nbVertices; ++i)
        {
            std::uint32_t j = i * 3;
            float vx = vAr[j + 0];
            float vy = vAr[j + 1];
            float vz = vAr[j + 2];
            if (vx < xmin) xmin = vx;
            if (vx > xmax) xmax = vx;
            if (vy < ymin) ymin = vy;
            if (vy > ymax) ymax = vy;
            if (vz < zmin) zmin = vz;
            if (vz > zmax) zmax = vz;
        }

        SixElemArray sea;
        sea[0] = xmin;
        sea[1] = ymin;
        sea[2] = zmin;
        sea[3] = xmax;
        sea[4] = ymax;
        sea[5] = zmax;
        return sea;
    }

    /** Compute the octree */
    void Mesh::computeOctree()
    {
        SixElemArray abRoot = computeAabb();
        float xmin = abRoot[0];
        float ymin = abRoot[1];
        float zmin = abRoot[2];
        float xmax = abRoot[3];
        float ymax = abRoot[4];
        float zmax = abRoot[5];
        float dx = xmax - xmin;
        float dy = ymax - ymin;
        float dz = zmax - zmin;

        // add minimal thickness
        float offset = std::sqrt(dx * dx + dy * dy + dz * dz) * 0.2f;
        float eps = 1e-16;

        if (xmax - xmin < eps)
        {
            xmin -= offset;
            xmax += offset;
        }

        if (ymax - ymin < eps)
        {
            ymin -= offset;
            ymax += offset;
        }

        if (zmax - zmin < eps)
        {
            zmin -= offset;
            zmax += offset;
        }

        // root octree bigger than minimum aabb... (to avoid too many octree rebuild)
        float dfx = dx * 0.3f;
        float dfy = dy * 0.3f;
        float dfz = dz * 0.3f;
        float xmin2 = xmin - dfx;
        float xmax2 = xmax + dfx;
        float ymin2 = ymin - dfy;
        float ymax2 = ymax + dfy;
        float zmin2 = zmin - dfz;
        float zmax2 = zmax + dfz;

        // octree construction
        _meshData->_octree = new OctreeCell(nullptr);
        _meshData->_octree->resetNbFaces(getNbFaces());
        _meshData->_octree->setAabbLoose(xmin, ymin, zmin, xmax, ymax, zmax);
        _meshData->_octree->setAabbSplit(xmin2, ymin2, zmin2, xmax2, ymax2, zmax2);
        _meshData->_octree->build(this);
    }

    Uint32Array Mesh::updateOctreeRemove(const Uint32Array& iFaces)
    {
        Float32Array& faceCenters = getFaceCenters();
        Float32Array& fboxes = getFaceBoxes();
        Uint32Array& facePosInLeaf = _meshData->_facePosInLeaf;
        std::vector<OctreeCell*>& faceLeaf = _meshData->_faceLeaf;
        std::uint32_t nbFaces = iFaces.size();
        std::uint32_t  acc = 0;
        Uint32Array facesToMove(nbFaces, 0);// = new int[nbFaces];//new Uint32Array(Utils.getMemory(4 * nbFaces), 0, nbFaces);

        // recompute position inside the octree
        for (std::uint32_t i = 0; i < nbFaces; ++i)
        {
            std::uint32_t idFace = iFaces[i];
            std::uint32_t idb = idFace * 6; // 这个face的包围盒，在face包围盒数组中的索引
            std::uint32_t idCen = idFace * 3; // 这个face的中心点，在face中心点数组中的索引
            OctreeCell* leaf = faceLeaf[idFace];
            SixElemArray& ab = leaf->_aabbSplit;

            float vx = faceCenters[idCen + 0];
            float vy = faceCenters[idCen + 1];
            float vz = faceCenters[idCen + 2];

            if (vx <= ab[0] || vy <= ab[1] || vz <= ab[2] || vx > ab[3] || vy > ab[4] || vz > ab[5])
            {
                // a face center has moved from its cell
                // 某面的中心点超出包围盒范围的话，就从本cell中将其移除出去
                facesToMove[acc++] = iFaces[i];
                Uint32Array& facesInLeaf = leaf->_iFaces;
                std::uint32_t facesInLeafCount = facesInLeaf.size();

                if (facesInLeafCount > 0) // remove faces from octree cell
                {
                    std::uint32_t iFaceLast = facesInLeaf[facesInLeafCount - 1]; // 拿到”叶子中face数组“中最后那个face的id
                    std::uint32_t iPos = facePosInLeaf[idFace]; // 拿到给定face的位置索引
                    facesInLeaf[iPos] = iFaceLast; // 把这个“最后的face id”插入到那个要删掉的位置
                    facePosInLeaf[iFaceLast] = iPos;
                    facesInLeaf.pop_back();//把最尾部的那个移除掉 ，js代码是facesInLeaf.pop();
                }
            }
            else
            {
                // expands cell aabb loose if necessary
                leaf->expandsAabbLoose(fboxes[idb], fboxes[idb + 1], fboxes[idb + 2], fboxes[idb + 3], fboxes[idb + 4], fboxes[idb + 5]);
            }
        }

        return Utils::subarray(facesToMove, 0, acc);//facesToMove.subarray(0, acc);//return new Uint32Array(facesToMove.subarray(0, acc));
    }

    void Mesh::updateOctreeAdd(const Uint32Array& facesToMove)
    {
        Float32Array& fc = getFaceCenters();
        Float32Array& fb = getFaceBoxes();
        Uint32Array& facePosInLeaf = _meshData->_facePosInLeaf;
        //std::vector<OctreeCell*>& faceLeaf = _meshData->_faceLeaf;
        std::uint32_t nbFacesToMove = facesToMove.size();

        OctreeCell* root = _meshData->_octree;
        //var rootSplit = root->_aabbSplit;
        float xmin = root->_aabbSplit[0];
        float ymin = root->_aabbSplit[1];
        float zmin = root->_aabbSplit[2];
        float xmax = root->_aabbSplit[3];
        float ymax = root->_aabbSplit[4];
        float zmax = root->_aabbSplit[5];

        for (std::uint32_t i = 0; i < nbFacesToMove; ++i) { // add face to the octree
            std::uint32_t idFace = facesToMove[i];
            std::uint32_t idb = idFace * 6;
            float ibux = fb[idb + 0];
            float ibuy = fb[idb + 1];
            float ibuz = fb[idb + 2];
            float iblx = fb[idb + 3];
            float ibly = fb[idb + 4];
            float iblz = fb[idb + 5];

            if (ibux > xmax || iblx < xmin || ibuy > ymax || ibly < ymin || ibuz > zmax || iblz < zmin) {
                // a face is outside the root node, we reconstruct the whole octree, slow... but rare
                computeOctree();
                _meshData->_leavesToUpdate.clear();// _meshData->_leavesToUpdate.length = 0;
                break;
            }

            std::uint32_t idc = idFace * 3;
            OctreeCell* newleaf = root->addFace(idFace, ibux, ibuy, ibuz, iblx, ibly, iblz, fc[idc], fc[idc + 1], fc[idc + 2]);
            if (newleaf != nullptr)
            {
                facePosInLeaf[idFace] = newleaf->_iFaces.size() - 1;
                _meshData->_faceLeaf[idFace] = newleaf;
            }
            else 
            { // failed to insert face in octree, re-insert it back
                Uint32Array& facesInLeaf = _meshData->_faceLeaf[idFace]->_iFaces;
                facePosInLeaf[idFace] = facesInLeaf.size();
                facesInLeaf.push_back(facesToMove[i]);
            }
        }
    }

    /** balance octree (cut empty leaves or go deeper if needed) */
    void Mesh::balanceOctree()
    {
        ++OctreeCell::FLAG;
        //var leavesToUpdate = _meshData->_leavesToUpdate;
        std::uint32_t nbLeaves = _meshData->_leavesToUpdate.size();// leavesToUpdate.length;

        for (std::uint32_t i = 0; i < nbLeaves; ++i) 
        {
            OctreeCell* leaf = _meshData->_leavesToUpdate[i];
            if (leaf->_flag == OctreeCell::FLAG)
                continue;

            if (leaf->_iFaces.size() == =0)
                leaf->pruneIfPossible();
            else if (leaf->_iFaces.size() > OctreeCell::MAX_FACES && leaf->_depth < OctreeCell::MAX_DEPTH)
                leaf->build(this);

            leaf->_flag = OctreeCell::FLAG;
        }

        _meshData->_leavesToUpdate.clear();// leavesToUpdate.length = 0;
    }
}