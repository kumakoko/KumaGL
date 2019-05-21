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
#ifndef skybox_render_module_h__
#define skybox_render_module_h__

#include "../klib/kgl_wfx_loader.h"
#include "../klib/kgl_wfx_terrain.h"
#include "../klib/kgl_wfx_light.h"
#include "../klib/kgl_camera.h"
#include "../klib/kgl_gpu_program.h"

namespace wfx = kgl::water_effect;

class SkyboxRenderModule
{
public:
	SkyboxRenderModule(wfx::LoaderSPtr loader, kgl::CameraSPtr camera);
	~SkyboxRenderModule();
	void render(kgl::CameraSPtr camera, float r, float g, float b);
	void InitShader();
private:
	void bind_textures();
private:
	wfx::RawModel		cube_;
	GLuint				texture_id_ = 0;
	GLuint				night_texture_id_ = 0;
	kgl::GPUProgram*	shader_ = nullptr;
	float				time = 0.0f;
	GLuint				loc_projection_matrix_ = -1;//uniform mat4 projectionMatrix;
	GLuint				loc_view_matrix_ = -1;//uniform mat4 viewMatrix;
	GLuint				loc_cube_map_ = -1;//uniform samplerCube cubeMap;
	GLuint				loc_cube_map_2_ = -1;//uniform samplerCube cubeMap2;
	GLuint				loc_blend_factor_ = -1;//uniform float blendFactor;
	GLuint				loc_fog_color_ = -1;//uniform vec3 fogColor;
};

#endif // skybox_render_module_h__