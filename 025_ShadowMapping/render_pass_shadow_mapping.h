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
#ifndef render_pass_shadow_mapping_h__
#define render_pass_shadow_mapping_h__

#include "../klib/kgl_render_pass.h"
#include "../klib/kgl_texture.h"

class RenderPassShadowMapping : public kgl::RenderPass
{
public:
    RenderPassShadowMapping();
    virtual void Initialize() override;
    virtual void Update(float current_time) override;
    void SetLightWorldPosition(const glm::vec3& pos);
    void SetLightSpaceMatrix(const glm::mat4& mat, bool need_transpose = false);
    void SetDiffuseTexture(kgl::TextureSPtr texture, GLuint slot_index);
    void SetShadowMap(kgl::TextureSPtr shadow_map_texture, GLuint slot_index);
    void SetBaisAndSampleForShadowLocation(bool use);
protected:
    void GetLightWorldPositionLocation(const char* uniform_name);
    void GetLightSpaceMatrixLocation(const char* uniform_name);
    void GetDiffuseTextureLocation(const char* uniform_name);
    void GetShadowMapLocation(const char* uniform_name);
    void GetBaisAndSampleForShadowLocation(const char* uniform_name);
protected:
    GLint location_light_world_position_ = -1;
    GLint location_light_space_matrix_ = -1;
    GLint location_diffuse_texture_ = -1;
    GLint location_shadow_map_ = 1;
    GLint bias_and_sample_position_ = -1;
    
};
#endif // render_pass_shadow_mapping_h__