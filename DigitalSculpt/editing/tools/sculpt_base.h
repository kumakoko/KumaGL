#ifndef sculpt_base_h__
#define sculpt_base_h__

#include <cmath>
#include "glm/vec3.hpp"
#include "./misc/enums.h"
#include "./misc/utils.h"

// Overview sculpt :
// start (check if we hit the mesh, start state stack) -> startSculpt
// startSculpt (init stuffs specific to the tool) -> sculptStroke

// sculptStroke (handle sculpt stroke by throttling/smoothing stroke) -> makeStroke
// makeStroke (handle symmetry and picking before sculping) -> stroke
// stroke (tool specific, move vertices, etc)

// update -> sculptStroke
namespace DigitalSculpt
{
    class SculptBase
    {
    protected:
        SceneSPtr _main;
        float _lastMouseX;
        float _lastMouseY;
        MeshSPtr _forceToolMesh;
        bool _lockPosition;
        float _radius;
    public:
        SculptBase(SceneSPtr main)
        {
            this->_main = main;
            this->_cbContinuous = this->updateContinuous.bind(this); // callback continuous
            this->_lastMouseX = 0.0f;
            this->_lastMouseY = 0.0f;
        }

        void setToolMesh(MeshSPtr mesh) {
            // to be called when we create a new instance of a tool operator
            // that is no part of the main application Sculpt container (e.g smooth)
            this->_forceToolMesh = mesh;
        }

        MeshSPtr getMesh()
        {
            if (this->_forceToolMesh != nullptr)
                return _forceToolMesh;
            return _main->getMesh();
            //return this->_forceToolMesh || this->_main->getMesh();
        }

        bool start(ctrl)
        {
            //auto main = this->_main;
            auto picking = _main->getPicking();

            if (!picking.intersectionMouseMeshes())
                return false;

            auto mesh = _main->setOrUnsetMesh(picking.getMesh(), ctrl);
            if (mesh == nullptr)
                return false;

            picking.initAlpha();
            auto pickingSym = _main->getSculptManager().getSymmetry() ? _main->getPickingSymmetry() : nullptr;

            if (pickingSym != nullptr)
            {
                pickingSym->intersectionMouseMesh(mesh);
                pickingSym->initAlpha();
            }

            this->pushState();
            this->_lastMouseX = _main->_mouseX;
            this->_lastMouseY = _main->_mouseY;
            this->startSculpt();

            return true;
        }

        void end()
        {
            auto mesh = this->getMesh();

            if (nullptr != mesh)
                mesh->balanceOctree();
        }

        void pushState()
        {
            this->_main->getStateManager().pushStateGeometry(this->getMesh());
        }

        void startSculpt()
        {
            if (this->_lockPosition)
                return;

            this->sculptStroke();
        }

        void preUpdate(bool canBeContinuous)
        {
            //auto main = this->_main;
            auto picking = _main->getPicking();
            auto isSculpting = _main->_action == Enums::Action::SCULPT_EDIT;

            if (isSculpting && !canBeContinuous)
                return;

            if (isSculpting)
                picking.intersectionMouseMesh();
            else
                picking.intersectionMouseMeshes();

            auto mesh = picking.getMesh();
            if (mesh != nullptr && _main->getSculptManager()->getSymmetry())
                _main->getPickingSymmetry().intersectionMouseMesh(mesh);
        }

        void update(bool continuous)
        {
            if (this->_lockPosition)
                return this->updateSculptLock(continuous);

            this->sculptStroke();
        }

        /** Update lock position */
        void updateSculptLock(bool continuous)
        {
            //auto main = this->_main;

            if (!continuous)
                this->_main->getStateManager().getCurrentState().undo(); // tricky

            auto picking = _main->getPicking();
            auto origRad = this->_radius;
            auto pickingSym = _main->getSculptManager().getSymmetry() ? _main->getPickingSymmetry() : null;

            float dx = _main->_mouseX - this->_lastMouseX;
            float dy = _main->_mouseY - this->_lastMouseY;
            this->_radius = sqrt(dx * dx + dy * dy);//Math.sqrt(dx * dx + dy * dy);
            // it's a bit hacky... I just simulate another stroke with a very small offset
            // so that the stroke still has a direction (the mask can be rotated correctly then)
            auto offx = dx / this->_radius;
            auto offy = dy / this->_radius;
            this->makeStroke(this->_lastMouseX + offx * 1e-3, this->_lastMouseY + offy * 1e-3, picking, pickingSym);
            this->_radius = origRad;

            this->updateRender();
            _main->setCanvasCursor("default");
        }

