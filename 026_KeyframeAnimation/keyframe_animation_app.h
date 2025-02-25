﻿/**************************************************************************************************************************
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
 * \file keyframe_animation_app.h
 * \date 2020/03/03 21:24
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief  演示在OpenGL中显示MD2模型，并且播放序列帧顶点动画的效果
*/
#ifndef keyframe_animation_app_h__
#define keyframe_animation_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_md2_model.h"

class KeyframeAnimationApp : public kgl::App
{
public:
    KeyframeAnimationApp();
    virtual ~KeyframeAnimationApp();
    virtual void InitScene();
protected:
    virtual void RenderScene() override;
    virtual void ProcessInput() override;
    virtual void InitMainCamera() override;
    virtual void RenderGUI() override;
    void RenderText();
private:
	/** * @brief 当前使用的拣选模式  */
    kgl::RenderStateCullMode cull_mode_;
	
	/** * @brief 当前使用的深度测试模式  */
    kgl::RenderStateDepth depth_test_mode_;
	
	/** * @brief 当前绘制的MD2模型  */
	kgl::MD2Model* model_;

	/** * @brief 当前播放的动作的动画状态信息 */
	kgl::MD2AnimationStatus	playing_animation_state_;

	/** * @brief 渲染模型并且播放动画要用到的shader */
	kgl::GPUProgramSPtr model_shader_;

	/** * @brief 模型绕着世界坐标系y轴旋转的角度 */
	float model_rotation_around_y_axis_;

	/** * @brief 模型绕着世界坐标系x轴旋转的角度 */
	float model_rotation_around_x_axis_;

	/** * @brief 动画的播放速度 */
	float animation_play_speed_;
};

#endif // keyframe_animation_app_h__