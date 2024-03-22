#ifndef digital_sculpt_scene_h__
#define digital_sculpt_scene_h__

#include <vector>
#include "misc/enums.h"
#include "mesh/mesh.h"
#include "math3d/camera.h"
#include "math3d/picking.h"
#include "drawables/rtt.h"

namespace DigitalSculpt
{
    class Scene
    {
    protected:
        //this->_gl = null; // webgl context

        float _cameraSpeed;

        // cache canvas stuffs
        float _pixelRatio;
        float _viewport;// = document.getElementById('viewport');
        float _canvas;// = document.getElementById('canvas');
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
        Mesh* _meshPreview;
        float _torusLength;
        float _torusWidth;
        float _torusRadius;
        float _torusRadial;
        float _torusTubular;

        // renderable stuffs
        //var opts = getOptionsURL();
        bool _showContour;// = opts.outline;
        bool _showGrid;// = opts.grid;
        Mesh* _grid;
        Background* _background;
        std::vector<Mesh*> _meshes; // the meshes
        std::vector<Mesh*> _selectMeshes; // multi selection
        Mesh* _mesh = null; // the selected mesh

        Rtt* _rttContour; // rtt for contour
        Rtt* _rttMerge; // rtt decode opaque + merge transparent
        Rtt* _rttOpaque; // rtt half float
        Rtt* _rttTransparent; // rtt rgbm

        // ui stuffs
        bool _focusGui; // if the gui is being focused
        Gui* _gui;// = new Gui(this);

        bool _preventRender; // prevent multiple render per frame
        bool _drawFullScene; // render everything on the rtt
        bool _autoMatrix;// = opts.scalecenter; // scale and center the imported meshes
        bool _vertexSRGB = true; // srgb vs linear colorspace for vertex color

    public:
        Scene();

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

        inline getViewport() 
        {
            return this->_viewport;
        }

        inline getCanvas() 
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

        Gui* getGui()
        {
            return this->_gui;
        }

        inline std::vector<Mesh*>& getMeshes()
        {
            return this->_meshes;
        }

        inline Mesh* getMesh()
        {
            return this->_mesh;
        }

        inline std::vector<Mesh*>& getSelectedMeshes()
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

        inline Picking* getStateManager()
        {
            return this->_stateManager;
        }

        inline Mesh* setMesh(Mesh* mesh)
        {
            return this->setOrUnsetMesh(mesh);
        }

        inline void setCanvasCursor(style)
        {
            this->_canvas.style.cursor = style;
        }
        
        void initGrid();

        Mesh* setOrUnsetMesh(Mesh* mesh, bool multiSelect = false);

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

        SixElemArray computeBoundingBoxMeshes(std::vector<Mesh*>& meshes);

        SixElemArray computeBoundingBoxScene();

        void normalizeAndCenterMeshes(std::vector<Mesh*>& meshes);

        Mesh* addSphere();

        Mesh* addCube();

        Mesh* addCylinder();

        void addTorus(bool preview);

        void subdivideClamp(Mesh* mesh, bool linear);

        void addNewMesh(Mesh* mesh);

        std::vector<Mesh*> loadScene(std::uint8_t* fileData, const std::string& fileType);

        void clearScene();

        void deleteCurrentSelection();

        void removeMeshes(std::vector<Mesh*> rm);

        std::int32_t getIndexMesh(Mesh* mesh, bool select);

        inline std::int32_t getIndexSelectMesh(Mesh* mesh);

        /** Replace a mesh in the scene */
        void replaceMesh(Mesh* mesh, Mesh* newMesh);

        void duplicateSelection();

        void onLoadAlphaImage(img, name, tool);
    }
}

#endif // digital_sculpt_scene_h__