        sculptStroke() {
            auto main = this->_main;
            auto picking = _main->getPicking();
            auto pickingSym = _main->getSculptManager().getSymmetry() ? _main->getPickingSymmetry() : nullptr;

            auto dx = _main->_mouseX - this->_lastMouseX;
            auto dy = _main->_mouseY - this->_lastMouseY;
            auto dist = std::sqrt(dx * dx + dy * dy);
            auto minSpacing = 0.15 * this->_radius * _main->getPixelRatio();

            if (dist <= minSpacing)
                return;

            auto step = 1.0 / std::floor(dist / minSpacing);
            dx *= step;
            dy *= step;
            auto mouseX = this->_lastMouseX + dx;
            auto mouseY = this->_lastMouseY + dy;

            for (auto i = step; i <= 1.0; i += step) {
                if (!this->makeStroke(mouseX, mouseY, picking, pickingSym))
                    break;
                mouseX += dx;
                mouseY += dy;
            }

            this->updateRender();

            this->_lastMouseX = _main->_mouseX;
            this->_lastMouseY = _main->_mouseY;
        }

        void updateRender()
        {
            this->updateMeshBuffers();
            this->_main->render();
        }

        void makeStroke(float mouseX, float mouseY, PickingSPtr picking, PickingSPtr pickingSym)
        {
            auto mesh = this->getMesh();
            picking.intersectionMouseMesh(mesh, mouseX, mouseY);
            auto pick1 = picking.getMesh();

            if (pick1 != nullptr)
            {
                picking.pickVerticesInSphere(picking.getLocalRadius2());
                picking.computePickedNormal();
            }

            // if dyn topo, we need to the picking and the sculpting altogether
            auto dynTopo = mesh.isDynamic && !this->_lockPosition;

            if (dynTopo && pick1 != nullptr)
            {
                this->stroke(picking, false);
            }

            PickingSPtr pick2;

            if (pickingSym != null)
            {
                pickingSym->intersectionMouseMesh(mesh, mouseX, mouseY);
                pick2 = pickingSym->getMesh();
                if (pick2) {
                    pickingSym->setLocalRadius2(picking.getLocalRadius2());
                    pickingSym->pickVerticesInSphere(pickingSym->getLocalRadius2());
                    pickingSym->computePickedNormal();
                }
            }

            if (!dynTopo && pick1 != nullptr)
                this->stroke(picking, false);

            if (pick2 != nullptr)
                this->stroke(pickingSym, true);

            return pick1 != nullptr || pick2 != nullptr;
        }

        void updateMeshBuffers()
        {
            auto mesh = this->getMesh();
            if (mesh.isDynamic)
                mesh.updateBuffers();
            else
                mesh.updateGeometryBuffers();
        }

        bool updateContinuous()
        {
            if (this->_lockPosition)
                return this->update(true);

            //auto main = this->_main;
            auto picking = _main->getPicking();
            auto pickingSym = _main->getSculptManager().getSymmetry() ? _main->getPickingSymmetry() : null;
            this->makeStroke(_main->_mouseX, _main->_mouseY, picking, pickingSym);
            this->updateRender();
        }

        /** Return the vertices that point toward the camera */
        UInt32Array getFrontVertices(const UInt32Array& iVertsInRadius, const glm::vec3& eyeDir)
        {
            auto nbVertsSelected = iVertsInRadius.size();//iVertsInRadius.length;
            UInt32Array iVertsFront;//auto iVertsFront = new Uint32Array(Utils.getMemory(4 * nbVertsSelected), 0, nbVertsSelected);
            auto acc = 0;
            auto nAr = this->getMesh().getNormals();
            auto eyeX = eyeDir[0];
            auto eyeY = eyeDir[1];
            auto eyeZ = eyeDir[2];

            for (auto i = 0; i < nbVertsSelected; ++i)
            {
                auto id = iVertsInRadius[i];
                auto j = id * 3;
                if ((nAr[j] * eyeX + nAr[j + 1] * eyeY + nAr[j + 2] * eyeZ) <= 0.0f)
                    iVertsFront[acc++] = id;
            }

            return Utils::subarray(iVertsFront, 0, acc);//new Uint32Array(iVertsFront.subarray(0, acc));
        }

