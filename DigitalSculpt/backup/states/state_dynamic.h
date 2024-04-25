#include "misc/utils.h"
#include "states/state.h"
#include "scene.h"
#include "mesh/mesh.h"
#include "glm/vec3.hpp"

namespace DigitalSculpt
{
    class StateDynamic : public State
    {
    private:
        glm::vec3 _center;
        MeshSPtr _mesh;
        std::uint32_t _nbFacesState;
        std::uint32_t _nbVerticesState;
        Uint32Array _idVertState;// ids of vertices
        Uint32Array _fRingState; // ring of faces around vertices
        Uint32Array _vRingState; // ring of faces around vertices
        Float32Array _vArState;// copies of vertices coordinates
        Float32Array _cArState;// copies of color vertices
        Float32Array _mArState;// copies of material vertices
        Uint32Array _idFaceState;// ids of faces
        Uint32Array _fArState;// copies of face indices
    public:
        StateDynamic(SceneSPtr main, MeshSPtr mesh);

        virtual ~StateDynamic();

        virtual bool isNoop() override;

        virtual void undo() override;

        virtual void redo() override;

        virtual std::shared_ptr<State> createRedo() override;
    private:
        void pushVertices(const Uint32Array& iVerts);

        void pushFaces(const Uint32Array& iFaces);

        void pushRedoVertices(std::shared_ptr<StateDynamic> redoState);

        void pushRedoFaces(std::shared_ptr<StateDynamic> redoState);

        void pullVertices();

        void pullFaces();
    };
}