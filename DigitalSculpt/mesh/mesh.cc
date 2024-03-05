#include <utility>
#include <iterator>
#include "mesh.h"
#include "misc/array_1d.h"
#include "misc/array_2d.h"

namespace DigitalSculpt
{
    float DEF_ROUGHNESS = 0.18; // 0.18;
    float DEF_METALNESS = 0.08; // 0.08;

    void Mesh::setVertices(const Float32Array& vAr)
    {
        _meshData->_verticesXYZ.clear();
        // std::copy函数相关的网页 https://blog.csdn.net/qq_39074243/article/details/120310870
        std::copy(vAr.begin(), vAr.end(), std::back_inserter(_meshData->_verticesXYZ));//this->_meshData->_verticesXYZ = vAr;
        _meshData->_nbVertices = vAr.size() / 3;
    }

    void Mesh::setFaces(const Uint32Array& fAr)
    {
        _meshData->_facesABCD.clear();
        std::copy(fAr.begin(), fAr.end(), std::back_inserter(_meshData->_facesABCD));//this->_meshData->_facesABCD = fAr;
        _meshData->_nbFaces = fAr.size() / 4;
    }

    void Mesh::setTexCoords(const Float32Array& tAr)
    {
        _meshData->_texCoordsST.clear();
        std::copy(tAr.begin(), tAr.end(), std::back_inserter(_meshData->_texCoordsST));//this->_meshData->_texCoordsST = tAr;
        this->_meshData->_nbTexCoords = tAr.size() / 2;
    }

    void Mesh::setColors(const Float32Array& cAr)
    {
        _meshData->_colorsRGB.clear();
        std::copy(cAr.begin(), cAr.end(), std::back_inserter(_meshData->_colorsRGB));//this->_meshData->_texCoordsST = tAr;
    }

    void Mesh::setMaterials(const Float32Array& mAr)
    {
        _meshData->_materialsPBR.clear();
        std::copy(mAr.begin(), mAr.end(), std::back_inserter(_meshData->_materialsPBR));
    }

    void Mesh::setVerticesDuplicateStartCount(const Uint32Array& startCount)
    {
        _meshData->_duplicateStartCount.clear();
        std::copy(startCount.begin(), startCount.end(), std::back_inserter(_meshData->_duplicateStartCount));
    }

    void Mesh::setFacesTexCoord(const Uint32Array& fuAr)
    {
        _meshData->_UVfacesABCD.clear();
        std::copy(fuAr.begin(), fuAr.end(), std::back_inserter(_meshData->_UVfacesABCD));
    }

    bool Mesh::hasOnlyTriangles() const
    {
        return getNbTriangles() == getNbFaces();
    }

    bool Mesh::hasOnlyQuads() const
    {
        return getNbTriangles() == getNbFaces() * 2;
    }

    Float32Array& Mesh::getVerticesDrawArrays()
    {
        if (_meshData->_DAverticesXYZ.size() > 0)
            updateDrawArrays();

        return _meshData->_DAverticesXYZ;
    }

    float Mesh::getScale2() const
    {
        const glm::mat4& m = _transformData->_matrix;
        //return m[0] * m[0] + m[4] * m[4] + m[8] * m[8];
        return m[0][0] * m[0][0] + m[1][0] * m[1][0] + m[2][0];
    }

    float Mesh::getScale() const
    {
        return std::sqrt(getScale2());
    }

    glm::vec3 Mesh::getSymmetryOrigin() const
    {
        /*
        var orig = vec3.create();
        var tdata = _transformData;
        var offset = tdata._symmetryOffset * computeLocalRadius();
        return vec3.scaleAndAdd(orig, tdata._center, tdata._symmetryNormal, offset);
        */
        glm::vec3 orig(0.0f, 0.0f, 0.0f);
        float offset = _transformData->_symmetryOffset * computeLocalRadius();
        return Utils::scaleAndAdd(orig, _transformData->_center, _transformData->_symmetryNormal, offset);
    }

    void Mesh::init()
    {
        initColorsAndMaterials();
        allocateArrays();
        initTopology();
        updateGeometry();
        if (_renderData != nullptr)
            updateDuplicateColorsAndMaterials();
        updateCenter();
    }

    void Mesh::initTopology()
    {
        initFaceRings();
        optimize();
        initEdges();
        initVertexRings();
        initRenderTriangles();
    }

    void Mesh::updateGeometry(iFaces, iVerts)
    {
        updateFacesAabbAndNormal(iFaces);
        updateVerticesNormal(iVerts);
        updateOctree(iFaces);
        if (_renderData) {
            updateDuplicateGeometry(iVerts);
            updateDrawArrays(iFaces);
        }
    }

    void Mesh::allocateArrays()
    {
        std::uint32_t nbVertices = getNbVertices();
        std::uint32_t nbFaces = getNbFaces();

        if (hasUV())
        {
            std::uint32_t nbTexCoords = _meshData->_texCoordsST.size() / 2;

            // https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/TypedArray/set
            // 上述的JS函数set，如果_meshData->_colorsRGB的长度超过了tmp的长度，则会抛出
            // 异常所以只考虑了因此C++代码中只需要考虑tmp比_meshData->_colorsRGB长的情况：
            std::uint32_t existLen = _meshData->_verticesXYZ.size();
            std::uint32_t newLen = nbTexCoords * 3;

            if (existLen > 0)
            {
                if (newLen > existLen)
                {
                    _meshData->_verticesXYZ.insert(_meshData->_verticesXYZ.end(), newLen - existLen, 0.0f);
                }
                else if (newLen == existLen)
                {
                    // do noting
                }
                else
                {
                    // 抛出异常
                }
            }
            else
            {
                _meshData->_verticesXYZ.resize(newLen, 0.0f);
            }

            _meshData->_normalsXYZ.resize(nbTexCoords * 3);


            existLen = _meshData->_colorsRGB.size();

            if (existLen > 0)
            {
                if (newLen > existLen)
                {
                    _meshData->_colorsRGB.insert(_meshData->_colorsRGB.end(), newLen - existLen, 0.0f);
                }
                else if (newLen == existLen)
                {
                    // do noting
                }
                else
                {
                    // 抛出异常
                }
            }
            else
            {
                _meshData->_colorsRGB.resize(newLen, 0.0f);
            }

            existLen = _meshData->_materialsPBR.size();

            if (existLen > 0)
            {
                if (newLen > existLen)
                {
                    _meshData->_materialsPBR.insert(_meshData->_materialsPBR.end(), newLen - existLen, 0.0f);
                }
                else if (newLen == existLen)
                {
                    // do noting
                }
                else
                {
                    // 抛出异常
                }
            }
            else
            {
                _meshData->_materialsPBR.resize(newLen, 0.0f);
            }
        }
        else
        {
            if (_meshData->_normalsXYZ.size() == 0)
                _meshData->_normalsXYZ.resize(nbVertices * 3, 0.0f);

            if (_meshData->_colorsRGB.size() == 0)
                _meshData->_colorsRGB.resize(nbVertices * 3, 0.0f);

            if (_meshData->_materialsPBR.size() == 0)
                _meshData->_materialsPBR.resize(nbVertices * 3, 0.0f);
        }

        _meshData->_vertOnEdge.resize(nbVertices, 0);// = new Uint8Array(nbVertices);
        _meshData->_vrvStartCount.resize(nbVertices * 2, 0);// = new Uint32Array(nbVertices * 2);
        _meshData->_vrfStartCount.resize(nbVertices * 2, 0);// = new Uint32Array(nbVertices * 2);
        _meshData->_vertTagFlags.resize(nbVertices, 0);// = new Int32Array(nbVertices);
        _meshData->_vertSculptFlags.resize(nbVertices, 0);// = new Int32Array(nbVertices);
        _meshData->_vertStateFlags.resize(nbVertices, 0);// = new Int32Array(nbVertices);
        _meshData->_vertProxy.resize(nbVertices * 3, 0.0f);// = new Float32Array(nbVertices * 3);


        _meshData->_faceEdges.resize(nbFaces * 4, 0);// = new Uint32Array(nbFaces * 4);
        _meshData->_facesToTriangles.resize(nbFaces, 0);//  = new Uint32Array(nbFaces);
        _meshData->_faceBoxes.resize(nbFaces * 6, 0.0f);//  = new Float32Array(nbFaces * 6);
        _meshData->_faceNormalsXYZ.resize(nbFaces * 3, 0.0f);//  = new Float32Array(nbFaces * 3);
        _meshData->_faceCentersXYZ.resize(nbFaces * 3, 0.0f);//  = new Float32Array(nbFaces * 3);
        _meshData->_facesTagFlags.resize(nbFaces, 0);//  = new Int32Array(nbFaces);
        _meshData->_facePosInLeaf.resize(nbFaces, 0);//  = new Uint32Array(nbFaces);

        /*
        var faceLeaf = _meshData->_faceLeaf;
        faceLeaf.length = nbFaces;
        for (std::uint32_t i = 0; i < nbFaces; ++i)
            faceLeaf[i] = null;
        */

        _meshData->_faceLeaf.clear();
        _meshData->_faceLeaf.resize(nbFaces, nullptr);
    }