        /** Compute average normal of a group of vertices with culling */
        glm::vec3 areaNormal(const UInt32Array& iVerts)
        {
            auto mesh = this->getMesh();
            auto nAr = mesh.getNormals();
            auto mAr = mesh.getMaterials();
            float anx = 0.0f;
            float any = 0.0f;
            float anz = 0.0f;
            auto l = iVerts.size();

            for (auto i = 0; i < l; ++i)
            {
                auto ind = iVerts[i] * 3;
                auto f = mAr[ind + 2];
                anx += nAr[ind] * f;
                any += nAr[ind + 1] * f;
                anz += nAr[ind + 2] * f;
            }

            auto len = std::sqrt(anx * anx + any * any + anz * anz);

            if (len == 0.0f)
                return;

            len = 1.0f / len;

            return glm::vec3(anx * len, any * len, anz * len);
        }

        /** Compute average center of a group of vertices (with culling) */
        glm::vec3 areaCenter(const UInt32Array& iVerts)
        {
            auto mesh = this->getMesh();
            auto vAr = mesh.getVertices();
            auto mAr = mesh.getMaterials();
            auto nbVerts = iVerts.size();//iVerts.length;
            float ax = 0.0f;
            float ay = 0.0f;
            float az = 0.0f;
            auto acc = 0;
            for (auto i = 0; i < nbVerts; ++i) {
                auto ind = iVerts[i] * 3;
                auto f = mAr[ind + 2];
                acc += f;
                ax += vAr[ind] * f;
                ay += vAr[ind + 1] * f;
                az += vAr[ind + 2] * f;
            }
            return glm::vec3(ax / acc, ay / acc, az / acc);
        }

        /** Updates the vertices original coords that are sculpted for the first time in this stroke */
        void updateProxy(const UInt32Array& iVerts)
        {
            auto mesh = this->getMesh();
            auto vAr = mesh.getVertices();
            auto vProxy = mesh.getVerticesProxy();
            
            if (vAr == vProxy)
                return;

            auto vertStateFlags = mesh.getVerticesStateFlags();
            auto stateFlag = Utils::STATE_FLAG;
            auto l = iVerts.size();
            
            for (auto i = 0; i < l; ++i)
            {
                auto id = iVerts[i];
                
                if (vertStateFlags[id] != stateFlag) 
                {
                    auto ind = id * 3;
                    vProxy[ind + 0] = vAr[ind + 0];
                    vProxy[ind + 1] = vAr[ind + 1];
                    vProxy[ind + 2] = vAr[ind + 2];
                }
            }
        }

        /** Laplacian smooth. Special rule for vertex on the edge of the mesh. */
        void laplacianSmooth(const UInt32Array& iVerts, smoothVerts, vField) {
            auto mesh = this->getMesh();
            auto vrvStartCount = mesh.getVerticesRingVertStartCount();
            auto vertRingVert = mesh.getVerticesRingVert();
            auto ringVerts = vertRingVert instanceof Array ? vertRingVert : null;
            auto vertOnEdge = mesh.getVerticesOnEdge();
            auto vAr = vField || mesh.getVertices();
            auto nbVerts = iVerts.length;

            for (auto i = 0; i < nbVerts; ++i) {
                auto i3 = i * 3;
                auto id = iVerts[i];

                auto start, end;
                if (ringVerts) {
                    vertRingVert = ringVerts[id];
                    start = 0;
                    end = vertRingVert.length;
                }
                else {
                    start = vrvStartCount[id * 2];
                    end = start + vrvStartCount[id * 2 + 1];
                }

                auto idv = 0;
                auto vcount = end - start;
                if (vcount <= 2) {
                    idv = id * 3;
                    smoothVerts[i3] = vAr[idv];
                    smoothVerts[i3 + 1] = vAr[idv + 1];
                    smoothVerts[i3 + 2] = vAr[idv + 2];
                    continue;
                }

                auto avx = 0.0;
                auto avy = 0.0;
                auto avz = 0.0;
                auto j = 0;

                if (vertOnEdge[id] == = 1) {
                    auto nbVertEdge = 0;
                    for (j = start; j < end; ++j) {
                        idv = vertRingVert[j];
                        // we average only with vertices that are also on the edge
                        if (vertOnEdge[idv] == = 1) {
                            idv *= 3;
                            avx += vAr[idv];
                            avy += vAr[idv + 1];
                            avz += vAr[idv + 2];
                            ++nbVertEdge;
                        }
                    }

                    if (nbVertEdge >= 2) {
                        smoothVerts[i3] = avx / nbVertEdge;
                        smoothVerts[i3 + 1] = avy / nbVertEdge;
                        smoothVerts[i3 + 2] = avz / nbVertEdge;
                        continue;
                    }
                    avx = avy = avz = 0.0;
                }

                for (j = start; j < end; ++j) {
                    idv = vertRingVert[j] * 3;
                    avx += vAr[idv];
                    avy += vAr[idv + 1];
                    avz += vAr[idv + 2];
                }

                smoothVerts[i3] = avx / vcount;
                smoothVerts[i3 + 1] = avy / vcount;
                smoothVerts[i3 + 2] = avz / vcount;
            }
        }

