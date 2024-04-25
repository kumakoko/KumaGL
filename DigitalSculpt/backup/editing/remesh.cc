#include <vector>
#include <cstdint>
#include <limits>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include "editing/voxels.h"
#include "math3d/geometry.h"
#include "mesh/mesh_static/mesh_static.h"
#include "misc/enums.h"
#include "misc/utils.h"
#include "editing/hole_filling.h"
#include "editing/surface_nets.h"
#include "editing/marching_cubes.h"
#include "editing/tools/smooth.h"
#include "editing/remesh.h"

namespace DigitalSculpt
{
    std::uint32_t Remesh::RESOLUTION = 150;
    bool Remesh::BLOCK = false;
    bool Remesh::SMOOTHING = true;

    void Remesh::floodFill(Voxels voxels)
    {
        auto step = voxels.step;
        Float32Array& res = voxels.dims;
        float rx = res[0];
        float ry = res[1];
        float rxy = rx * ry;

        Int32Array& crossedEdges = voxels.crossedEdges;
        std::vector<float>& distField = voxels.distanceField;
        std::size_t datalen = distField.size();
        Uint8Array tagCell(datalen, 0); //auto tagCell = new Uint8Array(datalen); // 0 interior, 1 exterior
        Int32Array stack(datalen, 0);//auto stack = new Int32Array(datalen);

        stack[0] = 0;
        auto curStack = 1;

        Float32Array dirs{ -1, 1, -rx, rx, -rxy, rxy };
        Uint32Array dirsEdge{ 0, 0, 1, 1, 2, 2 };
        std::size_t nbDir = dirs.size();
        auto i = 0;
        auto idNext = 0;

        while (curStack > 0)
        {
            std::int32_t cell = stack[--curStack];
            auto cellDist = distField[cell];

            if (cellDist < step)
            {
                // border hit
                for (i = 0; i < nbDir; ++i)
                {
                    auto off = dirs[i];
                    idNext = cell + off;

                    if (idNext >= datalen || idNext < 0)
                        continue; // range check
                    if (tagCell[idNext] == 1)
                        continue; // check if already tagged as exterior
                    if (distField[idNext] == std::numeric_limits<float>::infinity())//Infinity) 
                        continue; // check if we are in the far exterior zone
                    if (crossedEdges[(off >= 0 ? cell : idNext) * 3 + dirsEdge[i]] == 0)
                    {
                        tagCell[idNext] = 1;
                        stack[curStack++] = idNext;
                    }
                }
            }
            else
            {
                // exterior
                for (i = 0; i < nbDir; ++i)
                {
                    idNext = cell + dirs[i];
                    if (idNext >= datalen || idNext < 0)
                        continue; // range check
                    if (tagCell[idNext] == 1)
                        continue; // check if already tagged as exterior
                    tagCell[idNext] = 1;
                    stack[curStack++] = idNext;
                }
            }
        }

        for (auto id = 0; id < datalen; ++id)
        {
            if (distField[id] == 0)
                continue;////console.log('hit');

            if (tagCell[id] == 0)
                distField[id] = -distField[id];
        }
    }

