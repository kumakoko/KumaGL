#ifndef digital_sculpt_reversion_h__
#define digital_sculpt_reversion_h__

#include "mesh/mesh.h"
#include "misc/utils.h"
#include "misc/array_nd.h"

namespace DigitalSculpt
{
    class Reversion
    {
    public:

        static Int8Array detectExtraordinaryVertices(MeshSPtr mesh)
        {
            std::uint32_t nbVertices = mesh->getNbVertices();
            Uint32Array& fAr = mesh->getFaces();
            Uint8Array& onEdge = mesh->getVerticesOnEdge();
            Uint32Array& vrvStartCount = mesh->getVerticesRingVertStartCount();
            IUint32ArrayND* vrf = mesh->getVerticesRingFace();
            Uint32Array& vrfStartCount = mesh->getVerticesRingFaceStartCount();
            Int8Array vExtraTags(nbVertices, 0);// = new Int8Array(nbVertices);

            for (auto i = 0; i < nbVertices; ++i)
            {
                auto id = i * 2;
                auto len = vrvStartCount[id + 1];
                auto startFace = vrfStartCount[id];
                auto countFace = vrfStartCount[id + 1];
                auto vBorder = onEdge[i];
                auto nbQuad = 0;

                for (auto j = startFace, endFace = startFace + countFace; j < endFace; ++j)
                {
                    nbQuad += fAr[vrf[j] * 4 + 3] == Utils::TRI_INDEX ? 0 : 1;
                }

                if (nbQuad == 0)
                {
                    // tris
                    if ((!vBorder && len != 6) || (vBorder && len != 4))
                        vExtraTags[i] = 1;
                }
                else if (nbQuad == countFace)
                {
                    // quads
                    if ((!vBorder && len != 4) || (vBorder && len != 3))
                        vExtraTags[i] = 1;
                }
                else
                {
                    // quad and tri
                    if (vBorder || len != 5)
                        vExtraTags[i] = 1;
                }
            }
            return vExtraTags;
        };

        /** Return the first extraordinary vertex if it exists... or a random vertex otherwise */
        static std::uint32_t getSeed(MeshSPtr mesh, vEvenTags, vExtraTags)
        {
            auto i = 0;
            auto nbVertices = mesh->getNbVertices();

            for (i = 0; i < nbVertices; ++i)
            {
                if (vEvenTags[i] != 0)
                    continue;
                if (vExtraTags[i] == 1)
                    return i;
            }

            // no extraordinary vertices...
            Uint8Array& onEdge = mesh->getVerticesOnEdge();
            for (i = 0; i < nbVertices; ++i)
            {
                if (vEvenTags[i] != 0)
                    continue; // skip already processed vertex

                if (onEdge[i] != 1)
                    break;
            }

            // cancels reversion if there is no edge vertex
            if (i == nbVertices)
                return -1;

            // find the first non-already processed vertex

            for (i = 0; i < nbVertices; ++i)
            {
                if (vEvenTags[i] == 0)
                    return i;
            }

            return -1;
        };

