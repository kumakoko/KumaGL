#include "misc/utils.h"
#include "states/state_dynamic.h"
#include "scene.h"
#include "mesh/mesh.h"
#include "glm/vec3.hpp"

namespace DigitalSculpt
{
    StateDynamic::StateDynamic(SceneSPtr main, MeshSPtr mesh) :State(main)
    {
        this->_mesh = mesh; // the mesh
        this->_center = mesh->getCenter();

        this->_nbFacesState = mesh->getNbFaces(); // number of faces
        this->_nbVerticesState = mesh->getNbVertices(); // number of vertices

        this->_idVertState.clear(); // ids of vertices
        this->_fRingState.clear(); // ring of faces around vertices
        this->_vRingState.clear(); // ring of faces around vertices
        this->_vArState.clear(); // copies of vertices coordinates
        this->_cArState.clear(); // copies of color vertices
        this->_mArState.clear(); // copies of material vertices
        this->_idFaceState.clear(); // ids of faces
        this->_fArState.clear(); // copies of face indices
    }

    StateDynamic::~StateDynamic()
    {
        _mesh.reset();
    }

    bool StateDynamic::isNoop()
    {
        return this->_idVertState.size() == 0 && this->_idFaceState.size() == 0;
    }

    bool StateDynamic::undo()
    {
        this->pullVertices();
        this->pullFaces();

        // mesh.getVerticesRingFace().length = this->_nbVerticesState;
        // mesh.getVerticesRingVert().length = this->_nbVerticesState;
        _mesh->setNbVertices(this->_nbVerticesState);
        _mesh->setNbFaces(this->_nbFacesState);

        _mesh->updateTopology( /*this->_idFaceState, this->_idVertState*/); // TODO local update ?
        _mesh->updateGeometry( /*this->_idFaceState, this->_idVertState*/); // TODO local update ?
        _mesh->updateDuplicateColorsAndMaterials();
        _mesh->updateDrawArrays();
        _mesh->updateColorBuffer();
        _mesh->updateMaterialBuffer();
        _mesh->updateBuffers();
        _mesh->getCenter() = _center; //vec3.copy(mesh.getCenter(), this->_center);
        this->_main->setMesh(mesh);
    }

    void StateDynamic::redo()
    {
        this->undo();
    }

    std::shared_ptr<State> StateDynamic::createRedo()
    {
        auto redo = std::make_shared<StateDynamic>(new StateDynamic(this->_main, this->_mesh));
        this->pushRedoVertices(redo);
        this->pushRedoFaces(redo);
        return redo;
    }

    void StateDynamic::pushVertices(const Uint32Array& iVerts)
    {
        /*auto idVertState = this->_idVertState;
        auto fRingState = this->_fRingState;
        auto vRingState = this->_vRingState;
        auto vArState = this->_vArState;
        auto cArState = this->_cArState;
        auto mArState = this->_mArState;*/

        //auto mesh = this->_mesh;
        auto fRing = _mesh->getVerticesRingFace();
        auto vRing = _mesh->getVerticesRingVert();
        Float32Array& vAr = _mesh->getVertices();
        Float32Array& cAr = _mesh->getColors();
        Float32Array& mAr = _mesh->getMaterials();
        Int32Array& vStateFlags = _mesh->getVerticesStateFlags();

        auto stateFlag = Utils::STATE_FLAG;
        auto nbVerts = iVerts.size();

        for (auto i = 0; i < nbVerts; ++i)
        {
            auto id = iVerts[i];

            if (vStateFlags[id] == = stateFlag)
                continue;

            vStateFlags[id] = stateFlag;
            _fRingState.push_back(fRing[id].slice());
            _vRingState.push_back(vRing[id].slice());
            _idVertState.push_back(id);
            id *= 3;
            _vArState.push_back(vAr[id], vAr[id + 1], vAr[id + 2]);
            _cArState.push_back(cAr[id], cAr[id + 1], cAr[id + 2]);
            _mArState.push_back(mAr[id], mAr[id + 1], mAr[id + 2]);
        }
    }

