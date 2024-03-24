#ifndef digital_sculpt_hole_filling_h__
#define digital_sculpt_hole_filling_h__

#include <vector>
#include <memory>
#include <cmath>
#include "glm/vec3.hpp"

#include "misc/utils.h"
#include "mesh/mesh_static/mesh_static.h"
#include "mesh/mesh.h"

namespace DigitalSculpt
{
    struct Edge
    {
    public:
        std::shared_ptr<Edge> previous;
        std::shared_ptr<Edge> next;
        std::uint32_t v1;
        std::uint32_t v2;
        float angle;

        Edge(std::uint32_t v1, std::uint32_t v2)
        {
            previous = std::make_shared<Edge>(nullptr);
            next = std::make_shared<Edge>(nullptr);
            this->v1 = v1;
            this->v2 = v2;
            angle = 0.0f;
        }
    };

    typedef std::shared_ptr<Edge> EdgeSPtr;

    class HoleFilling
    {
    public:
        static EdgeSPtr detectHole(std::vector<EdgeSPtr>& borderEdges)
        {
            if (borderEdges.size() <= 2)
                return;

            auto nbEdges = borderEdges.size();
            std::uint32_t iEnd = borderEdges[0]->v1;
            std::uint32_t iLast = borderEdges[0]->v2;
            EdgeSPtr first = borderEdges[0];
            EdgeSPtr last = first;

            borderEdges[0] = borderEdges[--nbEdges];
            auto i = 0;

            while (i < nbEdges)
            {
                auto testEdge = borderEdges[i];

                if (testEdge->v1 == iLast)
                {
                    testEdge->previous = last;
                    last->next = testEdge;
                    last = testEdge;
                    iLast = borderEdges[i]->v2;
                    borderEdges[i] = borderEdges[--nbEdges];
                    if (iLast == iEnd)
                        break;
                    i = 0;
                }
                else
                    i++;
            }

            Utils::SetVectorLength<EdgeSPtr>(borderEdges, nbEdges, std::make_shared<Edge>(nullptr));//borderEdges.length = nbEdges;

            if (iLast != iEnd)
                return;

            first->previous = last;
            last->next = first;

            return first;
        }

        static std::vector<EdgeSPtr> detectHoles(MeshSPtr mesh)
        {
            Uint8Array& eAr = mesh->getEdges();
            Uint32Array& fAr = mesh->getFaces();
            Uint32Array& feAr = mesh->getFaceEdges();
            std::uint32_t len = mesh->getNbFaces();
            std::vector<EdgeSPtr> borderEdges;//auto borderEdges = [];

            for (auto i = 0; i < len; ++i)
            {
                std::uint32_t id = i * 4;
                std::uint32_t iv4 = feAr[id + 3];
                bool isQuad = iv4 != Utils::TRI_INDEX;

                if (eAr[feAr[id + 0]] == 1)
                    borderEdges.emplace_back(std::make_shared<Edge>(fAr[id], fAr[id + 1]));
                if (eAr[feAr[id + 1]] == 1)
                    borderEdges.emplace_back(std::make_shared<Edge>(fAr[id + 1], fAr[id + 2]));
                if (eAr[feAr[id + 2]] == 1)
                    borderEdges.emplace_back(std::make_shared<Edge>(fAr[id + 2], fAr[isQuad ? id + 3 : id]));
                if (isQuad && eAr[iv4] == 1)
                    borderEdges.emplace_back(std::make_shared<Edge>(fAr[id + 3], fAr[id]));
            }

            std::vector<EdgeSPtr> holes;

            while (true)
            {
                auto firstEdge = detectHole(borderEdges);

                if (!firstEdge)
                    break;

                holes.emplace_back(firstEdge);
            }

            return holes;
        };