    void Remesh::voxelize(MeshSPtr mesh, Voxels voxels)
    {
        //auto min = voxels.min;
        auto step = voxels.step;
        auto dims = voxels.dims;
        auto invStep = 1.0f / step;

        float vminx = voxels.min[0];
        float vminy = voxels.min[1];
        float vminz = voxels.min[2];

        float rx = dims[0];
        float ry = dims[1];
        float rxy = rx * ry;

        /* auto distField = voxels.distanceField;
         auto crossedEdges = voxels.crossedEdges;
         auto colors = voxels.colorField;
         auto materials = voxels.materialField;*/

        Uint32Array& iAr = mesh->getTriangles();
        Float32Array& vAr = mesh->getVertices();
        Float32Array& cAr = mesh->getColors();
        Float32Array& mAr = mesh->getMaterials();
        std::uint32_t nbTriangles = mesh->getNbTriangles();


        std::array<float, 3> v1{ 0.0f, 0.0f, 0.0f };
        std::array<float, 3> v2{ 0.0f, 0.0f, 0.0f };
        std::array<float, 3> v3{ 0.0f, 0.0f, 0.0f };
        std::array<float, 3> triEdge1{ 0.0f, 0.0f, 0.0f };
        std::array<float, 3> triEdge2{ 0.0f, 0.0f, 0.0f };
        std::array<float, 3> point{ 0.0f, 0.0f, 0.0f };
        std::array<float, 4> closest{ 0.0f, 0.0f, 0.0f,0.0f };

        std::array<std::array<float, 3>, 3> dirUnit{
            std::array<float, 3>{1.0f, 0.0f, 0.0f},
            std::array<float, 3>{0.0f, 1.0f, 0.0f},
            std::array<float, 3>{0.0f, 0.0f, 1.0f}
        };

        float inv3 = 1.0f / 3.0f;

        for (std::uint32_t iTri = 0; iTri < nbTriangles; ++iTri)
        {
            std::uint32_t  idTri = iTri * 3;

            std::uint32_t iv1 = iAr[idTri + 0] * 3;
            std::uint32_t iv2 = iAr[idTri + 1] * 3;
            std::uint32_t iv3 = iAr[idTri + 2] * 3;

            float v1x = v1[0] = vAr[iv1 + 0];
            float v1y = v1[1] = vAr[iv1 + 1];
            float v1z = v1[2] = vAr[iv1 + 2];
            float v2x = v2[0] = vAr[iv2 + 0];
            float v2y = v2[1] = vAr[iv2 + 1];
            float v2z = v2[2] = vAr[iv2 + 2];
            float v3x = v3[0] = vAr[iv3 + 0];
            float v3y = v3[1] = vAr[iv3 + 1];
            float v3z = v3[2] = vAr[iv3 + 2];

            float c1x = (cAr[iv1 + 0] + cAr[iv2 + 0] + cAr[iv3 + 0]) * inv3;
            float c1y = (cAr[iv1 + 1] + cAr[iv2 + 1] + cAr[iv3 + 1]) * inv3;
            float c1z = (cAr[iv1 + 2] + cAr[iv2 + 2] + cAr[iv3 + 2]) * inv3;
            float m1x = (mAr[iv1 + 0] + mAr[iv2 + 0] + mAr[iv3 + 0]) * inv3;
            float m1y = (mAr[iv1 + 1] + mAr[iv2 + 1] + mAr[iv3 + 1]) * inv3;
            float m1z = (mAr[iv1 + 2] + mAr[iv2 + 2] + mAr[iv3 + 2]) * inv3;

            // bounding box recomputation (we already have the bbox of the quad but
            // not of the triangles...)
            float xmin = v1x < v2x ? v1x < v3x ? v1x : v3x : v2x < v3x ? v2x : v3x;
            float xmax = v1x > v2x ? v1x > v3x ? v1x : v3x : v2x > v3x ? v2x : v3x;
            float ymin = v1y < v2y ? v1y < v3y ? v1y : v3y : v2y < v3y ? v2y : v3y;
            float ymax = v1y > v2y ? v1y > v3y ? v1y : v3y : v2y > v3y ? v2y : v3y;
            float zmin = v1z < v2z ? v1z < v3z ? v1z : v3z : v2z < v3z ? v2z : v3z;
            float zmax = v1z > v2z ? v1z > v3z ? v1z : v3z : v2z > v3z ? v2z : v3z;

            // cache what can be cached for faster ray-tri and point-tri tests
            // basically edge stuffs
            float e1x = triEdge1[0] = v2x - v1x;
            float e1y = triEdge1[1] = v2y - v1y;
            float e1z = triEdge1[2] = v2z - v1z;
            float e2x = triEdge2[0] = v3x - v1x;
            float e2y = triEdge2[1] = v3y - v1y;
            float e2z = triEdge2[2] = v3z - v1z;
            float a00 = e1x * e1x + e1y * e1y + e1z * e1z;
            float a01 = e1x * e2x + e1y * e2y + e1z * e2z;
            float a11 = e2x * e2x + e2y * e2y + e2z * e2z;

            std::uint32_t snapMinx = static_cast<std::uint32_t>(std::floor((xmin - vminx) * invStep));
            std::uint32_t snapMiny = static_cast<std::uint32_t>(std::floor((ymin - vminy) * invStep));
            std::uint32_t snapMinz = static_cast<std::uint32_t>(std::floor((zmin - vminz) * invStep));

            std::uint32_t snapMaxx = static_cast<std::uint32_t>(std::ceil((xmax - vminx) * invStep));
            std::uint32_t snapMaxy = static_cast<std::uint32_t>(std::ceil((ymax - vminy) * invStep));
            std::uint32_t snapMaxz = static_cast<std::uint32_t>(std::ceil((zmax - vminz) * invStep));

            for (std::uint32_t k = snapMinz; k <= snapMaxz; ++k)
            {
                for (std::uint32_t j = snapMiny; j <= snapMaxy; ++j)
                {
                    for (std::uint32_t i = snapMinx; i <= snapMaxx; ++i)
                    {
                        std::uint32_t x = vminx + i * step;
                        std::uint32_t y = vminy + j * step;
                        std::uint32_t z = vminz + k * step;
                        std::uint32_t n = i + j * rx + k * rxy;

                        point[0] = x;
                        point[1] = y;
                        point[2] = z;

                        auto newDist = Geometry::distance2PointTriangleEdges(point, triEdge1, triEdge2, v1, a00, a01, a11, closest);

                        newDist = std::sqrt(newDist);

                        if (newDist < voxels.distanceField[n])
                        {
                            voxels.distanceField[n] = newDist;
                            auto n3 = n * 3;
                            colors[n3 + 0] = c1x;
                            colors[n3 + 1] = c1y;
                            colors[n3 + 2] = c1z;
                            materials[n3 + 0] = m1x;
                            materials[n3 + 1] = m1y;
                            materials[n3 + 2] = m1z;
                        }

                        if (newDist > step)
                            continue;

                        for (auto it = 0; it < 3; ++it)
                        {
                            auto val = closest[it] - point[it];

                            if (val < 0.0 || val > step)
                                continue;

                            auto idEdge = n * 3 + it;

                            if (voxels.crossedEdges[idEdge] == 1)
                                continue;

                            auto dist = Geometry::intersectionRayTriangleEdges(point, dirUnit[it], triEdge1, triEdge2, v1);

                            if (dist < 0.0 || dist > step)
                                continue;

                            voxels.crossedEdges[idEdge] = 1;
                        }

                    }
                }
            }
        }
    }

