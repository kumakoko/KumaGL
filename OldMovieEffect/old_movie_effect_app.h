// 展示老电影风格的shader
#ifndef old_movie_effect_app_h__
#define old_movie_effect_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"

class OldMovieEffectApp : public kgl::App
{
public:
	OldMovieEffectApp();
	virtual ~OldMovieEffectApp();
    virtual void InitScene();
protected:
    virtual void RenderFrame();
    virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode);
    virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos);
private:
    kgl::GPUProgram*        shader_ = nullptr;
	kgl::Primitive*         rectangle_primitive_ = nullptr;
    kgl::SourceTextureSPtr  texture_scene_; // 主画面纹理
	kgl::SourceTextureSPtr  texture_dust_; // 灰尘纹理
	kgl::SourceTextureSPtr  texture_line_;	 // 扫描线纹理
	kgl::SourceTextureSPtr  texture_tv_; // tv纹理
	kgl::SourceTextureSPtr  texture_noise_; // 噪声纹理

    glm::vec2 screen_resolution_;
	glm::vec4 film_color_;
	float grain_amount_ = 0.8f;
	float scratches_level_ = 0.7f;
	float scratches_amount_ = 3.0f;
	float max_frame_jitter_ = 1.4f; // 每一帧的抖动范围值，可调配，最小0，最大10，默认1.4，每调整一次，数值变化值为0.1
	float frame_jitter_ = 4.7f; // 每一帧的抖动频率值，可调配，最小0，最大6，默认4.7，每调整一次，数值变化值为0.1
	float over_exposure_amount_ = 1.5f; // 曝光过度效果的数量值，可调配，最小0，最大10，默认1.5，每调整一次，数值变化值为0.1
	float grain_thickness_ = 1.0f; // 木纹效果的数量值，可调配，最小0.1，最大4，默认1，每调整一次，数值变化值为0.1
	float dust_amount_ = 4.0f; // 灰尘的数量值，可调配，最小0.0，最大4.0，默认4，每调整一次，数值变化值为1
};

#endif // old_movie_effect_app_h__