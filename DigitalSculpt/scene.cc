#include "scene.h"

namespace DigitalSculpt
{
    Scene::Scene()
    {
        _cameraSpeed = 0.25;

        //che canvas stuffs
        _pixelRatio = 1.0f;
       // _viewport = 0.0f; //= document.getElementById('viewport');
        //   _canvas;// = document.getElementById('canvas');
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
        //pts = getOptionsURL();
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

    Scene::~Scene()
    {
    }

    void Scene::start()
    {
        _sculptManager = new SculptManager(this);
        _background = new Background(this);

        _rttContour = new Rtt(Enums::Shader::CONTOUR, 0, false);
        _rttMerge = new Rtt(Enums::Shader::MERGE, 0, 0, false);

        GLuint rbOpaque = 0;
        glCreateRenderbuffers(1, &rbOpaque);
        _rttOpaque = new Rtt(Enums::Shader::FXAA, rbOpaque, false);
        _rttTransparent = new Rtt(nullptr, _rttOpaque->getDepth(), true);

        _grid = Primitives::createGrid();
        initGrid();
        loadTextures();
        //_gui.initGui();
        onCanvasResize();

        var modelURL = getOptionsURL().modelurl;
        if (modelURL)
            addModelURL(modelURL);
        else
            addSphere();
    }

    void Scene::addModelURL(const std::string& url)
    {
        /*
        var fileType = getFileType(url);
        if (!fileType)
            return;

        var xhr = new XMLHttpRequest();
        xhr.open('GET', url, true);

        xhr.responseType = fileType == 'obj' ? 'text' : 'arraybuffer';

        xhr.onload = function() {
            if (xhr.status == 200)
                loadScene(xhr.response, fileType);
        }.bind(this);

        xhr.send(null);
        */
    }

    void Scene::initGrid()
    {
        //var grid = _grid;
        _grid->normalizeSize();
        glm::mat4& gridm = _grid->getMatrix();
        gridm = glm::translate(gridm, glm::vec3(0.0f, -0.45f, 0.0f));//mat4.translate(gridm, gridm, [0.0, -0.45, 0.0]);
        float scale = 2.5f;
        gridm = glm::scale(gridm, glm::vec3(scale));//mat4.scale(gridm, gridm, [scale, scale, scale]);
        _grid->setShaderType(Enums::Shader::FLAT);
        _grid->setFlatColor(glm::vec3(0.04, 0.04, 0.04));
    }

    MeshSPtr Scene::setOrUnsetMesh(MeshSPtr mesh, bool multiSelect = false)
    {
        if (mesh == nullptr)
        {
            _selectMeshes.clear();// = 0;
        }
        else if (!multiSelect)
        {
            _selectMeshes.clear();// length = 0;
            _selectMeshes.emplace_back(mesh);//push(mesh);
        }
        else
        {
            std::int32_t id = getIndexSelectMesh(mesh);

            if (id >= 0)
            {
                if (_selectMeshes.size() > 1)
                {
                    Utils::splice(_selectMeshes, id, 1);//_selectMeshes.splice(id, 1);
                    mesh = _selectMeshes[0];
                }
            }
            else
            {
                _selectMeshes.emplace_back(mesh);//.push(mesh);
            }
        }

        _mesh = mesh;
        //getGui().updateMesh();
        render();
        return mesh;
    }

    void Scene::renderSelectOverRtt()
    {
        if (_requestRender())
            _drawFullScene = false;
    }

    bool Scene::_requestRender()
    {
        if (_preventRender == true)
            return false; // render already requested for the next frame

        // window.requestAnimationFrame(applyRender.bind(this));
        _preventRender = true;
        return true;
    }

    void Scene::render()
    {
        _drawFullScene = true;
        _requestRender();
    }

    void Scene::applyRender()
    {
        _preventRender = false;
        updateMatricesAndSort();

        //var gl = _gl;
        //if (!gl) return;

        if (_drawFullScene) _drawScene();

        glDisable(GL_DEPTH_TEST);//glDisable(gl.DEPTH_TEST);

        glBindFramebuffer(GL_FRAMEBUFFER, _rttMerge->getFramebuffer());//gl.bindFramebuffer(gl.FRAMEBUFFER, _rttMerge.getFramebuffer());
        _rttMerge->render(this); // merge + decode

        // render to screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);//gl.bindFramebuffer(gl.FRAMEBUFFER, null);

        _rttOpaque->render(this); // fxaa

        glEnable(GL_DEPTH_TEST);//gl.enable(gl.DEPTH_TEST);

        _sculptManager->postRender(); // draw sculpting gizmo stuffs
    }

    void Scene::_drawScene()
    {
        //var gl = _gl;
        int i = 0;
        //auto meshes = _meshes;
        auto nbMeshes = _meshes.size();

        ///////////////
        // CONTOUR 1/2
        ///////////////
        glDisable(GL_DEPTH_TEST);//glDisable(gl.DEPTH_TEST);
        bool showContour = _selectMeshes.size() > 0 &&
            _showContour &&
            ShaderLib[Enums::Shader::CONTOUR]->color[3] > 0.0f;

        if (showContour)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, _rttContour->getFramebuffer());
            glClear(GL_COLOR_BUFFER_BIT);
            auto nbSel = _selectMeshes.size();
            //for (var s = 0, sel = _selectMeshes, nbSel = sel.length; s < nbSel; ++s)
            for (auto s = 0; s < nbSel; ++s)
                _selectMeshes[s]->renderFlatColor(this);
        }