    // grid structure
    Voxels Remesh::createVoxelData(const SixElemArray& box)
    {
        float step = std::max((box[3] - box[0]), (box[4] - box[1]), (box[5] - box[2])) / Remesh::RESOLUTION;
        float stepMin = step * 1.51f;
        float stepMax = step * 1.51f;
        Float32Array min{ box[0] - stepMin, box[1] - stepMin, box[2] - stepMin };
        Float32Array max{ box[3] + stepMax, box[4] + stepMax, box[5] + stepMax };

        std::uint32_t rx = static_cast<std::uint32_t>(std::ceil((max[0] - min[0]) / step));
        std::uint32_t ry = static_cast<std::uint32_t>(std::ceil((max[1] - min[1]) / step));
        std::uint32_t rz = static_cast<std::uint32_t>(std::ceil((max[2] - min[2]) / step));

        std::uint32_t datalen = rx * ry * rz;
        //auto buffer = Utils.getMemory((4 * (1 + 3 + 3) + 3) * datalen);
        Float32Array distField(datalen, 0.0f);//auto distField = new Float32Array(buffer, 0, datalen);
        Float32Array colors(datalen * 3, 0.0f);//auto colors = new Float32Array(buffer, 4 * datalen, datalen * 3);
        Float32Array materials(datalen * 3, 0.0f);//auto materials = new Float32Array(buffer, 16 * datalen, datalen * 3);
        Uint8Array crossedEdges(datalen * 3, 0);//auto crossedEdges = new Uint8Array(buffer, 28 * datalen, datalen * 3);

        // Initialize data
        for (std::uint32_t idf = 0; idf < datalen; ++idf)
            distField[idf] = std::numeric_limits<float>::infinity();//Infinity;

        std::uint32_t datalene = datalen * 3;
        for (std::uint32_t ide = 0; ide < datalene; ++ide)
            crossedEdges[ide] = 0;

        std::uint32_t datalenc = datalen * 3;
        for (std::uint32_t idc = 0; idc < datalenc; ++idc)
            colors[idc] = materials[idc] = -1;

        Voxels voxels;
        Utils::PushToVector(voxels.dims, rx, ry, rz);
        voxels.step = step;
        voxels.min = min;
        voxels.max = max;
        voxels.crossedEdges = crossedEdges;
        voxels.distanceField = distField;
        voxels.colorField = colors;
        voxels.materialField = materials;
        return voxels;
    }

