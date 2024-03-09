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

        float _cameraSpeed = 0.25;

        // cache canvas stuffs
        float _pixelRatio = 1.0;
        float _viewport;// = document.getElementById('viewport');
        float _canvas;// = document.getElementById('canvas');
        float _canvasWidth = 0;
        float _canvasHeight = 0;
        float _canvasOffsetLeft = 0;
        float _canvasOffsetTop = 0;

        // core of the app
        StateManager* _stateManager = nullptr; // for undo-redo
        SculptManager* _sculptManager = nullptr;
        Camera* _camera = nullptr;
        Picking* _picking = nullptr; // the ray picking
        Picking* _pickingSym = nullptr; // the symmetrical picking

        // TODO primitive builder
        Mesh* _meshPreview = nullptr;
        float _torusLength = 0.5;
        float _torusWidth = 0.1;
        float _torusRadius = Math.PI * 2;
        float _torusRadial = 32;
        float _torusTubular = 128;

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
        bool _focusGui = false; // if the gui is being focused
        //this->_gui = new Gui(this);

        bool _preventRender = false; // prevent multiple render per frame
        bool _drawFullScene = false; // render everything on the rtt
        bool _autoMatrix;// = opts.scalecenter; // scale and center the imported meshes
        bool _vertexSRGB = true; // srgb vs linear colorspace for vertex color

    public:
        Scene()
        {
            _cameraSpeed = 0.25;

            //che canvas stuffs
            _pixelRatio = 1.0f;
            _viewport //= document.getElementById('viewport');
                _canvas;// = document.getElementById('canvas');
            _canvasWidth = 0.0f;
            _canvasHeight = 0.0f;
            _canvasOffsetLeft = 0.0f;
            _canvasOffsetTop = 0.0f;

            // re of the app
            _stateManager = new StateManager(this); // for undo-redo
            _sculptManager = null;
            _camera = new Camera(this);
            _picking = new Picking(this, false); // the ray picking
            _pickingSym = new Picking(this, true); // the symmetrical picking

            //DO primitive builder
            _meshPreview = null;
            _torusLength = 0.5f;
            _torusWidth = 0.1f;
            _torusRadius = glm::pi<float>() * 2.0f;
            _torusRadial = 32.0f;
            _torusTubular = 128.0f;

            //nderable stuffs
            pts = getOptionsURL();
            _showContour = false;// opts.outline;
            _showGrid = opts.grid;
            _grid = nullptr;
            _background = nullptr;
            //_meshes = []; // the meshes
            //_selectMeshes = []; // multi selection
            _mesh = nullptr; // the selected mesh

            _rttContour = nullptr; // rtt for contour
            _rttMerge = nullptr; // rtt decode opaque + merge transparent
            _rttOpaque = nullptr; // rtt half float
            _rttTransparent = nullptr; // rtt rgbm

            // stuffs
            _focusGui = false; // if the gui is being focused
            //_gui = new Gui(this);

            _preventRender = false; // prevent multiple render per frame
            _drawFullScene = false; // render everything on the rtt
            _autoMatrix = false;// opts.scalecenter; // scale and center the imported meshes
            _vertexSRGB = true; // srgb vs linear colorspace for vertex color
        }

        void start()
        {
            _sculptManager = new SculptManager(this);
            _background = new Background(this);

            _rttContour = new Rtt(Enums::Shader::CONTOUR, null);
            _rttMerge = new Rtt(Enums::Shader::MERGE, null);
            _rttOpaque = new Rtt(Enums::Shader::FXAA);
            _rttTransparent = new Rtt(nullptr, this->_rttOpaque.getDepth(), true);

            _grid = Primitives::createGrid();
            initGrid();
            loadTextures();
            //this->_gui.initGui();
            onCanvasResize();

            var modelURL = getOptionsURL().modelurl;
            if (modelURL)
                this->addModelURL(modelURL);
            else
                this->addSphere();
        }

        void addModelURL(const std::string& url)
        {
            /*
            var fileType = this->getFileType(url);
            if (!fileType)
                return;

            var xhr = new XMLHttpRequest();
            xhr.open('GET', url, true);

            xhr.responseType = fileType == 'obj' ? 'text' : 'arraybuffer';

            xhr.onload = function() {
                if (xhr.status == 200)
                    this->loadScene(xhr.response, fileType);
            }.bind(this);

            xhr.send(null);
            */
        }

        inline Background* getBackground()
        {
            return this->_background;
        }

        inline const Background* getBackground() const
        {
            return this->_background;
        }

        getViewport() {
            return this->_viewport;
        }

        getCanvas() {
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

        getGui() {
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

        inline Picking* getSculptManager()
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

        /*
        inline void setCanvasCursor(style)
        {
            this->_canvas.style.cursor = style;
        }
        */

        void initGrid()
        {
            //var grid = this->_grid;
            this->_grid->normalizeSize();
            glm::mat4& gridm = this->_grid->getMatrix();
            gridm = glm::translate(gridm, glm::vec3(0.0f, -0.45f, 0.0f));//mat4.translate(gridm, gridm, [0.0, -0.45, 0.0]);
            float scale = 2.5f;
            gridm = glm::scale(gridm, glm::vec3(scale));//mat4.scale(gridm, gridm, [scale, scale, scale]);
            this->_grid->setShaderType(Enums::Shader::FLAT);
            this->_grid->setFlatColor(glm::vec3(0.04, 0.04, 0.04));
        }

        Mesh* setOrUnsetMesh(Mesh* mesh, bool multiSelect = false)
        {
            if (mesh == nullptr)
            {
                this->_selectMeshes.clear();// = 0;
            }
            else if (!multiSelect)
            {
                this->_selectMeshes.clear();// length = 0;
                this->_selectMeshes.emplace_back(mesh);//push(mesh);
            }
            else
            {
                std::int32_t id = this->getIndexSelectMesh(mesh);

                if (id >= 0)
                {
                    if (this->_selectMeshes.size() > 1)
                    {
                        Utils::splice(this->_selectMeshes, id, 1);//this->_selectMeshes.splice(id, 1);
                        mesh = this->_selectMeshes[0];
                    }
                }
                else
                {
                    this->_selectMeshes.emplace_back(mesh);//.push(mesh);
                }
            }

            this->_mesh = mesh;
            this->getGui().updateMesh();
            this->render();
            return mesh;
        }

        void renderSelectOverRtt()
        {
            if (this->_requestRender())
                this->_drawFullScene = false;
        }

        bool _requestRender()
        {
            if (this->_preventRender == true)
                return false; // render already requested for the next frame

            // window.requestAnimationFrame(this->applyRender.bind(this));
            this->_preventRender = true;
            return true;
        }

        void render()
        {
            this->_drawFullScene = true;
            this->_requestRender();
        }

        void applyRender()
        {
            this->_preventRender = false;
            this->updateMatricesAndSort();

            //var gl = this->_gl;
            //if (!gl) return;

            if (this->_drawFullScene) this->_drawScene();

            glDisable(GL_DEPTH_TEST);//glDisable(gl.DEPTH_TEST);

            glBindFramebuffer(GL_FRAMEBUFFER, this->_rttMerge->getFramebuffer());//gl.bindFramebuffer(gl.FRAMEBUFFER, this->_rttMerge.getFramebuffer());
            this->_rttMerge->render(this); // merge + decode

            // render to screen
            glBindFramebuffer(GL_FRAMEBUFFER, 0);//gl.bindFramebuffer(gl.FRAMEBUFFER, null);

            this->_rttOpaque->render(this); // fxaa

            glEnable(GL_DEPTH_TEST);//gl.enable(gl.DEPTH_TEST);

            this->_sculptManager->postRender(); // draw sculpting gizmo stuffs
        }

        void _drawScene()
        {
            //var gl = this->_gl;
            int i = 0;
            //auto meshes = this->_meshes;
            auto nbMeshes = this->_meshes.size();

            ///////////////
            // CONTOUR 1/2
            ///////////////
            glDisable(GL_DEPTH_TEST);//glDisable(gl.DEPTH_TEST);
            bool showContour = this->_selectMeshes.size() > 0 &&
                this->_showContour &&
                ShaderLib[Enums::Shader::CONTOUR].color[3] > 0.0f;

            if (showContour)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, this->_rttContour->getFramebuffer());
                glClear(GL_COLOR_BUFFER_BIT);
                auto nbSel = this->_selectMeshes.size();
                //for (var s = 0, sel = this->_selectMeshes, nbSel = sel.length; s < nbSel; ++s)
                for (auto s = 0; s < nbSel; ++s)
                    this->_selectMeshes[s]->renderFlatColor(this);
            }

            glEnable(GL_DEPTH_TEST);

            ///////////////
            // OPAQUE PASS
            ///////////////
            glBindFramebuffer(GL_FRAMEBUFFER, this->_rttOpaque->getFramebuffer());
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // grid
            if (this->_showGrid)
                this->_grid->render(this);

            // (post opaque pass)
            for (i = 0; i < nbMeshes; ++i)
            {
                if (this->_meshes[i]->isTransparent()) break;
                this->_meshes[i]->render(this);
            }

            auto startTransparent = i;

            if (this->_meshPreview)
                this->_meshPreview->render(this);

            // background
            this->_background->render();

            ///////////////
            // TRANSPARENT PASS
            ///////////////
            glBindFramebuffer(GL_FRAMEBUFFER, this->_rttTransparent->getFramebuffer());
            glClear(GL_COLOR_BUFFER_BIT);

            glEnable(GL_BLEND);

            // wireframe for dynamic mesh has duplicate edges
            glDepthFunc(GL_LESS);

            for (i = 0; i < nbMeshes; ++i)
            {
                if (_meshes[i]->getShowWireframe())
                    _meshes[i]->renderWireframe(this);
            }

            glDepthFunc(GL_LEQUAL);

            glDepthMask(false);
            glEnable(GL_CULL_FACE);

            for (i = startTransparent; i < nbMeshes; ++i)
            {
                glCullFace(GL_FRONT); // draw back first
                _meshes[i]->render(this);
                glCullFace(GL_BACK); // ... and then front
                _meshes[i]->render(this);
            }

            glDisable(GL_CULL_FACE);

            ///////////////
            // CONTOUR 2/2
            ///////////////
            if (showContour) {
                this->_rttContour->render(this);
            }

            glDepthMask(true);
            glDisable(GL_BLEND);
        }

        /** Pre compute matrices and sort meshes */
        updateMatricesAndSort() {
            var meshes = this->_meshes;
            var cam = this->_camera;
            if (meshes.length > 0) {
                cam.optimizeNearFar(this->computeBoundingBoxScene());
            }

            for (var i = 0, nb = meshes.length; i < nb; ++i) {
                meshes[i].updateMatrices(cam);
            }

            meshes.sort(Mesh.sortFunction);

            if (this->_meshPreview) this->_meshPreview.updateMatrices(cam);
            if (this->_grid) this->_grid.updateMatrices(cam);
        }

        void initWebGL()
        {
            /* ÔÝÊ±×¢ÊÍµô
            var attributes = {
              antialias: false,
              stencil : true
            };

            var canvas = document.getElementById('canvas');
            var gl = this->_gl = canvas.getContext('webgl', attributes) || canvas.getContext('experimental-webgl', attributes);
            if (!gl) {
                window.alert('Could not initialise WebGL. No WebGL, no SculptGL. Sorry.');
                return;
            }

            WebGLCaps.initWebGLExtensions(gl);
            if (!WebGLCaps.getWebGLExtension('OES_element_index_uint'))
                RenderData.ONLY_DRAW_ARRAYS = true;
            */

            glPixelStorei(GL_UNPACK_FLIP_Y, true); //gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
            glPixelStorei(GL_UNPACK_PREMULTIPLY_ALPHA, false);
            glPixelStorei(GL_UNPACK_COLORSPACE_CONVERSION, GL_NONE);

            glDisable(GL_CULL_FACE);
            glFrontFace(GL_CCW);
            glCullFace(GL_BACK);

            glDisable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

            glDisable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            glDepthMask(true);

            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        /** Load textures (preload) */
        void loadTextures()
        {
            /*ÔÝÊ±×¢ÊÍ
            var self = this;
            var gl = this->_gl;
            var ShaderMatcap = ShaderLib[Enums::Shader::MATCAP];

            var loadTex = function(path, idMaterial) {
                var mat = new Image();
                mat.src = path;

                mat.onload = function() {
                    ShaderMatcap.createTexture(gl, mat, idMaterial);
                    self.render();
                };
            };

            for (var i = 0, mats = ShaderMatcap.matcaps, l = mats.length; i < l; ++i)
                loadTex(mats[i].path, i);

            this->initAlphaTextures();
            */
        }

        void initAlphaTextures() {
            var alphas = Picking::INIT_ALPHAS_PATHS;
            var names = Picking::INIT_ALPHAS_NAMES;
            /*
            for (var i = 0, nbA = alphas.length; i < nbA; ++i) {
                var am = new Image();
                am.src = 'resources/alpha/' + alphas[i];
                am.onload = this->onLoadAlphaImage.bind(this, am, names[i]);
            }
            */
        }

        /** Called when the window is resized */
        void onCanvasResize()
        {
            auto viewport = this->_viewport;
            auto newWidth = viewport.clientWidth * this->_pixelRatio;
            auto newHeight = viewport.clientHeight * this->_pixelRatio;

            this->_canvasOffsetLeft = viewport.offsetLeft;
            this->_canvasOffsetTop = viewport.offsetTop;
            this->_canvasWidth = newWidth;
            this->_canvasHeight = newHeight;

            this->_canvas.width = newWidth;
            this->_canvas.height = newHeight;

            glViewport(0, 0, newWidth, newHeight);//this->_gl.viewport(0, 0, newWidth, newHeight);
            this->_camera->onResize(newWidth, newHeight);
            this->_background->onResize(newWidth, newHeight);

            this->_rttContour->onResize(newWidth, newHeight);
            this->_rttMerge->onResize(newWidth, newHeight);
            this->_rttOpaque->onResize(newWidth, newHeight);
            this->_rttTransparent->onResize(newWidth, newHeight);

            this->render();
        }

        float computeRadiusFromBoundingBox(const SixElemArray& box)
        {
            float dx = box[3] - box[0];
            float dy = box[4] - box[1];
            float dz = box[5] - box[2];
            return 0.5 * Math.sqrt(dx * dx + dy * dy + dz * dz);
        }

        SixElemArray computeBoundingBoxMeshes(std::vector<Mesh*>& meshes)
        {
            SixElemArray bound = {
                std::numeric_limits<float>::infinity(),
                std::numeric_limits<float>::infinity() ,
                std::numeric_limits<float>::infinity() ,
                -std::numeric_limits<float>::infinity() ,
                -std::numeric_limits<float>::infinity() ,
                -std::numeric_limits<float>::infinity()
            };
                //var bound = [Infinity, Infinity, Infinity, -Infinity, -Infinity, -Infinity];
            auto l = meshes.size();
            for (auto i = 0; i < l; ++i) 
            {
                if (!meshes[i]->isVisible()) 
                    continue;
                auto bi = meshes[i]->computeWorldBound();
                if (bi[0] < bound[0]) bound[0] = bi[0];
                if (bi[1] < bound[1]) bound[1] = bi[1];
                if (bi[2] < bound[2]) bound[2] = bi[2];
                if (bi[3] > bound[3]) bound[3] = bi[3];
                if (bi[4] > bound[4]) bound[4] = bi[4];
                if (bi[5] > bound[5]) bound[5] = bi[5];
            }
            return bound;
        }

        SixElemArray computeBoundingBoxScene()
        {
            //var scene = this->_meshes.slice();
            scene.push(this->_grid);
            this->_sculptManager.addSculptToScene(scene);
            return this->computeBoundingBoxMeshes(scene);
        }

        normalizeAndCenterMeshes(meshes) {
            var box = this->computeBoundingBoxMeshes(meshes);
            var scale = Utils.SCALE / vec3.dist([box[0], box[1], box[2]], [box[3], box[4], box[5]]);

            var mCen = mat4.create();
            mat4.scale(mCen, mCen, [scale, scale, scale]);
            mat4.translate(mCen, mCen, [-(box[0] + box[3]) * 0.5, -(box[1] + box[4]) * 0.5, -(box[2] + box[5]) * 0.5]);

            for (var i = 0, l = meshes.length; i < l; ++i) {
                var mat = meshes[i].getMatrix();
                mat4.mul(mat, mCen, mat);
            }
        }

        addSphere() {
            // make a cube and subdivide it
            var mesh = new Multimesh(Primitives.createCube(this->_gl));
            mesh.normalizeSize();
            this->subdivideClamp(mesh);
            return this->addNewMesh(mesh);
        }

        addCube() {
            var mesh = new Multimesh(Primitives.createCube(this->_gl));
            mesh.normalizeSize();
            mat4.scale(mesh.getMatrix(), mesh.getMatrix(), [0.7, 0.7, 0.7]);
            this->subdivideClamp(mesh, true);
            return this->addNewMesh(mesh);
        }

        addCylinder() {
            var mesh = new Multimesh(Primitives.createCylinder(this->_gl));
            mesh.normalizeSize();
            mat4.scale(mesh.getMatrix(), mesh.getMatrix(), [0.7, 0.7, 0.7]);
            this->subdivideClamp(mesh);
            return this->addNewMesh(mesh);
        }

        addTorus(preview) {
            var mesh = new Multimesh(Primitives.createTorus(this->_gl, this->_torusLength, this->_torusWidth, this->_torusRadius, this->_torusRadial, this->_torusTubular));
            if (preview) {
                mesh.setShowWireframe(true);
                var scale = 0.3 * Utils.SCALE;
                mat4.scale(mesh.getMatrix(), mesh.getMatrix(), [scale, scale, scale]);
                this->_meshPreview = mesh;
                return;
            }
            mesh.normalizeSize();
            this->subdivideClamp(mesh);
            this->addNewMesh(mesh);
        }

        subdivideClamp(mesh, linear) {
            Subdivision.LINEAR = !!linear;
            while (mesh.getNbFaces() < 50000)
                mesh.addLevel();
            // keep at max 4 multires
            mesh._meshes.splice(0, Math.min(mesh._meshes.length - 4, 4));
            mesh._sel = mesh._meshes.length - 1;
            Subdivision.LINEAR = false;
        }

        addNewMesh(mesh) {
            this->_meshes.push(mesh);
            this->_stateManager.pushStateAdd(mesh);
            this->setMesh(mesh);
            return mesh;
        }

        loadScene(fileData, fileType) {
            var newMeshes;
            if (fileType == 'obj') newMeshes = Import.importOBJ(fileData, this->_gl);
            else if (fileType == 'sgl') newMeshes = Import.importSGL(fileData, this->_gl, this);
            else if (fileType == 'stl') newMeshes = Import.importSTL(fileData, this->_gl);
            else if (fileType == 'ply') newMeshes = Import.importPLY(fileData, this->_gl);

            var nbNewMeshes = newMeshes.length;
            if (nbNewMeshes == 0) {
                return;
            }

            var meshes = this->_meshes;
            for (var i = 0; i < nbNewMeshes; ++i) {
                var mesh = newMeshes[i] = new Multimesh(newMeshes[i]);

                if (!this->_vertexSRGB && mesh.getColors()) {
                    Utils.convertArrayVec3toSRGB(mesh.getColors());
                }

                mesh.init();
                mesh.initRender();
                meshes.push(mesh);
            }

            if (this->_autoMatrix) {
                this->normalizeAndCenterMeshes(newMeshes);
            }

            this->_stateManager.pushStateAdd(newMeshes);
            this->setMesh(meshes[meshes.length - 1]);
            this->resetCameraMeshes(newMeshes);
            return newMeshes;
        }

        clearScene() {
            this->getStateManager().reset();
            this->getMeshes().length = 0;
            this->getCamera().resetView();
            this->setMesh(null);
            this->_action = Enums.Action.NOTHING;
        }

        deleteCurrentSelection() {
            if (!this->_mesh)
                return;

            this->removeMeshes(this->_selectMeshes);
            this->_stateManager.pushStateRemove(this->_selectMeshes.slice());
            this->_selectMeshes.length = 0;
            this->setMesh(null);
        }

        removeMeshes(rm) {
            var meshes = this->_meshes;
            for (var i = 0; i < rm.length; ++i)
                meshes.splice(this->getIndexMesh(rm[i]), 1);
        }

        getIndexMesh(mesh, select) {
            var meshes = select ? this->_selectMeshes : this->_meshes;
            var id = mesh.getID();
            for (var i = 0, nbMeshes = meshes.length; i < nbMeshes; ++i) {
                var testMesh = meshes[i];
                if (testMesh == mesh || testMesh.getID() == id)
                    return i;
            }
            return -1;
        }

        getIndexSelectMesh(mesh) {
            return this->getIndexMesh(mesh, true);
        }

        /** Replace a mesh in the scene */
        replaceMesh(mesh, newMesh) {
            var index = this->getIndexMesh(mesh);
            if (index >= 0) this->_meshes[index] = newMesh;
            if (this->_mesh == mesh) this->setMesh(newMesh);
        }

        duplicateSelection() {
            var meshes = this->_selectMeshes.slice();
            var mesh = null;
            for (var i = 0; i < meshes.length; ++i) {
                mesh = meshes[i];
                var copy = new MeshStatic(mesh.getGL());
                copy.copyData(mesh);

                this->addNewMesh(copy);
            }

            this->setMesh(mesh);
        }

        onLoadAlphaImage(img, name, tool) {
            var can = document.createElement('canvas');
            can.width = img.width;
            can.height = img.height;

            var ctx = can.getContext('2d');
            ctx.drawImage(img, 0, 0);
            var u8rgba = ctx.getImageData(0, 0, img.width, img.height).data;
            var u8lum = u8rgba.subarray(0, u8rgba.length / 4);
            for (var i = 0, j = 0, n = u8lum.length; i < n; ++i, j += 4)
                u8lum[i] = Math.round((u8rgba[j] + u8rgba[j + 1] + u8rgba[j + 2]) / 3);

            name = Picking.addAlpha(u8lum, img.width, img.height, name)._name;

            var entry = {};
            entry[name] = name;
            this->getGui().addAlphaOptions(entry);
            if (tool && tool._ctrlAlpha)
                tool._ctrlAlpha.setValue(name);
        }
    }
}

#endif // digital_sculpt_scene_h__