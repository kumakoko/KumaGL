#include "editing/subdivision.h"

namespace DigitalSculpt
{
    OddVertexComputer::OddVertexComputer(MeshSPtr mesh, Float32Array* vArOut, Float32Array* cArOut, Float32Array* mArOut)
    {
        _mesh = mesh;
        this->_vArOut = vArOut;
        this->_cArOut = cArOut;
        this->_mArOut = mArOut;
        /* this->_vAr = mesh->getVertices();
         this->_cAr = mesh->getColors();
         this->_mAr = mesh->getMaterials();
         this->_eAr = mesh->getEdges();*/
        this->_nbVertices = mesh->getNbVertices();
        this->_tagEdges.resize(mesh->getNbEdges(), 0);// = new Int32Array(mesh->getNbEdges());
    }

    OddVertexComputer::~OddVertexComputer()
    {
        _mesh.reset();
    }

    std::int32_t OddVertexComputer::computeTriangleEdgeVertex(std::uint32_t iv1, std::uint32_t iv2, std::uint32_t iv3, std::uint32_t  ide)
    {
        Float32Array& vAr = _mesh->getVertices();
        Float32Array& cAr = _mesh->getColors();
        Float32Array& mAr = _mesh->getMaterials();
        Uint8Array& eAr = _mesh->getEdges();
        // auto vAr = this->_vAr;
        // auto cAr = this->_cAr;
        // auto mAr = this->_mAr;
        // auto eAr = this->_eAr;

        Float32Array& vArOut = *_vArOut;
        Float32Array& cArOut = *_cArOut;
        Float32Array& mArOut = *_mArOut;
        // auto tagEdges = this->_tagEdges;


        std::uint32_t id1 = iv1 * 3;
        std::uint32_t id2 = iv2 * 3;
        std::uint32_t idOpp = iv3 * 3;

        auto testEdge = _tagEdges[ide] - 1;
        auto ivMid = testEdge == -1 ? this->_nbVertices++ : testEdge;
        auto idMid = ivMid * 3;
        auto edgeValue = eAr[ide];

        if (edgeValue == 1 || edgeValue >= 3 || Subdivision::LINEAR)
        {
            // mid edge vertex or non manifold shit
            if (testEdge != -1) // no need to recompute weird non manifold stuffs
                return ivMid;

            _tagEdges[ide] = ivMid + 1;

            vArOut[idMid + 0] = 0.5f * (vAr[id1 + 0] + vAr[id2 + 0]);
            vArOut[idMid + 1] = 0.5f * (vAr[id1 + 1] + vAr[id2 + 1]);
            vArOut[idMid + 2] = 0.5f * (vAr[id1 + 2] + vAr[id2 + 2]);

            cArOut[idMid + 0] = 0.5f * (cAr[id1 + 0] + cAr[id2 + 0]);
            cArOut[idMid + 1] = 0.5f * (cAr[id1 + 1] + cAr[id2 + 1]);
            cArOut[idMid + 2] = 0.5f * (cAr[id1 + 2] + cAr[id2 + 2]);

            mArOut[idMid + 0] = 0.5f * (mAr[id1 + 0] + mAr[id2 + 0]);
            mArOut[idMid + 1] = 0.5f * (mAr[id1 + 1] + mAr[id2 + 1]);
            mArOut[idMid + 2] = 0.5f * (mAr[id1 + 2] + mAr[id2 + 2]);
        }
        else if (testEdge == -1)
        {
            // new mid vertex
            _tagEdges[ide] = ivMid + 1;
            vArOut[idMid + 0] = 0.125f * vAr[idOpp + 0] + 0.375f * (vAr[id1 + 0] + vAr[id2 + 0]);
            vArOut[idMid + 1] = 0.125f * vAr[idOpp + 1] + 0.375f * (vAr[id1 + 1] + vAr[id2 + 1]);
            vArOut[idMid + 2] = 0.125f * vAr[idOpp + 2] + 0.375f * (vAr[id1 + 2] + vAr[id2 + 2]);

            cArOut[idMid + 0] = 0.125f * cAr[idOpp + 0] + 0.375f * (cAr[id1 + 0] + cAr[id2 + 0]);
            cArOut[idMid + 1] = 0.125f * cAr[idOpp + 1] + 0.375f * (cAr[id1 + 1] + cAr[id2 + 1]);
            cArOut[idMid + 2] = 0.125f * cAr[idOpp + 2] + 0.375f * (cAr[id1 + 2] + cAr[id2 + 2]);

            mArOut[idMid + 0] = 0.125f * mAr[idOpp + 0] + 0.375f * (mAr[id1 + 0] + mAr[id2 + 0]);
            mArOut[idMid + 1] = 0.125f * mAr[idOpp + 1] + 0.375f * (mAr[id1 + 1] + mAr[id2 + 1]);
            mArOut[idMid + 2] = 0.125f * mAr[idOpp + 2] + 0.375f * (mAr[id1 + 2] + mAr[id2 + 2]);
        }
        else
        {
            // mid vertex already exists
            vArOut[idMid + 0] += 0.125f * vAr[idOpp + 0];
            vArOut[idMid + 1] += 0.125f * vAr[idOpp + 1];
            vArOut[idMid + 2] += 0.125f * vAr[idOpp + 2];

            cArOut[idMid + 0] += 0.125f * cAr[idOpp + 0];
            cArOut[idMid + 1] += 0.125f * cAr[idOpp + 1];
            cArOut[idMid + 2] += 0.125f * cAr[idOpp + 2];

            mArOut[idMid + 0] += 0.125f * mAr[idOpp + 0];
            mArOut[idMid + 1] += 0.125f * mAr[idOpp + 1];
            mArOut[idMid + 2] += 0.125f * mAr[idOpp + 2];
        }

        return ivMid;
    }