    void Mesh::initColorsAndMaterials()
    {
        std::uint32_t nbVertices = getNbVertices();
        std::uint32_t i = 0;
        std::uint32_t len = nbVertices * 3;

        if (_meshData->_colorsRGB.size() == 0 || _meshData->_colorsRGB.size() != len)
        {
            _meshData->_colorsRGB.resize(len, 1.0f);
        }

        if (_meshData->_materialsPBR.size() == 0 || _meshData->_materialsPBR.size() != len)
        {
            _meshData->_materialsPBR.resize(len, 1.0f);

            for (i = 0; i < nbVertices; ++i)
            {
                std::uint32_t j = i * 3;
                _meshData->_materialsPBR[j + 0] = DEF_ROUGHNESS;
                _meshData->_materialsPBR[j + 1] = DEF_METALNESS;
                _meshData->_materialsPBR[j + 2] = 1.0;
            }
        }
    }


    /** Computes faces ring around vertices */
    void Mesh::initFaceRings()
    {
        const Uint32Array& fAr = getFaces();
        std::uint32_t nbVertices = getNbVertices();
        std::uint32_t nbFaces = getNbFaces();
        std::uint32_t i = 0;
        std::uint32_t id = 0;
        Uint32Array countRing;
        countRing.resize(nbVertices, 0); //var countRing = new Uint32Array(nbVertices);

        for (i = 0; i < nbFaces; ++i)
        {
            id = i * 4;
            countRing[fAr[id + 0]]++;
            countRing[fAr[id + 1]]++;
            countRing[fAr[id + 2]]++;
            std::uint32_t i4 = fAr[id + 3];
            if (i4 != Utils::TRI_INDEX)
                countRing[i4]++;
        }

        Uint32Array& ringFace = getVerticesRingFaceStartCount();
        std::uint32_t acc = 0;
        for (i = 0; i < nbVertices; ++i)
        {
            std::uint32_t count = countRing[i];
            ringFace[i * 2 + 0] = acc;
            ringFace[i * 2 + 1] = count;
            acc += count;
        }

        //var vrf = new Uint32Array(Utils.getMemory(4 * nbFaces * 6), 0, nbFaces * 6);
        Uint32Array vrf;
        vrf.resize(nbFaces * 6, 0);
        acc = 0;
        for (i = 0; i < nbFaces; ++i)
        {
            id = i * 4;
            std::uint32_t iv1 = fAr[id + 0];
            std::uint32_t iv2 = fAr[id + 1];
            std::uint32_t iv3 = fAr[id + 2];
            std::uint32_t iv4 = fAr[id + 3];
            vrf[ringFace[iv1 * 2] + (--countRing[iv1])] = i;
            vrf[ringFace[iv2 * 2] + (--countRing[iv2])] = i;
            vrf[ringFace[iv3 * 2] + (--countRing[iv3])] = i;

            if (iv4 != Utils::TRI_INDEX)
            {
                vrf[ringFace[iv4 * 2] + (--countRing[iv4])] = i;
                ++acc;
            }
        }

        //_meshData->_vertRingFace = new Uint32Array(vrf.subarray(0, nbFaces * 3 + acc));
        std::uint32_t* tmp = Utils::subarray<std::uint32_t>(vrf, 0, nbFaces * 3 + acc);
        Array1D<std::uint32_t>* array1d = new Array1D<std::uint32_t>(tmp);
        _meshData->_vertRingFace = array1d;
        delete[] tmp;
    }

    void Mesh::updateVerticesNormal(const Uint32Array& iVerts)
    {
        const Uint32Array& vrfStartCount = getVerticesRingFaceStartCount();
        IUint32ArrayND* vertRingFace = getVerticesRingFace();
        bool is2DArray = vertRingFace->Dimension() == 2;

        /*
        * 下面的这句表示，在Java中，vertRingFace可能有两种类型，一种是UInt32Array，一种是原生的Array
        * 在Mesh.js中，vertRingFace是Uint32Array类型，是一个一维数组
        * 在MeshDynmic.js中，vertRingFace是原生Array类型，且是这个Array里面的每一个元素，又是一个数组，即是一个二维数组
        * 用C#转写，分别用了Array1D和Array2D这两个类去代替
        */
        //var ringFaces = vertRingFace instanceof Array ? vertRingFace : null;
        Float32Array& nAr = getNormals();
        const Float32Array& faceNormals = getFaceNormals();

        bool full = iVerts.size() == 0; //var full = iVerts == = undefined;
        std::uint32_t nbVerts = full ? getNbVertices() : iVerts.size();

        if (is2DArray)
        {
            Uint32Array2D* array2D = dynamic_cast<Uint32Array2D*>(vertRingFace);// as Array2D<int>;

            for (std::uint32_t i = 0; i < nbVerts; ++i)
            {
                int ind = full ? i : static_cast<int>(iVerts[i]);
                std::uint32_t* ring = (*array2D)[ind];//int[] ring = array2D[ind];
                int end = static_cast<int>(Utils::GetArrayElementsCount(ring));//int end = ring.Length;
                int start = 0;

                float nx = 0.0f, ny = 0.0f, nz = 0.0f;

                for (int j = start; j < end; ++j)
                {
                    // 此时的vertRingFace
                    int id = static_cast<int>(ring[j] * 3);
                    nx += faceNormals[id + 0];
                    ny += faceNormals[id + 1];
                    nz += faceNormals[id + 2];
                }

                float len = static_cast<float>(end - start);
                if (len != 0.0f)
                    len = 1.0f / len;

                ind *= 3;

                nAr[ind + 0] = nx * len;
                nAr[ind + 1] = ny * len;
                nAr[ind + 2] = nz * len;
            }
        }
        else
        {
            Uint32Array1D* array1D = dynamic_cast<Uint32Array1D*>(vertRingFace); //Array1D<int> array1D = vertRingFace as Array1D<int>;

            for (int i = 0; i < nbVerts; ++i)
            {
                int ind = full ? i : static_cast<int>(iVerts[i]);
                int start = vrfStartCount[ind * 2 + 0];
                int end = start + vrfStartCount[ind * 2 + 1];

                float nx = 0.0f, ny = 0.0f, nz = 0.0f;

                for (int j = start; j < end; ++j)
                {
                    int id = static_cast<int>((*array1D)[j]) * 3;
                    nx += faceNormals[id + 0];
                    ny += faceNormals[id + 1];
                    nz += faceNormals[id + 2];
                }

                float len = static_cast<float>(end - start);

                if (len != 0.0f)
                    len = 1.0f / len;

                ind *= 3;

                nAr[ind + 0] = nx * len;
                nAr[ind + 1] = ny * len;
                nAr[ind + 2] = nz * len;
            }
        }










    }

