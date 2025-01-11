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

/*
间接绘制命令 (Indirect Drawing Commands)
间接绘制是一种高级渲染技术，允许将绘制命令（如顶点数量、实例数量等）存储在 GPU 缓冲区中，而不是通过 CPU 直接调用绘制函数。

间接绘制的作用:
1 减少 CPU-GPU 通信：
    传统的绘制调用（如 glDrawArrays 或 glDrawElements）需要 CPU 向 GPU 发送绘制命令。
    间接绘制将绘制命令存储在 GPU 缓冲区中，GPU 可以直接读取这些命令，从而减少 CPU 的负担。
2 动态渲染：
    间接绘制允许 GPU 根据缓冲区中的内容动态决定如何渲染，适合需要频繁更新绘制命令的场景（如粒子系统、实例化渲染等）。

3 多线程渲染：
    间接绘制可以与多线程渲染结合使用，进一步提高渲染效率。

======================================================================================================

间接绘制命令的结构

间接绘制命令是一个结构体，具体格式取决于使用的绘制函数。以下是两种常见的间接绘制命令：

1. glDrawArraysIndirect 的命令结构

typedef struct {
    GLuint count;         // 顶点数量
    GLuint instanceCount; // 实例数量
    GLuint first;         // 起始顶点
    GLuint baseInstance;  // 起始实例
} DrawArraysIndirectCommand;

2. glDrawElementsIndirect 的命令结构

typedef struct {
    GLuint count;         // 索引数量
    GLuint instanceCount; // 实例数量
    GLuint firstIndex;    // 起始索引
    GLuint baseVertex;    // 起始顶点
    GLuint baseInstance;  // 起始实例
} DrawElementsIndirectCommand;

使用 GL_DRAW_INDIRECT_BUFFER 进行间接绘制的步骤
1 创建缓冲区：    使用 glGenBuffers 创建一个缓冲区对象。
2 绑定缓冲区：    使用 glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer) 将缓冲区绑定到 GL_DRAW_INDIRECT_BUFFER 目标。
3 上传绘制命令：   使用 glBufferData 或 glBufferSubData 将间接绘制命令上传到缓冲区。
4 执行间接绘制：   使用 glDrawArraysIndirect 或 glDrawElementsIndirect 执行间接绘制。
*/

struct BufferBlob
{
    uint32_t buffer_index;
    GLuint buffer[INDIRECT_BUFFERS_COUNT];      // 存储间接绘制命令的缓冲区中的， 每个间接绘制命令缓冲区的大小为 SPHERE_LODS_COUNT * sizeof(IndirectCommand)
    GLuint instance_buffer[SPHERE_LODS_COUNT];  // 存储着小球实例的位置数据的缓冲区对象ID，参见SphereInstance类
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
    void RenderSpheres(glm::vec3 color_mod);

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
    void RenderDepthMap();

    /*********************************************************
    设置不同模式下的拣选方法
    @param  CullingMethod method
    @return void
    *********************************************************/
    void SetCullingMethod(CullingMethod method);

    /*********************************************************
    在compute shader 中计算小球的运动，让它动起来
    @param  float delta_time 先后两幀之间的时间间隔
    @return void
    *********************************************************/
    void ApplyPhysics(float delta_time);

    inline void set_physics_speed(float speed) { physics_speed_ = speed; }
    inline float get_physics_speed() const { return physics_speed_; }
    inline void set_show_redundant(bool enable) { show_redundant_ = enable; }
    inline bool get_show_redundant() const { return show_redundant_; }
private:
    GLDrawable* box_;
    GLDrawable* sphere_[SPHERE_LODS_COUNT];
    GLDrawable* quad_;

    kgl::ComputeShaderProgram* physics_program_;

    kgl::ShaderBuffer* occluder_instances_buffer_;
    kgl::ShaderBuffer* sphere_instances_buffer_; // 存放着每一个小球的，参见SphereInstance

    kgl::GPUProgram* occluder_program_;
    kgl::GPUProgram* sphere_program_;
    kgl::GPUProgram* quad_program_;

    glm::mat4 view_matrix_;
    glm::mat4 projection_matrix_;

    float camera_rotation_y_;
    float camera_rotation_x_;
    float physics_speed_;

    uint32_t num_sphere_render_lods_;
    uint32_t num_render_sphere_instances_; // 要渲染的小球的实例个数
    uint32_t num_occluder_instances_;
    uint32_t culling_implementation_index_;

    BufferBlob indirect_;

    /* bool show_demo_window_ = false;
     bool show_another_window_ = false;*/
    bool show_redundant_;
    bool enable_culling_;

    std::vector<CullingInterface*> culling_implementations_;


};


#endif // hiz_occlusion_culling_app_h__