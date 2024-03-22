#include "states/state_color_and_material.h"
#include "misc/utils.h"

namespace DigitalSculpt
{
    StateColorAndMaterial::StateColorAndMaterial(SceneSPtr main, MeshSPtr mesh) : State(main)
    {
        _main = main; // main application
        _mesh = mesh; // the mesh
        _idVertState.clear(); // ids of vertices
        _cArState.clear(); // copies of color vertices
        _mArStat.clear(); // copies of material vertices
    }

    StateColorAndMaterial::~StateColorAndMaterial()
    {
        _main.reset(); // main application
        _mesh.reset(); // the mesh
        _idVertState.clear(); // ids of vertices
        _cArState.clear(); // copies of color vertices
        _mArStat.clear(); // copies of material vertices
    }

    bool StateColorAndMaterial::isNoop()
    {
        return _idVertState.size() == 0;
    }

    void StateColorAndMaterial::undo()
    {
        pullVertices();
        _mesh->updateDuplicateColorsAndMaterials();
        _mesh->updateDrawArrays();
        _mesh->updateColorBuffer();
        _mesh->updateMaterialBuffer();
        _main->setMesh(mesh);
    }

    void StateColorAndMaterial::redo()
    {
        undo();
    }

    std::shared_ptr<State> StateColorAndMaterial::createRedo()
    {
        auto redo = std::make_shared<StateColorAndMaterial>(new StateColorAndMaterial(_main, _mesh));
        pushRedoVertices(redo);
        return redo;
    }

    void StateColorAndMaterial::pushVertices(const Uint32Array& iVerts)
    {
        /*var idVertState = _idVertState;
        var cArState = _cArState;
        var mArState = _mArState;

        var mesh = _mesh;*/
        Float32Array& cAr = _mesh->getColors();
        Float32Array& mAr = _mesh->getMaterials();
        Int32Array& vertStateFlags = _mesh->getVerticesStateFlags();

        auto stateFlag = Utils::STATE_FLAG;
        std::size_t nbVerts = iVerts.size();//var nbVerts = iVerts.length;

        for (std::size_t i = 0; i < nbVerts; ++i)
        {
            std::uint32_t id = iVerts[i];

            if (vertStateFlags[id] == stateFlag)
                continue;

            vertStateFlags[id] = stateFlag;
            _idVertState.push_back(id);
            id *= 3;

            _cArState.push_back(cAr[id + 0]);
            _cArState.push_back(cAr[id + 1]);
            _cArState.push_back(cAr[id + 2]);
            _mArState.push_back(mAr[id + 0]);
            _mArState.push_back(mAr[id + 1]);
            _mArState.push_back(mAr[id + 2]);
        }
    }

    void StateColorAndMaterial::pushRedoVertices(std::shared_ptr<StateColorAndMaterial> redoState)
    {
        Float32Array& cAr = redoState->_mesh->getColors();
        Float32Array& mAr = redoState->_mesh->getMaterials();

        Uint32Array& idVertUndoState = _idVertState;
        std::size_t nbVerts = idVertUndoState.size();//idVertUndoState.length;

        //var cArRedoState = redoState->_cArState = new Float32Array(nbVerts * 3);
        //var mArRedoState = redoState->_mArState = new Float32Array(nbVerts * 3);
        //var idVertRedoState = redoState->_idVertState = new Uint32Array(nbVerts);
        Float32Array& cArRedoState = redoState->_cArState;
        Float32Array& mArRedoState = redoState->_mArState;
        Uint32Array& idVertRedoState = redoState->_idVertState;
        cArRedoState.resize(nbVerts * 3, 0.0f);
        mArRedoState.resize(nbVerts * 3, 0.0f);
        idVertRedoState.resize(nbVerts * 3, 0);

        for (std::size_t i = 0; i < nbVerts; ++i)
        {
            std::uint32_t id = idVertRedoState[i] = idVertUndoState[i];
            id *= 3;
            std::uint32_t j = i * 3;
            cArRedoState[j + 0] = cAr[id + 0];
            cArRedoState[j + 1] = cAr[id + 1];
            cArRedoState[j + 2] = cAr[id + 2];
            mArRedoState[j + 0] = mAr[id + 0];
            mArRedoState[j + 1] = mAr[id + 1];
            mArRedoState[j + 2] = mAr[id + 2];
        }
    }

    void StateColorAndMaterial::pullVertices()
    {
        /* var cArState = _cArState;
         var mArState = _mArState;
         var idVertState = _idVertState;*/
        auto nbVerts = _idVertState.size();

        //var mesh = _mesh;
        Float32Array& cAr = _mesh->getColors();
        Float32Array& mAr = _mesh->getMaterials();

        for (auto i = 0; i < nbVerts; ++i)
        {
            std::uint32_t id = _idVertState[i] * 3;
            std::uint32_t j = i * 3;
            cAr[id + 0] = _cArState[j + 0];
            cAr[id + 1] = _cArState[j + 1];
            cAr[id + 2] = _cArState[j + 2];
            mAr[id + 0] = _mArState[j + 0];
            mAr[id + 1] = _mArState[j + 1];
            mAr[id + 2] = _mArState[j + 2];
        }
    }
}