    std::uint32_t OddVertexComputer::computeQuadEdgeVertex(std::uint32_t iv1, std::uint32_t iv2, std::uint32_t iv3, std::uint32_t  iv4, std::uint32_t ide)
    {
        Float32Array& vAr = _mesh->getVertices();
        Float32Array& cAr = _mesh->getColors();
        Float32Array& mAr = _mesh->getMaterials();
        Uint8Array& eAr = _mesh->getEdges();

        Float32Array& vArOut = *_vArOut;
        Float32Array& cArOut = *_cArOut;
        Float32Array& mArOut = *_mArOut;

        //auto tagEdges = this->_tagEdges;
        auto id1 = iv1 * 3;
        auto id2 = iv2 * 3;
        auto idOpp = iv3 * 3;
        auto idOpp2 = iv4 * 3;
        auto testEdge = _tagEdges[ide] - 1;
        auto ivMid = testEdge == -1 ? this->_nbVertices++ : testEdge;
        auto idMid = ivMid * 3;
        auto edgeValue = eAr[ide];

        if (edgeValue == 1 || edgeValue >= 3 || Subdivision::LINEAR)
        {
            // mid edge vertex or non manifold shit
            if (testEdge != -1) // no need to recompute weird non manifold stuffs
                return ivMid;

            _tagEdges[ide] = ivMid + 1;
            vArOut[idMid + 0] = 0.5f * (vAr[id1 + 0] + vAr[id2 + 0]);
            vArOut[idMid + 1] = 0.5f * (vAr[id1 + 1] + vAr[id2 + 1]);
            vArOut[idMid + 2] = 0.5f * (vAr[id1 + 2] + vAr[id2 + 2]);

            cArOut[idMid + 0] = 0.5f * (cAr[id1 + 0] + cAr[id2 + 0]);
            cArOut[idMid + 1] = 0.5f * (cAr[id1 + 1] + cAr[id2 + 1]);
            cArOut[idMid + 2] = 0.5f * (cAr[id1 + 2] + cAr[id2 + 2]);

            mArOut[idMid + 0] = 0.5f * (mAr[id1 + 0] + mAr[id2 + 0]);
            mArOut[idMid + 1] = 0.5f * (mAr[id1 + 1] + mAr[id2 + 1]);
            mArOut[idMid + 2] = 0.5f * (mAr[id1 + 2] + mAr[id2 + 2]);
        }
        else if (testEdge == -1)
        { // new mid vertex
            _tagEdges[ide] = ivMid + 1;

            vArOut[idMid + 0] = 0.0625f * (vAr[idOpp + 0] + vAr[idOpp2 + 0]) + 0.375f * (vAr[id1 + 0] + vAr[id2 + 0]);
            vArOut[idMid + 1] = 0.0625f * (vAr[idOpp + 1] + vAr[idOpp2 + 1]) + 0.375f * (vAr[id1 + 1] + vAr[id2 + 1]);
            vArOut[idMid + 2] = 0.0625f * (vAr[idOpp + 2] + vAr[idOpp2 + 2]) + 0.375f * (vAr[id1 + 2] + vAr[id2 + 2]);

            cArOut[idMid + 0] = 0.0625f * (cAr[idOpp + 0] + cAr[idOpp2 + 0]) + 0.375f * (cAr[id1 + 0] + cAr[id2 + 0]);
            cArOut[idMid + 1] = 0.0625f * (cAr[idOpp + 1] + cAr[idOpp2 + 1]) + 0.375f * (cAr[id1 + 1] + cAr[id2 + 1]);
            cArOut[idMid + 2] = 0.0625f * (cAr[idOpp + 2] + cAr[idOpp2 + 2]) + 0.375f * (cAr[id1 + 2] + cAr[id2 + 2]);

            mArOut[idMid + 0] = 0.0625f * (mAr[idOpp + 0] + mAr[idOpp2 + 0]) + 0.375f * (mAr[id1 + 0] + mAr[id2 + 0]);
            mArOut[idMid + 1] = 0.0625f * (mAr[idOpp + 1] + mAr[idOpp2 + 1]) + 0.375f * (mAr[id1 + 1] + mAr[id2 + 1]);
            mArOut[idMid + 2] = 0.0625f * (mAr[idOpp + 2] + mAr[idOpp2 + 2]) + 0.375f * (mAr[id1 + 2] + mAr[id2 + 2]);
        }
        else
        {
            // mid vertex already exists
            vArOut[idMid + 0] += 0.0625f * (vAr[idOpp + 0] + vAr[idOpp2 + 0]);
            vArOut[idMid + 1] += 0.0625f * (vAr[idOpp + 1] + vAr[idOpp2 + 1]);
            vArOut[idMid + 2] += 0.0625f * (vAr[idOpp + 2] + vAr[idOpp2 + 2]);

            cArOut[idMid + 0] += 0.0625f * (cAr[idOpp + 0] + cAr[idOpp2 + 0]);
            cArOut[idMid + 1] += 0.0625f * (cAr[idOpp + 1] + cAr[idOpp2 + 1]);
            cArOut[idMid + 2] += 0.0625f * (cAr[idOpp + 2] + cAr[idOpp2 + 2]);

            mArOut[idMid + 0] += 0.0625f * (mAr[idOpp + 0] + mAr[idOpp2 + 0]);
            mArOut[idMid + 1] += 0.0625f * (mAr[idOpp + 1] + mAr[idOpp2 + 1]);
            mArOut[idMid + 2] += 0.0625f * (mAr[idOpp + 2] + mAr[idOpp2 + 2]);
        }
        return ivMid;
    }