        /** Tag the even vertices */
        static bool tagVertices(MeshSPtr mesh, vExtraTags, vEvenTags)
        {
            int tagFlag = ++Utils::TAG_FLAG;
            Int32Array& vFlags = mesh->getVerticesTagFlags();
            Uint32Array& vrvSC = mesh->getVerticesRingVertStartCount();
            IUint32ArrayND* vrv = mesh->getVerticesRingVert();
            Uint8Array& onEdge = mesh->getVerticesOnEdge();

            std::uint32_t vSeed = getSeed(mesh, vEvenTags, vExtraTags);
            if (vSeed < 0)
                return false;

            vEvenTags[vSeed] = 1;
            Uint32Array stack(mesh->getNbVertices(), 0);//auto stack = new Uint32Array(Utils.getMemory(mesh->getNbVertices() * 4), 0, mesh->getNbVertices());
            stack[0] = vSeed;
            std::uint32_t curStack = 1;

            while (curStack > 0)
            {
                std::uint32_t idVert = stack[--curStack];
                std::uint32_t start = vrvSC[idVert * 2];
                std::uint32_t end = start + vrvSC[idVert * 2 + 1];
                std::uint32_t i = 0;
                std::int32_t stamp = ++tagFlag;

                // tag the odd vertices
                for (i = start; i < end; ++i)
                {
                    auto oddi = vrv[i];
                    vFlags[oddi] = stamp;

                    // already an even vertex
                    if (vEvenTags[oddi] == 1)
                    {
                        Utils::TAG_FLAG = tagFlag;
                        return false;
                    }

                    vEvenTags[oddi] = -1; //odd vertex
                    vFlags[oddi] = stamp;
                }

                // stamp-1 means odd vertex, while stamp ==> locally already
                // visited candidates opposites even vertex
                stamp = ++tagFlag;

                for (i = start; i < end; ++i)
                {
                    auto oddId = vrv[i];
                    // extraordinary vertex marked as odd vertex
                    if (vExtraTags[oddId] != 0 && !onEdge[oddId]) {
                        Utils::TAG_FLAG = tagFlag;
                        return false;
                    }
                    auto oddStart = vrvSC[oddId * 2];
                    auto oddEnd = oddStart + vrvSC[oddId * 2 + 1];
                    // find opposite vertex
                    for (auto j = oddStart; j < oddEnd; ++j) {
                        auto evenj = vrv[j];
                        if (evenj == idVert)
                            continue;
                        if (vFlags[evenj] >= (stamp - 1)) // see comments above
                            continue;
                        vFlags[evenj] = stamp;
                        if (vEvenTags[evenj] != 0) // already processed
                            continue;
                        auto oppStart = vrvSC[evenj * 2];
                        auto oppEnd = oppStart + vrvSC[evenj * 2 + 1];
                        auto nbOdd = 0;
                        for (auto k = oppStart; k < oppEnd; ++k) {
                            if (vFlags[vrv[k]] == (stamp - 1))
                                nbOdd++;
                        }
                        if (nbOdd == 2) {
                            vEvenTags[evenj] = -1;
                        }
                        else {
                            vEvenTags[evenj] = 1;
                            stack[curStack++] = evenj;
                        }
                    }
                }
            }
            Utils::TAG_FLAG = tagFlag;
            return true;
        };

        /** Tag the even vertices */
        static bool tagEvenVertices(MeshSPtr mesh, vEvenTags)
        {
            std::uint32_t  nbVertices = mesh->getNbVertices();
            Int8Array  vExtraTags = detectExtraordinaryVertices(mesh);
            bool running = true;

            while (running)
            {
                if (!tagVertices(mesh, vExtraTags, vEvenTags))
                    return false;

                running = false;

                for (auto i = 0; i < nbVertices; ++i)
                {
                    if (vEvenTags[i] == 0)
                    {
                        running = true;
                        break;
                    }
                }
            }

            return true;
        }

