#include <utility>
#include <iterator>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "mesh/mesh.h"
#include "render/buffer.h"
#include "misc/array_2d.h"
#include "math3d/octree_cell.h"

namespace DigitalSculpt
{
    std::uint32_t Mesh::getRenderNbVertices() 
    {
        if (isUsingDrawArrays())
            return getCount();
        
        return isUsingTexCoords() ? getNbTexCoords() : getNbVertices();
    }

    void Mesh::updateVertexBuffer()
    {
        const Float32Array& vertices = isUsingDrawArrays() ? getVerticesDrawArrays() : getVertices();
        getVertexBuffer()->update(vertices, getRenderNbVertices() * 3);
    }

    void Mesh::updateNormalBuffer() 
    {
        const Float32Array& normals = isUsingDrawArrays() ? getNormalsDrawArrays() : getNormals();
        getNormalBuffer()->update(normals, getRenderNbVertices() * 3);
    }

    void Mesh::updateColorBuffer()
    {
        const Float32Array& colors = isUsingDrawArrays() ? getColorsDrawArrays() : getColors();
        getColorBuffer()->update(colors, getRenderNbVertices() * 3);
    }

    void Mesh::updateMaterialBuffer() 
    {
        const Float32Array& materials = isUsingDrawArrays() ? getMaterialsDrawArrays() : getMaterials();
        getMaterialBuffer()->update(materials, getRenderNbVertices() * 3);
    }

    void Mesh::updateTexCoordBuffer() 
    {
        if (isUsingTexCoords())
        {
            const Float32Array& texCoords = isUsingDrawArrays() ? getTexCoordsDrawArrays() : getTexCoords();
            getTexCoordBuffer()->update(texCoords, getRenderNbVertices() * 2);
        }
    }

    void Mesh::updateIndexBuffer()
    {
        if (!isUsingDrawArrays())
        {
            const Uint32Array& triangles = isUsingTexCoords() ? getTrianglesTexCoord() : getTriangles();
            getIndexBuffer()->update(triangles, getNbTriangles() * 3);
        }
    }

    void Mesh::updateWireframeBuffer() 
    {
        if (getShowWireframe())
            getWireframeBuffer()->update(getWireframe(), getNbEdges() * 2);
    }

    void Mesh::updateGeometryBuffers()
    {
        updateVertexBuffer();
        updateNormalBuffer();
    }

    void Mesh::updateBuffers()
    {
        updateGeometryBuffers();
        updateColorBuffer();
        updateMaterialBuffer();
        updateTexCoordBuffer();
        updateIndexBuffer();
        updateWireframeBuffer();
    }

    void Mesh::release()
    {
        _renderData->_texture0.reset();
        _renderData->_vertexBuffer->release();
        _renderData->_vertexBuffer->release(); //getNormalBuffer().release();
        _renderData->_colorBuffer->release(); //getColorBuffer().release();
        _renderData->_materialBuffer->release(); //getMaterialBuffer().release();
        _renderData->_indexBuffer->release(); //getIndexBuffer().release();
        _renderData->_wireframeBuffer->release(); //getWireframeBuffer().release();
    }

    void Mesh::copyRenderConfig(Mesh* mesh) 
    {
        setFlatShading(mesh->getFlatShading());
        setShowWireframe(mesh->getShowWireframe());
        setShaderType(mesh->getShaderType());
        setMatcap(mesh->getMatcap());
        setTexture0(mesh->getTexture0());
        setCurvature(mesh->getCurvature());
        setOpacity(mesh->getOpacity());
    }

    void Mesh::copyTransformData(Mesh* mesh)
    {
        _transformData->_center = mesh->getCenter();// vec3.copy(getCenter(), mesh->getCenter());
        _transformData->_matrix = mesh->getMatrix();// mat4.copy(getMatrix(), mesh->getMatrix());
        _transformData->_editMatrix = mesh->getEditMatrix();//mat4.copy(getEditMatrix(), mesh->getEditMatrix());
        _transformData->_symmetryNormal = mesh->getSymmetryNormal();// vec3.copy(getSymmetryNormal(), mesh->getSymmetryNormal());
    }