    void Mesh::initVertexRings()
    {
        Uint32Array& vrvStartCount = getVerticesRingVertStartCount();
        //var vertRingVert = _meshData->_vertRingVert = new Uint32Array(getNbEdges() * 2);
        Uint32Array1D* tmp = new Uint32Array1D(getNbEdges() * 2);
        _meshData->_vertRingVert = tmp;
        Uint32Array1D& vertRingVert = *tmp;

        const Uint32Array& vrfStartCount = getVerticesRingFaceStartCount();
        Uint32Array1D& vertRingFace = *(dynamic_cast<Uint32Array1D*>(getVerticesRingFace()));
        Int32Array& vertTagFlags = getVerticesTagFlags();
        Uint8Array& vertOnEdge = getVerticesOnEdge();
        const Uint32Array& fAr = getFaces();
        int vrvStart = 0;
        int l = static_cast<int>(getNbVertices());

        for (int i = 0; i < l; ++i)
        {
            int tagFlag = ++Utils::TAG_FLAG;
            int vrfStart = static_cast<int>(vrfStartCount[i * 2]);
            int vrfEnd = vrfStart + static_cast<int>(vrfStartCount[i * 2 + 1]);
            int vrvCount = 0;

            for (int j = vrfStart; j < vrfEnd; ++j)
            {
                int ind = static_cast<int>(vertRingFace[j] * 4);
                std::uint32_t iVer1 = fAr[ind + 0];
                std::uint32_t iVer2 = fAr[ind + 1];
                std::uint32_t iVer3 = fAr[ind + 2];
                std::uint32_t iVer4 = fAr[ind + 3];

                if (iVer1 == i)
                    iVer1 = iVer4 != Utils::TRI_INDEX ? iVer4 : iVer3;
                else if (iVer2 == i || iVer4 == i)
                    iVer2 = iVer3;
                else if (iVer3 == i && iVer4 != Utils::TRI_INDEX)
                    iVer1 = iVer4;

                if (vertTagFlags[iVer1] != tagFlag)
                {
                    vertRingVert[vrvStart + (vrvCount++)] = iVer1;
                    vertTagFlags[iVer1] = tagFlag;
                }

                if (vertTagFlags[iVer2] != tagFlag)
                {
                    vertRingVert[vrvStart + (vrvCount++)] = iVer2;
                    vertTagFlags[iVer2] = tagFlag;
                }
            }

            vrvStartCount[i * 2 + 0] = vrvStart;
            vrvStartCount[i * 2 + 1] = vrvCount;
            vrvStart += vrvCount;
            if ((vrfEnd - vrfStart) != vrvCount)
                vertOnEdge[i] = 1;
        }
    }

    /** Get more vertices (n-ring) */
    Uint32Array Mesh::expandsVertices(const Uint32Array& iVerts, int nRing)
    {
        int tagFlag = ++Utils::TAG_FLAG;
        std::uint32_t nbVerts = static_cast<std::uint32_t>(iVerts.size());//iVerts.length;
        const Uint32Array& vrvStartCount = getVerticesRingVertStartCount();
        IUint32ArrayND* vertRingVert = getVerticesRingVert();
        bool is2DArray = vertRingVert->Dimension() == 2;//var ringVerts = vertRingVert instanceof Array ? vertRingVert : null;
        Int32Array& vertTagFlags = getVerticesTagFlags();
        std::uint32_t acc = nbVerts;
        std::uint32_t  nbVertices = getNbVertices();
        Uint32Array iVertsExpanded; iVertsExpanded.resize(4 * nbVertices, 0);//var iVertsExpanded = new Uint32Array(Utils.getMemory(4 * nbVertices), 0, nbVertices);
        Utils::set(iVertsExpanded, iVerts);//iVertsExpanded.set(iVerts);

        std::uint32_t i = 0;

        for (i = 0; i < nbVerts; ++i)
            vertTagFlags[iVertsExpanded[i]] = tagFlag;

        std::uint32_t iBegin = 0;

        if (is2DArray)
        {
            Uint32Array2D& array2D = *(dynamic_cast<Uint32Array2D*>(vertRingVert));

            while (nRing != 0)
            {
                --nRing;

                for (i = iBegin; i < nbVerts; ++i)
                {
                    std::uint32_t idVert = iVertsExpanded[i];
                    std::uint32_t* ring = array2D[idVert];
                    std::uint32_t end = static_cast<std::uint32_t>(Utils::GetArrayElementsCount(ring));
                    std::uint32_t start = 0;

                    for (int j = start; j < end; ++j)
                    {
                        int id = ring[j];

                        if (vertTagFlags[id] == tagFlag)
                            continue;

                        vertTagFlags[id] = tagFlag;
                        iVertsExpanded[acc++] = id;
                    }
                }

                iBegin = nbVerts;
                nbVerts = acc;
            }
        }
        else
        {
            Uint32Array1D& array1D = *(dynamic_cast<Uint32Array1D*>(vertRingVert));

            while (nRing != 0)
            {
                --nRing;

                for (i = iBegin; i < nbVerts; ++i)
                {
                    std::uint32_t idVert = iVertsExpanded[i];
                    std::uint32_t start = vrvStartCount[idVert * 2 + 0];
                    std::uint32_t end = start + vrvStartCount[idVert * 2 + 1];

                    for (std::uint32_t j = start; j < end; ++j)
                    {
                        std::uint32_t id = array1D[j];

                        if (vertTagFlags[id] == tagFlag)
                            continue;

                        vertTagFlags[id] = tagFlag;
                        iVertsExpanded[acc++] = id;
                    }
                }

                iBegin = nbVerts;
                nbVerts = acc;
            }
        }

        return Utils::subarray(iVertsExpanded, 0, acc);
    }

    Uint32Array Mesh::getVerticesFromFaces(const Uint32Array& iFaces)
    {
        int tagFlag = ++Utils::TAG_FLAG;
        std::uint32_t nbFaces = iFaces.size();//iFaces.length;
        Int32Array& vertTagFlags = getVerticesTagFlags();
        const Uint32Array& fAr = getFaces();
        std::uint32_t acc = 0;
        Uint32Array verts(iFaces.size() * 4, 0);//std::uint32_t verts = new Uint32Array(Utils.getMemory(4 * iFaces.length * 4), 0, iFaces.length * 4);

        for (std::uint32_t i = 0; i < nbFaces; ++i)
        {
            std::uint32_t ind = iFaces[i] * 4;
            std::uint32_t iVer1 = fAr[ind + 0];
            std::uint32_t iVer2 = fAr[ind + 1];
            std::uint32_t iVer3 = fAr[ind + 2];
            std::uint32_t iVer4 = fAr[ind + 3];

            if (vertTagFlags[iVer1] != tagFlag)
            {
                vertTagFlags[iVer1] = tagFlag;
                verts[acc++] = iVer1;
            }

            if (vertTagFlags[iVer2] != tagFlag)
            {
                vertTagFlags[iVer2] = tagFlag;
                verts[acc++] = iVer2;
            }

            if (vertTagFlags[iVer3] != tagFlag)
            {
                vertTagFlags[iVer3] = tagFlag;
                verts[acc++] = iVer3;
            }

            if (iVer4 != Utils::TRI_INDEX && vertTagFlags[iVer4] != tagFlag)
            {
                vertTagFlags[iVer4] = tagFlag;
                verts[acc++] = iVer4;
            }
        }

        return Utils::subarray(verts, 0, acc);
    }