        glEnable(GL_DEPTH_TEST);

        ///////////////
        // OPAQUE PASS
        ///////////////
        glBindFramebuffer(GL_FRAMEBUFFER, _rttOpaque->getFramebuffer());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // grid
        if (_showGrid)
            _grid->render(this);

        // (post opaque pass)
        for (i = 0; i < nbMeshes; ++i)
        {
            if (_meshes[i]->isTransparent()) break;
            _meshes[i]->render(this);
        }

        auto startTransparent = i;

        if (_meshPreview)
            _meshPreview->render(this);

        // background
        _background->render();

        ///////////////
        // TRANSPARENT PASS
        ///////////////
        glBindFramebuffer(GL_FRAMEBUFFER, _rttTransparent->getFramebuffer());
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
            _rttContour->render(this);
        }

        glDepthMask(true);
        glDisable(GL_BLEND);
    }

    /** Pre compute matrices and sort meshes */
    void Scene::updateMatricesAndSort()
    {
        //var meshes = _meshes;
        //var cam = _camera;
        if (_meshes.size() > 0)
        {
            _camera->optimizeNearFar(computeBoundingBoxScene());
        }

        std::size_t nb = _meshes.size();
        for (std::size_t i = 0; i < nb; ++i)
        {
            _meshes[i]->updateMatrices(*_camera);
        }

        //_meshes.sort(Mesh.sortFunction);
        std::sort(_meshes.begin(), _meshes.end(), [](MeshSPtr meshA, MeshSPtr meshB)
            {
                bool aTr = meshA->isTransparent();
                bool bTr = meshB->isTransparent();
                if (aTr && !bTr)
                    return true;//a透明，b不透明，先绘制b，再绘制a，所以算作b深度值大于a，返回true;
                if (!aTr && bTr)
                    return false;//a不透明，b透明，先绘制a，再绘制b，所以算作a深度值大于b，返回false;

                // 都透明或者都不透明，则比较深度值
                return meshA->getDepth() < meshB->getDepth();
                //return (meshB.getDepth() - meshA.getDepth()) * (aTr && bTr ? 1.0 : -1.0);
            });

        if (_meshPreview)
            _meshPreview->updateMatrices(*_camera);
        if (_grid)
            _grid->updateMatrices(*_camera);
    }

    void Scene::initWebGL()
    {
        /* 暂时注释掉
        var attributes = {
          antialias: false,
          stencil : true
        };

        var canvas = document.getElementById('canvas');
        var gl = _gl = canvas.getContext('webgl', attributes) || canvas.getContext('experimental-webgl', attributes);
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
    void Scene::loadTextures()
    {
        /*暂时注释
        var self = this;
        var gl = _gl;
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

        initAlphaTextures();
        */
    }

    void Scene::initAlphaTextures()
    {
        const char* const alphas = Picking::INIT_ALPHAS_PATHS;
        const char* const names = Picking::INIT_ALPHAS_NAMES;
        /*
        for (var i = 0, nbA = alphas.length; i < nbA; ++i) {
            var am = new Image();
            am.src = 'resources/alpha/' + alphas[i];
            am.onload = onLoadAlphaImage.bind(this, am, names[i]);
        }
        */
    }

    /** Called when the window is resized */
    void Scene::onCanvasResize()
    {
        auto viewport = _viewport;
        auto newWidth = viewport.clientWidth * _pixelRatio;
        auto newHeight = viewport.clientHeight * _pixelRatio;

        _canvasOffsetLeft = viewport.offsetLeft;
        _canvasOffsetTop = viewport.offsetTop;
        _canvasWidth = newWidth;
        _canvasHeight = newHeight;

        _canvas.width = newWidth;
        _canvas.height = newHeight;

        glViewport(0, 0, newWidth, newHeight);//_gl.viewport(0, 0, newWidth, newHeight);
        _camera->onResize(newWidth, newHeight);
        _background->onResize(newWidth, newHeight);

        _rttContour->onResize(newWidth, newHeight);
        _rttMerge->onResize(newWidth, newHeight);
        _rttOpaque->onResize(newWidth, newHeight);
        _rttTransparent->onResize(newWidth, newHeight);

        render();
    }

    float Scene::computeRadiusFromBoundingBox(const SixElemArray& box)
    {
        float dx = box[3] - box[0];
        float dy = box[4] - box[1];
        float dz = box[5] - box[2];
        return 0.5 * Math.sqrt(dx * dx + dy * dy + dz * dz);
    }

    SixElemArray Scene::computeBoundingBoxMeshes(std::vector<MeshSPtr>& meshes)
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

    SixElemArray Scene::computeBoundingBoxScene()
    {
        //var scene = _meshes.slice();
        //slice()数组等同于直接复制一个新的数组出来
        std::vector<MeshSPtr> scene(_meshes);
        scene.emplace_back(_grid);//scene.push(_grid);
        _sculptManager->addSculptToScene(scene);
        return computeBoundingBoxMeshes(scene);
    }

    void Scene::normalizeAndCenterMeshes(std::vector<MeshSPtr>& meshes)
    {
        SixElemArray box = computeBoundingBoxMeshes(meshes);
        //var scale = Utils::SCALE / vec3.dist([box[0], box[1], box[2]], [box[3], box[4], box[5]]);
        float scale = Utils::SCALE / glm::distance(glm::vec3(box[0], box[1], box[2]), glm::vec3(box[3], box[4], box[5]));

        //var mCen = mat4.create();
        glm::mat4 mCen = glm::scale(glm::mat4(1.0f), glm::vec3(scale));//mat4.scale(mCen, mCen, [scale, scale, scale]);
        mCen = glm::translate(mCen, glm::vec3(-(box[0] + box[3]) * 0.5f, -(box[1] + box[4]) * 0.5f, -(box[2] + box[5]) * 0.5f));
        //mat4.translate(mCen, mCen, [-(box[0] + box[3]) * 0.5, -(box[1] + box[4]) * 0.5, -(box[2] + box[5]) * 0.5]);
        auto l = meshes.size();
        for (auto i = 0; i < l; ++i)
        {
            glm::mat4& mat = meshes[i]->getMatrix();
            mat *= mCen;//mat4.mul(mat, mCen, mat);
        }
    }

    MeshSPtr Scene::addSphere()
    {
        // make a cube and subdivide it
        Multimesh* mesh = new Multimesh(Primitives::createCube());
        mesh->normalizeSize();
        subdivideClamp(mesh);
        return addNewMesh(mesh);
    }

    MeshSPtr Scene::addCube()
    {
        Multimesh* mesh = new Multimesh(Primitives::createCube());
        mesh->normalizeSize();
        glm::mat4& mat = mesh->getMatrix();
        mat = glm::scale(mat, glm::vec3(0.7f));//mat4.scale(mesh.getMatrix(), mesh.getMatrix(), [0.7, 0.7, 0.7]);
        subdivideClamp(mesh, true);
        return addNewMesh(mesh);
    }

    MeshSPtr Scene::addCylinder()
    {
        Multimesh* mesh = new Multimesh(Primitives::createCylinder());
        mesh->normalizeSize();
        glm::mat4& mat = mesh->getMatrix();
        mat = glm::scale(mat, glm::vec3(0.7f));//mat4.scale(mesh.getMatrix(), mesh.getMatrix(), [0.7, 0.7, 0.7]);
        subdivideClamp(mesh, true);
        return addNewMesh(mesh);
    }

    void Scene::addTorus(bool preview)
    {
        Multimesh* mesh = new Multimesh(Primitives::createTorus(_torusLength, _torusWidth, _torusRadius, _torusRadial, _torusTubular));

        if (preview)
        {
            mesh.setShowWireframe(true);
            float scale = 0.3f * Utils::SCALE;
            glm::mat4& mat = mesh->getMatrix();
            mat = glm::scale(mat, glm::vec3(scale));//mat4.scale(mesh.getMatrix(), mesh.getMatrix(), [scale, scale, scale]);
            _meshPreview = mesh;
            return;
        }

        mesh->normalizeSize();
        subdivideClamp(mesh);
        addNewMesh(mesh);
    }

    void Scene::subdivideClamp(MeshSPtr mesh, bool linear)
    {
        Subdivision::LINEAR = !!linear;
        while (mesh->getNbFaces() < 50000)
            mesh->addLevel();
        // keep at max 4 multires
        Utils::splice(mesh._meshes, 0, Math.min(mesh._meshes.size() - 4, 4));//mesh._meshes.splice(0, Math.min(mesh._meshes.length - 4, 4));
        mesh._sel = mesh._meshes.length - 1;
        Subdivision::LINEAR = false;
    }

    void Scene::addNewMesh(MeshSPtr mesh)
    {
        _meshes.emplace_back(mesh);//push(mesh);
        _stateManager.pushStateAdd(mesh);
        setMesh(mesh);
        return mesh;
    }

    std::vector<MeshSPtr> Scene::loadScene(std::uint8_t* fileData, const std::string& fileType)
    {
        var newMeshes;
        if (fileType == "obj") newMeshes = Import::importOBJ(fileData);
        else if (fileType == "sgl") newMeshes = Import::importSGL(fileData);
        else if (fileType == "stl") newMeshes = Import::importSTL(fileData);
        else if (fileType == "ply") newMeshes = Import::importPLY(fileData);

        var nbNewMeshes = newMeshes.length;
        if (nbNewMeshes == 0) {
            return;
        }

        var meshes = _meshes;
        for (var i = 0; i < nbNewMeshes; ++i) {
            var mesh = newMeshes[i] = new Multimesh(newMeshes[i]);

            if (!_vertexSRGB && mesh.getColors()) {
                Utils.convertArrayVec3toSRGB(mesh.getColors());
            }

            mesh.init();
            mesh.initRender();
            meshes.push(mesh);
        }

        if (_autoMatrix) {
            normalizeAndCenterMeshes(newMeshes);
        }

        _stateManager->pushStateAdd(newMeshes);
        setMesh(meshes[meshes.length - 1]);
        resetCameraMeshes(newMeshes);
        return newMeshes;
    }

    void Scene::clearScene()
    {
        getStateManager().reset();
        getMeshes().length = 0;
        getCamera().resetView();
        setMesh(null);
        _action = Enums::Action::NOTHING;
    }

    void Scene::deleteCurrentSelection()
    {
        if (!_mesh)
            return;

        removeMeshes(_selectMeshes);
        _stateManager.pushStateRemove(_selectMeshes.slice());
        _selectMeshes.length = 0;
        setMesh(null);
    }

    void Scene::removeMeshes(std::vector<MeshSPtr>& rm)
    {
        //var meshes = _meshes;
        std::size_t len = rm.size();
        for (var i = 0; i < len; ++i)
        {
            Utils::splice(_meshes, getIndexMesh(rm[i]), 1);
            //meshes.splice(getIndexMesh(rm[i]), 1);
        }
    }

    std::int32_t Scene::getIndexMesh(MeshSPtr mesh, bool select)
    {
        std::vector<MeshSPtr>& meshes = select ? _selectMeshes : _meshes;
        std::uint32_t id = mesh->getID();
        std::size_t nbMeshes = meshes.size();
        for (std::size_t i = 0; i < nbMeshes; ++i)
        {
            if (meshes[i] == mesh || meshes[i]->getID() == id)
                return i;
        }
        return -1;
    }


    /** Replace a mesh in the scene */
    void Scene::replaceMesh(MeshSPtr mesh, MeshSPtr newMesh)
    {
        std::int32_t index = getIndexMesh(mesh);
        
        if (index >= 0) 
            _meshes[index] = newMesh;
        if (_mesh == mesh)
            setMesh(newMesh);
    }

    void Scene::duplicateSelection()
    {
        var meshes = _selectMeshes.slice();
        var mesh = null;
        for (var i = 0; i < meshes.length; ++i) {
            mesh = meshes[i];
            var copy = new MeshStatic(mesh.getGL());
            copy.copyData(mesh);

            addNewMesh(copy);
        }

        setMesh(mesh);
    }

    void Scene::onLoadAlphaImage(const kgl::ImageFileReader& img, name, tool)
    {
        /*
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
        getGui().addAlphaOptions(entry);
        if (tool && tool._ctrlAlpha)
            tool._ctrlAlpha.setValue(name);

        */
    }
}