    void StateDynamic::pushFaces(const Uint32Array& iFaces)
    {
        /*auto idFaceState = this->_idFaceState;
        auto fArState = this->_fArState;
        auto mesh = this->_mesh;*/

        Uint32Array& fAr = mesh->getFaces();
        auto fStateFlags = mesh->getFacesStateFlags();
        auto stateFlag = Utils::STATE_FLAG;
        auto nbFaces = iFaces.size();

        for (auto i = 0; i < nbFaces; ++i)
        {
            auto id = iFaces[i];

            if (fStateFlags[id] == stateFlag)
                continue;

            fStateFlags[id] = stateFlag;
            _idFaceState.push_back(id);
            id *= 4;

            _fArState.push_back(fAr[id + 0]);
            _fArState.push_back(fAr[id + 1]);
            _fArState.push_back(fAr[id + 2]);
            _fArState.push_back(fAr[id + 3]);
        }
    }

    void StateDynamic::pushRedoVertices(std::shared_ptr<StateDynamic> redoState)
    {
        MeshSPtr mesh = redoState->_mesh;
        std::uint32_t nbMeshVertices = mesh->getNbVertices();
        IUint32ArrayND* fRing = mesh->getVerticesRingFace();
        IUint32ArrayND* vRing = mesh->getVerticesRingVert();
        Float32Array& vAr = mesh->getVertices();
        Float32Array& cAr = mesh->getColors();
        Float32Array& mAr = mesh->getMaterials();

        auto i = 0;
        auto id = 0;
        auto acc = 0;
        //auto idVertUndoState = this->_idVertState;
        auto nbVerts = _idVertState.size();
        //auto nbVerticesState = this->_nbVerticesState;
        std::uint32_t nbMin = Math.min(_nbVerticesState, nbMeshVertices);
        Uint32Array tmp_idVertRedoState(nbMeshVertices * 4, 0);//auto idVertRedoState = new Uint32Array(Utils.getMemory(nbMeshVertices * 4), 0, nbMeshVertices);
        for (i = 0; i < nbVerts; ++i) {
            id = _idVertState[i];
            if (id < nbMin)
                tmp_idVertRedoState[acc++] = id;
        }

        for (i = _nbVerticesState; i < nbMeshVertices; ++i) {
            tmp_idVertRedoState[acc++] = i;
        }

        nbVerts = acc;
        //idVertRedoState = redoState->_idVertState = new Uint32Array(idVertRedoState.subarray(0, nbVerts));
        redoState->_idVertState = Utils::subarray(tmp_idVertRedoState, 0, nbVerts);
        Uint32Array& idVertRedoState = redoState->_idVertState;

        auto fRingRedoState = redoState->_fRingState = new Array(nbVerts);
        auto vRingRedoState = redoState->_vRingState = new Array(nbVerts);

        redoState->_vArState.resize(nbVerts * 3, 0.0f);// = new Float32Array(nbVerts * 3);
        redoState->_cArState.resize(nbVerts * 3, 0.0f);// = new Float32Array(nbVerts * 3);
        redoState->_mArState.resize(nbVerts * 3, 0.0f);// = new Float32Array(nbVerts * 3);

        Float32Array& vArRedoState = redoState->_vArState;
        Float32Array& cArRedoState = redoState->_cArState;
        Float32Array& mArRedoState = redoState->_mArState;

        for (i = 0; i < nbVerts; ++i)
        {
            id = idVertRedoState[i];
            fRingRedoState[i] = fRing[id].slice();
            vRingRedoState[i] = vRing[id].slice();
            id *= 3;
            auto j = i * 3;
            vArRedoState[j + 0] = vAr[id + 0];
            vArRedoState[j + 1] = vAr[id + 1];
            vArRedoState[j + 2] = vAr[id + 2];
            cArRedoState[j + 0] = cAr[id + 0];
            cArRedoState[j + 1] = cAr[id + 1];
            cArRedoState[j + 2] = cAr[id + 2];
            mArRedoState[j + 0] = mAr[id + 0];
            mArRedoState[j + 1] = mAr[id + 1];
            mArRedoState[j + 2] = mAr[id + 2];
        }
    }