    /// <summary>
    /// 根据传递进来的面信息，计算AABB信息和面法线信息
    /// </summary>
    /// <param name="iFaces"></param>
    void Mesh::updateFacesAabbAndNormal(const Uint32Array& iFaces)
    {
        // 拿到面法线数组准备填充
        Float32Array& faceNormals = getFaceNormals();
        Float32Array& faceBoxes = getFaceBoxes();
        Float32Array& faceCenters = getFaceCenters();
        const Float32Array& vAr = getVertices();
        const Uint32Array& fAr = getFaces();

        bool full = iFaces.size() == 0;
        std::uint32_t nbFaces = full ? getNbFaces() : iFaces.size();

        for (std::uint32_t i = 0; i < nbFaces; ++i)
        {
            std::uint32_t ind = full ? i : iFaces[i];
            std::uint32_t idTri = ind * 3;
            std::uint32_t idFace = ind * 4;
            std::uint32_t idBox = ind * 6;

            std::uint32_t ind1 = fAr[idFace + 0] * 3;
            std::uint32_t ind2 = fAr[idFace + 1] * 3;
            std::uint32_t ind3 = fAr[idFace + 2] * 3;
            std::uint32_t ind4 = fAr[idFace + 3];

            bool isQuad = ind4 != Utils::TRI_INDEX;

            if (isQuad)
                ind4 *= 3;

            // 拿到三个三角形的顶点
            float v1x = vAr[ind1 + 0];
            float v1y = vAr[ind1 + 1];
            float v1z = vAr[ind1 + 2];

            float v2x = vAr[ind2 + 0];
            float v2y = vAr[ind2 + 1];
            float v2z = vAr[ind2 + 2];

            float v3x = vAr[ind3 + 0];
            float v3y = vAr[ind3 + 1];
            float v3z = vAr[ind3 + 2];

            // 计算法线 。首先第一步计算三角形的三个边的连线向量
            float ax = v2x - v1x;
            float ay = v2y - v1y;
            float az = v2z - v1z;
            float bx = v3x - v1x;
            float by = v3y - v1y;
            float bz = v3z - v1z;

            // 计算叉积
            float crx = ay * bz - az * by;
            float cry = az * bx - ax * bz;
            float crz = ax * by - ay * bx;

            float cr_len = std::sqrt(crz * crz + cry * cry + crx * crx);
            crx /= cr_len;
            cry /= cr_len;
            crz /= cr_len;

            // compute boxes
            float xmax = 0.0f, xmin = 0.0f;
            float ymax = 0.0f, ymin = 0.0f;
            float zmax = 0.0f, zmin = 0.0f;

            Utils::FindMaxMin(v1x, v2x, v3x, xmax, xmin);
            Utils::FindMaxMin(v1y, v2y, v3y, ymax, ymin);
            Utils::FindMaxMin(v1z, v2z, v3z, zmax, zmin);

            if (isQuad)
            {
                float v4x = vAr[ind4 + 0];
                float v4y = vAr[ind4 + 1];
                float v4z = vAr[ind4 + 2];
                if (v4x < xmin) xmin = v4x;
                if (v4x > xmax) xmax = v4x;
                if (v4y < ymin) ymin = v4y;
                if (v4y > ymax) ymax = v4y;
                if (v4z < zmin) zmin = v4z;
                if (v4z > zmax) zmax = v4z;
                ax = v3x - v4x;
                ay = v3y - v4y;
                az = v3z - v4z;
                crx += ay * bz - az * by;
                cry += az * bx - ax * bz;
                crz += ax * by - ay * bx;
            }

            // normals
            faceNormals[idTri + 0] = crx;
            faceNormals[idTri + 1] = cry;
            faceNormals[idTri + 2] = crz;

            // boxes
            faceBoxes[idBox + 0] = xmin;
            faceBoxes[idBox + 1] = ymin;
            faceBoxes[idBox + 2] = zmin;
            faceBoxes[idBox + 3] = xmax;
            faceBoxes[idBox + 4] = ymax;
            faceBoxes[idBox + 5] = zmax;

            // compute centers
            faceCenters[idTri + 0] = (xmin + xmax) * 0.5f;
            faceCenters[idTri + 1] = (ymin + ymax) * 0.5f;
            faceCenters[idTri + 2] = (zmin + zmax) * 0.5f;
        }
    }

    /** Get more faces (n-ring) */
    Uint32Array Mesh::expandsFaces(const Uint32Array& iFaces, std::uint32_t nRing)
    {
        int tagFlag = ++Utils::TAG_FLAG;
        std::uint32_t nbFaces = iFaces.size();
        const Uint32Array& vrfStartCount = getVerticesRingFaceStartCount();
        IUint32ArrayND* vertRingFace = getVerticesRingFace();
        bool is2DArray = vertRingFace->Dimension() == 2;//var ringFaces = vertRingFace instanceof Array ? vertRingFace: null;
        Int32Array& ftf = getFacesTagFlags();
        const Uint32Array& fAr = getFaces();
        std::uint32_t acc = nbFaces;
        Uint32Array iFacesExpanded(getNbFaces(), 0);// = new int[getNbFaces()];//var iFacesExpanded = new Uint32Array(Utils.getMemory(4 * getNbFaces()), 0, getNbFaces());
        Utils::set(iFacesExpanded, iFaces);

        std::uint32_t i = 0;
        for (i = 0; i < nbFaces; ++i)
            ftf[iFacesExpanded[i]] = tagFlag;

        if (is2DArray)
        {
            Uint32Array2D& array2D = *(dynamic_cast<Uint32Array2D*>(vertRingFace));// as Array2D<int>;
            std::uint32_t iBegin = 0;

            while (nRing != 0)
            {
                --nRing;

                for (i = iBegin; i < nbFaces; ++i)
                {
                    std::uint32_t ind = iFacesExpanded[i] * 4;

                    for (std::uint32_t j = 0; j < 4; ++j)
                    {
                        std::uint32_t idv = fAr[ind + j];

                        if (idv == Utils::TRI_INDEX)
                            continue;

                        std::uint32_t* ring = array2D[idv];
                        std::uint32_t end = Utils::GetArrayElementsCount(ring);//ring.Length;
                        std::uint32_t start = 0;

                        for (std::uint32_t k = start; k < end; ++k)
                        {
                            int id = ring[k];

                            if (ftf[id] == tagFlag)
                                continue;

                            ftf[id] = tagFlag;
                            iFacesExpanded[acc++] = id;
                        }
                    }
                }

                iBegin = nbFaces;
                nbFaces = acc;
            }
        }
        else
        {
            Uint32Array1D& array1D = *(dynamic_cast<Uint32Array1D*>(vertRingFace));//Array1D<int> array1D = vertRingFace as Array1D<int>;
            std::uint32_t iBegin = 0;

            while (nRing != 0)
            {
                --nRing;

                for (i = iBegin; i < nbFaces; ++i)
                {
                    std::uint32_t ind = iFacesExpanded[i] * 4;

                    for (std::uint32_t j = 0; j < 4; ++j)
                    {
                        std::uint32_t idv = fAr[ind + j];

                        if (idv == Utils::TRI_INDEX)
                            continue;

                        std::uint32_t start = vrfStartCount[idv * 2];
                        std::uint32_t end = start + vrfStartCount[idv * 2 + 1];

                        for (std::uint32_t k = start; k < end; ++k)
                        {
                            int id = array1D[k];
                            if (ftf[id] == tagFlag)
                                continue;
                            ftf[id] = tagFlag;
                            iFacesExpanded[acc++] = id;
                        }
                    }
                }
                iBegin = nbFaces;
                nbFaces = acc;
            }
        }

        return Utils::subarray(iFacesExpanded, 0, acc);//return iFacesExpanded.subarray(0, acc);//return new Uint32Array(iFacesExpanded.subarray(0, acc));
    }

    Uint32Array Mesh::getFacesFromVertices(const Uint32Array& iVerts)
    {
        int tagFlag = ++Utils::TAG_FLAG;
        std::uint32_t nbVerts = iVerts.size();
        const Uint32Array& vrfStartCount = getVerticesRingFaceStartCount();
        IUint32ArrayND* vertRingFace = getVerticesRingFace();
        bool is2DArray = vertRingFace->Dimension() == 2; //int ringFaces = vertRingFace instanceof Array ? vertRingFace: null;
        Int32Array& ftf = getFacesTagFlags();
        std::uint32_t  acc = 0;
        Uint32Array iFaces(getNbFaces(), 0);// = new int[getNbFaces()];//int iFaces = new Uint32Array(Utils.getMemory(4 * getNbFaces()), 0, getNbFaces());

        if (is2DArray)
        {
            Uint32Array2D& array2D = *(dynamic_cast<Uint32Array2D*>(vertRingFace));

            for (std::uint32_t i = 0; i < nbVerts; ++i)
            {
                std::uint32_t idVert = iVerts[i];
                std::uint32_t* ring = array2D[idVert];
                std::uint32_t end = Utils::GetArrayElementsCount(ring);
                std::uint32_t start = 0;

                for (std::uint32_t j = start; j < end; ++j)
                {
                    std::uint32_t iFace = ring[j];

                    if (ftf[iFace] != tagFlag)
                    {
                        ftf[iFace] = tagFlag;
                        iFaces[acc++] = iFace;
                    }
                }
            }
        }
        else
        {
            Uint32Array1D& array1D = *(dynamic_cast<Uint32Array1D*>(vertRingFace));

            for (std::uint32_t i = 0; i < nbVerts; ++i)
            {
                std::uint32_t idVert = iVerts[i];
                std::uint32_t start = vrfStartCount[idVert * 2];
                std::uint32_t end = start + vrfStartCount[idVert * 2 + 1];

                for (std::uint32_t j = start; j < end; ++j)
                {
                    std::uint32_t iFace = array1D[j];

                    if (ftf[iFace] != tagFlag)
                    {
                        ftf[iFace] = tagFlag;
                        iFaces[acc++] = iFace;
                    }
                }
            }

        }

        return Utils::subarray(iFaces, 0, acc); //return iFaces.subarray(0, acc);
    }

