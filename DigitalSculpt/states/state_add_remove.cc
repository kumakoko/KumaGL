#include "states/state_add_remove.h"

namespace DigitalSculpt
{
    StateAddRemove::StateAddRemove(SceneSPtr main, std::vector<MeshSPtr>& addedMeshes, std::vector<MeshSPtr>& removedMeshes)
    {
        this->_addedMeshes = addedMeshes.length != = undefined ? addedMeshes : [addedMeshes] ; // the added meshes
        this->_removedMeshes = removedMeshes.length != = undefined ? removedMeshes : [removedMeshes] ; // the deleted meshes
        this->_selectMeshes = main.getSelectedMeshes().slice();
    }

    StateAddRemove::~StateAddRemove()
    {
    }

    bool StateAddRemove::isNoop()
    {
        return this->_addedMeshes.size() == 0 && this->_removedMeshes.size() == 0;
    }

    void StateAddRemove::undo()
    {
        //var main = this->_main;
        auto meshesMain = _main->getMeshes();
        //var addMeshes = this->_addedMeshes;

        std::size_t i;
        std::size_t l = _addMeshes.size();
        for (i = 0, i < l; ++i)
            meshesMain.splice(main.getIndexMesh(addMeshes[i]), 1);

        //var remMeshes = this->_removedMeshes;
        l = _removedMeshes.size();
        for (i = 0, i < l; ++i)
            meshesMain.emplace_back(_removedMeshes[i]);

        l = meshesMain.size();
        for (i = 0, i < l; ++i)
        {
            meshesMain[i]->initRender();
        }

        //var sel = this->_selectMeshes;
        main->setMesh(_selectMeshes[0] ? sel[0] : std::make_shared<Mesh>(nullptr));
        auto sMeshes = main->getSelectedMeshes();
        sMeshes.clear();//sMeshes.length = 0;
        l = _selectMeshes.size();

        for (i = 0; i < l; ++i)
            sMeshes.emplace_back(sel[i]);
    }

    void StateAddRemove::redo()
    {
        this->undo();
    }

    std::shared_ptr<State> StateAddRemove::createRedo()
    {
        return std::make_shared<StateAddRemove>(new StateAddRemove(this->_main, this->_removedMeshes, this->_addedMeshes));
    }
}