    MeshSPtr Remesh::createMesh(MeshSPtr mesh, const Uint32Array& faces, const Float32Array& vertices, const Float32Array& colors, const Float32Array& materials)
    {
        MeshSPtr newMesh = std::make_shared<MeshStatic>(new MeshStatic());
        newMesh->setID(mesh->getID());
        newMesh->setFaces(faces);
        newMesh->setVertices(vertices);

        if (!colors.empty())
            newMesh->setColors(colors);

        if (!materials.empty())
            newMesh->setMaterials(materials);

        newMesh->setRenderData(mesh->getRenderData());
        newMesh->init();
        newMesh->initRender();
        return newMesh;
    }

    // hole filling + transform to world + ComputeBox
    SixElemArray Remesh::prepareMeshes(std::vector<MeshSPtr>& meshes)
    {
        float Infinity = std::numeric_limits<float>::infinity();
        SixElemArray box{ Infinity, Infinity, Infinity, -Infinity, -Infinity, -Infinity };
        glm::vec3 tmp(0.0f);
        std::size_t nbm = meshes.size();

        for (std::size_t i = 0; i < nbm; ++i)
        {
            if (meshes[i]->isUsingTexCoords())
                meshes[i]->setShaderType(Enums::Shader::MATCAP);

            glm::mat4& matrix = meshes[i]->getMatrix();
            meshes[i] = HoleFilling::createClosedMesh(mesh);
            Float32Array& vAr = meshes[i]->getVertices();
            std::uint32_t nbv = meshes[i]->getNbVertices();

            for (std::uint32_t j = 0; j < nbv; ++j)
            {
                std::uint32_t id = j * 3;
                tmp = meshes[i]->getMatrix() * glm::vec4(vAr[id + 0], vAr[id + 1], vAr[id + 2], 1.0f);////vec3.transformMat4(tmp, tmp, matrix);

                float x = vAr[id + 0] = tmp.x;
                float y = vAr[id + 1] = tmp.y;
                float z = vAr[id + 2] = tmp.z;

                if (x < box[0]) box[0] = x;
                if (y < box[1]) box[1] = y;
                if (z < box[2]) box[2] = z;
                if (x > box[3]) box[3] = x;
                if (y > box[4]) box[4] = y;
                if (z > box[5]) box[5] = z;
            }
        }

        return box;
    }

    void Remesh::alignMeshBound(MeshSPtr mesh, const SixElemArray& box)
    {
        glm::vec3 oldMin(box[0], box[1], box[2]);
        glm::vec3 oldMax(box[3], box[4], box[5]);

        float oldRadius = glm::distance(oldMin, oldMax);//auto oldRadius = vec3.dist(oldMin, oldMax);
        glm::vec3 oldCenter = (oldMin + oldMax) * 0.5f; //auto oldCenter = vec3.add([], oldMin, oldMax);
        //vec3.scale(oldCenter, oldCenter, 0.5);

        const SixElemArray& newBox = mesh->getLocalBound();
        glm::vec3 newMin{ newBox[0], newBox[1], newBox[2] };
        glm::vec3 newMax{ newBox[3], newBox[4], newBox[5] };
        float newRadius = glm::distance(newMin, newMax);//auto newRadius = vec3.dist(newMin, newMax);
        glm::vec3 newCenter = (newMin + newMax) * 0.5f; // auto newCenter = vec3.add([], newMin, newMax);
        //vec3.scale(newCenter, newCenter, 0.5);

        float scale = oldRadius / newRadius;
        glm::vec3 tr = oldCenter / scale; //auto tr = vec3.scale([], oldCenter, 1.0 / scale);
        tr -= newCenter;//vec3.sub(tr, tr, newCenter);

        glm::mat4& mat = mesh->getMatrix();
        mat = glm::identity<glm::mat4>();//mat4.identity(mat);
        mat = glm::scale(mat, glm::vec3(scale));//mat4.scale(mat, mat, [scale, scale, scale]);
        mat = glm::translate(mat, glm::vec3(tr));//mat4.translate(mat, mat, tr);
    }