    void Mesh::initRenderTriangles()
    {
        if (hasUV())
        {
            const Uint32Array& src = computeTrianglesFromFaces(getFacesTexCoord());
            _meshData->_UVtrianglesABC.clear();
            std::copy(src.begin(), src.end(), std::back_inserter(_meshData->_UVtrianglesABC));
            // _meshData->_UVtrianglesABC = _meshData->_UVtrianglesABC = computeTrianglesFromFaces(getFacesTexCoord());
        }

        const Uint32Array& src2 = computeTrianglesFromFaces(getFaces());
        _meshData->_trianglesABC.clear();
        std::copy(src2.begin(), src2.end(), std::back_inserter(_meshData->_trianglesABC));
        //_meshData->_trianglesABC = computeTrianglesFromFaces(getFaces());
    }

    Uint32Array Mesh::computeTrianglesFromFaces(const Uint32Array& faces)
    {
        std::uint32_t  nbFaces = getNbFaces();
        Uint32Array& facesToTris = getFacesToTriangles();
        Uint32Array iAr(nbFaces * 6, 0);//var iAr = new Uint32Array(Utils.getMemory(4 * nbFaces * 6), 0, nbFaces * 6);
        std::uint32_t  acc = 0;

        for (std::uint32_t i = 0; i < nbFaces; ++i)
        {
            facesToTris[i] = acc;
            std::uint32_t iFace = i * 4;
            std::uint32_t iv1 = faces[iFace + 0];
            std::uint32_t iv2 = faces[iFace + 1];
            std::uint32_t iv3 = faces[iFace + 2];
            std::uint32_t iv4 = faces[iFace + 3];
            std::uint32_t iTri = acc * 3;
            iAr[iTri + 0] = iv1;
            iAr[iTri + 1] = iv2;
            iAr[iTri + 2] = iv3;
            ++acc;
            if (iv4 != Utils::TRI_INDEX)
            {
                iTri = acc * 3;
                iAr[iTri + 0] = iv1;
                iAr[iTri + 1] = iv3;
                iAr[iTri + 2] = iv4;
                ++acc;
            }
        }

        return Utils::subarray(iAr, 0, acc * 3);//return new Uint32Array(iAr.subarray(0, acc * 3));
    }

    void Mesh::initEdges()
    {
        const Uint32Array& fAr = getFaces();
        Uint32Array& feAr = getFaceEdges();
        int nbEdges = 0;
        Uint32Array vertEdgeTemp(getNbVertices(), 0);//int[] vertEdgeTemp = new int[getNbVertices()];//var vertEdgeTemp = new Uint32Array(getNbVertices());
        Uint32Array vrfStartCount = getVerticesRingFaceStartCount();
        IUint32ArrayND* vertRingFace = getVerticesRingFace();//var vertRingFace = getVerticesRingFace();

        Uint32Array1D* ring_ptr = dynamic_cast<Uint32Array1D*>(vertRingFace);

        if (nullptr == ring_ptr)
            return;

        Uint32Array1D& ring = *ring_ptr;
        std::uint32_t nbVerts = getNbVertices();

        for (std::uint32_t i = 0; i < nbVerts; ++i)
        {
            std::uint32_t start = vrfStartCount[i * 2];
            std::uint32_t end = start + vrfStartCount[i * 2 + 1];
            std::uint32_t compTest = nbEdges;

            for (std::uint32_t j = start; j < end; ++j)
            {
                std::uint32_t id = ring[j] * 4;
                std::uint32_t iv1 = fAr[id + 0];
                std::uint32_t iv2 = fAr[id + 1];
                std::uint32_t iv3 = fAr[id + 2];
                std::uint32_t iv4 = fAr[id + 3];
                std::uint32_t t = 0;
                std::uint32_t idEdge = 0;

                if (iv4 == Utils::TRI_INDEX)
                {
                    if (i > iv1)
                    {
                        t = vertEdgeTemp[iv1];

                        idEdge = id + (i == iv2 ? 0 : 2);

                        if (t <= compTest)
                        {
                            feAr[idEdge] = nbEdges;
                            vertEdgeTemp[iv1] = ++nbEdges;
                        }
                        else
                        {
                            feAr[idEdge] = t - 1;
                        }
                    }

                    if (i > iv2)
                    {
                        t = vertEdgeTemp[iv2];
                        idEdge = id + (i == iv1 ? 0 : 1);

                        if (t <= compTest)
                        {
                            feAr[idEdge] = nbEdges;
                            vertEdgeTemp[iv2] = ++nbEdges;
                        }
                        else
                        {
                            feAr[idEdge] = t - 1;
                        }
                    }

                    if (i > iv3)
                    {
                        t = vertEdgeTemp[iv3];
                        idEdge = id + (i == iv1 ? 2 : 1);

                        if (t <= compTest)
                        {
                            feAr[idEdge] = nbEdges;
                            vertEdgeTemp[iv3] = ++nbEdges;
                        }
                        else
                        {
                            feAr[idEdge] = t - 1;
                        }
                    }

                    feAr[id + 3] = Utils::TRI_INDEX;
                }
                else
                {
                    if (i > iv1 && i != iv3)
                    {
                        t = vertEdgeTemp[iv1];
                        idEdge = id + (i == iv2 ? 0 : 3);

                        if (t <= compTest)
                        {
                            feAr[idEdge] = nbEdges;
                            vertEdgeTemp[iv1] = ++nbEdges;
                        }
                        else
                        {
                            feAr[idEdge] = t - 1;
                        }
                    }

                    if (i > iv2 && i != iv4)
                    {
                        t = vertEdgeTemp[iv2];
                        idEdge = id + (i == iv1 ? 0 : 1);

                        if (t <= compTest)
                        {
                            feAr[idEdge] = nbEdges;
                            vertEdgeTemp[iv2] = ++nbEdges;
                        }
                        else
                        {
                            feAr[idEdge] = t - 1;
                        }
                    }

                    if (i > iv3 && i != iv1)
                    {
                        t = vertEdgeTemp[iv3];
                        idEdge = id + (i == iv2 ? 1 : 2);

                        if (t <= compTest)
                        {
                            feAr[idEdge] = nbEdges;
                            vertEdgeTemp[iv3] = ++nbEdges;
                        }
                        else
                        {
                            feAr[idEdge] = t - 1;
                        }
                    }

                    if (i > iv4 && i != iv2)
                    {
                        t = vertEdgeTemp[iv4];
                        idEdge = id + (i == iv1 ? 3 : 2);
                        if (t <= compTest)
                        {
                            feAr[idEdge] = nbEdges;
                            vertEdgeTemp[iv4] = ++nbEdges;
                        }
                        else
                        {
                            feAr[idEdge] = t - 1;
                        }
                    }
                }
            }
        }

        _meshData->_edges.clear(); _meshData->_edges.resize(nbEdges, 0); //var eAr = _meshData._edges = new Uint8ClampedArray(nbEdges);
        std::uint32_t nbFaces = getNbFaces();

        for (int k = 0; k < nbFaces; ++k)
        {
            std::uint32_t idf = k * 4;
            _meshData->_edges[feAr[idf + 0]]++;
            _meshData->_edges[feAr[idf + 1]]++;
            _meshData->_edges[feAr[idf + 2]]++;
            std::uint32_t i4 = feAr[idf + 3];
            if (i4 != Utils::TRI_INDEX)
                _meshData->_edges[i4]++;
        }
    }