    void Mesh::copyData(Mesh* mesh)
    {
        // https://cloud.tencent.com/developer/section/1191555
        /*
        arraybuffer.slice(begin[, end])
        复制
        参数begin: 从零开始的字节索引，切片从这开始。
        参数end:  结束切片的字节索引。如果没指定end，新的ArrayBuffer将包含这个ArrayBuffer
                 从头到尾的所有字节。由begin和end指定的这个范围夹在当前数组的有效索引范围内。
                 如果新ArrayBuffer的长度在计算后为负，它将强制为0 。
        返回值:   一个新的ArrayBuffer对象。
        描述:     slice方法复制到但不包括由end参数指示的字节。如果begin或end是负数，则指的
                 是从数组末尾开始的索引，而不是从头开始。
        */
        setVertices(mesh->getVertices()/*.slice()*/);
        setFaces(mesh->getFaces()/*.slice()*/);
        setColors(mesh->getColors()/*.slice()*/);
        setMaterials(mesh->getMaterials()/*.slice()*/);
        
        if (mesh->hasUV()) 
        {
            initTexCoordsDataFromOBJData(mesh->getTexCoords(), mesh->getFacesTexCoord());
        }

        init();
        initRender();

        copyTransformData(mesh);
        copyRenderConfig(mesh);
    }

    void Mesh::optimize()
    {
        if (getNbFaces() == 0 || !Mesh::OPTIMIZE)
            return;

        // lower is better :
        // ACTVR : ~1 is optimal (soup or not)
        // ACMR : ~0.5 optimal ratio, 3 for triangle soup
        // var data = computeCacheScore();

        optimizePostTransform();
        optimizePreTransform();
        initFaceRings();

        // console.log('\nbefore ACMR ' + data.ACMR);
        // console.log('before ACTVR ' + data.ACTVR);
        // data = computeCacheScore();
        // console.log('after ACMR ' + data.ACMR);
        // console.log('after ACTVR ' + data.ACTVR);
    }

    CacheStoreInfo Mesh::computeCacheScore() 
    {
        const Uint32Array& fAr = getFaces();
        std::uint32_t nbFaces = getNbFaces();

        std::uint32_t sizeCache = 32;
        Uint32Array cache(sizeCache,-1);//var cache = [];
        //cache.length = sizeCache;

        float cacheMiss = 0;
        std::uint32_t  k = 0;
        
        for (std::uint32_t  i = 0; i < nbFaces; ++i)
        {
            std::uint32_t  id = i * 3;
            std::uint32_t  nbPoly = fAr[id + 3] != Utils::TRI_INDEX ? 4 : 3;

            // check in cache
            for (std::uint32_t  j = 0; j < nbPoly; ++j) 
            {

                std::uint32_t  idFace = fAr[id + j];
                
                // check in cache
                for (k = 0; k < sizeCache; ++k) 
                {
                    if (cache[k] == idFace)
                    {
                        break;
                    }
                    else if (cache[k] == -1)
                    {
                        cache[k] = idFace;
                        cacheMiss++;
                        break;
                    }
                }

                // k == sizeCache 表示k跑完一个循环，都没有碰到一个cache
                if (k == sizeCache) 
                {
                    cacheMiss++;
                    /*JavaScript中的shift()方法用于把数组的第一个元素从其中删除，并返回第一个元素的值
                    返回值：数组原来的第一个元素的值。
                    说明：如果数组是空的，那么 shift() 方法将不进行任何操作，返回 undefined 值。请注意，
                         该方法不创建新数组，而是直接修改原有的 arrayObject。
                    注释：该方法会改变数组的长度。
                    提示：要删除并返回数组的最后一个元素，请使用 pop() 方法。*/
                    /* 下面的两句js代码用C++改写如下：
                    cache.shift();
                    cache.push(idFace);
                    */
                    std::rotate(cache.begin(), cache.begin() + 1, cache.end()); // 将 [cache.begin() + 1, cache.end() )范围的元素左移，将cache.begin() + 1放到最左
                    cache.back() = idFace; // js中的cache.shift会把原来最左的那个元素给pop掉，std::rotate不会改变原长度，只会把最左的移动到最右，所以这里要在back位置写入idFace
                }
            }
        }

        CacheStoreInfo csi;
        csi.ACMR = static_cast<float>(cacheMiss) / nbFaces;
        csi.ACTVR = static_cast<float>(cacheMiss) / getNbVertices();
        return csi;
    }