        /** Creates the coarse faces from the tagged vertices */
        bool createFaces(MeshSPtr baseMesh, MeshSPtr newMesh, vEvenTags, triFaceOrQuadCenter)
        {
            Uint32Array& feAr = baseMesh->getFaceEdges();
            Uint32Array& fArUp = baseMesh->getFaces();
            Int32Array tagEdges(baseMesh->getNbEdges(), 0);// auto tagEdges = new Int32Array(baseMesh->getNbEdges());
            auto i = 0;
            std::uint32_t  nbFaces = baseMesh->getNbFaces();
            std::uint32_t  acc = 0;
            Uint32Array centerQuadUp(baseMesh->getNbVertices(), 0);// = new Uint32Array(baseMesh->getNbVertices());

            Uint32Array fArDown(nbFaces, 0.0f);// auto fArDown = new Uint32Array(nbFaces);

            for (i = 0; i < nbFaces; ++i)
                fArDown[i] = Utils::TRI_INDEX;

            for (i = 0; i < nbFaces; ++i)
            {
                std::uint32_t j = i * 4;
                std::uint32_t iv1 = fArUp[j + 0];
                std::uint32_t iv2 = fArUp[j + 1];
                std::uint32_t iv3 = fArUp[j + 2];
                std::uint32_t iv4 = fArUp[j + 3];

                auto tag1 = vEvenTags[iv1];
                auto tag2 = vEvenTags[iv2];
                auto tag3 = vEvenTags[iv3];

                if (iv4 == Utils::TRI_INDEX)
                {
                    // center tri
                    if (tag1 + tag2 + tag3 == -3)
                    {
                        triFaceOrQuadCenter[acc++] = i;
                        continue;
                    }
                    // tri
                    if (tag1 == 1)
                        tagEdges[feAr[j + 1]] = iv1 + 1;
                    else if (tag2 == 1)
                        tagEdges[feAr[j + 2]] = iv2 + 1;
                    else if (tag3 == 1)
                        tagEdges[feAr[j + 0]] = iv3 + 1;
                    continue;
                }

                //quad
                auto ivCorner = 0;
                auto ivCenter = 0;
                auto oppEdge = 0;

                if (tag1 == 1)
                {
                    ivCorner = iv1;
                    ivCenter = iv3;
                    oppEdge = tagEdges[feAr[j + 1]] - 1;
                    tagEdges[feAr[j + 2]] = iv1 + 1;
                }
                else if (tag2 == 1)
                {
                    ivCorner = iv2;
                    ivCenter = iv4;
                    oppEdge = tagEdges[feAr[j + 2]] - 1;
                    tagEdges[feAr[j + 3]] = iv2 + 1;
                }
                else if (tag3 == 1)
                {
                    ivCorner = iv3;
                    ivCenter = iv1;
                    oppEdge = tagEdges[feAr[j + 3]] - 1;
                    tagEdges[feAr[j]] = iv3 + 1;
                }
                else
                {
                    ivCorner = iv4;
                    ivCenter = iv2;
                    oppEdge = tagEdges[feAr[j]] - 1;
                    tagEdges[feAr[j + 1]] = iv4 + 1;
                }

                auto quad = centerQuadUp[ivCenter] - 1;

                if (quad < 0)
                {
                    triFaceOrQuadCenter[acc] = -ivCenter - 1;
                    fArDown[acc * 4 + 3] = ivCorner;
                    centerQuadUp[ivCenter] = ++acc;
                    continue;
                }

                auto idQuad = quad * 4;

                if (oppEdge < 0)
                {
                    // no opposite edge
                    if (fArDown[idQuad + 2] >= (Utils::TRI_INDEX - 1)) {
                        fArDown[idQuad + 2] = ivCorner;
                        fArDown[idQuad] = Utils::TRI_INDEX - 1;
                    }
                    else if (fArDown[idQuad] == Utils::TRI_INDEX) {
                        fArDown[idQuad + 1] = ivCorner;
                    }
                    else {
                        fArDown[idQuad + 1] = fArDown[idQuad + 2];
                        fArDown[idQuad + 2] = ivCorner;
                    }
                }
                else
                {
                    // insert after oppEdge
                    if (fArDown[idQuad + 1] == oppEdge)
                    {
                        fArDown[idQuad] = ivCorner;
                    }
                    else
                    {
                        fArDown[idQuad] = fArDown[idQuad + 1];

                        if (fArDown[idQuad + 2] == oppEdge)
                        {
                            fArDown[idQuad + 1] = ivCorner;
                        }
                        else
                        {
                            fArDown[idQuad + 1] = fArDown[idQuad + 2];
                            fArDown[idQuad + 2] = ivCorner;
                        }
                    }
                }
            }

            nbFaces /= 4;

            for (i = 0; i < nbFaces; ++i)
            {
                auto cen = triFaceOrQuadCenter[i];
                auto idFace = i * 4;

                if (cen < 0)
                { // quad
                    // Sometimes... the way we revert quads does not always work
                    // because of non-consistency clock wise order between neighbor quads
                    auto cmp = Utils::TRI_INDEX - 1;
                    if (fArDown[idFace] >= cmp || fArDown[idFace + 1] >= Utils::TRI_INDEX || fArDown[idFace + 2] >= Utils::TRI_INDEX)
                        return false;
                    continue;
                }

                // tri
                auto id = cen * 4;
                fArDown[idFace + 0] = tagEdges[feAr[id + 0]] - 1;
                fArDown[idFace + 1] = tagEdges[feAr[id + 1]] - 1;
                fArDown[idFace + 2] = tagEdges[feAr[id + 2]] - 1;
            }

            newMesh->setFaces(fArDown);
            return true;
        };

