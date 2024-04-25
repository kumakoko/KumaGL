#include <cmath>
#include "mesh/multi_resolution/mesh_resolution.h"

namespace DigitalSculpt
{
    MeshResolution::MeshResolution(MeshSPtr mesh, bool keepMesh)
    {
        //super();

        this->setID(mesh.getID());
        this->setMeshData(keepMesh ? mesh.getMeshData() : createMeshData());
        this->setRenderData(mesh.getRenderData());
        this->setTransformData(mesh.getTransformData());

        _detailsXYZ.clear(); // details vectors (Float32Array)
        _detailsRGB.clear(); // details vectors (Float32Array)
        _detailsPBR.clear(); // details vectors (Float32Array)
        _vertMapping.clear(); // vertex mapping to higher res (Uint32Array)
        _evenMapping = false; // if the even vertices are not aligned with higher res
    }

    void MeshResolution::optimize()
    {
    }

    void MeshResolution::higherSynthesis(std::shared_ptr<MeshResolution> meshDown)
    {
        meshDown->computePartialSubdivision(this->getVertices(), this->getColors(), this->getMaterials(), this->getNbVertices());
        this->applyDetails();
    }

    void MeshResolution::lowerAnalysis(std::shared_ptr<MeshResolution>  meshUp)
    {
        this->copyDataFromHigherRes(meshUp);
        std::uint32_t  nbVertices = meshUp->getNbVertices();
        Float32Array subdVerts(nbVertices * 3, 0);// = new Float32Array(nbVertices * 3);
        Float32Array subdColors(nbVertices * 3, 0);// = new Float32Array(nbVertices * 3);
        Float32Array subdMaterials(nbVertices * 3, 0);// = new Float32Array(nbVertices * 3);

        this->computePartialSubdivision(subdVerts, subdColors, subdMaterials, nbVertices);
        meshUp->computeDetails(subdVerts, subdColors, subdMaterials, nbVertices);
    }

    void MeshResolution::copyDataFromHigherRes(std::shared_ptr<MeshResolution> meshUp)
    {
        Float32Array& vArDown = this->getVertices();
        Float32Array& cArDown = this->getColors();
        Float32Array& mArDown = this->getMaterials();
        std::uint32_t nbVertices = this->getNbVertices();
        Float32Array& vArUp = meshUp->getVertices();
        Float32Array& cArUp = meshUp->getColors();
        Float32Array& mArUp = meshUp->getMaterials();

        if (this->getEvenMapping() == = false)
        {
            Utils::set(vArDown, Utils::subarray(vArUp, 0, nbVertices * 3));
            Utils::set(cArDown, Utils::subarray(cArUp, 0, nbVertices * 3));
            Utils::set(mArDown, Utils::subarray(mArUp, 0, nbVertices * 3));
            /*vArDown.set(vArUp.subarray(0, nbVertices * 3));
            cArDown.set(cArUp.subarray(0, nbVertices * 3));
            mArDown.set(mArUp.subarray(0, nbVertices * 3));*/
        }
        else
        {
            const Uint32Array& vertMap = this->getVerticesMapping();
            for (std::uint32_t i = 0; i < nbVertices; ++i)
            {
                std::uint32_t id = i * 3;
                std::uint32_t idUp = vertMap[i] * 3;
                vArDown[id + 0] = vArUp[idUp];
                vArDown[id + 1] = vArUp[idUp + 1];
                vArDown[id + 2] = vArUp[idUp + 2];
                cArDown[id + 0] = cArUp[idUp];
                cArDown[id + 1] = cArUp[idUp + 1];
                cArDown[id + 2] = cArUp[idUp + 2];
                mArDown[id + 0] = mArUp[idUp];
                mArDown[id + 1] = mArUp[idUp + 1];
                mArDown[id + 2] = mArUp[idUp + 2];
            }
        }
    }