    /*
    void Mesh::optimizePostTransform()
    {
        // post transform optimization (index buffer re-index), it implements tipsy
        // http://gfx.cs.princeton.edu/pubs/Sander_2007_%3ETR/tipsy.pdf

        std::uint32_t i = 0;
        std::uint32_t cacheSize = 32;
        bool hasUV = this->hasUV();
        Uint32Array& fAr = getFaces();
        Uint32Array fArUV = hasUV ? getFacesTexCoord() : fAr;

        std::uint32_t nbFaces = getNbFaces();
        std::uint32_t nbUniqueVertices = getNbVertices();
        std::uint32_t nbVertices = hasUV ? getNbTexCoords() : nbUniqueVertices;

        Uint32Array& dupUV = getVerticesDuplicateStartCount();
        Uint32Array mapToUnique(nbVertices - nbUniqueVertices, 0);// = new Uint32Array(nbVertices - nbUniqueVertices);
        
        if (hasUV) 
        {
            for (i = 0; i < nbVertices; ++i)
            {
                std::uint32_t dupStart = dupUV[i * 2];
                std::uint32_t dupEnd = dupStart + dupUV[i * 2 + 1];
                
                for (std::uint32_t j = dupStart; j < dupEnd; ++j) 
                {
                    mapToUnique[j - nbUniqueVertices] = i;
                }
            }
        }

        Uint32Array& fringsStartCount = getVerticesRingFaceStartCount();
        IUint32ArrayND* frings = getVerticesRingFace();

        Int32Array livesTriangles(nbVertices, 0);//var livesTriangles = new Int32Array(nbVertices);
        
        for (i = 0; i < nbUniqueVertices; ++i) 
        {
            livesTriangles[i] = fringsStartCount[i * 2 + 1];
        }

        for (i = nbUniqueVertices; i < nbVertices; ++i) 
        {
            livesTriangles[i] = fringsStartCount[mapToUnique[i - nbUniqueVertices] * 2 + 1];
        }

        Uint32Array vTimeStamp(nbVertices);//var vTimeStamp = new Uint32Array(nbVertices);
        std::uint32_t timeStamp = cacheSize + 1;
        std::uint32_t  cursor = 0;

        Uint32Array deadEndStack(nbFaces * 4,0);// = new Uint32Array(Utils.getMemory(4 * nbFaces * 4), 0, nbFaces * 4);
        std::uint32_t deadEndCount = 0;
        Uint8Array emittedFaces(nbFaces,0);// = new Uint8Array(nbFaces);

        Uint32Array fArUVNew(nbFaces * 4,0);// = new Uint32Array(nbFaces * 4);
        Uint32Array& fArNew = hasUV ? Uint32Array(nbFaces * 4,0) : fArUVNew;
        std::uint32_t fcount = 0;

        std::uint32_t fanningVertex = 0;

        while (fanningVertex >= 0) {

            var ringCandidates = [];

            var idRing = fanningVertex >= nbUniqueVertices ? mapToUnique[fanningVertex - nbUniqueVertices] : fanningVertex;
            var start = fringsStartCount[idRing * 2];
            var end = start + fringsStartCount[idRing * 2 + 1];

            for (i = start; i < end; ++i) {
                var idFace = frings[i];
                if (emittedFaces[idFace]) continue;
                emittedFaces[idFace] = 1;

                var idf = idFace * 4;

                var idv1 = deadEndStack[deadEndCount++] = fArUVNew[fcount++] = fArUV[idf];
                var idv2 = deadEndStack[deadEndCount++] = fArUVNew[fcount++] = fArUV[idf + 1];
                var idv3 = deadEndStack[deadEndCount++] = fArUVNew[fcount++] = fArUV[idf + 2];
                var idv4 = fArUVNew[fcount++] = fArUV[idf + 3];
                var isQuad = idv4 != = Utils.TRI_INDEX;

                if (hasUV) {
                    fArNew[fcount - 4] = fAr[idf];
                    fArNew[fcount - 3] = fAr[idf + 1];
                    fArNew[fcount - 2] = fAr[idf + 2];
                    fArNew[fcount - 1] = fAr[idf + 3];
                }

                ringCandidates.push(idv1, idv2, idv3);

                --livesTriangles[idv1];
                --livesTriangles[idv2];
                --livesTriangles[idv3];

                if (timeStamp - vTimeStamp[idv1] > cacheSize) vTimeStamp[idv1] = timeStamp++;
                if (timeStamp - vTimeStamp[idv2] > cacheSize) vTimeStamp[idv2] = timeStamp++;
                if (timeStamp - vTimeStamp[idv3] > cacheSize) vTimeStamp[idv3] = timeStamp++;

                if (isQuad) {
                    deadEndStack[deadEndCount++] = idv4;
                    ringCandidates.push(idv4);
                    --livesTriangles[idv4];
                    if (timeStamp - vTimeStamp[idv4] > cacheSize) vTimeStamp[idv4] = timeStamp++;
                }

            }

            // get emitted next vertex
            fanningVertex = -1;
            var bestPriority = -1;
            var nbCandidates = ringCandidates.length;
            for (i = 0; i < nbCandidates; ++i) {
                var idc = ringCandidates[i];
                var liveCount = livesTriangles[idc];
                if (liveCount <= 0) continue;

                var priority = 0;
                var posCache = timeStamp - vTimeStamp[idc];
                if (posCache + 2 * liveCount <= cacheSize) {
                    priority = posCache;
                }

                if (priority > bestPriority) {
                    bestPriority = priority;
                    fanningVertex = idc;
                }
            }

            if (fanningVertex != = -1) continue;

            while (deadEndCount > 0) {
                var vEnd = deadEndStack[--deadEndCount];
                if (livesTriangles[vEnd] > 0) {
                    fanningVertex = vEnd;
                    break;
                }
            }

            if (fanningVertex != = -1) continue;

            while (cursor < nbVertices) {
                if (livesTriangles[cursor++] > 0) {
                    fanningVertex = cursor - 1;
                    break;
                }
            }

        }

        fArUV.set(fArUVNew.subarray(0, nbFaces * 4));
        if (hasUV) fAr.set(fArNew.subarray(0, nbFaces * 4));

    }

    optimizePreTransform() {
        // pre transform optimization (not as important as post transform though)
        // it also removes unused vertices

        var vArOld = getVertices();
        var cArOld = getColors();
        var mArOld = getMaterials();
        var nbVertices = getNbVertices();

        var fAr = getFaces();
        var fArCount = getNbFaces() * 4;

        var vArNew = new Float32Array(nbVertices * 3);
        var cArNew = new Float32Array(nbVertices * 3);
        var mArNew = new Float32Array(nbVertices * 3);

        var idvPos = new Uint32Array(nbVertices);
        var acc = 0;
        var i = 0;
        for (i = 0; i < fArCount; ++i) {
            var iv = fAr[i];
            if (iv == = Utils.TRI_INDEX) continue;

            var tag = idvPos[iv] - 1;
            if (tag == = -1) {
                var idNew = acc * 3;
                var idOld = iv * 3;
                vArNew[idNew] = vArOld[idOld];
                vArNew[idNew + 1] = vArOld[idOld + 1];
                vArNew[idNew + 2] = vArOld[idOld + 2];

                cArNew[idNew] = cArOld[idOld];
                cArNew[idNew + 1] = cArOld[idOld + 1];
                cArNew[idNew + 2] = cArOld[idOld + 2];

                mArNew[idNew] = mArOld[idOld];
                mArNew[idNew + 1] = mArOld[idOld + 1];
                mArNew[idNew + 2] = mArOld[idOld + 2];

                tag = acc++;
                idvPos[iv] = tag + 1;
            }

            fAr[i] = tag;
        }

        var nbUnusedVertex = nbVertices - acc;
        if (nbUnusedVertex > 0)
            setNbVertices(acc);

        // Only the unique "positoned" vertices are pre transform, because sculptgl 
        // requires the duplicate vertices to be after the uniques positioned vertices
        if (hasUV()) {
            var fArUV = getFacesTexCoord();
            // remap unique vertex i
            for (i = 0; i < fArCount; ++i) {
                var iduv = fArUV[i];
                if (iduv < nbVertices) fArUV[i] = idvPos[iduv] - 1;
                else if (iduv != = Utils.TRI_INDEX) fArUV[i] -= nbUnusedVertex;
            }

            var nbUV = getNbTexCoords();
            var nbUVNew = getNbTexCoords() - nbUnusedVertex;

            var tAr = getTexCoords();
            var tArNew = new Float32Array(nbUVNew * 2);
            var dupUVNew = new Uint32Array(acc * 2);
            var dupUV = getVerticesDuplicateStartCount();

            for (i = 0; i < nbVertices; ++i) {
                var i2 = i * 2;
                var start = dupUV[i2];
                var newiv = (idvPos[i] - 1) * 2;
                if (newiv < 0) continue;

                if (start > 0) {
                    dupUVNew[newiv] = start - nbUnusedVertex;
                    dupUVNew[newiv + 1] = dupUV[i2 + 1];
                }

                tArNew[newiv] = tAr[i2];
                tArNew[newiv + 1] = tAr[i2 + 1];
            }

            for (i = nbVertices; i < nbUV; ++i) {
                var ivd = i * 2;
                var ivdnew = (i - nbUnusedVertex) * 2;
                tArNew[ivdnew] = tAr[ivd];
                tArNew[ivdnew + 1] = tAr[ivd + 1];
            }

            setVerticesDuplicateStartCount(dupUVNew);
            setTexCoords(tArNew);
        }

        vArOld.set(vArNew);
        cArOld.set(cArNew);
        mArOld.set(mArNew);

    }
    */
}