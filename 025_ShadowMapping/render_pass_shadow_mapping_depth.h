/*!
 * \file render_pass_shadow_mapping.h
 * \date 2018/01/14 10:25
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief  执行shadow mapping操作的render pass
 *
 * TODO: 执行shadow mapping操作的render pass
 *
 * \note
*/
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
#ifndef render_pass_shadow_mapping_depth_h__
#define render_pass_shadow_mapping_depth_h__

#include "../klib/kgl_render_pass.h"
#include "../klib/kgl_texture.h"
#include "../klib/kgl_frame_buffer.h"

class RenderPassShadowMappingDepth : public kgl::RenderPass
{
public:
    RenderPassShadowMappingDepth(GLint wnd_width, GLint wnd_height);
    /// <summary>
    /// Initializes this instance.
    /// </summary>
    virtual void Initialize() override;

    /// <summary>
    /// Renders the specified current_time.
    /// </summary>
    /// <param name="current_time">The current_time.</param>
    virtual void Update(float current_time) override;

    /// <summary>
    /// Sets the light space matrix.
    /// </summary>
    /// <param name="mat">The mat.</param>
    /// <param name="need_transpose">The need_transpose.</param>
    void SetLightSpaceMatrix();

    /// <summary>
    /// Attaches the frame buffer.
    /// </summary>
    /// <param name="frame_buffer">The frame_buffer.</param>
    inline void AttachFrameBuffer(kgl::FrameBuffer* frame_buffer)
    {
        frame_buffer_ = frame_buffer;
    }

    inline const glm::mat4& GetLightSpaceMatrix() const
    {
        return lightSpaceMatrix;
    }

    inline const glm::vec3& GetLightPosition() const
    {
        return lightPos;
    }

protected:
    /// <summary>
    /// Gets the light space matrix location.
    /// </summary>
    /// <param name="uniform_name">The uniform_name.</param>
    void GetLightSpaceMatrixLocation(const char* uniform_name);

    /// <summary>
    /// Builds the light space matrix.
    /// </summary>
    void BuildLightSpaceMatrix();
protected:
    GLint location_light_space_matrix_ = -1;
    glm::mat4 lightProjection;
    glm::mat4 lightView;
    glm::mat4 lightSpaceMatrix;
    glm::vec3 lightPos;
    kgl::FrameBuffer* frame_buffer_ = nullptr;
    GLint window_width_ = 0;
    GLint window_height_ = 0;
};
#endif // render_pass_shadow_mapping_depth_h__