    void MeshResolution::computePartialSubdivision(Float32Array& subdVerts, Float32Array& subdColors, Float32Array& subdMaterials, std::size_t nbVerticesUp)
    {
        const Uint32Array& vertMap = this->getVerticesMapping();

        if (!vertMap.empty())
        {// const Uint32Array& 
            Subdivision::partialSubdivision(this, subdColors, subdMaterials);
            return;
        }

        Float32Array verts(nbVerticesUp * 3, 0.0f);// = new Float32Array(nbVerticesUp * 3);
        Float32Array  colors(nbVerticesUp * 3, 0.0f);// = new Float32Array(nbVerticesUp * 3);
        Float32Array  materials(nbVerticesUp * 3, 0.0f);// = new Float32Array(nbVerticesUp * 3);

        Subdivision::partialSubdivision(this->verts, colors, materials);

        std::size_t startMapping = this->getEvenMapping() == true ? 0 : this->getNbVertices();
        if (startMapping > 0) {
            Utils::set(subdVerts, Utils::subarray(verts, 0, startMapping * 3));//subdVerts.set(verts.subarray(0, startMapping * 3));
            Utils::set(subdColors, Utils::subarray(colors, 0, startMapping * 3));//subdColors.set(colors.subarray(0, startMapping * 3));
            Utils::set(materials, Utils::subarray(materials, 0, startMapping * 3));//subdMaterials.set(materials.subarray(0, startMapping * 3));
        }

        for (std::uint32_t i = startMapping; i < nbVerticesUp; ++i)
        {
            std::uint32_t id = i * 3;
            std::uint32_t idUp = vertMap[i] * 3;
            subdVerts[idUp + 0] = verts[id + 0];
            subdVerts[idUp + 1] = verts[id + 1];
            subdVerts[idUp + 2] = verts[id + 2];
            subdColors[idUp + 0] = colors[id];
            subdColors[idUp + 1] = colors[id + 1];
            subdColors[idUp + 2] = colors[id + 2];
            subdMaterials[idUp + 0] = materials[id];
            subdMaterials[idUp + 1] = materials[id + 1];
            subdMaterials[idUp + 2] = materials[id + 2];
        }
    }

