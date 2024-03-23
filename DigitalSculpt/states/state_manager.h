#include <vector>
#include <functional>
#include "states/state.h"
#include "misc/utils.h"
#include "misc/enums.h"
#include "scene.h"

namespace DigitalSculpt
{
    class StateManager
    {
    private:
        int STACK_LENGTH = 15;

        SceneSPtr _main;
        std::vector<State*> _undos;// = []; // undo actions
        std::vector<State*> _redos;// = []; // redo actions
        int _curUndoIndex = -1; // current index in undo

    public:
        StateManager(SceneSPtr main);

        void pushStateCustom(std::function<void(void)> undocb, std::function<void(void)> redocb, bool squash);

        void pushStateAddRemove(std::vector<MeshSPtr>& addMesh, std::vector<MeshSPtr>& remMesh, bool squash);

        void pushStateRemove(std::vector<MeshSPtr>& remMesh);

        void pushStateAdd(std::vector<MeshSPtr>& addMesh);

        void pushStateColorAndMaterial(MeshSPtr mesh);

        void pushStateGeometry(MeshSPtr mesh);

        void pushStateMultiresolution(multimesh, Enums::Action type);

        void setNewMaxStack(int maxStack);
    private:
        void pushState(State* state);

        State* getCurrentState();

        void pushVertices(const Uint32Array& iVerts);

        void pushFaces(const Uint32Array& iFaces);

        void undo();

        void redo();

        void reset();

        void cleanNoop();
    };


}