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
#include "culling.h"

struct BufferBlob
{
    uint32_t buffer_index;
    GLuint buffer[INDIRECT_BUFFERS];
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
    
    virtual void RenderScene() override;
private:

    /*********************************************************
    移动相机。视图投影矩阵将会在其他地方重新计算

    @param  float delta_x
    @param  float delta_y
    @return void
    *********************************************************/
    void MoveCamera(float delta_x, float delta_y);

    /*********************************************************

    @param  glm::vec3 color_mod
    @return void
    *********************************************************/
    void render_spheres(glm::vec3 color_mod);

    /*********************************************************
    将实例化的遮挡物的实例对象的几何体烘焙到单个顶点缓冲区和索引缓
    冲区中。使实现更加通用和简单。
    @param  std::vector<glm::vec4> & occluder_positions
    @param  std::vector<uint32_t> & occluder_indices
    @param  const Mesh & box_mesh
    @param  const std::vector<glm::vec4>& instances
    @return void
    *********************************************************/
    void BakeOccluderGeometry(std::vector<glm::vec4>& occluder_positions,std::vector<uint32_t>& occluder_indices,const Mesh& box_mesh, const std::vector<glm::vec4>& instances_pos);

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
    void render_depth_map();

    /*********************************************************

    @param  uint32_t width
    @param  uint32_t height
    @return void
    *********************************************************/
    void render(uint32_t width, uint32_t height);

    /*********************************************************

    @param  CullingMethod method
    @return void
    *********************************************************/
    void set_culling_method(CullingMethod method);

    /*********************************************************
    在compute shader中计算小球的运动，让它动起来
    @param  float delta_time
    @return void
    *********************************************************/
    void ApplyPhysics(float delta_time);

    inline void set_physics_speed(float speed) { physics_speed_ = speed; }
    inline float get_physics_speed() const { return physics_speed_; }
    inline void set_show_redundant(bool enable) { show_redundant_ = enable; }
    inline bool get_show_redundant() const { return show_redundant_; }
private:
    GLDrawable* box_;
    GLDrawable* sphere_[SPHERE_LODS];
    GLDrawable* quad_;

    kgl::ComputeShaderProgram* physics_program_;

    kgl::ShaderBuffer* occluder_instances_buffer_;
    kgl::ShaderBuffer* sphere_instances_buffer_;

    kgl::GPUProgram* occluder_program_;
    kgl::GPUProgram* sphere_program_;
    kgl::GPUProgram* quad_program_;

    glm::mat4 view_matrix_;
    glm::mat4 projection_matrix_;

    float camera_rotation_y_;
    float camera_rotation_x_;
    float physics_speed_;

    uint32_t num_sphere_render_lods;
    uint32_t num_render_sphere_instances_; // 要渲染的小球的实例个数
    uint32_t num_occluder_instances;
    uint32_t culling_implementation_index_;

    BufferBlob indirect_;

    bool show_demo_window_ = false;
    bool show_another_window_ = false;
    bool show_redundant_;
    bool enable_culling_;

    std::vector<CullingInterface*> culling_implementations_;
};


#endif // hiz_occlusion_culling_app_h__