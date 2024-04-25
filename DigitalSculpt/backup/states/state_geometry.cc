#include "states/state_geometry.h"

namespace DigitalSculpt
{

    StateGeometry::StateGeometry(SceneSPtr main, MeshSPtr mesh) :State(main)
    {
        _main = main; // main application
        _mesh = mesh; // the mesh
        _center = mesh.getCenter();

        _idVertState.clear(); // ids of vertices
        _vArState.clear(); // copies of vertices coordinates
    }

    StateGeometry::~StateGeometry()
    {
    }

    bool StateGeometry::isNoop()
    {
        return _idVertState.length == 0;
    }

    bool StateGeometry::undo()
    {
        pullVertices();
        //auto mesh = _mesh;
        _mesh->updateGeometry(mesh.getFacesFromVertices(_idVertState), _idVertState);
        _mesh->updateGeometryBuffers();
        _mesh->getCenter() = _center;// vec3.copy(mesh.getCenter(), _center);
        _main->setMesh(_mesh);
    }

    bool StateGeometry::redo()
    {
        undo();
    }

    std::shared_ptr<State> StateGeometry::createRedo()
    {
        auto redo = std::make_shared<StateGeometry>(new StateGeometry(_main, _mesh));
        pushRedoVertices(redo);
        return redo;
    }

    void StateGeometry::pushVertices(const Uint32Array& iVerts)
    {
        auto vAr = _mesh->getVertices();
        auto vertStateFlags = _mesh->getVerticesStateFlags();
        auto nbVerts = iVerts.size();

        for (auto i = 0; i < nbVerts; ++i)
        {
            auto id = iVerts[i];

            if (vertStateFlags[id] == Utils::STATE_FLAG)
                continue;

            vertStateFlags[id] = Utils::STATE_FLAG;
            _idVertState.push_back(id);
            id *= 3;

            _vArState.push_back(vAr[id + 0]);
            _vArState.push_back(vAr[id + 1]);
            _vArState.push_back(vAr[id + 2]);
        }
    }

    void StateGeometry::pushRedoVertices(std::shared_ptr<StateGeometry> redoState)
    {
        auto vAr = _mesh->getVertices();
        auto nbVerts = _idVertState.size();

        Float32Array& vArRedoState = redoState->_vArState;// = new Float32Array(nbVerts * 3);
        vArRedoState.resize(nbVerts * 3, 0.0f);// = new Float32Array(nbVerts * 3);

        Uint32Array& idVertRedoState = redoState->_idVertState;// = new Uint32Array(nbVerts);
        idVertRedoState.resize(nbVerts, 0);

        for (auto i = 0; i < nbVerts; ++i)
        {
            auto id = idVertRedoState[i] = _idVertState[i];
            id *= 3;
            auto j = i * 3;
            vArRedoState[j + 0] = vAr[id + 0];
            vArRedoState[j + 1] = vAr[id + 1];
            vArRedoState[j + 2] = vAr[id + 2];
        }
    }

    void StateGeometry::pullVertices()
    {
        auto nbVerts = _idVertState.size();
        Float32Array& vAr = _mesh->getVertices();

        for (auto i = 0; i < nbVerts; ++i)
        {
            auto id = _idVertState[i] * 3;
            auto j = i * 3;
            vAr[id + 0] = _vArState[j + 0];
            vAr[id + 1] = _vArState[j + 1];
            vAr[id + 2] = _vArState[j + 2];
        }
    }
}