    /** Return wireframe array (or compute it if not up to date) */
    Uint32Array& Mesh::getWireframe()
    {
        std::uint32_t  nbEdges = getNbEdges();
        bool useDrawArrays = isUsingDrawArrays();
        Uint32Array* cdw_ptr = nullptr;

        if (useDrawArrays)
        {
            if (_meshData->_drawArraysWireframe.size() > 0 && _meshData->_drawArraysWireframe.size() == nbEdges * 2)
            {
                return _meshData->_drawArraysWireframe;
            }

            _meshData->_drawArraysWireframe.resize(nbEdges * 2, 0);// = new Uint32Array(nbEdges * 2);
            cdw_ptr = &_meshData->_drawArraysWireframe;
        }
        else
        {
            if (_meshData->_drawElementsWireframe.size() > 0 && _meshData->_drawElementsWireframe.size() > 0 == nbEdges * 2)
            {
                return _meshData->_drawElementsWireframe;
            }

            _meshData->_drawElementsWireframe.resize(nbEdges * 2, 0);// cdw = _meshData->_drawElementsWireframe = new Uint32Array(nbEdges * 2);
            cdw_ptr = &_meshData->_drawElementsWireframe;
        }

        const Uint32Array& fAr = getFaces();
        const Uint32Array& feAr = getFaceEdges();
        std::uint32_t nbFaces = getNbFaces();
        const Uint32Array& facesToTris = getFacesToTriangles();

        std::uint32_t nbLines = 0;
        Uint8Array tagEdges(nbEdges);//var tagEdges = new Uint8Array(nbEdges);
        Uint32Array& cdw = *cdw_ptr;

        for (std::uint32_t i = 0; i < nbFaces; ++i)
        {
            std::uint32_t id = i * 4;

            std::uint32_t iv1, iv2, iv3;
            std::uint32_t iv4 = fAr[id + 3];
            bool isQuad = iv4 != Utils::TRI_INDEX;

            if (useDrawArrays)
            {
                std::uint32_t idTri = facesToTris[i] * 3;
                iv1 = idTri + 0;
                iv2 = idTri + 1;
                iv3 = idTri + 2;
                if (isQuad)
                    iv4 = idTri + 5;
            }
            else
            {
                iv1 = fAr[id];
                iv2 = fAr[id + 1];
                iv3 = fAr[id + 2];
            }

            std::uint32_t ide1 = feAr[id + 0];
            std::uint32_t ide2 = feAr[id + 1];
            std::uint32_t ide3 = feAr[id + 2];
            std::uint32_t ide4 = feAr[id + 3];

            if (tagEdges[ide1] == 0)
            {
                tagEdges[ide1] = 1;
                cdw[nbLines * 2 + 0] = iv1;
                cdw[nbLines * 2 + 1] = iv2;
                nbLines++;
            }

            if (tagEdges[ide2] == 0)
            {
                tagEdges[ide2] = 1;
                cdw[nbLines * 2 + 0] = iv2;
                cdw[nbLines * 2 + 1] = iv3;
                nbLines++;
            }
            if (tagEdges[ide3] == 0)
            {
                tagEdges[ide3] = 1;
                cdw[nbLines * 2 + 0] = iv3;
                cdw[nbLines * 2 + 1] = isQuad ? iv4 : iv1;
                nbLines++;
            }

            if (isQuad && tagEdges[ide4] == 0)
            {
                tagEdges[ide4] = 1;
                cdw[nbLines * 2 + 0] = iv4;
                cdw[nbLines * 2 + 1] = iv1;
                nbLines++;
            }
        }

        return useDrawArrays ? _meshData->_drawArraysWireframe : _meshData->_drawElementsWireframe;
    }

    void Mesh::updateDuplicateGeometry(const Uint32Array& iVerts)
    {
        if (!isUsingTexCoords() || !hasUV())
            return;

        Float32Array& vAr = getVertices();
        Float32Array& cAr = getColors();
        Float32Array& mAr = getMaterials();
        Float32Array& nAr = getNormals();
        const Uint32Array& startCount = getVerticesDuplicateStartCount();

        bool full = iVerts.size() == 0;
        std::uint32_t nbVerts = full ? getNbVertices() : iVerts.size();

        for (std::uint32_t i = 0; i < nbVerts; ++i)
        {
            std::uint32_t ind = full ? i : iVerts[i];
            std::uint32_t start = startCount[ind * 2];

            if (start == 0)
                continue;

            std::uint32_t end = start + startCount[ind * 2 + 1];
            std::uint32_t idOrig = ind * 3;
            std::uint32_t vx = vAr[idOrig + 0];
            std::uint32_t vy = vAr[idOrig + 1];
            std::uint32_t vz = vAr[idOrig + 2];
            std::uint32_t nx = nAr[idOrig + 0];
            std::uint32_t ny = nAr[idOrig + 1];
            std::uint32_t nz = nAr[idOrig + 2];
            std::uint32_t cx = cAr[idOrig + 0];
            std::uint32_t cy = cAr[idOrig + 1];
            std::uint32_t cz = cAr[idOrig + 2];
            std::uint32_t mx = mAr[idOrig + 0];
            std::uint32_t my = mAr[idOrig + 1];
            std::uint32_t mz = mAr[idOrig + 2];

            for (std::uint32_t j = start; j < end; ++j)
            {
                std::uint32_t idDup = j * 3;
                vAr[idDup + 0] = vx;
                vAr[idDup + 1] = vy;
                vAr[idDup + 2] = vz;
                nAr[idDup + 0] = nx;
                nAr[idDup + 1] = ny;
                nAr[idDup + 2] = nz;
                cAr[idDup + 0] = cx;
                cAr[idDup + 1] = cy;
                cAr[idDup + 2] = cz;
                mAr[idDup + 0] = mx;
                mAr[idDup + 1] = my;
                mAr[idDup + 2] = mz;
            }
        }
    }

    void Mesh::updateDuplicateColorsAndMaterials(const Uint32Array& iVerts)
    {
        if (!isUsingTexCoords() || !hasUV())
            return;

        Float32Array& cAr = getColors();
        Float32Array& mAr = getMaterials();
        const Uint32Array& startCount = getVerticesDuplicateStartCount();

        bool full = iVerts.size() == 0; //var full = iVerts == = undefined;
        std::uint32_t nbVerts = full ? getNbVertices() : iVerts.size();

        for (std::uint32_t i = 0; i < nbVerts; ++i)
        {
            std::uint32_t ind = full ? i : iVerts[i];
            std::uint32_t start = startCount[ind * 2];

            if (start == 0)
                continue;

            std::uint32_t end = start + startCount[ind * 2 + 1];
            std::uint32_t idOrig = ind * 3;
            std::uint32_t cx = cAr[idOrig + 0];
            std::uint32_t cy = cAr[idOrig + 1];
            std::uint32_t cz = cAr[idOrig + 2];
            std::uint32_t mx = mAr[idOrig + 0];
            std::uint32_t my = mAr[idOrig + 1];
            std::uint32_t mz = mAr[idOrig + 2];

            for (std::uint32_t j = start; j < end; ++j)
            {
                std::uint32_t idDup = j * 3;
                cAr[idDup + 0] = cx;
                cAr[idDup + 1] = cy;
                cAr[idDup + 2] = cz;
                mAr[idDup + 0] = mx;
                mAr[idDup + 1] = my;
                mAr[idDup + 2] = mz;
            }
        }
    }

