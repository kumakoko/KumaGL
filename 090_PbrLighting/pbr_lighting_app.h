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

const int MATERIAL_COUNT = 5;

class PbrLightingApp : public kgl::App
{
public:
	PbrLightingApp();
	virtual ~PbrLightingApp();
	virtual void InitScene() override;
protected:
    virtual void RenderScene() override;
    virtual void ProcessInput() override;
    virtual void InitMainCamera() override;
    virtual void RenderGUI() override;
	virtual void InitModel() override;
	virtual void InitShaders() override;
	virtual void InitLights() override;
	void LoadTextures();
    void RenderText();
private:
	/** * @brief 当前使用的拣选模式  */
    kgl::RenderStateCullMode cull_mode_;
	
	/** * @brief 当前使用的深度测试模式  */
    kgl::RenderStateDepth depth_test_mode_;
	
	/** * @brief 当前使用的反照率贴图 */
	kgl::TextureSPtr current_albedo_texture_;

	/** * @brief 当前使用的法线贴图 */
	kgl::TextureSPtr current_normal_texture_;

	/** * @brief 当前使用的金属度贴图 */
	kgl::TextureSPtr current_metallic_texture_;

	/** * @brief 当前使用的粗糙度贴图 */
	kgl::TextureSPtr current_roughness_texture_;

	/** * @brief 当前使用的环境遮蔽贴图 */
	kgl::TextureSPtr current_ambient_occlusion_texture_;

	/** * @brief 所有的反照率贴图 */
	kgl::TextureSPtr albedo_textures_[MATERIAL_COUNT];

	/** * @brief 所有的法线贴图 */
	kgl::TextureSPtr normal_textures_[MATERIAL_COUNT];

	/** * @brief 所有的金属度贴图 */
	kgl::TextureSPtr metallic_textures_[MATERIAL_COUNT];

	/** * @brief 所有的粗糙度贴图 */
	kgl::TextureSPtr roughness_textures_[MATERIAL_COUNT];

	/** * @brief 所有的环境遮蔽贴图 */
	kgl::TextureSPtr ambient_occlusion_textures_[MATERIAL_COUNT];

	/** * @brief 球体模型 */
	kgl::PrimitiveSPtr sphere_;

	/** * @brief PBR shader */
	kgl::GPUProgramSPtr shader_;

	/** * @brief 光源在世界空间中的位置 */
	glm::vec3 light_position_;

	/** * @brief光源的颜色 */
	glm::vec3 light_color_;

	/** * @brief  x分量为是否使用金属度贴图去控制金属度，y分量为是否使用粗糙度贴图去控制粗糙度，大于0表示使用，小于0表示不使用；
	z分量为如果不使用金属度贴图的话，金属度参数为多少；w分量为如果不使用粗糙度贴图的话，粗糙度参数为多少*/
	glm::vec4 metallic_roughness_;

	/** * @brief 非金属材质用到的基础反射率 */
	glm::vec3 base_reflectivity_;

	/** * @brief 使用金属度贴图控制金属度吗 */
	bool use_metallic_texture_;

	/** * @brief 使用粗糙度贴图控制粗糙度吗 */
	bool use_roughness_texture_;
};

#endif // pbr_lighitng_app_h__