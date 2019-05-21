/**************************************************************************************************************************
Copyright(C) 2014-2019 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_defines.h"
#include "main_render_module.h"

const float FOV = 70.0f;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 1000.0f;
const float RED = 0.5444f;
const float GREEN = 0.62f;
const float BLUE = 0.69f;

MainRenderModule::MainRenderModule(kgl::CameraSPtr camera, wfx::LoaderSPtr loader) /*: skyboxRenderer_(loader) */
{
    enableCulling();
    glEnable(GL_DEPTH_TEST);
    terrain_render_module_ = new TerrainRenderModule;
    terrain_render_module_->InitShader();
    skybox_render_module_ = new SkyboxRenderModule(loader, camera);
}

MainRenderModule::~MainRenderModule()
{
    KGL_SAFE_DELETE(terrain_render_module_);
    KGL_SAFE_DELETE(skybox_render_module_);
}

void MainRenderModule::prepare() const
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(RED, GREEN, BLUE, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MainRenderModule::render(const std::vector<wfx::Terrain>& terrains, const std::vector<wfx::Light>& lights, const glm::vec4& clip_plane, kgl::CameraSPtr camera)
{
    prepare();

    if (is_render_terrain_)
    {
        terrain_render_module_->render(terrains, lights, clip_plane, camera);
        skybox_render_module_->render(camera, RED, GREEN, BLUE);
    }
}

void MainRenderModule::enableCulling()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void MainRenderModule::disableCulling()
{
    glDisable(GL_CULL_FACE);
}