    void Mesh::initTexCoordsDataFromOBJData(const Float32Array& uvAr, Uint32Array& uvfArOrig)
    {
        Uint32Array& fAr = getFaces();
        std::uint32_t len = fAr.size();

        if (len != uvfArOrig.size())
            return;

        std::uint32_t nbUV = uvAr.size() / 2;

        // fix integrity (bug with sgl export??)
        for (std::uint32_t i = 0; i < len; ++i)
        {
            if (uvfArOrig[i] >= nbUV)
            {
                uvfArOrig[i] = fAr[i];
            }
        }

        std::uint32_t nbVertices = getNbVertices();
        std::uint32_t i = 0;
        std::uint32_t j = 0;
        std::uint32_t iv = 0;
        std::uint32_t tag = 0;

        // detect duplicates vertices because of tex coords
        Int32Array tagV(nbVertices, 0);// = new Int32Array(nbVertices);
        // vertex without uv might receive random values...
        Float32Array tArTemp(nbVertices * 2, 0);//var tArTemp = new Float32Array(Utils.getMemory(nbVertices * 4 * 2), 0, nbVertices * 2);
        std::vector<Uint32Array> dup;// = [];
        std::uint32_t acc = 0;
        std::uint32_t nbDuplicates = 0;

        for (i = 0; i < len; ++i)
        {
            iv = fAr[i];

            if (iv == Utils::TRI_INDEX)
                continue;

            std::uint32_t uv = uvfArOrig[i];
            tag = tagV[iv];

            if (tag == (uv + 1))
                continue;

            if (tag == 0)
            {
                tagV[iv] = uv + 1;
                tArTemp[iv * 2] = uvAr[uv * 2];
                tArTemp[iv * 2 + 1] = uvAr[uv * 2 + 1];
                continue;
            }

            // first duplicate
            if (tag > 0)
            {
                tagV[iv] = --acc;
                dup.emplace_back(); //dup.push([uv]);
                ++nbDuplicates;
                continue;
            }

            // check if we need to insert a new duplicate
            Uint32Array& dupArray = dup[-tag - 1];//var dupArray = dup[-tag - 1];
            std::uint32_t nbDup = dupArray.size();//var nbDup = dupArray.length;

            for (j = 0; j < nbDup; ++j)
            {
                if (dupArray[j] == uv)
                    break;
            }

            // insert new duplicate
            if (j == nbDup)
            {
                ++nbDuplicates;
                dupArray.emplace_back(uv);//dupArray.push(uv);
            }
        }

        // order the duplicates vertices (and tex coords)
        Float32Array tAr((nbVertices + nbDuplicates) * 2, 0);// var tAr = new Float32Array((nbVertices + nbDuplicates) * 2);
        Utils::set(tAr, tArTemp); //tAr.set(tArTemp);
        _meshData->_duplicateStartCount.resize(nbVertices * 2, 0);//var startCount = _meshData->_duplicateStartCount = new Uint32Array(nbVertices * 2);
        acc = 0;

        for (i = 0; i < nbVertices; ++i)
        {
            tag = tagV[i];

            if (tag >= 0)
                continue;

            Uint32Array& dAr = dup[-tag - 1];
            std::uint32_t nbDu = dAr.size();//dAr.length;
            std::uint32_t start = nbVertices + acc;
            _meshData->_duplicateStartCount[i * 2 + 0] = start;
            _meshData->_duplicateStartCount[i * 2 + 1] = nbDu;
            acc += nbDu;

            for (j = 0; j < nbDu; ++j)
            {
                std::uint32_t idUv = dAr[j] * 2;
                std::uint32_t idUvCoord = (start + j) * 2;
                tAr[idUvCoord + 0] = uvAr[idUv];
                tAr[idUvCoord + 1] = uvAr[idUv + 1];
            }
        }

        // create faces that uses duplicates vertices (with textures coordinates)
        Uint32Array uvfAr;//var uvfAr = new Uint32Array(fAr);
        len = fAr.size();

        for (i = 0; i < len; ++i)
        {
            iv = uvfAr[i];

            if (iv == Utils::TRI_INDEX)
                continue;

            tag = tagV[iv];

            if (tag > 0)
                continue;

            std::uint32_t idtex = uvfArOrig[i];
            Uint32Array  dArray = dup[-tag - 1];
            std::uint32_t nbEl = dArray.size();

            for (j = 0; j < nbEl; ++j)
            {
                if (idtex == dArray[j])
                {
                    uvfAr[i] = _meshData->_duplicateStartCount[iv * 2] + j;
                    break;
                }
            }
        }

        setTexCoords(tAr);
        setFacesTexCoord(uvfAr);
    }

    void Mesh::setAlreadyDrawArrays()
    {
        // kind of a hack, to be used if the main arrays are already draw arrays
        _meshData->_DAverticesXYZ = getVertices();
        _meshData->_DAnormalsXYZ = getNormals();
        _meshData->_DAcolorsRGB = getColors();
        _meshData->_DAmaterialsPBR = getMaterials();
    }