    std::uint32_t OddVertexComputer::computeFaceVertex(std::uint32_t iv1, std::uint32_t  iv2, std::uint32_t  iv3, std::uint32_t  iv4)
    {
        auto id1 = iv1 * 3;
        auto id2 = iv2 * 3;
        auto id3 = iv3 * 3;
        auto id4 = iv4 * 3;

        Float32Array& vAr = _mesh->getVertices();
        Float32Array& cAr = _mesh->getColors();
        Float32Array& mAr = _mesh->getMaterials();
        Uint8Array& eAr = _mesh->getEdges();

        Float32Array& vArOut = *_vArOut;
        Float32Array& cArOut = *_cArOut;
        Float32Array& mArOut = *_mArOut;

        auto ivCen = this->_nbVertices++;
        auto idCen = ivCen * 3;
        vArOut[idCen + 0] = 0.25f * (vAr[id1 + 0] + vAr[id2 + 0] + vAr[id3 + 0] + vAr[id4 + 0]);
        vArOut[idCen + 1] = 0.25f * (vAr[id1 + 1] + vAr[id2 + 1] + vAr[id3 + 1] + vAr[id4 + 1]);
        vArOut[idCen + 2] = 0.25f * (vAr[id1 + 2] + vAr[id2 + 2] + vAr[id3 + 2] + vAr[id4 + 2]);

        cArOut[idCen + 0] = 0.25f * (cAr[id1 + 0] + cAr[id2 + 0] + cAr[id3 + 0] + cAr[id4 + 0]);
        cArOut[idCen + 1] = 0.25f * (cAr[id1 + 1] + cAr[id2 + 1] + cAr[id3 + 1] + cAr[id4 + 1]);
        cArOut[idCen + 2] = 0.25f * (cAr[id1 + 2] + cAr[id2 + 2] + cAr[id3 + 2] + cAr[id4 + 2]);

        mArOut[idCen + 0] = 0.25f * (mAr[id1 + 0] + mAr[id2 + 0] + mAr[id3 + 0] + mAr[id4 + 0]);
        mArOut[idCen + 1] = 0.25f * (mAr[id1 + 1] + mAr[id2 + 1] + mAr[id3 + 1] + mAr[id4 + 1]);
        mArOut[idCen + 2] = 0.25f * (mAr[id1 + 2] + mAr[id2 + 2] + mAr[id3 + 2] + mAr[id4 + 2]);
        return ivCen;
    }

