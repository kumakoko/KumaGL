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
        delete [] tmp;
    }


}