        /** Creates the vertices of the mesh */
        static void createVertices(MeshSPtr baseMesh, MeshSPtr newMesh, triFaceOrQuadCenter)
        {
            auto acc = 0;
            Uint32Array vertexMapUp(baseMesh->getNbVertices()); //auto vertexMapUp = new Uint32Array(baseMesh->getNbVertices());
            newMesh->setVerticesMapping(vertexMapUp);
            auto fArDown = newMesh->getFaces();
            auto tagVert = new Int32Array(baseMesh->getNbVertices());
            auto i = 0;
            auto len = newMesh->getNbFaces() * 4;

            for (i = 0; i < len; ++i) {
                auto iv = fArDown[i];
                if (iv == Utils::TRI_INDEX)
                    continue;

                auto tag = tagVert[iv] - 1;
                if (tag == -1) {
                    tag = acc++;
                    tagVert[iv] = tag + 1;
                    vertexMapUp[tag] = iv;
                }
                fArDown[i] = tag;
            }

            newMesh->setVertices(new Float32Array(acc * 3));
            auto fArUp = baseMesh->getFaces();
            auto vrf = baseMesh->getVerticesRingFace();
            auto vrfStartCount = baseMesh->getVerticesRingFaceStartCount();
            auto tagMid = new Uint8Array(baseMesh->getNbVertices());
            len /= 4;
            for (i = 0; i < len; ++i) {
                auto iCenter = triFaceOrQuadCenter[i];
                auto mid1, mid2, mid3, mid4, mid5;
                auto tag1, tag2, tag3, tag4, tag5;
                if (iCenter >= 0) {
                    // tri
                    auto id = iCenter * 4;
                    mid1 = fArUp[id + 1];
                    mid2 = fArUp[id + 2];
                    mid3 = fArUp[id];
                    mid4 = Utils::TRI_INDEX;
                    mid5 = Utils::TRI_INDEX;
                }
                else {
                    // quad
                    mid5 = -iCenter - 1;
                    auto idQuadDown = i * 4;
                    auto corner1 = vertexMapUp[fArDown[idQuadDown + 0]];
                    auto corner2 = vertexMapUp[fArDown[idQuadDown + 1]];
                    auto corner3 = vertexMapUp[fArDown[idQuadDown + 2]];
                    auto corner4 = vertexMapUp[fArDown[idQuadDown + 3]];
                    auto start = vrfStartCount[mid5 * 2];
                    auto end = start + 4;
                    for (auto j = start; j < end; ++j) {
                        auto idQuad = vrf[j] * 4;
                        auto id1 = fArUp[idQuad];
                        auto id2 = fArUp[idQuad + 1];
                        auto id3 = fArUp[idQuad + 2];
                        auto id4 = fArUp[idQuad + 3];
                        if (id1 == corner1) mid1 = id2;
                        else if (id2 == corner1) mid1 = id3;
                        else if (id3 == corner1) mid1 = id4;
                        else if (id4 == corner1) mid1 = id1;

                        if (id1 == corner2) mid2 = id2;
                        else if (id2 == corner2) mid2 = id3;
                        else if (id3 == corner2) mid2 = id4;
                        else if (id4 == corner2) mid2 = id1;

                        if (id1 == corner3) mid3 = id2;
                        else if (id2 == corner3) mid3 = id3;
                        else if (id3 == corner3) mid3 = id4;
                        else if (id4 == corner3) mid3 = id1;

                        if (id1 == corner4) mid4 = id2;
                        else if (id2 == corner4) mid4 = id3;
                        else if (id3 == corner4) mid4 = id4;
                        else if (id4 == corner4) mid4 = id1;
                    }
                }

                tag1 = tagMid[mid1];
                tag2 = tagMid[mid2];
                tag3 = tagMid[mid3];
                tag4 = mid4 != Utils::TRI_INDEX ? tagMid[mid4] : -1;
                tag5 = mid5 != Utils::TRI_INDEX ? tagMid[mid5] : -1;
                if (tag1 == 0) {
                    tagMid[mid1] = 1;
                    vertexMapUp[acc++] = mid1;
                }
                if (tag2 == 0) {
                    tagMid[mid2] = 1;
                    vertexMapUp[acc++] = mid2;
                }
                if (tag3 == 0) {
                    tagMid[mid3] = 1;
                    vertexMapUp[acc++] = mid3;
                }
                if (tag4 == 0) {
                    tagMid[mid4] = 1;
                    vertexMapUp[acc++] = mid4;
                }
                if (tag5 == 0) {
                    tagMid[mid5] = 1;
                    vertexMapUp[acc++] = mid5;
                }
            }
        };