    bool Subdivision::LINEAR = false;

    void Subdivision::applyEvenSmooth(MeshSPtr baseMesh, Float32Array& even, Float32Array& colorOut, Float32Array& materialOut)
    {
        std::uint32_t nbVerts = baseMesh->getNbVertices();

        colorOut = Utils::subarray(baseMesh->getColors(), 0, nbVerts * 3);
        materialOut = Utils::subarray(baseMesh->getMaterials(), 0, nbVerts * 3);
        //colorOut.set(baseMesh->getColors().subarray(0, nbVerts * 3));
        //materialOut.set(baseMesh->getMaterials().subarray(0, nbVerts * 3));

        Float32Array& vArOld = baseMesh->getVertices();
        Uint32Array& fArOld = baseMesh->getFaces();
        Uint8Array& eArOld = baseMesh->getEdges();
        Uint32Array& feArOld = baseMesh->getFaceEdges();
        Uint8Array& vertOnEdgeOld = baseMesh->getVerticesOnEdge();
        Uint32Array& vrvStartCount = baseMesh->getVerticesRingVertStartCount();
        IUint32ArrayND* vertRingVert = baseMesh->getVerticesRingVert();
        Uint32Array& vrfStartCount = baseMesh->getVerticesRingFaceStartCount();
        IUint32ArrayND* vertRingFace = baseMesh->getVerticesRingFace();
        bool onlyTri = baseMesh->hasOnlyTriangles();

        for (auto i = 0; i < nbVerts; ++i)
        {
            auto j = i * 3;
            float avx = 0.0f;
            float avy = 0.0f;
            float avz = 0.0f;
            float beta = 0.0f;
            float alpha = 0.0f;
            auto k = 0;
            auto id = 0;

            // edge vertex
            if (vertOnEdgeOld[i] || Subdivision::LINEAR)
            {
                std::uint32_t startF = vrfStartCount[i * 2];
                std::uint32_t endF = startF + vrfStartCount[i * 2 + 1];

                for (k = startF; k < endF; ++k)
                {
                    auto idFace = vertRingFace[k] * 4;
                    auto i1 = fArOld[idFace + 0];
                    auto i2 = fArOld[idFace + 1];
                    auto i3 = fArOld[idFace + 2];
                    auto i4 = fArOld[idFace + 3];
                    auto isTri = i4 == Utils::TRI_INDEX;
                    id = Utils::TRI_INDEX;

                    if (i1 == i)
                    {
                        if (eArOld[feArOld[idFace]] == 1)
                            id = i2;
                        else if (eArOld[feArOld[isTri ? idFace + 2 : idFace + 3]] == 1)
                            id = isTri ? i3 : i4;
                    }
                    else if (i2 == i)
                    {
                        if (eArOld[feArOld[idFace]] == 1)
                            id = i1;
                        else if (eArOld[feArOld[idFace + 1]] == 1)
                            id = i3;
                    }
                    else if (i3 == i)
                    {
                        if (eArOld[feArOld[idFace + 1]] == 1)
                            id = i2;
                        else if (eArOld[feArOld[idFace + 2]] == 1)
                            id = isTri ? i1 : i4;
                    }
                    else if (i4 == i)
                    {
                        if (eArOld[feArOld[idFace + 2]] == 1)
                            id = i3;
                        else if (eArOld[feArOld[idFace + 3]] == 1)
                            id = i1;
                    }

                    if (id == Utils::TRI_INDEX)
                        continue;

                    id *= 3;
                    avx += vArOld[id + 0];
                    avy += vArOld[id + 1];
                    avz += vArOld[id + 2];
                    beta++;
                }

                if (beta < 2)
                {
                    // non manifold boring stuffs
                    even[j + 0] = vArOld[j + 0];
                    even[j + 1] = vArOld[j + 1];
                    even[j + 2] = vArOld[j + 2];
                }
                else
                {
                    beta = 0.25 / beta;
                    alpha = 0.75;
                    even[j + 0] = vArOld[j + 0] * alpha + avx * beta;
                    even[j + 1] = vArOld[j + 1] * alpha + avy * beta;
                    even[j + 2] = vArOld[j + 2] * alpha + avz * beta;
                }
                continue;
            }

            std::uint32_t start = vrvStartCount[i * 2 + 0];
            std::uint32_t count = vrvStartCount[i * 2 + 1];
            std::uint32_t end = start + count;
            // interior vertex
            for (k = start; k < end; ++k)
            {
                id = vertRingVert[k] * 3;
                avx += vArOld[id + 0];
                avy += vArOld[id + 1];
                avz += vArOld[id + 2];
            }

            // only vertex tri
            if (onlyTri)
            {
                if (count == 6)
                {
                    beta = 0.0625f;
                    alpha = 0.625f;
                }
                else if (count == 3)
                {
                    // warren weights
                    beta = 0.1875f;
                    alpha = 0.4375f;
                }
                else
                {
                    beta = 0.375f / count;
                    alpha = 0.625f;
                }

                even[j + 0] = vArOld[j + 0] * alpha + avx * beta;
                even[j + 1] = vArOld[j + 1] * alpha + avy * beta;
                even[j + 2] = vArOld[j + 2] * alpha + avz * beta;

                continue;
            }

            float oppx = 0.0f;
            float oppy = 0.0f;
            float oppz = 0.0f;
            float gamma = 0.0f;

        std:uint32_t startFace = vrfStartCount[i * 2];
        std:uint32_t endFace = startFace + vrfStartCount[i * 2 + 1];
        std:uint32_t nbQuad = 0;

            for (k = startFace; k < endFace; ++k)
            {
                id = vertRingFace[k] * 4;
                auto iv4 = fArOld[id + 3];
                if (iv4 == Utils::TRI_INDEX) continue;

                nbQuad++;
                auto iv1 = fArOld[id];
                auto iv2 = fArOld[id + 1];
                auto iv3 = fArOld[id + 2];
                auto ivOpp = 0;
                if (iv1 == i) ivOpp = iv3 * 3;
                else if (iv2 == i) ivOpp = iv4 * 3;
                else if (iv3 == i) ivOpp = iv1 * 3;
                else ivOpp = iv2 * 3;
                oppx += vArOld[ivOpp];
                oppy += vArOld[ivOpp + 1];
                oppz += vArOld[ivOpp + 2];
            }

            // interior vertex quad
            if (nbQuad == (endFace - startFace))
            {
                if (count == 4)
                {
                    alpha = 0.5625f;
                    beta = 0.09375f;
                    gamma = 0.015625f;
                }
                else
                {
                    beta = 1.5f / (count * count);
                    gamma = 0.25f / (count * count);
                    alpha = 1.0f - (beta + gamma) * count;
                }

                even[j + 0] = vArOld[j + 0] * alpha + avx * beta + oppx * gamma;
                even[j + 1] = vArOld[j + 1] * alpha + avy * beta + oppy * gamma;
                even[j + 2] = vArOld[j + 2] * alpha + avz * beta + oppz * gamma;
                continue;
            }

            // interior vertex tri
            if (nbQuad == 0)
            {
                if (count == 6)
                {
                    beta = 0.0625f;
                    alpha = 0.625f;
                }
                else if (count == 3)
                { // warren weights
                    beta = 0.1875f;
                    alpha = 0.4375f;
                }
                else
                {
                    beta = 0.375f / count;
                    alpha = 0.625f;
                }

                even[j + 0] = vArOld[j + 0] * alpha + avx * beta;
                even[j + 1] = vArOld[j + 1] * alpha + avy * beta;
                even[j + 2] = vArOld[j + 2] * alpha + avz * beta;
                continue;
            }

            // interior tri-quad
            alpha = 1.0f / (1.0f + count * 0.5f + nbQuad * 0.25f);
            beta = alpha * 0.5f;
            gamma = alpha * 0.25f;
            even[j + 0] = vArOld[j + 0] * alpha + avx * beta + oppx * gamma;
            even[j + 1] = vArOld[j + 1] * alpha + avy * beta + oppy * gamma;
            even[j + 2] = vArOld[j + 2] * alpha + avz * beta + oppz * gamma;
        }
    }

