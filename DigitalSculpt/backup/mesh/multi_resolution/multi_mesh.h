#ifndef digital_sculpt_multi_mesh_h__
#define digital_sculpt_multi_mesh_h__

#include <cstdint>
#include <vector>

#include "../klib/kgl_lib_pch.h"

#include "mesh/multi_resolution/mesh_resolution.h"
#include "mesh/mesh.h"
#include "render/buffer.h"
#include "editing/subdivision.h"
#include "editing/reversion.h"

namespace DigitalSculpt
{
    class Multimesh : public Mesh
    {
    private:
        static std::uint32_t RENDER_HINT;
    private:
        std::vector<MeshResolutionSPtr> _meshes;
        Buffer* _indexBuffer;
        Buffer* _wireframeBuffer;
        std::uint32_t _sel;
    public:
        static std::uint32_t NONE()
        {
            return 0;
        }

        static std::uint32_t SCULPT()
        {
            return 1;
        }

        static std::uint32_t CAMERA()
        {
            return 2;
        }

        static std::uint32_t PICKING()
        {
            return 3;
        }

        Multimesh(MeshSPtr mesh);

        inline MeshSPtr getCurrentMesh()
        {
            return this->_meshes[this->_sel];
        }

        void setSelection(std::uint32_t sel);

        MeshSPtr addLevel();

        MeshSPtr computeReverse();

        MeshSPtr lowerLevel();

        MeshSPtr higherLevel();

        void updateResolution();

        void selectResolution(std::uint32_t sel);

        std::uint32_t findIndexFromMesh(MeshSPtr mesh);

        void selectMesh(MeshSPtr mesh);

        void pushMesh(MeshSPtr mesh);

        void unshiftMesh(MeshSPtr mesh);

        void popMesh();

        void shiftMesh();

        void deleteLower();

        void deleteHigher();

        void getLowIndexRender();

        void _renderLow(Scene* main);

        void _renderWireframeLow(Scene* main);

        bool _canUseLowRender(Scene* main);

        virtual void render(Scene* main) override;

        virtual void renderWireframe(Scene* main) override;
    };
}

#endif // digital_sculpt_multi_mesh_h__