        /** Copy the vertices data from up to low */
        auto copyVerticesData = function(baseMesh, newMesh) {
            auto vArUp = baseMesh->getVertices();
            auto cArUp = baseMesh->getColors();
            auto mArUp = baseMesh->getMaterials();
            auto vArDown = newMesh->getVertices();
            auto cArDown = new Float32Array(vArDown.length);
            auto mArDown = new Float32Array(vArDown.length);
            newMesh->setColors(cArDown);
            newMesh->setMaterials(mArDown);
            auto vertexMapUp = newMesh->getVerticesMapping();
            auto i = 0;
            auto nbVertices = newMesh->getNbVertices();
            for (i = 0; i < nbVertices; ++i) {
                if (vertexMapUp[i] >= nbVertices)
                    break;
            }

            if (i == nbVertices) {
                // we don't have to keep the vertex mapping
                auto fArDown = newMesh->getFaces();
                auto nb = fArDown.length;
                for (i = 0; i < nb; ++i) {
                    auto idv = fArDown[i];
                    if (idv != Utils::TRI_INDEX)
                        fArDown[i] = vertexMapUp[idv];
                }

                // direct mapping for even vertices
                for (i = 0; i < nbVertices; ++i)
                    vertexMapUp[i] = i;
                vArDown.set(vArUp.subarray(0, nbVertices * 3));
                cArDown.set(cArUp.subarray(0, nbVertices * 3));
                mArDown.set(mArUp.subarray(0, nbVertices * 3));
            }
            else {
                // we keep the vertex mapping
                newMesh->setEvenMapping(true);
                for (i = 0; i < nbVertices; ++i) {
                    auto id = i * 3;
                    auto idUp = vertexMapUp[i] * 3;
                    vArDown[id] = vArUp[idUp];
                    vArDown[id + 1] = vArUp[idUp + 1];
                    vArDown[id + 2] = vArUp[idUp + 2];
                    cArDown[id] = cArUp[idUp];
                    cArDown[id + 1] = cArUp[idUp + 1];
                    cArDown[id + 2] = cArUp[idUp + 2];
                    mArDown[id] = mArUp[idUp];
                    mArDown[id + 1] = mArUp[idUp + 1];
                    mArDown[id + 2] = mArUp[idUp + 2];
                }
            }
        };

        /** Computes uv faces and uv coordinates for center vertices */
        auto computeFaceTexCoords = function(baseMesh, newMesh, triFaceOrQuadCenter, uvMap) {
            auto fArUp = baseMesh->getFaces();
            auto fArUVUp = baseMesh->getFacesTexCoord();
            auto vrfSC = baseMesh->getVerticesRingFaceStartCount();
            auto vrf = baseMesh->getVerticesRingFace();
            auto dupUp = baseMesh->getVerticesDuplicateStartCount();

            auto fAr = newMesh->getFaces();
            auto fArUV = new Uint32Array(fAr);
            auto vertexMapUp = newMesh->getVerticesMapping();

            for (auto i = 0, len = fAr.length; i < len; ++i) {
                auto iv = fAr[i];
                if (iv == Utils::TRI_INDEX)
                    continue;

                auto ivUp = vertexMapUp[iv];
                if (dupUp[ivUp * 2] == 0)
                    continue;

                // vertex with duplicates
                auto index = i % 4;
                auto iCen = triFaceOrQuadCenter[(i - index) / 4];
                auto vertUV = Utils::TRI_INDEX;
                if (iCen >= 0) {
                    // tri
                    auto idCen = iCen * 4;
                    auto mid1, mid2;
                    if (index == 0) {
                        mid1 = fArUp[idCen + 1];
                        mid2 = fArUp[idCen];
                    }
                    else if (index == 1) {
                        mid1 = fArUp[idCen + 2];
                        mid2 = fArUp[idCen + 1];
                    }
                    else {
                        mid1 = fArUp[idCen];
                        mid2 = fArUp[idCen + 2];
                    }
                    auto startTri = vrfSC[ivUp * 2];
                    auto endTri = startTri + vrfSC[ivUp * 2 + 1];
                    for (auto idt = startTri; idt < endTri; ++idt) {
                        auto idTri = vrf[idt] * 4;
                        auto idMid1 = fArUp[idTri];
                        auto idMid2 = fArUp[idTri + 1];
                        auto idMid3 = fArUp[idTri + 2];
                        if (idMid1 == mid1) {
                            if (idMid2 == mid2) vertUV = fArUVUp[idTri + 2];
                        }
                        else if (idMid2 == mid1) {
                            if (idMid3 == mid2) vertUV = fArUVUp[idTri];
                        }
                        else if (idMid3 == mid1) {
                            if (idMid1 == mid2) vertUV = fArUVUp[idTri + 1];
                        }
                        if (vertUV != Utils::TRI_INDEX) break;
                    }
                }
                else {
                    // quad
                    iCen = -iCen - 1;
                    auto startQuad = vrfSC[iCen * 2];
                    auto endQuad = startQuad + 4;
                    for (auto idq = startQuad; idq < endQuad; ++idq) {
                        auto idQuad = vrf[idq] * 4;
                        if (fArUp[idQuad] == ivUp) vertUV = fArUVUp[idQuad];
                        else if (fArUp[idQuad + 1] == ivUp) vertUV = fArUVUp[idQuad + 1];
                        else if (fArUp[idQuad + 2] == ivUp) vertUV = fArUVUp[idQuad + 2];
                        else if (fArUp[idQuad + 3] == ivUp) vertUV = fArUVUp[idQuad + 3];
                        if (vertUV != Utils::TRI_INDEX) break;
                    }
                }
                fArUV[i] = vertUV == ivUp ? vertUV : vertUV - uvMap[iv];
            }

            newMesh->setFacesTexCoord(fArUV);
        };