    /** Odd vertices smoothing */
    Int32Array Subdivision::applyOddSmooth(MeshSPtr mesh, Float32Array* odds, Float32Array* colorOut, Float32Array* materialOut, Uint32Array* fArOut)
    {
        OddVertexComputer oddComputer(mesh, odds, colorOut, materialOut);

        auto fAr = mesh->getFaces();
        auto feAr = mesh->getFaceEdges();
        std::uint32_t len = mesh->getNbFaces();

        for (std::uint32_t i = 0; i < len; ++i)
        {
            std::uint32_t id = i * 4;
            float iv1 = fAr[id + 0];
            float iv2 = fAr[id + 1];
            float iv3 = fAr[id + 2];
            float iv4 = fAr[id + 3];
            bool isQuad = iv4 != Utils::TRI_INDEX;
            std::uint32_t ivMid1, ivMid2, ivMid3, ivMid4, ivCen;

            if (isQuad)
            {
                ivMid1 = oddComputer.computeQuadEdgeVertex(iv1, iv2, iv3, iv4, feAr[id + 0]);
                ivMid2 = oddComputer.computeQuadEdgeVertex(iv2, iv3, iv4, iv1, feAr[id + 1]);
                ivMid3 = oddComputer.computeQuadEdgeVertex(iv3, iv4, iv1, iv2, feAr[id + 2]);
                ivMid4 = oddComputer.computeQuadEdgeVertex(iv4, iv1, iv2, iv3, feAr[id + 3]);
                ivCen = oddComputer.computeFaceVertex(iv1, iv2, iv3, iv4);
            }
            else
            {
                ivMid1 = oddComputer.computeTriangleEdgeVertex(iv1, iv2, iv3, feAr[id + 0]);
                ivMid2 = oddComputer.computeTriangleEdgeVertex(iv2, iv3, iv1, feAr[id + 1]);
                ivMid3 = oddComputer.computeTriangleEdgeVertex(iv3, iv1, iv2, feAr[id + 2]);
            }

            if (fArOut == nullptr)
                continue;

            id *= 4;

            if (isQuad)
            {
                fArOut->at(id + 1) = fArOut->at(id + 4) = ivMid1;
                fArOut->at(id + 6) = fArOut->at(id + 9) = ivMid2;
                fArOut->at(id + 11) = fArOut->at(id + 14) = ivMid3;
                fArOut->at(id + 3) = fArOut->at(id + 12) = ivMid4;
                fArOut->at(id + 2) = fArOut->at(id + 7) = fArOut->at(id + 8) = fArOut->at(id + 13) = ivCen;
                fArOut->at(id + 0) = iv1;
                fArOut->at(id + 5) = iv2;
                fArOut->at(id + 10) = iv3;
                fArOut->at(id + 15) = iv4;
            }
            else
            {
                fArOut->at(id + 0) = fArOut->at(id + 5) = fArOut->at(id + 8) = ivMid1;
                fArOut->at(id + 1) = fArOut->at(id + 10) = fArOut->at(id + 12) = ivMid2;
                fArOut->at(id + 2) = fArOut->at(id + 6) = fArOut->at(id + 14) = ivMid3;
                fArOut->at(id + 3) = fArOut->at(id + 7) = fArOut->at(id + 11) = fArOut->at(id + 15) = Utils::TRI_INDEX;
                fArOut->at(id + 4) = iv1;
                fArOut->at(id + 9) = iv2;
                fArOut->at(id + 13) = iv3;
            }
        }

        return oddComputer.getTagEdges();
    }

