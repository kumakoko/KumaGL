#include <vector>
#include <cstdint>
#include <memory>
#include "scene.h"
#include "mesh/mesh.h"
#include "states/state.h"
#include "misc/utils.h"

namespace DigitalSculpt
{
    class StateColorAndMaterial : public State
    {
    private:
        Uint32Array _idVertState; // ids of vertices
        Float32Array _cArState; // copies of color vertices
        Float32Array _mArState; // copies of material vertices
        MeshSPtr _mesh; // the mesh
    public:
        StateColorAndMaterial(SceneSPtr main, MeshSPtr mesh);

        virtual ~StateColorAndMaterial();

        virtual bool isNoop() override;

        virtual void undo() override;

        virtual void redo() override;

        virtual std::shared_ptr<State> createRedo() override;

    private:
        void pushVertices(const Uint32Array& iVerts);

        void pushRedoVertices(std::shared_ptr<StateColorAndMaterial> redoState);

        void pullVertices();
    };

}
