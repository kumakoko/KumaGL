#include "misc/utils.h"
#include "states/state.h"
#include "scene.h"
#include "mesh/mesh.h"
#include "glm/vec3.hpp"

namespace DigitalSculpt
{
    class StateGeometry :public State
    {
    private:
        MeshSPtr _mesh;
        glm::vec3 _center;
        Uint32Array _idVertState;
        Float32Array _vArState;
    public:
        StateGeometry(SceneSPtr main, MeshSPtr mesh);

        virtual ~StateGeometry();

        virtual bool isNoop() override;

        virtual bool undo() override;

        virtual bool redo() override;

        virtual std::shared_ptr<State> createRedo() override;
    private:
        void pushVertices(const Uint32Array& iVerts);

        void pushRedoVertices(std::shared_ptr<StateGeometry> redoState);

        void pullVertices();
    };
}