    void Remesh::tangentialSmoothing(MeshSPtr mesh)
    {
        std::uint32_t nbVertices = mesh->getNbVertices();
        Uint32Array indices(nbVertices, 0);//auto indices = new Uint32Array(nbVertices);

        for (std::uint32_t i = 0; i < nbVertices; ++i)
            indices[i] = i;

        auto smo = std::make_shared<Smooth>(new Smooth());//auto smo = new Smooth();
        smo->setToolMesh(mesh);
        smo->smoothTangent(indices, 1.0);
        mesh->updateGeometry();
        mesh->updateGeometryBuffers();
    }

    MeshSPtr Remesh::remesh(std::vector<MeshSPtr>& meshes, MeshSPtr baseMesh, bool manifold)
    {
        //console.time('remesh total');

        //console.time('1. prepareMeshes');
        //这句meshes = meshes.slice()表示不破坏原本的mesh数组，但是创建一个新的mesh数组，但是//meshes = meshes.slice();
        auto box = prepareMeshes(meshes);
        //console.timeEnd('1. prepareMeshes');

        //console.time('2. voxelization');
        auto voxels = createVoxelData(box);
        std::size_t l = meshes.size();
        for (std::size_t i = 0; i < l; ++i)
            voxelize(meshes[i], voxels);
        //console.timeEnd('2. voxelization');

        //console.time('3. flood');
        floodFill(voxels);
        //console.timeEnd('3. flood');

        auto res;
        
        if (manifold) 
        {
            //console.time('4. marchingCubes');
            MarchingCubes::BLOCK = Remesh::BLOCK;
            res = MarchingCubes::computeSurface(voxels);
            //console.timeEnd('4. marchingCubes');
        }
        else 
        {
            //console.time('4. surfaceNets');
            SurfaceNets.BLOCK = Remesh::BLOCK;
            res = SurfaceNets.computeSurface(voxels);
            //console.timeEnd('4. surfaceNets');
        }

        //console.time('5. createMesh');
        auto nmesh = createMesh(baseMesh, res.faces, res.vertices, res.colors, res.materials);
        //console.timeEnd('5. createMesh');

        alignMeshBound(nmesh, box);

        if (manifold && Remesh::SMOOTHING)
        {
            //console.time('6. tangential smoothing');
            tangentialSmoothing(nmesh);
            //console.timeEnd('6. tangential smoothing');
        }

        //console.timeEnd('remesh total');
        //console.log('\n');
        return nmesh;
    }

