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
/*!
 * \file kgl_water_renderer.h
 * \date 2019/01/26 22:22
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
#ifndef kgl_water_renderer_h__
#define kgl_water_renderer_h__

#include "kgl_wfx_loader.h"
#include "kgl_wfx_water_frame_buffer.h"
#include "kgl_wfx_raw_model.h"
#include "kgl_wfx_water_tile.h"
#include "kgl_wfx_light.h"
#include "kgl_camera.h"
#include "kgl_gpu_program.h"
#include "kgl_render_state_blend.h"

namespace kgl
{
    namespace water_effect
    {
        class WaterRenderer
        {
        public:
            /// <summary>
            /// 类WaterRenderer<see cref="WaterRenderer"/> 的构造函数.
            /// </summary>
            /// <param name="loader">装载图片用到的loader</param>
            /// <param name="fbos">渲染水面效果要用到的frame buffer object</param>
            WaterRenderer(LoaderSPtr loader, WaterFrameBuffersSPtr fbos);

            void render(const std::vector<WaterTile>& water, CameraSPtr camera, const Light& sun, float frame_interval);

            void InitWaterShader();

            ~WaterRenderer();

        private:
            /// <summary>
            /// Prepares the render.
            /// </summary>
            /// <param name="camera">The camera.</param>
            /// <param name="sun">The sun.</param>
            /// <param name="frame_interval">The frame_interval.</param>
            void prepareRender(CameraSPtr camera, const Light& sun, float frame_interval);
            void unbind()   ;
            void loadProjectionMatrix(glm::mat4 const& projection) const;

            RawModel quad_;
            GPUProgram* water_shader_ = nullptr; // WaterShader shader_{};
            GLuint dudvTextureId_;
            GLuint normalMapTextureId_;
            float moveFactor_ = 0.0f;

            /// <summary>
            /// 水面=渲染器用到的frame buffer对象
            /// </summary>
            WaterFrameBuffersSPtr   fbos_;
            RenderStateBlend        rs_blend_reder_water_;
            LoaderSPtr              loader_;


            GLuint loc_projection_matrix_ = -1;//uniform mat4 projectionMatrix;
            GLuint loc_view_matrix_ = -1;       //uniform mat4 viewMatrix;
            GLuint loc_model_matrix_ = -1; //uniform mat4 modelMatrix;
            GLuint loc_camera_position_ = -1;//uniform vec3 cameraPosition;
            GLuint loc_light_position_ = -1;//uniform vec3 lightPosition;

            GLuint loc_reflection_texture_ = -1;//uniform sampler2D reflectionTexture;
            GLuint loc_refraction_texture_ = -1;//uniform sampler2D refractionTexture;
            GLuint loc_dudv_map_ = -1;//uniform sampler2D dudvMap;
            GLuint loc_normal_map_ = -1;//uniform sampler2D normalMap;
            GLuint loc_depth_map_ = -1;//uniform sampler2D depthMap;

            GLuint loc_light_color_ = -1;//uniform vec3 lightColor;
            GLuint loc_move_factor_ = -1;//uniform float moveFactor;
        };
    }

}

#endif // kgl_water_renderer_h__
