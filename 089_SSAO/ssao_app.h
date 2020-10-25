/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

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
 * \file pbr_lighting_app.h
 * \date 2020/03/02 21:24
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief  演示在OpenGL中显示MD2模型，并且播放序列帧顶点动画的效果
*/
#ifndef pbr_lighitng_app_h__
#define pbr_lighitng_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_gbuffer.h"
#include "../klib/kgl_frame_buffer.h"
#include "../klib/kgl_basic_static_mesh.h"

const int MATERIAL_COUNT = 5;

class SSAOApp : public kgl::App
{
public:
	SSAOApp();
	virtual ~SSAOApp();
	virtual void InitScene() override;
protected:
    virtual void RenderScene() override;
    virtual void ProcessInput() override;
    virtual void InitMainCamera() override;
    virtual void RenderGUI() override;
	virtual void InitModel() override;
	virtual void InitShaders() override;
	virtual void InitLights() override;
	void BuildSSAOKernel();
	void BuildNoiseTexture();
	void RenderCube();
	void RenderQuad();
private:
	/** * @brief  用来进行延迟渲染操作用的G-buffer */
    kgl::GBuffer* gbuffer_;

	/** * @brief   */
	kgl::FrameBuffer* ssao_fb_;

	/** * @brief   */
	kgl::FrameBuffer* ssao_blur_fb_;

	/** * @brief 深度状态类 */
	kgl::RenderStateDepth rs_depth_;

	kgl::GPUProgramSPtr geometry_pass_shader_;
	kgl::GPUProgramSPtr lighting_pass_shader_;
	kgl::GPUProgramSPtr ssao_shader_;
	kgl::GPUProgramSPtr ssao_blur_shader_;
	kgl::SourceTextureSPtr noise_texture_;
	std::array<std::string,64> uniform_var_name_samples_;
	std::vector<glm::vec3> ssaoKernel;

	kgl::BasicStaticMesh* model_;

	glm::vec3 lightPos;

	glm::vec3 lightColor;
	
	GLuint draw_mode = 1;

	/// <summary>
	/// 基于NDC坐标的，占满了整个屏幕的矩形图元
	/// </summary>
	kgl::PrimitiveSPtr screen_rectangle_;
};

#endif // pbr_lighitng_app_h__