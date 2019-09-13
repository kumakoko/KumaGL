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
* \file transform_app.h
* \date 2017/12/31 14:43
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief 演示图元的坐标变换操作
*
* TODO: 演示使用kgl::ParticleSystem类
*       演示使用kgl::RenderStateDepth类
*
* \note
*/
#ifndef particle_system_app_h__
#define particle_system_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_render_state_draw_mode.h"
#include "../klib/kgl_gpu_particle_system.h"

class ParticleSystemApp : public kgl::App
{
public:
	ParticleSystemApp();
	virtual ~ParticleSystemApp();
    virtual void InitScene();
protected:
    virtual void RenderScene() override;
    virtual void ProcessInput() override;
    virtual void InitMainCamera() override;
    virtual void RenderGUI() override;
    virtual void PreRenderFrame() override;
private:
    //kgl::RenderStateCullMode cull_mode_;
    //kgl::RenderStateDepth depth_test_mode_;
	kgl::GPUParticleSystem* ps_ = nullptr;
    
    /// <summary>
    /// 执行当前帧的时刻
    /// </summary>
    double current_frame_time_ = 0.0;
    
    /// <summary>
    /// 执行上一帧的时刻
    /// </summary>
    double last_frame_time_ = 0.0;

    /// <summary>
    /// 绘制模式，比如使用线框还是填充模式显示图元
    /// </summary>
    kgl::RenderStateDrawMode draw_mode_;

    /// <summary>
    /// 是否以线框模式绘制场景
    /// </summary>
    bool wireframe_mode_ = false;
    
    /// <summary>
    /// 粒子生成位置坐标
    /// </summary>
    float particle_generated_pos_[3];
    
    /// <summary>
    /// 粒子颜色RGB值
    /// </summary>
    float particle_color_[3];
    
    /// <summary>
    /// 最小发射速度
    /// </summary>
    float particle_min_velocity_[3] ;
    
    /// <summary>
    /// 最大发射速度
    /// </summary>
    float particle_max_velocity_[3];
    
    /// <summary>
    /// 作用于粒子上的重力值
    /// </summary>
    float particle_gravity_[3];
    
    /// <summary>
    /// 粒子的最小生存时间，单位秒
    /// </summary>
    float particle_min_life_;
    
    /// <summary>
    /// 粒子的最大生存时间，单位秒
    /// </summary>
    float particle_max_life_;
    
    /// <summary>
    /// 粒子的生成尺寸
    /// </summary>
    float particle_render_size_;
    
    /// <summary>
    /// 每隔多少秒发射一批粒子
    /// </summary>
    float particle_spawn_time_interval_;
    
    /// <summary>
    /// 粒子的一批次生成个数
    /// </summary>
    int particle_spawn_num_once_ ;
    
    /// <summary>
    /// 粒子总个数
    /// </summary>
    int particle_number_;
};

#endif // particle_system_app_h__