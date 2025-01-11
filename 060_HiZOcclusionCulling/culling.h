/**************************************************************************************************************************
Copyright(C) 2014-2025 www.xionggf.com

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
#ifndef culling_h__
#define culling_h__

#include "mesh.h"
#include "constants.h"
#include "kgl_compute_shader_program.h"
#include "kgl_gpu_program.h"
#include "kgl_shader_buffer.h"

enum CullingMethod
{
    CullHiZ = 0,
    CullHiZNoLOD = 1,
    CullNone = -1
};

/*
glDrawElementsIndirect 使用的间接绘制命令是一个结构体，其定义如下：

cpp
复制
typedef struct {
    GLuint count;         // 要绘制的索引数量
    GLuint instanceCount; // 要绘制的实例数量
    GLuint firstIndex;    // 索引缓冲区中的起始索引。
    GLuint baseVertex;    // 顶点缓冲区中的起始顶点。
    GLuint baseInstance;  // 实例化渲染中的起始实例。
} DrawElementsIndirectCommand;
*/
struct IndirectCommand
{
    GLuint count;
    GLuint instanceCount;
    GLuint zero[3]; // 在本demo中后三项不用管它，所以可以以这种方式定义之
};

class CullingInterface
{
public:
    virtual ~CullingInterface();

    /*********************************************************
    设置遮挡几何体。这些几何体基本上是静态的，应该在场景启动时完成。
    @param  const std::vector<glm::vec4> & positions
    @param  const std::vector<uint32_t> & indices
    @return void    
    *********************************************************/
    virtual void SetupOccluderGeometry(const std::vector<glm::vec4>& positions, const std::vector<uint32_t>& indices) = 0;

    /*********************************************************
    设置观察矩阵和投影矩阵
    @param  const glm::mat4 & projection
    @param  const glm::mat4 & view
    @param  const glm::vec2 & zNearFar
    @return void    
    *********************************************************/
    virtual void SetViewProjectionMatrix(const glm::mat4& projection, const glm::mat4& view, const glm::vec2& zNearFar) = 0;

    /*********************************************************
    把遮挡体的深度值渲染到深度图中去
    @return void    
    *********************************************************/
    virtual void RasterizeOccluders() = 0;

    /*********************************************************
    Test bounding boxes in our scene.
    @param  GLuint counter_buffer 要执行的间接绘制缓冲区的buffer id
    @param  const uint32_t * counter_offsets 每一个IndirectCommand指令，相对于指令空间首地址的偏移量，所有偏移量都存在一个数组中，counter_offsets就是数组的首指针
    @param  uint32_t num_offsets 每一个IndirectCommand指令，相对于指令空间首地址的偏移量，所有偏移量都存在一个数组中，num_offsets就是数组中元素的个数
    @param  const GLuint * culled_instance_buffer
    @param  kgl::ShaderBuffer * instance_data_buffer
    @param  uint32_t num_instances
    @return void    
    *********************************************************/
    virtual void TestBoundingBoxes(GLuint counter_buffer, const uint32_t* counter_offsets, uint32_t num_offsets,
        const GLuint* culled_instance_buffer, kgl::ShaderBuffer* instance_data_buffer,uint32_t num_instances) = 0;

    virtual GLuint GetDepthTexture() const;

    virtual uint32_t GetLodsCount() const;

protected:
    /*********************************************************
    各种遮挡剔除实现的通用功能。
    @param  glm::vec4 * planes
    @param  const glm::mat4 & view_projection
    @return void    
    *********************************************************/
    void ComputeFrustumFromViewProjection(glm::vec4* planes, const glm::mat4& view_projection);
};





#endif // culling_h__