    /** Computes uv faces and uv coordinates for center vertices */
    void Subdivision::computeFaceTexCoords(MeshSPtr mesh, MeshSPtr newMesh, Uint32Array& tagEdges)
    {
        Uint32Array& fArUVOld = mesh->getFacesTexCoord();
        Uint32Array& fAr = newMesh->getFaces();
        Uint32Array fArUV(fAr.size(), 0);// = new Uint32Array(fAr.length);
        Uint32Array& feAr = mesh->getFaceEdges();

        std::uint32_t nbVertices = mesh->getNbVertices();
        std::uint32_t offset = newMesh->getNbVertices() - nbVertices;

        Uint32Array& startCount = newMesh->getVerticesDuplicateStartCount();
        Float32Array& uvAr = newMesh->getTexCoords();
        std::uint32_t len = mesh->getNbFaces();

        for (auto i = 0; i < len; ++i)
        {
            std::uint32_t id = i * 4;
            std::uint32_t iuv1 = fArUVOld[id + 0];
            std::uint32_t iuv2 = fArUVOld[id + 1];
            std::uint32_t iuv3 = fArUVOld[id + 2];
            std::uint32_t iuv4 = fArUVOld[id + 3];

            if (iuv1 >= nbVertices)
                iuv1 += offset;
            if (iuv2 >= nbVertices)
                iuv2 += offset;
            if (iuv3 >= nbVertices)
                iuv3 += offset;

            if (iuv4 != Utils::TRI_INDEX && iuv4 >= nbVertices)
                iuv4 += offset;

            std::uint32_t ide = feAr[id];
            std::uint32_t tg1 = tagEdges[ide] - 1;

            if (tg1 < 0)
                tg1 = startCount[(-tg1 - 1) * 2];
            else if (startCount[tg1 * 2] > 0)
                tagEdges[ide] = -tg1;

            ide = feAr[id + 1];
            std::uint32_t tg2 = tagEdges[ide] - 1;

            if (tg2 < 0)
                tg2 = startCount[(-tg2 - 1) * 2];
            else if (startCount[tg2 * 2] > 0)
                tagEdges[ide] = -tg2;

            ide = feAr[id + 2];
            auto tg3 = tagEdges[ide] - 1;
            if (tg3 < 0)
                tg3 = startCount[(-tg3 - 1) * 2];
            else if (startCount[tg3 * 2] > 0)
                tagEdges[ide] = -tg3;

            id *= 4;

            if (iuv4 != Utils::TRI_INDEX)
            {
                ide = feAr[i * 4 + 3];
                auto tg4 = tagEdges[ide] - 1;
                if (tg4 < 0)
                    tg4 = startCount[(-tg4 - 1) * 2];
                else if (startCount[tg4 * 2] > 0)
                    tagEdges[ide] = -tg4;

                fArUV[id + 1] = fArUV[id + 4] = tg1;
                fArUV[id + 6] = fArUV[id + 9] = tg2;
                fArUV[id + 11] = fArUV[id + 14] = tg3;
                fArUV[id + 3] = fArUV[id + 12] = tg4;
                fArUV[id + 2] = fArUV[id + 7] = fArUV[id + 8] = fArUV[id + 13] = fAr[id + 2];
                fArUV[id] = iuv1;
                fArUV[id + 5] = iuv2;
                fArUV[id + 10] = iuv3;
                fArUV[id + 15] = iuv4;

                // even averaging for center quad
                auto im = fAr[id + 2] * 2;
                uvAr[im] = (uvAr[iuv1 * 2] + uvAr[iuv2 * 2] + uvAr[iuv3 * 2] + uvAr[iuv4 * 2]) * 0.25;
                uvAr[im + 1] = (uvAr[iuv1 * 2 + 1] + uvAr[iuv2 * 2 + 1] + uvAr[iuv3 * 2 + 1] + uvAr[iuv4 * 2 + 1]) * 0.25;
            }
            else
            {
                fArUV[id] = fArUV[id + 5] = fArUV[id + 8] = tg1;
                fArUV[id + 1] = fArUV[id + 10] = fArUV[id + 12] = tg2;
                fArUV[id + 2] = fArUV[id + 6] = fArUV[id + 14] = tg3;
                fArUV[id + 3] = fArUV[id + 7] = fArUV[id + 11] = fArUV[id + 15] = Utils::TRI_INDEX;
                fArUV[id + 4] = iuv1;
                fArUV[id + 9] = iuv2;
                fArUV[id + 13] = iuv3;
            }
        }

        newMesh->setFacesTexCoord(fArUV);
    }

