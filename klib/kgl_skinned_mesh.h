// 蒙皮网格类基类
#ifndef kgl_skinned_mesh_h__
#define kgl_skinned_mesh_h__

#include "kgl_vertex_type.h"
#include "kgl_gpu_program.h"
#include "kgl_mesh.h"

namespace kgl
{
    class SkinnedMesh : public Mesh
    {
    public:
        SkinnedMesh();
        virtual ~SkinnedMesh();

        /// <summary>
        /// Setups the specified vertex_type.
        /// </summary>
        /// <param name="vertex_type">The vertex_type.</param>
        /// <param name="vertex_data">The vertex_data.</param>
        /// <param name="vertex_byte_count">The vertex_byte_count.</param>
        /// <param name="index_data">The index_data.</param>
        /// <param name="index_byte_count">The index_byte_count.</param>
        /// <param name="texture_array">The texture_array.</param>
        /// <param name="local_transform_matrix">本mesh的模型变换矩阵</param>
        void Setup(VertexType vertex_type, const unsigned char* vertex_data, uint32_t vertex_byte_count, const uint32_t* index_data, uint32_t index_byte_count, const std::vector<TextureSPtr>& texture_array, const glm::mat4& local_transform_matrix);

        /// <summary>
        /// Draws this instance.
        /// </summary>
        virtual void Draw();

        /// <summary>
        /// Sets the shader.
        /// </summary>
        /// <param name="shader">The shader.</param>
        inline void SetShader(GPUProgramSPtr shader)
        {
            shader_ = shader;
        }

        /// <summary>
        /// Gets the shader.
        /// </summary>
        /// <returns>GPUProgramSPtr.</returns>
        inline GPUProgramSPtr GetShader()
        {
            return shader_;
        }

        inline const glm::mat4& GetLocalTransformMatrix() const
        {
            return local_transform_matrix_;
        }

    protected:
        /// <summary>
        /// Clears this instance.
        /// </summary>
        virtual void Clear() override;

        /// <summary>
        /// Setups the vertex attribute buffer pn bone t1.
        /// </summary>
        virtual void SetupVertexAttributeBufferPNBoneT1() override;
    };
}

#endif // kgl_skinned_mesh_h__