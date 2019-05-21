/**************************************************************************************************************************
Copyright(C) 2014-2018 www.xionggf.com

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
/*!
 * \file main_render_module.h
 * \date 2019/01/19 12:32
 *
 * \author Xiong Xinke
 * Contact: sun_of_lover@sina.com
 *
 * \brief
 *
 * TODO: 负责水面反射/折射效果的纹理生成类
 *
 * \note
 */
#ifndef main_render_module_h__
#define main_render_module_h__

#include "../klib/kgl_wfx_loader.h"
#include "../klib/kgl_wfx_terrain.h"
#include "../klib/kgl_wfx_light.h"
#include "../klib/kgl_camera.h"
#include "../klib/kgl_gpu_program.h"

#include "terrain_render_module.h"
#include "skybox_render_module.h"

namespace wfx = kgl::water_effect;

class MainRenderModule
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="MainRenderModule"/> class.
    /// </summary>
    /// <param name="camera">The camera.</param>
    /// <param name="loader">The loader.</param>
    MainRenderModule(kgl::CameraSPtr camera, wfx::LoaderSPtr loader);

    /// <summary>
    /// Finalizes an instance of the <see cref="MainRenderModule"/> class.
    /// </summary>
    ~MainRenderModule();

    /// <summary>
    /// Renders the scene.
    /// </summary>
    /// <param name="terrains">The terrains.</param>
    /// <param name="lights">The lights.</param>
    /// <param name="clip_plane">The clip_plane.</param>
    /// <param name="camera">The camera.</param>
    void renderScene(
        const std::vector<wfx::Terrain>& terrains,
        const std::vector<wfx::Light>& lights,
        const glm::vec4& clip_plane, kgl::CameraSPtr camera);

    /// <summary>
    /// Prepares this instance.
    /// </summary>
    void prepare() const;

    /// <summary>
    /// Renders the specified terrains.
    /// </summary>
    /// <param name="terrains">The terrains.</param>
    /// <param name="lights">The lights.</param>
    /// <param name="clip_plane">The clip_plane.</param>
    /// <param name="camera">The camera.</param>
    void render(const std::vector<wfx::Terrain>& terrains,
        const std::vector<wfx::Light>& lights,
        const glm::vec4& clip_plane, kgl::CameraSPtr camera);

    /// <summary>
    /// Processes the terrain.
    /// </summary>
    /// <param name="terrain">The terrain.</param>
    void processTerrain(const wfx::Terrain* terrain);

    /// <summary>
    /// Enables the culling.
    /// </summary>
    static void enableCulling();
    /// <summary>
    /// Disables the culling.
    /// </summary>
    static void disableCulling();

    /// <summary>
    /// Resets the projection matrix.
    /// </summary>
    void resetProjectionMatrix();

    /// <summary>
    /// Gets the projection matrix.
    /// </summary>
    /// <returns>const glm.mat4 &.</returns>
    const glm::mat4& getProjectionMatrix() const;

private:
    /// <summary>
    /// The projection matrix_
    /// </summary>
    glm::mat4               projectionMatrix_;

    /// <summary>
    /// The terrain_render_module_
    /// </summary>
    TerrainRenderModule*    terrain_render_module_ = nullptr;

    /// <summary>
    /// The skybox_render_module_
    /// </summary>
    SkyboxRenderModule*     skybox_render_module_ = nullptr;
public:
    /// <summary>
    /// 渲染天空盒吗
    /// </summary>
    bool is_render_skybox_ = true;

    /// <summary>
    /// 渲染地形吗
    /// </summary>
    bool is_render_terrain_ = true;
};

#endif // main_render_module_h__