#include "states/state.h"
#include "states/state_add_remove.h";
#include "states/state_color_and_material.h";
#include "states/state_geometry.h";
#include "states/state_dynamic.h";
#include "states/state_multi_resolution.h";
#include "states/state_custom.h";
#include "misc/utils.h"
#include "scene.h"

namespace DigitalSculpt
{
    StateManager::StateManager(SceneSPtr main)
    {
        this->_main = main; // main
        this->_undos = []; // undo actions
        this->_redos = []; // redo actions
        this->_curUndoIndex = -1; // current index in undo
    }

    void StateManager::pushStateCustom(std::function<void(void)> undocb, std::function<void(void)> redocb, bool squash)
    {
        auto st = new StateCustom(undocb, redocb);
        st.squash = squash;
        this->pushState(st);
    }

    void StateManager::pushStateAddRemove(std::vector<MeshSPtr>& addMesh, std::vector<MeshSPtr>& remMesh, bool squash)
    {
        auto st = new StateAddRemove(this->_main, addMesh, remMesh);
        st.squash = squash;
        this->pushState(st);
    }

    void StateManager::pushStateRemove(std::vector<MeshSPtr>& remMesh)
    {
        this->pushState(new StateAddRemove(this->_main, [], remMesh));
    }

    void StateManager::pushStateAdd(std::vector<MeshSPtr>& addMesh) 
    {
        this->pushState(new StateAddRemove(this->_main, addMesh, []));
    }

    void StateManager::pushStateColorAndMaterial(MeshSPtr mesh) 
    {
        if (mesh.isDynamic)
            this->pushState(new StateDynamic(this->_main, mesh));
        else
            this->pushState(new StateColorAndMaterial(this->_main, mesh));
    }

    void StateManager::pushStateGeometry(MeshSPtr mesh) 
    {
        if (mesh.isDynamic)
            this->pushState(new StateDynamic(this->_main, mesh));
        else
            this->pushState(new StateGeometry(this->_main, mesh));
    }

    void StateManager::pushStateMultiresolution(multimesh, Enums::Action type)
    {
        this->pushState(new StateMultiResolution(this->_main, multimesh, type));
    }

   void StateManager::setNewMaxStack(int maxStack) 
   {
        StateManager.STACK_LENGTH = maxStack;
        auto undos = this->_undos;
        auto redos = this->_redos;

        while (this->_curUndoIndex >= maxStack) 
        {
            undos.shift();
            --this->_curUndoIndex;
        }

        while (undos.length > maxStack)
        {
            undos.pop();
            redos.shift();
        }
    }

    void StateManager::pushState(State* state) 
    {
        ++Utils.STATE_FLAG;
        auto undos = this->_undos;
        if (this->_curUndoIndex == = -1) undos.length = 0;
        else if (undos.length >= StateManager.STACK_LENGTH) {
            undos.shift();
            --this->_curUndoIndex;
        }
        this->_redos.length = 0;
        ++this->_curUndoIndex;
        if (undos.length > 0)
            undos.length = this->_curUndoIndex;
        undos.push(state);
    }

    void StateManager::getCurrentState() 
    {
        return this->_undos[this->_curUndoIndex];
    }

    void StateManager::pushVertices(iVerts) 
    {
        if (iVerts && iVerts.length > 0)
            this->getCurrentState().pushVertices(iVerts);
    }

    void StateManager::pushFaces(iFaces) 
    {
        if (iFaces && iFaces.length > 0)
            this->getCurrentState().pushFaces(iFaces);
    }

    void StateManager::undo()
    {
        if (!this->_undos.length || this->_curUndoIndex < 0)
            return;

        auto state = this->getCurrentState();
        auto redoState = state.createRedo();
        redoState.squash = state.squash;
        this->_redos.push(redoState);
        state.undo();

        this->_curUndoIndex--;
        if (state.squash == = true)
            this->undo();
    }

    void StateManager::redo() 
    {
        if (!this->_redos.length)
            return;

        auto state = this->_redos[this->_redos.length - 1];
        state.redo();
        this->_curUndoIndex++;
        this->_redos.pop();
        if (this->_redos.length&& this->_redos[this->_redos.length - 1].squash == true)
            this->redo();
    }

    void StateManager::reset()
    {
        this->_undos.length = 0;
        this->_redos.length = 0;
        this->_curUndoIndex = -1;
    }

    void StateManager::cleanNoop()
    {
        while (this->_curUndoIndex >= 0 && this->getCurrentState().isNoop()) {
            this->_undos.length--;
            this->_curUndoIndex--;
            this->_redos.length = 0;
        }
    }



}