    MeshAttribute& Remesh::mergeArrays(std::vector<MeshSPtr>& meshes, MeshAttribute& arr)
    {
        std::uint32_t nbVertices = 0;
        std::uint32_t nbFaces = 0;
        std::uint32_t nbQuads = 0;
        std::uint32_t nbTriangles = 0;
        std::uint32_t i, j;

        std::size_t nbMeshes = meshes.size();
        std::uint32_t k = 0;

        for (i = 0; i < nbMeshes; ++i)
        {
            nbVertices += meshes[i]->getNbVertices();
            nbFaces += meshes[i]->getNbFaces();
            nbQuads += meshes[i]->getNbQuads();
            nbTriangles += meshes[i]->getNbTriangles();
        }

        arr.nbVertices = nbVertices;
        arr.nbFaces = nbFaces;
        arr.nbQuads = nbQuads;
        arr.nbTriangles = nbTriangles;


        if (!arr.vertices.empty())
            arr.vertices.resize(nbVertices * 3);

        if (!arr.colors.empty())
            arr.colors.resize(nbVertices * 3);

        if (!arr.materials.empty())
            arr.materials.resize(nbVertices * 3);

        if (!arr.faces.empty())
            arr.faces.resize(nbVertices * 4);

        if (!arr.triangles.empty())
            arr.triangles.resize(nbVertices * 3);

        /*
        auto vAr = arr.vertices = arr.vertices != undefined ? new Float32Array(nbVertices * 3) : null;
        auto cAr = arr.colors = arr.colors != undefined ? new Float32Array(nbVertices * 3) : null;
        auto mAr = arr.materials = arr.materials != undefined ? new Float32Array(nbVertices * 3) : null;
        auto fAr = arr.faces = arr.faces != undefined ? new Uint32Array(nbFaces * 4) : null;
        auto iAr = arr.triangles = arr.triangles != undefined ? new Uint32Array(nbTriangles * 3) : null;
        */

        glm::vec3 ver(0.0f);// = [0.0, 0.0, 0.0];
        std::uint32_t offsetVerts = 0;
        std::uint32_t offsetFaces = 0;
        std::uint32_t offsetTris = 0;
        std::uint32_t offsetIndex = 0;

        for (i = 0; i < nbMeshes; ++i)
        {
            //auto mesh = meshes[i];
            Float32Array& mVerts = meshes[i]->getVertices();
            Float32Array& mCols = meshes[i]->getColors();
            Float32Array& mMats = meshes[i]->getMaterials();
            Uint32Array& mFaces = meshes[i]->getFaces();
            Uint32Array& mTris = meshes[i]->getTriangles();

            auto mNbVertices = meshes[i]->getNbVertices();
            auto mNbFaces = meshes[i]->getNbFaces();
            auto mNbTriangles = meshes[i]->getNbTriangles();
            const glm::mat4& matrix = meshes[i]->getMatrix();

            for (j = 0; j < mNbVertices; ++j)
            {
                k = j * 3;
                /*ver[0] = mVerts[k + 0];
                ver[1] = mVerts[k + 1];
                ver[2] = mVerts[k + 2];
                vec3.transformMat4(ver, ver, matrix);*/
                ver = matrix * glm::vec4(mVerts[k + 0], mVerts[k + 1], mVerts[k + 2], 1.0f);

                arr.vertices[offsetVerts + k + 0] = ver[0];
                arr.vertices[offsetVerts + k + 1] = ver[1];
                arr.vertices[offsetVerts + k + 2] = ver[2];

                if (!arr.colors.empty())
                {
                    arr.colors[offsetVerts + k + 0] = mCols[k + 0];
                    arr.colors[offsetVerts + k + 1] = mCols[k + 1];
                    arr.colors[offsetVerts + k + 2] = mCols[k + 2];
                }

                if (!arr.materials.empty())
                {
                    arr.materials[offsetVerts + k + 0] = mMats[k + 0];
                    arr.materials[offsetVerts + k + 1] = mMats[k + 1];
                    arr.materials[offsetVerts + k + 2] = mMats[k + 2];
                }
            }

            offsetVerts += mNbVertices * 3;

            if (!arr.faces.empty())
            {
                for (j = 0; j < mNbFaces; ++j)
                {
                    k = j * 4;
                    arr.faces[offsetFaces + k + 0] = mFaces[k + 0] + offsetIndex;
                    arr.faces[offsetFaces + k + 1] = mFaces[k + 1] + offsetIndex;
                    arr.faces[offsetFaces + k + 2] = mFaces[k + 2] + offsetIndex;
                    arr.faces[offsetFaces + k + 3] = mFaces[k + 3] == Utils::TRI_INDEX ? Utils::TRI_INDEX : mFaces[k + 3] + offsetIndex;
                }
            }

            if (!arr.triangles.empty())
            {
                for (j = 0; j < mNbTriangles; ++j)
                {
                    k = j * 3;
                    arr.triangles[offsetTris + k + 0] = mTris[k + 0] + offsetIndex;
                    arr.triangles[offsetTris + k + 1] = mTris[k + 1] + offsetIndex;
                    arr.triangles[offsetTris + k + 2] = mTris[k + 2] + offsetIndex;
                }
            }

            offsetIndex += mNbVertices;
            offsetFaces += mNbFaces * 4;
            offsetTris += mNbTriangles * 3;
        }

        return arr;
    }

    MeshSPtr Remesh::mergeMeshes(std::vector<MeshSPtr>& meshes, MeshSPtr baseMesh)
    {
        MeshAttribute arr;// = { vertices: null, colors : null, materials : null, faces : null };
        mergeArrays(meshes, arr);
        return createMesh(baseMesh, arr.faces, arr.vertices, arr.colors, arr.materials);
    }
}