        /** Apply the reverse of a subdivision for the texCoord mesh */
        auto computeTexCoords = function(baseMesh, newMesh, triFaceOrQuadCenter) {
            auto dupUp = baseMesh->getVerticesDuplicateStartCount();

            auto nbVertices = newMesh->getNbVertices();
            auto dup = new Uint32Array(nbVertices * 2);
            auto vertexMapUp = newMesh->getVerticesMapping();

            auto uvArUp = baseMesh->getTexCoords();
            auto uvAr = new Float32Array(Utils.getMemory(baseMesh->getNbTexCoords() * 4 * 2), 0, baseMesh->getNbTexCoords() * 2);
            auto uvMap = new Uint32Array(nbVertices);
            auto nbTexCoords = nbVertices;
            for (auto i = 0; i < nbVertices; ++i) {
                auto ivUp = vertexMapUp[i];
                auto start = dupUp[ivUp * 2];
                uvAr[i * 2] = uvArUp[ivUp * 2];
                uvAr[i * 2 + 1] = uvArUp[ivUp * 2 + 1];
                if (start == 0)
                    continue;
                // vertex with duplicates
                auto startOld = uvMap[i] = start - nbTexCoords;
                auto nbDupl = dupUp[ivUp * 2 + 1];
                for (auto j = nbTexCoords, end = nbTexCoords + nbDupl; j < end; ++j) {
                    uvAr[j * 2] = uvArUp[(startOld + j) * 2];
                    uvAr[j * 2 + 1] = uvArUp[(startOld + j) * 2 + 1];
                }
                dup[i * 2] = nbTexCoords;
                dup[i * 2 + 1] = nbDupl;
                nbTexCoords += nbDupl;
            }
            newMesh->setTexCoords(new Float32Array(uvAr.subarray(0, nbTexCoords * 2)));
            newMesh->setVerticesDuplicateStartCount(dup);

            computeFaceTexCoords(baseMesh, newMesh, triFaceOrQuadCenter, uvMap);
        };

        /** Apply the reverse of a subdivision */
        Reversion.computeReverse = function(baseMesh, newMesh) {
            auto nbFaces = baseMesh->getNbFaces();
            if (nbFaces % 4 != 0)
                return false;

            // 0 not processed, -1 odd vertex, 1 even vertex
            auto vEvenTags = new Int8Array(baseMesh->getNbVertices());
            if (!tagEvenVertices(baseMesh, vEvenTags))
                return false;

            auto triFaceOrQuadCenter = new Int32Array(nbFaces / 4);
            if (!createFaces(baseMesh, newMesh, vEvenTags, triFaceOrQuadCenter))
                return false;

            createVertices(baseMesh, newMesh, triFaceOrQuadCenter);
            copyVerticesData(baseMesh, newMesh);

            if (baseMesh->hasUV())
                computeTexCoords(baseMesh, newMesh, triFaceOrQuadCenter);

            newMesh->allocateArrays();
            return true;
        };

    }
#endif // digital_sculpt_reversion_h__