/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

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
#include "kgl_lib_pch.h"
#include "kgl_primitive.h"
#include "kgl_defines.h"
#include "kgl_gpu_particle_system.h"

namespace kgl
{
    GPUParticleSystem::GPUParticleSystem()
    {
        is_initialized_ = false;
        current_read_buffer_ = 0;
    }

    GPUParticleSystem::~GPUParticleSystem()
    {
        this->ReleaseParticleSystem();
    }

	bool GPUParticleSystem::InitalizeParticleSystem(const char* update_vs_file_path, const char* update_gs_file_path, const char* render_vs_file_path, const char* render_gs_file_path, const char* render_fs_file_path, const std::vector<const char*>& varings)
    {
        if (is_initialized_)
            return false;

        update_shader_ = new GPUProgram();
        update_shader_->CreateTransformFeedbackShaderFromFile(update_vs_file_path, update_gs_file_path, varings);
        render_shader_ = new GPUProgram();
        render_shader_->CreateFromFile(render_vs_file_path, render_fs_file_path, render_gs_file_path);

        glGenTransformFeedbacks(1, &transform_feedback_buf_id_);

        // glGenQueries(GLsizei n, GLuint *ids)函数返回n个当前未使用的query object ID，
        // 可用于表示遮挡对象的名称。这些名称是在ids数组中返回的。
        glGenQueries(1, &query_object_id_);

        // 生成粒子顶点缓冲区，vertex buffer object
        glGenBuffers(2, particle_vbo_id_);

        // 创建两个vertex array object，VAO （ Vertex Array Object ）是OpenGL用来处理顶点数据的
        // 一个缓冲区对象，它不能单独使用，都是结合VBO来一起使用的。VAO是OpenGL CoreProfile 引入的
        // 一个特性。事实上在CoreProfile中做顶点数据传入时，必须使用VAO方式。
        glGenVertexArrays(2, particle_vao_id_);

        CParticle initialization_particle;
        initialization_particle.iType = PARTICLE_TYPE_GENERATOR;

        for (int i = 0; i < 2; ++i)
        {
            glBindVertexArray(particle_vao_id_[i]); // 绑定一个vertex array object
            glBindBuffer(GL_ARRAY_BUFFER, particle_vbo_id_[i]); // 绑定一个vertex buffer object

            // glBufferData函数分配缓冲区对象并初始化，制定读写标志为 GL_DYNAMIC_DRAW 表示数据可以多次指定，
            // 并且可以多次作为绘图和图像指定函数的源数据
            glBufferData(GL_ARRAY_BUFFER, sizeof(CParticle)*MAX_PARTICLES_ON_SCENE, nullptr, GL_DYNAMIC_DRAW);

            /* 前面都是分配了一个VBO，然后将顶点属性数据直接拷贝到整个VBO，实际上可以先为VBO分配空间，然后根据需要更新
               VBO中一段内容，这里使用到的方法就是glBufferSubData。该方法的原型如下：

               void glBufferSubData( GLenum target,        // 绑定到的目标对象
                                     GLintptr offset,      // 在缓存对象中的偏移
                                     GLsizeiptr size,      // 指定更改的数据大小
                                     const GLvoid * data); // 用指针指定原始数据的位置*/
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(CParticle), &initialization_particle);

            for (int j = 0; j < NUM_PARTICLE_ATTRIBUTES; ++j)
                glEnableVertexAttribArray(j);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CParticle), (const GLvoid*)0);  // 位置
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CParticle), (const GLvoid*)12); // 发射速度
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(CParticle), (const GLvoid*)24); // 颜色
            glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(CParticle), (const GLvoid*)36); // 生命周期
            glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(CParticle), (const GLvoid*)40); // 粒子渲染尺寸
            glVertexAttribPointer(5, 1, GL_INT, GL_FALSE, sizeof(CParticle), (const GLvoid*)44);   // 粒子类型
        }

        current_read_buffer_ = 0;
        particles_num_ = 1;
        is_initialized_ = true;
        return true;
    }

    float grandf(float fMin, float fAdd)
    {
        float fRandom = float(rand() % (RAND_MAX + 1)) / float(RAND_MAX);
        return fMin + fAdd*fRandom;
    }

    void GPUParticleSystem::UpdateParticles(float time_passed)
    {
        if (!is_initialized_)return;

        update_shader_->Use();


        update_shader_->ApplyFloat(time_passed, "fTimePassed");
        update_shader_->ApplyVector3(glm::value_ptr(generated_position_), "vGenPosition");
        update_shader_->ApplyVector3(glm::value_ptr(generated_velocity_min_), "vGenVelocityMin");
        update_shader_->ApplyVector3(glm::value_ptr(generated_velocity_range_), "vGenVelocityRange");
        update_shader_->ApplyVector3(glm::value_ptr(generated_particle_color_), "vGenColor");
        update_shader_->ApplyVector3(glm::value_ptr(generated_gravity_vector_), "vGenGravityVector");
        // 
        update_shader_->ApplyFloat(generated_life_min_, "fGenLifeMin");
        update_shader_->ApplyFloat(generated_life_range_, "fGenLifeRange");
        // 
        update_shader_->ApplyFloat(generated_size_, "fGenSize");
        update_shader_->ApplyInt(0, "iNumToGenerate");

        elapsed_time_ += time_passed;

        if (elapsed_time_ > next_generation_time_)
        {
            update_shader_->ApplyInt(generated_particles_count_, "iNumToGenerate");// spUpdateParticles.SetUniform("iNumToGenerate", iNumToGenerate);
            elapsed_time_ -= next_generation_time_;

            glm::vec3 vRandomSeed = glm::vec3(grandf(-10.0f, 20.0f), grandf(-10.0f, 20.0f), grandf(-10.0f, 20.0f));
            update_shader_->ApplyVector3(glm::value_ptr(vRandomSeed), "vRandomSeed");//spUpdateParticles.SetUniform("vRandomSeed", &vRandomSeed);
        }

        // 参考链接 https://www.jianshu.com/p/b9115545fe06

        glEnable(GL_RASTERIZER_DISCARD);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transform_feedback_buf_id_);

        glBindVertexArray(particle_vao_id_[current_read_buffer_]);
        glEnableVertexAttribArray(1); // Re-enable velocity

        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particle_vbo_id_[1 - current_read_buffer_]);

        glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query_object_id_);
        glBeginTransformFeedback(GL_POINTS);

        glDrawArrays(GL_POINTS, 0, particles_num_);

        glEndTransformFeedback();

        glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
        glGetQueryObjectiv(query_object_id_, GL_QUERY_RESULT, &particles_num_);

        current_read_buffer_ = 1 - current_read_buffer_;

        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    }

    void GPUParticleSystem::RenderParticles()
    {
        if (!is_initialized_)
            return;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glDepthMask(0);

        glDisable(GL_RASTERIZER_DISCARD);
        render_shader_->Use();// .UseProgram();
        render_shader_->ApplyMatrix(glm::value_ptr(projection_matrix_),"matrices.mProj");
        render_shader_->ApplyMatrix(glm::value_ptr(view_matrix_), "matrices.mView");
        render_shader_->ApplyVector3(glm::value_ptr(vQuad1), "vQuad1");
        render_shader_->ApplyVector3(glm::value_ptr(vQuad2), "vQuad2");
        render_shader_->ApplyTexture(particle_texture_, "gSampler",  0);

        glBindVertexArray(particle_vao_id_[current_read_buffer_]);
        glDisableVertexAttribArray(1); // Disable velocity, because we don't need it for rendering

        glDrawArrays(GL_POINTS, 0, particles_num_);

        glDepthMask(1);
        glDisable(GL_BLEND);
    }

	void GPUParticleSystem::SetMatrices(const glm::mat4& view_matrix, const glm::mat4& projection_matrix, const glm::vec3& camera_forward_direction, const glm::vec3& camera_up_direction)
    {
        projection_matrix_ = projection_matrix;
		view_matrix_ = view_matrix;
		vQuad1 = glm::cross(camera_forward_direction, camera_up_direction);
        vQuad1 = glm::normalize(vQuad1); // 构造出一个垂直于camera forward向量和camera up向量都垂直的向量vQuadl
		vQuad2 = glm::cross(camera_forward_direction, vQuad1); // 构造出一个垂直于camera forward向量和vQuad1向量都垂直的向量vQuad2
        vQuad2 = glm::normalize(vQuad2);
    }

    void GPUParticleSystem::SetGeneratorProperties(const glm::vec3& gen_position, const glm::vec3& gen_velocity_min, const glm::vec3& gen_velocity_max, const glm::vec3& gen_gravity_vector, const glm::vec3& gen_color, float gen_life_min, float gen_life_max, float gen_size, float every, int num_to_generate)
    {
        generated_position_ = gen_position;
        generated_velocity_min_ = gen_velocity_min;
        generated_velocity_range_ = gen_velocity_max - gen_velocity_min;
        generated_gravity_vector_ = gen_gravity_vector;
        generated_size_ = gen_size;
        generated_life_min_ = gen_life_min;
        generated_life_range_ = gen_life_max - gen_life_min;
        next_generation_time_ = every;
        elapsed_time_ = 0.8f;
        generated_particles_count_ = num_to_generate;
        generated_particle_color_ = gen_color;
    }

    int GPUParticleSystem::GetNumParticles()
    {
        return particles_num_;
    }

    bool GPUParticleSystem::ReleaseParticleSystem()
    {
        particle_texture_.reset();
        KGL_SAFE_DELETE(update_shader_);
        KGL_SAFE_DELETE(render_shader_);
        return true;
    }
}