    void StateDynamic::pushRedoFaces(std::shared_ptr<StateDynamic> redoState)
    {
        auto mesh = redoState->_mesh;
        auto nbMeshFaces = mesh.getNbFaces();
        auto fAr = mesh.getFaces();

        auto i = 0;
        auto id = 0;
        auto acc = 0;
        auto idFaceUndoState = this->_idFaceState;
        auto nbFaces = idFaceUndoState.length;
        auto nbFacesState = this->_nbFacesState;
        auto nbMin = Math.min(nbFacesState, nbMeshFaces);
        auto idFaceRedoState = new Uint32Array(Utils.getMemory(nbMeshFaces * 4), 0, nbMeshFaces);
        for (i = 0; i < nbFaces; ++i) {
            id = idFaceUndoState[i];
            if (id < nbMin)
                idFaceRedoState[acc++] = id;
        }
        for (i = nbFacesState; i < nbMeshFaces; ++i) {
            idFaceRedoState[acc++] = i;
        }

        nbFaces = acc;
        idFaceRedoState = redoState->_idFaceState = new Uint32Array(idFaceRedoState.subarray(0, nbFaces));
        auto fArRedoState = redoState->_fArState = new Int32Array(nbFaces * 4);
        for (i = 0; i < nbFaces; ++i)
        {
            id = idFaceRedoState[i];
            id *= 4;
            auto j = i * 4;
            fArRedoState[j + 0] = fAr[id + 0];
            fArRedoState[j + 1] = fAr[id + 1];
            fArRedoState[j + 2] = fAr[id + 2];
            fArRedoState[j + 3] = fAr[id + 3];
        }
    }

    void StateDynamic::pullVertices()
    {
        std::size_t nbVerts = _idVertState.size();
        IUint32ArrayND* fRing = _mesh->getVerticesRingFace();
        IUint32ArrayND* vRing = _mesh->getVerticesRingVert();
        Float32Array& vAr = _mesh->getVertices();
        Float32Array& cAr = _mesh->getColors();
        Float32Array& mAr = _mesh->getMaterials();

        for (auto i = 0; i < nbVerts; ++i)
        {
            auto id = idVertState[i];

            if (id >= _nbVerticesState)
                continue;

            fRing[id] = _fRingState[i].slice();
            vRing[id] = _vRingState[i].slice();

            id *= 3;
            auto j = i * 3;

            vAr[id + 0] = vArState[j + 0];
            vAr[id + 1] = vArState[j + 1];
            vAr[id + 2] = vArState[j + 2];
            cAr[id + 0] = cArState[j + 0];
            cAr[id + 1] = cArState[j + 1];
            cAr[id + 2] = cArState[j + 2];
            mAr[id + 0] = mArState[j + 0];
            mAr[id + 1] = mArState[j + 1];
            mAr[id + 2] = mArState[j + 2];
        }
    }

    void StateDynamic::pullFaces()
    {
        /* auto nbMeshFaces = this->_nbFacesState;
         auto fArState = this->_fArState;
         auto idFaceState = this->_idFaceState;*/
        auto nbFaces = _idFaceState.size();

        // auto mesh = this->_mesh;
        Uint32Array& fAr = _mesh->getFaces();

        for (auto i = 0; i < nbFaces; ++i)
        {
            auto id = _idFaceState[i];
            if (id >= _nbFacesState)
                continue;
            id *= 4;
            auto j = i * 4;
            fAr[id + 0] = _fArState[j + 0];
            fAr[id + 1] = _fArState[j + 1];
            fAr[id + 2] = _fArState[j + 2];
            fAr[id + 3] = _fArState[j + 3];
        }
    }
}