/*!
 * \file pixel_magic_app.h
 * \date 2017/12/31 15:26
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
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
#ifndef digital_sculpt_app_h__
#define digital_sculpt_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"

class DigitalSculptApp : public kgl::App
{
public:
    DigitalSculptApp();
    virtual ~DigitalSculptApp();
    virtual void InitScene()  override;
protected:
    virtual void RenderScene() override;
    virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode) override;
    virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos) override;
    virtual void ProcessInput() override;
    void RenderBeatHeart();
    void RenderColorfulRing();
    void RenderMobius();
    void RenderSun();
    void RenderJapanFlag();
    void RenderChinaFlag();
private:
    kgl::GPUProgram*        colorful_ring_shader_;
    kgl::GPUProgram*        heart_beat_shader_;
    kgl::GPUProgram*        mobius_shader_;
    kgl::GPUProgram*        sun_shader_;
    kgl::GPUProgram*        japan_flag_shader_;
    kgl::GPUProgram*        china_flag_shader_;
    kgl::Primitive*         rectangle_primitive_;
    kgl::SourceTextureSPtr  texture_1_;
    kgl::SourceTextureSPtr  texture_2_;
    kgl::SourceTextureSPtr  texture_rock_;
    glm::vec2               screen_resolution_;
    glm::vec2               mouse_input_pos_;
    int                     current_effect_index_;      // 当前要渲染的效果
    int                     effect_count_;              // 待渲染的效果的总数
    float                   china_flag_scale_ = 150.0f;
};

#endif // digital_sculpt_app_h__