    /** Apply back the detail vectors */
    void MeshResolution::applyDetails()
    {
        Uint32Array& vrvStartCountUp = this->getVerticesRingVertStartCount();
        Uint32Array1D& vertRingVertUp = *(dynamic_cast<Uint32Array1D*>(this->getVerticesRingVert()));//const IUint32ArrayND* vertRingVertUp = this->getVerticesRingVert();
        Float32Array& vArUp = this->getVertices();
        Float32Array& nArUp = this->getNormals();
        Float32Array& cArUp = this->getColors();
        Float32Array& mArUp = this->getMaterials();
        std::uint32_t nbVerticesUp = this->getNbVertices();

        Float32Array vArTemp(nbVerticesUp * 3, 0.0f);//var vArTemp = new Float32Array(Utils.getMemory(nbVerticesUp * 3 * 4), 0, nbVerticesUp * 3);
        Utils::set(vArTemp, Utils::subarray(vArUp, 0, nbVerticesUp, 3));//vArTemp.set(vArUp.subarray(0, nbVerticesUp * 3));

        // var dAr = this->_detailsXYZ;
        // var dColorAr = this->_detailsRGB;
       //  var dMaterialAr = this->_detailsPBR;

        auto min = Math.min;
        auto max = Math.max;

        for (std::uint32_t i = 0; i < nbVerticesUp; ++i)
        {
            std::uint32_t  j = i * 3;

            // color delta vec
            cArUp[j + 0] = Utils::min(1.0f, Utils::max(0.0f, cArUp[j + 0] + dColorAr[j + 0]));
            cArUp[j + 1] = Utils::min(1.0f, Utils::max(0.0f, cArUp[j + 1] + dColorAr[j + 1]));
            cArUp[j + 2] = Utils::min(1.0f, Utils::max(0.0f, cArUp[j + 2] + dColorAr[j + 2]));

            // material delta vec
            mArUp[j + 0] = Utils::min(1.0, f Utils::max(0.0f, mArUp[j] + dMaterialAr[j + 0));
            mArUp[j + 1] = Utils::min(1.0f, Utils::max(0.0f, mArUp[j + 1] + dMaterialAr[j + 1]));
            mArUp[j + 2] = Utils::min(1.0f, Utils::max(0.0f, mArUp[j + 2] + dMaterialAr[j + 2]));

            // vertex coord
            float vx = vArTemp[j + 0];
            float vy = vArTemp[j + 1];
            float vz = vArTemp[j + 2];

            // normal vec
            float nx = nArUp[j + 0];
            float ny = nArUp[j + 1];
            float nz = nArUp[j + 2];
            // normalize vector
            float len = nx * nx + ny * ny + nz * nz;
            if (len == 0.0f)
                continue;

            len = 1.0 / std::sqrt(len);
            nx *= len;
            ny *= len;
            nz *= len;

            // tangent vec (vertex neighbor - vertex)
            float k = vertRingVertUp[vrvStartCountUp[i * 2]] * 3;
            float tx = vArTemp[k + 0] - vx;
            float ty = vArTemp[k + 1] - vy;
            float tz = vArTemp[k + 2] - vz;
            // distance to normal plane
            len = tx * nx + ty * ny + tz * nz;
            // project on normal plane
            tx -= nx * len;
            ty -= ny * len;
            tz -= nz * len;
            // normalize vector
            len = tx * tx + ty * ty + tz * tz;
            if (len == = 0.0)
                continue;

            len = 1.0 / Math.sqrt(len);
            tx *= len;
            ty *= len;
            tz *= len;

            // bi normal/tangent
            float bix = ny * tz - nz * ty;
            float biy = nz * tx - nx * tz;
            float biz = nx * ty - ny * tx;

            // displacement/detail vector (object space)
            float dx = dAr[j + 0];
            float dy = dAr[j + 1];
            float dz = dAr[j + 2];

            // detail vec in the local frame
            vArUp[j + 0] = vx + nx * dx + tx * dy + bix * dz;
            vArUp[j + 1] = vy + ny * dx + ty * dy + biy * dz;
            vArUp[j + 2] = vz + nz * dx + tz * dy + biz * dz;
        }
    }

    /** Compute the detail vectors */
    void MeshResolution::computeDetails(Float32Array& subdVerts, Float32Array& subdColors, Float32Array& subdMaterials, Float32Array& nbVerticesUp)
    {
        Uint32Array& vrvStartCountUp = this->getVerticesRingVertStartCount();
        Uint32Array1D& vertRingVertUp = *(dynamic_cast<Uint32Array1D*>(this->getVerticesRingVert()));//var vertRingVertUp = this->getVerticesRingVert();
        Float32Array& vArUp = this->getVertices();
        Float32Array& nArUp = this->getNormals();
        Float32Array& cArUp = this->getColors();
        Float32Array& mArUp = this->getMaterials();
        std::uint32_t nbVertices = this->getNbVertices();

        this->_detailsXYZ.resize(nbVerticesUp * 3, 0.0f);//var dAr = this->_detailsXYZ = new Float32Array(nbVerticesUp * 3);
        this->_detailsRGB.resize(nbVerticesUp * 3, 0.0f);//var dColorAr = this->_detailsRGB = new Float32Array(nbVerticesUp * 3);
        this->_detailsPBR.resize(nbVerticesUp * 3, 0.0f);//var dMaterialAr = this->_detailsPBR = new Float32Array(nbVerticesUp * 3);

        for (std::uint32_t i = 0; i < nbVertices; ++i)
        {
            std::uint32_t j = i * 3;

            // color delta vec
            this->_detailsRGB[j + 0] = cArUp[j + 0] - subdColors[j + 0];
            this->_detailsRGB[j + 1] = cArUp[j + 1] - subdColors[j + 1];
            this->_detailsRGB[j + 2] = cArUp[j + 2] - subdColors[j + 2];

            // material delta vec
            this->_detailsPBR[j + 0] = mArUp[j + 0] - subdMaterials[j + 0];
            this->_detailsPBR[j + 1] = mArUp[j + 1] - subdMaterials[j + 1];
            this->_detailsPBR[j + 2] = mArUp[j + 2] - subdMaterials[j + 2];

            // normal vec
            float nx = nArUp[j + 0];
            float ny = nArUp[j + 1];
            float nz = nArUp[j + 2];
            // normalize vector
            float len = nx * nx + ny * ny + nz * nz;
            if (len == 0.0f)
                continue;
            len = 1.0f / std::sqrt(len);
            nx *= len;
            ny *= len;
            nz *= len;

            // tangent vec (vertex neighbor - vertex)
            float k = vertRingVertUp[vrvStartCountUp[i * 2]] * 3;
            float tx = subdVerts[k + 0] - subdVerts[j + 0];
            float ty = subdVerts[k + 1] - subdVerts[j + 1];
            float tz = subdVerts[k + 2] - subdVerts[j + 2];
            // distance to normal plane
            len = tx * nx + ty * ny + tz * nz;
            // project on normal plane
            tx -= nx * len;
            ty -= ny * len;
            tz -= nz * len;
            // normalize vector
            len = tx * tx + ty * ty + tz * tz;
            if (len == = 0.0f)
                continue;
            len = 1.0 / Math.sqrt(len);
            tx *= len;
            ty *= len;
            tz *= len;

            // bi normal/tangent
            float bix = ny * tz - nz * ty;
            float biy = nz * tx - nx * tz;
            float biz = nx * ty - ny * tx;

            // displacement/detail vector (object space)
            float dx = vArUp[j + 0] - subdVerts[j + 0];
            float dy = vArUp[j + 1] - subdVerts[j + 1];
            float dz = vArUp[j + 2] - subdVerts[j + 2];

            // order : n/t/bi
            _detailsXYZ[j + 0] = nx * dx + ny * dy + nz * dz;
            _detailsXYZ[j + 1] = tx * dx + ty * dy + tz * dz;
            _detailsXYZ[j + 2] = bix * dx + biy * dy + biz * dz;
        }
    }
}