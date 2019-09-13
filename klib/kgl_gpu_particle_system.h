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
 * \file kgl_gpu_particle_system.h
 * \date 2019/03/11 14:18
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
#ifndef kgl_gpu_particle_system_h__
#define kgl_gpu_particle_system_h__

#include "kgl_gpu_program.h"

namespace kgl
{
#define NUM_PARTICLE_ATTRIBUTES 6
#define MAX_PARTICLES_ON_SCENE 100000

#define PARTICLE_TYPE_GENERATOR 0
#define PARTICLE_TYPE_NORMAL 1
    class CParticle
    {
    public:
        glm::vec3 vPosition;
        glm::vec3 vVelocity;
        glm::vec3 vColor;
        float fLifeTime;
        float fSize;
        int iType;
    };

    class GPUParticleSystem
    {
    public:
		//************************************
		// Method:    InitalizeParticleSystem
		// FullName:  kgl::CParticleSystemTransformFeedback::InitalizeParticleSystem
		// Access:    public 
		// Returns:   bool 初始化成功返回true，失败返回false
		// Qualifier: 初始化粒子系统所需要的各种buffer，以及着色器等等
		// Parameter: const char * update_vs_file_path update粒子时用到的顶点着色器
		// Parameter: const char * update_gs_file_path update粒子时用到的顶点着色器
		// Parameter: const char * render_vs_file_path render粒子时用到的顶点着色器
		// Parameter: const char * render_gs_file_path render粒子时用到的几何着色器
		// Parameter: const char * render_fs_file_path render粒子时用到的片元着色器
		// Parameter: const std::vector<const char * > & varings 用作着色器vary变量的变量名数组
		//************************************
		bool InitalizeParticleSystem(const char* update_vs_file_path, const char* update_gs_file_path, const char* render_vs_file_path, const char* render_gs_file_path, const char* render_fs_file_path, const std::vector<const char*>& varings);

        //************************************
        // Method:    RenderParticles
        // FullName:  kgl::CParticleSystemTransformFeedback::RenderParticles
        // Access:    public 
        // Returns:   void
        // Qualifier: 在GPU中渲染粒子
        //************************************
        void RenderParticles();

        //************************************
        // Method:    UpdateParticles
        // FullName:  kgl::CParticleSystemTransformFeedback::UpdateParticles
        // Access:    public 
        // Returns:   void
        // Qualifier:
        // Parameter: float time_passed
        //************************************
        void UpdateParticles(float time_passed);

        //************************************
        // Method:    SetGeneratorProperties
        // FullName:  kgl::CParticleSystemTransformFeedback::SetGeneratorProperties
        // Access:    public 
        // Returns:   void
        // Qualifier:
        // Parameter: const glm::vec3 & gen_position
        // Parameter: const glm::vec3 & gen_velocity_min
        // Parameter: const glm::vec3 & gen_velocity_max
        // Parameter: const glm::vec3 & gen_gravity_vector
        // Parameter: const glm::vec3 & gen_color
        // Parameter: float gen_life_min
        // Parameter: float gen_life_max
        // Parameter: float gen_size
        // Parameter: float every
        // Parameter: int num_to_generate
        //************************************
        void SetGeneratorProperties(const glm::vec3& gen_position, const glm::vec3& gen_velocity_min, const glm::vec3& gen_velocity_max, const glm::vec3& gen_gravity_vector, const glm::vec3& gen_color, float gen_life_min, float gen_life_max, float gen_size, float every, int num_to_generate);

        //************************************
        // Method:    ClearAllParticles
        // FullName:  kgl::CParticleSystemTransformFeedback::ClearAllParticles
        // Access:    public 
        // Returns:   void
        // Qualifier:
        //************************************
        void ClearAllParticles();
        
        //************************************
        // Method:    ReleaseParticleSystem
        // FullName:  kgl::CParticleSystemTransformFeedback::ReleaseParticleSystem
        // Access:    public 
        // Returns:   bool
        // Qualifier:
        //************************************
        bool ReleaseParticleSystem();

        //************************************
        // Method:    GetNumParticles
        // FullName:  kgl::CParticleSystemTransformFeedback::GetNumParticles
        // Access:    public 
        // Returns:   int
        // Qualifier:
        //************************************
        int GetNumParticles();

        //************************************
        // Method:    SetMatrices
        // FullName:  kgl::CParticleSystemTransformFeedback::SetMatrices
        // Access:    public 
        // Returns:   void
        // Qualifier:
        // Parameter: const glm::mat4 & view_matrix
        // Parameter: const glm::mat4 & projection_matrix
        // Parameter: const glm::vec3 & camera_forward_direction
        // Parameter: const glm::vec3 & camera_up_direction
        //************************************
        void SetMatrices(const glm::mat4& view_matrix, const glm::mat4& projection_matrix, const glm::vec3& camera_forward_direction, const glm::vec3& camera_up_direction);

        //************************************
        // Method:    CParticleSystemTransformFeedback
        // FullName:  kgl::CParticleSystemTransformFeedback::CParticleSystemTransformFeedback
        // Access:    public 
        // Returns:   
        // Qualifier:
        //************************************
        GPUParticleSystem();

        //************************************
        // Method:    ~CParticleSystemTransformFeedback
        // FullName:  kgl::CParticleSystemTransformFeedback::~CParticleSystemTransformFeedback
        // Access:    public 
        // Returns:   
        // Qualifier:
        //************************************
        ~GPUParticleSystem();

        //************************************
        // Method:    SetParticleTexture
        // FullName:  kgl::CParticleSystemTransformFeedback::SetParticleTexture
        // Access:    public 
        // Returns:   void
        // Qualifier: 设置粒子所用到的纹理
        // Parameter: TextureSPtr particle_texture 粒子所用到的纹理
        //************************************
        inline void SetParticleTexture(TextureSPtr particle_texture)
        {
            particle_texture_ = particle_texture;
        }

    protected:        
        /// <summary>
        /// 本粒子系统初始化完成了吗
        /// </summary>
        bool is_initialized_;
        
        /// <summary>
        /// 表征一个transform feedback 缓冲区的id
        /// </summary>
        GLuint transform_feedback_buf_id_ = 0;
        
        /// <summary>
        /// 粒子顶点用到的vbo对象id
        /// </summary>
        GLuint particle_vbo_id_[2];
        
        /// <summary>
        /// 粒子顶点用到的vao对象id
        /// </summary>
        GLuint particle_vao_id_[2];
        
        /// <summary>
        /// The query_object_id_
        /// </summary>
        GLuint query_object_id_;

        // UINT uiTexture;

        /// <summary>
        /// 当前正在读取的缓冲区的索引
        /// </summary>
        int current_read_buffer_;

        int particles_num_;
        glm::mat4 projection_matrix_;
        glm::mat4 view_matrix_;
        glm::vec3 vQuad1, vQuad2;
        glm::vec3 generated_position_;
        glm::vec3 generated_velocity_min_;
        glm::vec3 generated_velocity_range_;
        glm::vec3 generated_gravity_vector_;
        glm::vec3 generated_particle_color_; // 生成的粒子的RGB颜色
        float generated_life_min_;
        float generated_life_range_;
        float generated_size_;
        float elapsed_time_;
        float next_generation_time_;
        int generated_particles_count_;
        GPUProgram* render_shader_ = nullptr;
        GPUProgram* update_shader_ = nullptr;
        TextureSPtr particle_texture_;
    };
}

#endif // kgl_gpu_particle_system_h__