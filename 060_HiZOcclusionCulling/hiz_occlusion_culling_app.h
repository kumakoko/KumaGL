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
#ifndef hiz_occlusion_culling_app_h__
#define hiz_occlusion_culling_app_h__

#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_compute_shader_program.h"
#include "../klib/kgl_shader_buffer.h"

#include "constants.h"
#include "mesh.h"
#include "culling.h"

struct IndirectInfo
{
    GLuint buffer[INDIRECT_BUFFERS];
    uint32_t buffer_index;
    GLuint instance_buffer[SPHERE_LODS];
};

class HizOcclusionCullingApp : public kgl::App
{
public:
    HizOcclusionCullingApp();
    virtual ~HizOcclusionCullingApp();
    virtual void InitScene() override;
protected:
    /*********************************************************

    @return void
    *********************************************************/

    virtual void InitModel() override;
    /*********************************************************

    @return void
    *********************************************************/
    virtual void RenderGUI() override;

    /*********************************************************

    @return void
    *********************************************************/
    virtual void InitShaders() override;

    /*********************************************************

    @return void
    *********************************************************/
    virtual void UpdateScene() override;

    /*********************************************************

    @return void
    *********************************************************/
    virtual void PreRenderFrame() override;

    /*********************************************************

    @return void
    *********************************************************/
    virtual void PostRenderFrame() override;

    /*********************************************************
    
    @return void    
    *********************************************************/
    virtual void RenderScene() override;
private:
    /*********************************************************
    
    @param  float delta_x
    @param  float delta_y
    @return void    
    *********************************************************/
    void MoveCamera(float delta_x, float delta_y);

    /*********************************************************
    
    @param  CullingMethod method
    @return void    
    *********************************************************/
    void SetCullingMethod(CullingMethod method);

    void set_physics_speed(float speed) { physics_speed_ = speed; }
    float physics_speed() const { return physics_speed_; }
    void set_show_redundant(bool enable) { show_redundant_ = enable; }
    bool show_redundant() const { return show_redundant_; }

    /*********************************************************
    
    @param  const glm::vec3 & color_mod
    @return void    
    *********************************************************/
    void RenderSpheres(const glm::vec3& color_mod);

    /*********************************************************
    
    @param  std::vector<glm::vec4> & occluder_positions
    @param  std::vector<uint32_t> & occluder_indices
    @param  const Mesh & box_mesh
    @param  const glm::vec4 * instances
    @param  uint32_t num_instances
    @return void    
    *********************************************************/
    void BakeOccluderGeometry(std::vector<glm::vec4>& occluder_positions,
        std::vector<uint32_t>& occluder_indices,
        const Mesh& box_mesh, const glm::vec4* instances, uint32_t num_instances);

    /*********************************************************
    
    @return void    
    *********************************************************/
    void InitInstances();

    /*********************************************************
    
    @param  float delta_time
    @return void    
    *********************************************************/
    void ApplyPhysics(float delta_time);

    /*********************************************************
    
    @param  float rotation_y
    @param  float rotation_x
    @param  uint32_t viewport_width
    @param  uint32_t viewport_height
    @return void    
    *********************************************************/
    void UpdateCamera(float rotation_y, float rotation_x, uint32_t viewport_width, uint32_t viewport_height);

    /*********************************************************
    
    @return void    
    *********************************************************/
    void RenderDepthMap();
private:
    GLDrawable* quad_;
    GLDrawable* box_;
    GLDrawable* sphere_[SPHERE_LODS];

    std::vector<Culling*> culling_implementations_;

    bool show_redundant_;
    bool enable_culling_;

    IndirectInfo indirect_;

    GLuint occluder_program_;
    GLuint sphere_program_;
    GLuint quad_program_;
    GLuint physics_program_;
    GLuint occluder_instances_buffer_;
    GLuint sphere_instances_buffer_;

    uint32_t culling_implementation_index_;
    uint32_t num_occluder_instances_;
    uint32_t num_sphere_render_lods_;
    uint32_t num_render_sphere_instances_;

    glm::mat4 projection_matrix_;
    glm::mat4 view_matrix_;

    float physics_speed_;
    float camera_rotation_y_;
    float camera_rotation_x_;
};


#endif // hiz_occlusion_culling_app_h__