    void Mesh::updateDrawArrays(Uint32Array* iFaces)
    {
        if (!isUsingDrawArrays())
            return;

        const Float32Array& vAr = getVertices();
        const Float32Array& nAr = getNormals();
        const Float32Array& cAr = getColors();
        const Float32Array& mAr = getMaterials();
        const Uint32Array& fAr = getFaces();

        std::uint32_t nbTriangles = getNbTriangles();
        bool has_only_triangles = hasOnlyTriangles();
        bool full = iFaces == nullptr;// undefined;

        if (cdv.size() == 0 || cdv.size() < nbTriangles * 9)
        {
            //if (!cdv || cdv.length < nbTriangles * 9) {
            _meshData->_DAverticesXYZ.resize(nbTriangles * 9, 0.0f);// = _meshData->_DAverticesXYZ = new Float32Array(nbTriangles * 9);
            _meshData->_DAnormalsXYZ.resize(nbTriangles * 9, 0.0f);// = _meshData->_DAnormalsXYZ = new Float32Array(nbTriangles * 9);
            _meshData->_DAcolorsRGB.resize(nbTriangles * 9, 0.0f);// = _meshData->_DAcolorsRGB = new Float32Array(nbTriangles * 9);
            _meshData->_DAmaterialsPBR.resize(nbTriangles * 9, 0.0f);// = _meshData->_DAmaterialsPBR = new Float32Array(nbTriangles * 9);
        }

        std::uint32_t  nbFaces = full ? getNbFaces() : iFaces->size();

        for (std::uint32_t i = 0; i < nbFaces; ++i)
        {
            std::uint32_t idFace = full ? i : iFaces->at(i);
            std::uint32_t ftt = has_only_triangles ? getFacesToTriangles()[idFace] : idFace;
            std::uint32_t vId = ftt * 9;

            idFace *= 4;
            std::uint32_t id1 = fAr[idFace] * 3;
            std::uint32_t id2 = fAr[idFace + 1] * 3;
            std::uint32_t id3 = fAr[idFace + 2] * 3;

            // coordinates
            _meshData->_DAverticesXYZ[vId + 0] = vAr[id1 + 0];
            _meshData->_DAverticesXYZ[vId + 1] = vAr[id1 + 1];
            _meshData->_DAverticesXYZ[vId + 2] = vAr[id1 + 2];
            _meshData->_DAverticesXYZ[vId + 3] = vAr[id2 + 0];
            _meshData->_DAverticesXYZ[vId + 4] = vAr[id2 + 1];
            _meshData->_DAverticesXYZ[vId + 5] = vAr[id2 + 2];
            _meshData->_DAverticesXYZ[vId + 6] = vAr[id3 + 0];
            _meshData->_DAverticesXYZ[vId + 7] = vAr[id3 + 1];
            _meshData->_DAverticesXYZ[vId + 8] = vAr[id3 + 2];

            // color
            _meshData->_DAcolorsRGB[vId + 0] = cAr[id1 + 0];
            _meshData->_DAcolorsRGB[vId + 1] = cAr[id1 + 1];
            _meshData->_DAcolorsRGB[vId + 2] = cAr[id1 + 2];
            _meshData->_DAcolorsRGB[vId + 3] = cAr[id2 + 0];
            _meshData->_DAcolorsRGB[vId + 4] = cAr[id2 + 1];
            _meshData->_DAcolorsRGB[vId + 5] = cAr[id2 + 2];
            _meshData->_DAcolorsRGB[vId + 6] = cAr[id3 + 0];
            _meshData->_DAcolorsRGB[vId + 7] = cAr[id3 + 1];
            _meshData->_DAcolorsRGB[vId + 8] = cAr[id3 + 2];

            // material
            _meshData->_DAmaterialsPBR[vId] = mAr[id1 + 0];
            _meshData->_DAmaterialsPBR[vId + 1] = mAr[id1 + 1];
            _meshData->_DAmaterialsPBR[vId + 2] = mAr[id1 + 2];
            _meshData->_DAmaterialsPBR[vId + 3] = mAr[id2 + 0];
            _meshData->_DAmaterialsPBR[vId + 4] = mAr[id2 + 1];
            _meshData->_DAmaterialsPBR[vId + 5] = mAr[id2 + 2];
            _meshData->_DAmaterialsPBR[vId + 6] = mAr[id3 + 0];
            _meshData->_DAmaterialsPBR[vId + 7] = mAr[id3 + 1];
            _meshData->_DAmaterialsPBR[vId + 8] = mAr[id3 + 2];

            // normals
            _meshData->_DAnormalsXYZ[vId] = nAr[id1];
            _meshData->_DAnormalsXYZ[vId + 1] = nAr[id1 + 1];
            _meshData->_DAnormalsXYZ[vId + 2] = nAr[id1 + 2];
            _meshData->_DAnormalsXYZ[vId + 3] = nAr[id2];
            _meshData->_DAnormalsXYZ[vId + 4] = nAr[id2 + 1];
            _meshData->_DAnormalsXYZ[vId + 5] = nAr[id2 + 2];
            _meshData->_DAnormalsXYZ[vId + 6] = nAr[id3];
            _meshData->_DAnormalsXYZ[vId + 7] = nAr[id3 + 1];
            _meshData->_DAnormalsXYZ[vId + 8] = nAr[id3 + 2];

            std::uint32_t id4 = fAr[idFace + 3];

            if (id4 == Utils::TRI_INDEX)
                continue;

            id4 *= 3;
            vId += 9;

            // coordinates
            _meshData->_DAverticesXYZ[vId + 0] = vAr[id1 + 0];
            _meshData->_DAverticesXYZ[vId + 1] = vAr[id1 + 1];
            _meshData->_DAverticesXYZ[vId + 2] = vAr[id1 + 2];
            _meshData->_DAverticesXYZ[vId + 3] = vAr[id3 + 0];
            _meshData->_DAverticesXYZ[vId + 4] = vAr[id3 + 1];
            _meshData->_DAverticesXYZ[vId + 5] = vAr[id3 + 2];
            _meshData->_DAverticesXYZ[vId + 6] = vAr[id4 + 0];
            _meshData->_DAverticesXYZ[vId + 7] = vAr[id4 + 1];
            _meshData->_DAverticesXYZ[vId + 8] = vAr[id4 + 2];

            // colors
            _meshData->_DAcolorsRGB[vId + 0] = cAr[id1 + 0];
            _meshData->_DAcolorsRGB[vId + 1] = cAr[id1 + 1];
            _meshData->_DAcolorsRGB[vId + 2] = cAr[id1 + 2];
            _meshData->_DAcolorsRGB[vId + 3] = cAr[id3 + 0];
            _meshData->_DAcolorsRGB[vId + 4] = cAr[id3 + 1];
            _meshData->_DAcolorsRGB[vId + 5] = cAr[id3 + 2];
            _meshData->_DAcolorsRGB[vId + 6] = cAr[id4 + 0];
            _meshData->_DAcolorsRGB[vId + 7] = cAr[id4 + 1];
            _meshData->_DAcolorsRGB[vId + 8] = cAr[id4 + 2];

            // materials
            _meshData->_DAmaterialsPBR[vId + 0] = mAr[id1 + 0];
            _meshData->_DAmaterialsPBR[vId + 1] = mAr[id1 + 1];
            _meshData->_DAmaterialsPBR[vId + 2] = mAr[id1 + 2];
            _meshData->_DAmaterialsPBR[vId + 3] = mAr[id3 + 0];
            _meshData->_DAmaterialsPBR[vId + 4] = mAr[id3 + 1];
            _meshData->_DAmaterialsPBR[vId + 5] = mAr[id3 + 2];
            _meshData->_DAmaterialsPBR[vId + 6] = mAr[id4 + 0];
            _meshData->_DAmaterialsPBR[vId + 7] = mAr[id4 + 1];
            _meshData->_DAmaterialsPBR[vId + 8] = mAr[id4 + 2];

            // normals
            _meshData->_DAnormalsXYZ[vId + 0] = nAr[id1 + 0];
            _meshData->_DAnormalsXYZ[vId + 1] = nAr[id1 + 1];
            _meshData->_DAnormalsXYZ[vId + 2] = nAr[id1 + 2];
            _meshData->_DAnormalsXYZ[vId + 3] = nAr[id3 + 0];
            _meshData->_DAnormalsXYZ[vId + 4] = nAr[id3 + 1];
            _meshData->_DAnormalsXYZ[vId + 5] = nAr[id3 + 2];
            _meshData->_DAnormalsXYZ[vId + 6] = nAr[id4 + 0];
            _meshData->_DAnormalsXYZ[vId + 7] = nAr[id4 + 1];
            _meshData->_DAnormalsXYZ[vId + 8] = nAr[id4 + 2];
        }

        if (isUsingTexCoords())
            updateDrawArraysTexCoord(iFaces);
    }

    void Mesh::updateDrawArraysTexCoord(Uint32Array* iFaces)
    {
        std::uint32_t  nbTriangles = getNbTriangles();
        const Uint32Array& facesToTris = getFacesToTriangles();

        bool full = iFaces == nullptr;// .size() == 0;// == = undefined;
        //var cdt = _meshData->_DAtexCoordsST;
        //if (!cdt || cdt.length != = nbTriangles * 6)
        if (_meshData->_DAtexCoordsST.size() == 0 || _meshData->_DAtexCoordsST.size() != nbTriangles * 6)
            _meshData->_DAtexCoordsST.resize(nbTriangles * 6, 0.0f);//cdt = _meshData->_DAtexCoordsST = new Float32Array(nbTriangles * 6);

        const Float32Array& tAr = getTexCoords();
        const Uint32Array& fArUV = getFacesTexCoord();
        std::uint32_t nbFaces = full ? getNbFaces() : iFaces->size();

        for (std::uint32_t i = 0; i < nbFaces; ++i)
        {
            std::uint32_t idFace = full ? i : iFaces->at(i);
            std::uint32_t ftt = facesToTris[idFace];
            std::uint32_t vIduv = ftt * 6;

            idFace *= 4;
            std::uint32_t id1uv = fArUV[idFace + 0] * 2;
            std::uint32_t id2uv = fArUV[idFace + 1] * 2;
            std::uint32_t id3uv = fArUV[idFace + 2] * 2;

            _meshData->_DAtexCoordsST[vIduv + 0] = tAr[id1uv + 0];
            _meshData->_DAtexCoordsST[vIduv + 1] = tAr[id1uv + 1];
            _meshData->_DAtexCoordsST[vIduv + 2] = tAr[id2uv + 0];
            _meshData->_DAtexCoordsST[vIduv + 3] = tAr[id2uv + 1];
            _meshData->_DAtexCoordsST[vIduv + 4] = tAr[id3uv + 0];
            _meshData->_DAtexCoordsST[vIduv + 5] = tAr[id3uv + 1];

            std::uint32_t  id4uv = fArUV[idFace + 3];

            if (id4uv == Utils::TRI_INDEX)
                continue;
            
            id4uv *= 3;
            vIduv += 6;

            _meshData->_DAtexCoordsST[vIduv + 0] = tAr[id1uv + 0];
            _meshData->_DAtexCoordsST[vIduv + 1] = tAr[id1uv + 1];
            _meshData->_DAtexCoordsST[vIduv + 2] = tAr[id3uv + 0];
            _meshData->_DAtexCoordsST[vIduv + 3] = tAr[id3uv + 1];
            _meshData->_DAtexCoordsST[vIduv + 4] = tAr[id4uv + 0];
            _meshData->_DAtexCoordsST[vIduv + 5] = tAr[id4uv + 1];
        }
    }

}