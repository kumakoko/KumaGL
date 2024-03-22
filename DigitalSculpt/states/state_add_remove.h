#include <vector>
#include "mesh/mesh.h"
#include "scene.h"
#include "states/state.h"

namespace DigitalSculpt
{
    class StateAddRemove : public State
    {
    private:
        std::vector<MeshSPtr> _addedMeshes;
        std::vector<MeshSPtr> _removedMeshes;
        std::vector<MeshSPtr> _selectMeshes;
    public:
        StateAddRemove(SceneSPtr main, std::vector<MeshSPtr>& addedMeshes, std::vector<MeshSPtr>& removedMeshes);
        
        virtual ~StateAddRemove();

        virtual bool isNoop() override;
       
        virtual void undo() override;

        virtual void redo() override;

        virtual std::shared_ptr<State> createRedo() override;
    };
}