        static void advancingFrontMesh(MeshSPtr mesh, EdgeSPtr firstEdge, Uint32Array& newTris, Float32Array& newVerts, Float32Array& newColors, Float32Array& newMaterials)
        {
            Float32Array& vAr = mesh->getVertices();
            Float32Array& cAr = mesh->getColors();
            Float32Array& mAr = mesh->getMaterials();
            // auto current = firstEdge;
            // auto count = 1;
            // while (current.next !== firstEdge) {
            //   current = current.next;
            //   count++;
            // }
            // console.log(count)

            // TODO : stupid naive hole filling for now
            auto last = mesh->getNbVertices() + newVerts.size() / 3;
            EdgeSPtr current = firstEdge;

            float avx = 0.0f;
            float avy = 0.0f;
            float avz = 0.0f;

            float colr = 0.0f;
            float colg = 0.0f;
            float colb = 0.0f;

            float mat1 = 0.0f;
            float mat2 = 0.0f;
            float mat3 = 0.0f;
            auto count = 0;

            do
            {
                EdgeSPtr next = current->next;
                std::uint32_t iv1 = current->v1;
                std::uint32_t iv2 = current->v2;
                std::uint32_t iv3 = next->v2;

                //newTris.push(iv1, iv2, last, Utils::TRI_INDEX);
                newTris.push_back(iv1);
                newTris.push_back(iv2);
                newTris.push_back(last);
                newTris.push_back(Utils::TRI_INDEX);

                iv1 *= 3;
                iv2 *= 3;
                iv3 *= 3;

                count++;

                avx += vAr[iv1 + 0];
                avy += vAr[iv1 + 1];
                avz += vAr[iv1 + 2];

                colr += cAr[iv1 + 0];
                colg += cAr[iv1 + 1];
                colb += cAr[iv1 + 2];

                mat1 += mAr[iv1 + 0];
                mat2 += mAr[iv1 + 1];
                mat3 += mAr[iv1 + 2];

                float v2x = vAr[iv2 + 0];
                float v2y = vAr[iv2 + 1];
                float v2z = vAr[iv2 + 2];

                // compute normals
                float ax = vAr[iv1 + 0] - v2x;
                float ay = vAr[iv1 + 1] - v2y;
                float az = vAr[iv1 + 2] - v2z;
                float bx = vAr[iv3 + 0] - v2x;
                float by = vAr[iv3 + 1] - v2y;
                float bz = vAr[iv3 + 2] - v2z;
                float alen = ax * ax + ay * ay + az * az;
                float blen = bx * bx + by * by + bz * bz;

                current->angle = std::acosf((ax * bx + ay * by + az * bz) / std::sqrt(alen * blen));
                current = next;
            } 
            while (current != firstEdge);

            newVerts.push_back(avx / count);
            newVerts.push_back(avy / count);
            newVerts.push_back(avz / count);

            newColors.push_back(colr / count);
            newColors.push_back(colg / count);
            newColors.push_back(colb / count);

            newMaterials.push_back(mat1 / count);
            newMaterials.push_back(mat2 / count);
            newMaterials.push_back(mat3 / count);

            //newVerts.push(avx / count, avy / count, avz / count);
            //newColors.push(colr / count, colg / count, colb / count);
            //newMaterials.push(mat1 / count, mat2 / count, mat3 / count);
        }

        auto createMesh = function(mesh, vertices, faces, colors, materials) {
            auto newMesh = new MeshStatic();
            newMesh.setID(mesh.getID());
            newMesh.setVertices(vertices);
            if (colors) newMesh.setColors(colors);
            if (materials) newMesh.setMaterials(materials);
            newMesh.setFaces(faces);

            // small hack
            newMesh.setTransformData(mesh.getTransformData());
            newMesh.setRenderData(mesh.getRenderData());

            Mesh.OPTIMIZE = false;
            newMesh.init();
            Mesh.OPTIMIZE = true;

            return newMesh;
        };

        auto closeHoles = function(mesh) {
            auto holes = detectHoles(mesh);
            if (holes.length == 0)
                return mesh;

            auto newFaces = [];
            auto newVerts = [];
            auto newColors = [];
            auto newMaterials = [];
            // console.time('closeHoles');
            for (auto i = 0, nbHoles = holes.length; i < nbHoles; ++i)
                advancingFrontMesh(mesh, holes[i], newFaces, newVerts, newColors, newMaterials);
            // console.timeEnd('closeHoles');

            auto oldVertsLen = mesh.getNbVertices() * 3;
            auto newVertsLen = oldVertsLen + newVerts.length;

            // set vertices
            auto vertices = new Float32Array(newVertsLen);
            vertices.set(mesh.getVertices().subarray(0, oldVertsLen));
            // set colors
            auto colors = new Float32Array(newVertsLen);
            colors.set(mesh.getColors().subarray(0, oldVertsLen));
            // set materials
            auto materials = new Float32Array(newVertsLen);
            materials.set(mesh.getMaterials().subarray(0, oldVertsLen));

            if (newVertsLen > oldVertsLen) {
                vertices.set(newVerts, oldVertsLen);
                colors.set(newColors, oldVertsLen);
                materials.set(newMaterials, oldVertsLen);
            }

            // set faces
            auto faces = new Uint32Array(mesh.getNbFaces() * 4 + newFaces.length);
            faces.set(mesh.getFaces());
            if (newFaces.length > 0)
                faces.set(newFaces, mesh.getNbFaces() * 4);

            return createMesh(mesh, vertices, faces, colors, materials);
        };

        auto HoleFilling = {};

        HoleFilling.createClosedMesh = function(mesh) {
            auto closed = closeHoles(mesh);
            if (closed == mesh) {
                auto lenv = mesh.getNbVertices() * 3;
                auto lenf = mesh.getNbFaces() * 4;
                auto faces = new Uint32Array(mesh.getFaces().subarray(0, lenf));
                auto vertices = new Float32Array(mesh.getVertices().subarray(0, lenv));
                auto colors = new Float32Array(mesh.getColors().subarray(0, lenv));
                auto materials = new Float32Array(mesh.getMaterials().subarray(0, lenv));
                closed = createMesh(mesh, vertices, faces, colors, materials);
            }
            return closed;
        };
    };
}

#endif // digital_sculpt_hole_filling_h__