        UInt32Array dynamicTopology(PickingSPtr picking)
        {
            auto mesh = this->getMesh();
            auto iVerts = picking.getPickedVertices();
            if (!mesh.isDynamic)
                return iVerts;

            auto subFactor = mesh.getSubdivisionFactor();
            auto decFactor = mesh.getDecimationFactor();
            if (subFactor == = 0.0 && decFactor == = 0.0)
                return iVerts;

            if (iVerts.length == = 0) {
                iVerts = mesh.getVerticesFromFaces([picking.getPickedFace()]);
                // undo-redo
                this->_main->getStateManager().pushVertices(iVerts);
            }

            auto iFaces = mesh.getFacesFromVertices(iVerts);
            auto radius2 = picking.getLocalRadius2();
            auto center = picking.getIntersectionPoint();
            auto d2Max = radius2 * (1.1 - subFactor) * 0.2;
            auto d2Min = (d2Max / 4.2025) * decFactor;

            // undo-redo
            this->_main->getStateManager().pushFaces(iFaces);

            if (subFactor)
                iFaces = mesh.subdivide(iFaces, center, radius2, d2Max, this->_main->getStateManager());
            if (decFactor)
                iFaces = mesh.decimate(iFaces, center, radius2, d2Min, this->_main->getStateManager());
            iVerts = mesh.getVerticesFromFaces(iFaces);

            auto nbVerts = iVerts.length;
            auto sculptFlag = Utils.SCULPT_FLAG;
            auto vscf = mesh.getVerticesSculptFlags();
            auto iVertsInRadius = new Uint32Array(Utils.getMemory(nbVerts * 4), 0, nbVerts);
            auto acc = 0;
            for (auto i = 0; i < nbVerts; ++i) {
                auto iVert = iVerts[i];
                if (vscf[iVert] == sculptFlag)
                    iVertsInRadius[acc++] = iVert;
            }

            iVertsInRadius = new Uint32Array(iVertsInRadius.subarray(0, acc));
            mesh.updateTopology(iFaces, iVerts);
            mesh.updateGeometry(iFaces, iVerts);

            return iVertsInRadius;
        }

        getUnmaskedVertices() {
            return this->filterMaskedVertices(0.0, Infinity);
        }

        getMaskedVertices() {
            return this->filterMaskedVertices(-Infinity, 1.0);
        }

        filterMaskedVertices(lowerBound = -Infinity, upperBound = Infinity) {
            auto mesh = this->getMesh();
            auto mAr = mesh.getMaterials();
            auto nbVertices = mesh.getNbVertices();
            auto cleaned = new Uint32Array(Utils.getMemory(4 * nbVertices), 0, nbVertices);
            auto acc = 0;
            for (auto i = 0; i < nbVertices; ++i) {
                auto mask = mAr[i * 3 + 2];
                if (mask > lowerBound && mask < upperBound)
                    cleaned[acc++] = i;
            }
            return new Uint32Array(cleaned.subarray(0, acc));
        }

        void postRender(selection) {
            selection.render(this->_main);
        }

        addSculptToScene()
        {
        }

        getScreenRadius()
        {
            return (this->_radius || 1) * this->_main->getPixelRatio();
        }
    }
}

#endif // sculpt_base_h__