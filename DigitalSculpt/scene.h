#ifndef digital_sculpt_scene_h__
#define digital_sculpt_scene_h__

#include <memory>
#include <vector>

#include "../klib/kgl_image_file_reader.h"
#include "glm/mat4x4.hpp"
#include "boost/intrusive_ptr.hpp"
#include "reference_counter.h"

#include "misc/enums.h"
#include "mesh/mesh.h"
#include "math3d/camera.h"
#include "math3d/picking.h"
#include "drawables/rtt.h"
#include "drawables/back_ground.h"
#include "states/state_manager.h"
#include "editing/sculpt_manager.h"
#include "render/canvas.h"
#include "render/view_port.h"

namespace DigitalSculpt
{
    class Scene : public ReferenceCounter
    {
    protected:
        //this->_gl = null; // webgl context

        float _cameraSpeed;

        // cache canvas stuffs
        float _pixelRatio;
        Viewport _viewport;// = document.getElementById('viewport');
        Canvas _canvas;// = document.getElementById('canvas');
        float _canvasWidth = 0;
        float _canvasHeight = 0;
        float _canvasOffsetLeft = 0;
        float _canvasOffsetTop = 0;

        // core of the app
        StateManager* _stateManager; // for undo-redo
        SculptManager* _sculptManager;
        Camera* _camera;
        Picking* _picking; // the ray picking
        Picking* _pickingSym; // the symmetrical picking

        // TODO primitive builder
        MeshSPtr _meshPreview;
        float _torusLength;
        float _torusWidth;
        float _torusRadius;
        float _torusRadial;
        float _torusTubular;

        // renderable stuffs
        //var opts = getOptionsURL();
        bool _showContour;// = opts.outline;
        bool _showGrid;// = opts.grid;
        MeshSPtr _grid;
        Background* _background;
        std::vector<MeshSPtr> _meshes; // the meshes
        std::vector<MeshSPtr> _selectMeshes; // multi selection
        MeshSPtr _mesh;// = null; // the selected mesh

        Rtt* _rttContour; // rtt for contour
        Rtt* _rttMerge; // rtt decode opaque + merge transparent
        Rtt* _rttOpaque; // rtt half float
        Rtt* _rttTransparent; // rtt rgbm

        // ui stuffs
        bool _focusGui; // if the gui is being focused
        //Gui* _gui;// = new Gui(this);

        bool _preventRender; // prevent multiple render per frame
        bool _drawFullScene; // render everything on the rtt
        bool _autoMatrix;// = opts.scalecenter; // scale and center the imported meshes
        bool _vertexSRGB = true; // srgb vs linear colorspace for vertex color

        Scene();
    public:
        virtual ~Scene();

        void start();

        void addModelURL(const std::string& url);

        inline Background* getBackground()
        {
            return this->_background;
        }

        inline const Background* getBackground() const
        {
            return this->_background;
        }

        inline Viewport& getViewport() 
        {
            return this->_viewport;
        }

        inline const Viewport& getViewport() const
        {
            return this->_viewport;
        }

        inline Canvas& getCanvas() 
        {
            return this->_canvas;
        }

        inline const Canvas& getCanvas() const
        {
            return this->_canvas;
        }

        inline float getPixelRatio() const
        {
            return this->_pixelRatio;
        }

        inline const float getCanvasWidth() const
        {
            return this->_canvasWidth;
        }

        inline float getCanvasHeight() const
        {
            return this->_canvasHeight;
        }

        inline Camera* getCamera()
        {
            return this->_camera;
        }

        inline const Camera* getCamera() const
        {
            return this->_camera;
        }

        /*
        Gui* getGui()
        {
            return this->_gui;
        }
        */

        inline std::vector<MeshSPtr>& getMeshes()
        {
            return this->_meshes;
        }

        inline MeshSPtr getMesh()
        {
            return this->_mesh;
        }

        inline std::vector<MeshSPtr>& getSelectedMeshes()
        {
            return this->_selectMeshes;
        }

        inline Picking* getPicking()
        {
            return this->_picking;
        }

        inline Picking* getPickingSymmetry()
        {
            return this->_pickingSym;
        }

        inline SculptManager* getSculptManager()
        {
            return this->_sculptManager;
        }

        inline StateManager* getStateManager()
        {
            return this->_stateManager;
        }

        inline MeshSPtr setMesh(MeshSPtr mesh)
        {
            return this->setOrUnsetMesh(mesh);
        }

        inline void setCanvasCursor(style)
        {
            this->_canvas.style.cursor = style;
        }
        
        void initGrid();

        MeshSPtr setOrUnsetMesh(MeshSPtr mesh, bool multiSelect = false);

        void renderSelectOverRtt();

        bool _requestRender();

        void render();

        void applyRender();

        void _drawScene();

        /** Pre compute matrices and sort meshes */
        void updateMatricesAndSort();

        void initWebGL();

        /** Load textures (preload) */
        void loadTextures();

        void initAlphaTextures();

        /** Called when the window is resized */
        void onCanvasResize();

        float computeRadiusFromBoundingBox(const SixElemArray& box);

        SixElemArray computeBoundingBoxMeshes(std::vector<MeshSPtr>& meshes);

        SixElemArray computeBoundingBoxScene();

        void normalizeAndCenterMeshes(std::vector<MeshSPtr>& meshes);

        MeshSPtr addSphere();

        MeshSPtr addCube();

        MeshSPtr addCylinder();

        void addTorus(bool preview);

        void subdivideClamp(MeshSPtr mesh, bool linear);

        void addNewMesh(MeshSPtr mesh);

        std::vector<MeshSPtr> loadScene(std::uint8_t* fileData, const std::string& fileType);

        void clearScene();

        void deleteCurrentSelection();

        void removeMeshes(std::vector<MeshSPtr> rm);

        std::int32_t getIndexMesh(MeshSPtr mesh, bool select);

        inline std::int32_t getIndexSelectMesh(MeshSPtr mesh);

        /** Replace a mesh in the scene */
        void replaceMesh(MeshSPtr mesh, MeshSPtr newMesh);

        void duplicateSelection();

        void onLoadAlphaImage(const kgl::ImageFileReader& img, name, tool);
    };

    typedef boost::intrusive_ptr<Scene> SceneSPtr;
}

#endif // digital_sculpt_scene_h__