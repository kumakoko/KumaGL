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
 * \file terrain_render_module.h
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
#ifndef terrain_render_module_h__
#define terrain_render_module_h__

#include "../klib/kgl_wfx_loader.h"
#include "../klib/kgl_wfx_terrain.h"
#include "../klib/kgl_wfx_light.h"
#include "../klib/kgl_camera.h"
#include "../klib/kgl_gpu_program.h"

namespace wfx = kgl::water_effect;

class TerrainRenderModule
{
public:
	TerrainRenderModule();
	~TerrainRenderModule();
	void render(const std::vector<wfx::Terrain>& terrains, const std::vector<wfx::Light>& lights, const glm::vec4& clip_plane, kgl::CameraSPtr camera);
	void InitShader();
private:
	void unbindTerrain() const;
	void ApplyLights(const std::vector<wfx::Light>& lights);
private:
	kgl::GPUProgram*	shader_ = nullptr;

	GLuint loc_transformation_matrix_ = 0;  // uniform mat4 transformationMatrix;
	GLuint loc_projection_matrix_ = 0;		// uniform mat4 projectionMatrix;
	GLuint loc_view_matrix_ = 0;			// uniform mat4 viewMatrix;
	GLuint loc_shine_damper_ = 0;			// uniform float shineDamper;
	GLuint loc_reflectivity_ = 0;			// uniform float reflectivity;
	GLuint loc_sky_color_ = 0;				// uniform vec3 skyColor;
	GLuint loc_clip_plane_ = 0;				// uniform vec4 plane;
	GLuint loc_bg_texture_ = 0;				// uniform sampler2D backgroundTexture;
	GLuint loc_red_channel_ = 0;			// uniform sampler2D rTexture;
	GLuint loc_green_channel_ = 0;			// uniform sampler2D gTexture;
	GLuint loc_blue_channel_ = 0;			// uniform sampler2D bTexture;
	GLuint loc_blend_map_ = 0;				// uniform sampler2D blendMap;
	GLuint loc_light_color_[4];				// uniform vec3 lightColor[4];
	GLuint loc_attenuation_[4];				// uniform vec3 attenuation[4];
	GLuint loc_light_pos_[4];				// uniform vec3 lightPosition[4];
};

#endif // terrain_render_module_h__