    /** Subdivide tex coords mesh */
    void Subdivision::computeTexCoords(MeshSPtr mesh, MeshSPtr newMesh, Int32Array& tagEdges)
    {
        std::uint32_t  newNbVertices = newMesh->getNbVertices();
        Uint32Array startCount(newNbVertices * 2);// = new Uint32Array(newNbVertices * 2);
        //startCount.set(mesh->getVerticesDuplicateStartCount());
        Utils::set(startCount, mesh->getVerticesDuplicateStartCount(), 0);

        Uint32Array& fArOld = mesh->getFaces();
        Uint32Array& fArUVOld = mesh->getFacesTexCoord();

        std::uint32_t bound = newMesh->getNbFaces() * 3;
        Float32Array& uvArOld = mesh->getTexCoords();
        Float32Array uvAr(bound * 2, 0.0f);//auto uvAr = new Float32Array(Utils.getMemory(bound * 4 * 2), 0, bound * 2);

        std::uint32_t i = 0;
        std::uint32_t len = mesh->getNbVertices();
        std::uint32_t offset = newNbVertices - len;

        // reorder even duplicates vertex indices
        for (i = 0; i < len; ++i)
        {
            auto start = startCount[i * 2];
            if (start > 0)
                startCount[i * 2] = start + offset;
        }
        uvAr.set(uvArOld);
        uvAr.set(uvArOld.subarray(len * 2), (len + offset) * 2);
        auto nbTexCoords = mesh.getNbTexCoords();

        auto acc = offset + nbTexCoords;
        auto feAr = mesh.getFaceEdges();

        // compute uv for new odd vertices
        auto tagUVMin = new Uint32Array(tagEdges.size());
        auto tagUVMax = new Uint32Array(tagEdges.size());
        len = fArOld.length;
        for (i = 0; i < len; ++i) {
            auto ide = feAr[i];
            if (ide == Utils::TRI_INDEX)
                continue;

            auto iNext = (i + 1) % 4 == 0 ? i - 3 : i + 1;
            auto iuv1 = fArUVOld[i];
            auto iuv2 = fArUVOld[iNext];
            if (iuv2 == Utils::TRI_INDEX)
                iuv2 = fArUVOld[i - 2];
            auto tg = tagEdges[ide] - 1;

            // test if we already processed this edge
            auto tgMax = tagUVMax[ide];
            auto iuvMax = iuv1 > iuv2 ? iuv1 : iuv2;
            auto iuvMin = iuv1 < iuv2 ? iuv1 : iuv2;
            if (tgMax != 0) {
                // test if we already processed this UV edge or if it's a duplicate
                if (tgMax != iuvMax || tagUVMin[ide] != iuvMin) {
                    uvAr[acc * 2] = (uvArOld[iuv1 * 2] + uvArOld[iuv2 * 2]) * 0.5;
                    uvAr[acc * 2 + 1] = (uvArOld[iuv1 * 2 + 1] + uvArOld[iuv2 * 2 + 1]) * 0.5;
                    startCount[tg * 2] = acc++;
                    startCount[tg * 2 + 1] = 1;
                }
            }
            else {
                // first time we process this edge
                uvAr[tg * 2] = (uvArOld[iuv1 * 2] + uvArOld[iuv2 * 2]) * 0.5;
                uvAr[tg * 2 + 1] = (uvArOld[iuv1 * 2 + 1] + uvArOld[iuv2 * 2 + 1]) * 0.5;
                tagUVMin[ide] = iuvMin;
                tagUVMax[ide] = iuvMax;
            }
        }
        auto texCoords = new Float32Array(acc * 2);
        texCoords.set(uvAr.subarray(0, acc * 2));

        newMesh->setTexCoords(texCoords);
        newMesh->setVerticesDuplicateStartCount(startCount);

        computeFaceTexCoords(mesh, newMesh, tagEdges);
    }

    /** Apply a complete subdivision (by updating the topology) */
    void Subdivision::fullSubdivision(MeshSPtr baseMesh, MeshSPtr newMesh)
    {
        auto nbVertices = baseMesh->getNbVertices() + baseMesh->getNbEdges() + baseMesh->getNbQuads();
        newMesh->setVertices(Float32Array(nbVertices * 3));
        newMesh->setColors(Float32Array(nbVertices * 3));
        newMesh->setMaterials(Float32Array(nbVertices * 3));
        newMesh->setFaces(Uint32Array(baseMesh->getNbFaces() * 4 * 4));
        applyEvenSmooth(baseMesh, newMesh->getVertices(), newMesh->getColors(), newMesh->getMaterials());
        auto tags = applyOddSmooth(baseMesh, &(newMesh->getVertices()), &(newMesh->getColors()), &(newMesh->getMaterials()), &(newMesh->getFaces()));
        if (baseMesh->hasUV())
            computeTexCoords(baseMesh, newMesh, tags);
        newMesh->allocateArrays();
    }

    /** Apply subdivision without topology computation */
    void Subdivision::partialSubdivision(MeshSPtr baseMesh, Float32Array& vertOut, Float32Array& colorOut, Float32Array& materialOut)
    {
        applyEvenSmooth(baseMesh, vertOut, colorOut, materialOut);
        applyOddSmooth(baseMesh, &vertOut, &colorOut, &materialOut);
    }
}