#include "kgl_lib_pch.h"
#include "kgl_math_tools.h"
#include "kgl_defines.h"
#include "kgl_wfx_water_renderer.h"

namespace kgl
{
    namespace water_effect
    {
        const char* VERTEX_SHADER_FILE = "resources/shader/080_water_vs.glsl";
        const char* FRAGMENT_SHADER_FILE = "resources/shader/080_water_fs.glsl";

        const char* DUDV_MAP = "resources/image/framework/water_effect/water_dudv.png";
        const char* NORMAL_MAP = "resources/image/framework/water_effect/normal_map.png";
        const float WAVE_SPEED = 0.03f;
        const std::vector<GLfloat> POSITIONS{ -1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1 };

        WaterRenderer::WaterRenderer(LoaderSPtr loader, WaterFrameBuffersSPtr fbos) : loader_(loader), fbos_(fbos)
        {
            quad_ = loader_->loadToVao(POSITIONS, 2);
            dudvTextureId_ = loader_->loadTexture(DUDV_MAP);
            normalMapTextureId_ = loader_->loadTexture(NORMAL_MAP);
            rs_blend_reder_water_.SetBlendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        WaterRenderer::~WaterRenderer()
        {
            loader_.reset();
            fbos_.reset();
            KGL_SAFE_DELETE(water_shader_);
        }

        void WaterRenderer::InitWaterShader()
        {
            water_shader_ = new GPUProgram();
            water_shader_->CreateFromFile(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE, nullptr);

            loc_projection_matrix_ = water_shader_->GetUniformLocation("projectionMatrix");//uniform mat4 projectionMatrix;
            loc_view_matrix_ = water_shader_->GetUniformLocation("viewMatrix");       //uniform mat4 viewMatrix;
            loc_model_matrix_ = water_shader_->GetUniformLocation("modelMatrix"); //uniform mat4 modelMatrix;
            loc_camera_position_ = water_shader_->GetUniformLocation("cameraPosition");//uniform vec3 cameraPosition;
            loc_light_position_ = water_shader_->GetUniformLocation("lightPosition");//uniform vec3 lightPosition;

            loc_reflection_texture_ = water_shader_->GetUniformLocation("reflectionTexture");//uniform sampler2D reflectionTexture;
            loc_refraction_texture_ = water_shader_->GetUniformLocation("refractionTexture");//uniform sampler2D refractionTexture;
            loc_dudv_map_ = water_shader_->GetUniformLocation("dudvMap");//uniform sampler2D dudvMap;
            loc_normal_map_ = water_shader_->GetUniformLocation("normalMap");//uniform sampler2D normalMap;
            loc_depth_map_ = water_shader_->GetUniformLocation("depthMap");//uniform sampler2D depthMap;

            loc_light_color_ = water_shader_->GetUniformLocation("lightColor");//uniform vec3 lightColor;
            loc_move_factor_ = water_shader_->GetUniformLocation("moveFactor");//uniform float moveFactor;
        }

        void WaterRenderer::render(const std::vector<WaterTile>& water, CameraSPtr camera, const Light& sun, float frame_interval)
        {
            prepareRender(camera, sun, frame_interval);

            for (const WaterTile& tile : water)
            {
                glm::mat4 modelMatrix = MathTools::CreateTransformationMatrix(
                    glm::vec3(tile.getX(), tile.getHeight(), tile.getZ()),
                    glm::vec3(0),
                    glm::vec3(WaterTile::TILE_SIZE));

                water_shader_->ApplyMatrix(glm::value_ptr(modelMatrix), loc_model_matrix_);//shader_.loadModelMatrix(modelMatrix);
                glDrawArrays(GL_TRIANGLES, 0, quad_.getVertexCount());
            }

            unbind();
        }

        void WaterRenderer::prepareRender(CameraSPtr camera, const Light& sun, float frame_interval)
        {
            water_shader_->Use();//shader_.start();

            water_shader_->ApplyMatrix(glm::value_ptr(camera->GetViewMatrix()), loc_view_matrix_);//shader_.loadViewMatrix(camera);
            water_shader_->ApplyMatrix(glm::value_ptr(camera->GetProjectionMatrix()), loc_projection_matrix_);//shader_.loadProjectionMatrix(projection);
            water_shader_->ApplyVector3(glm::value_ptr(sun.getPosition()), loc_light_position_);//shader_.loadLight(sun);
            water_shader_->ApplyVector3(glm::value_ptr(sun.getColor()), loc_light_color_);
            water_shader_->ApplyVector3(glm::value_ptr(camera->GetPosition()), loc_camera_position_);

            moveFactor_ += WAVE_SPEED * frame_interval;// moveFactor_ += WAVE_SPEED * DisplayManager::getFrameTime().asSeconds();
            moveFactor_ = static_cast<float>(std::fmod(moveFactor_, 1));
            water_shader_->ApplyFloat(moveFactor_, loc_move_factor_);//shader_.loadMoveFactor(moveFactor_);

            glBindVertexArray(quad_.getVaoId());
            glEnableVertexAttribArray(0);

            //glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_2D, fbos_.getReflectionTexture());
            water_shader_->ApplyTexture(loc_reflection_texture_, 0, fbos_->getReflectionTexture());

            //glActiveTexture(GL_TEXTURE1);
            //glBindTexture(GL_TEXTURE_2D, fbos_.getRefractionTexture());
            water_shader_->ApplyTexture(loc_refraction_texture_, 1, fbos_->getRefractionTexture());

            //glActiveTexture(GL_TEXTURE2);
            //glBindTexture(GL_TEXTURE_2D, dudvTextureId_);
            water_shader_->ApplyTexture(loc_dudv_map_, 2, dudvTextureId_);

            //glActiveTexture(GL_TEXTURE3);
            //glBindTexture(GL_TEXTURE_2D, normalMapTextureId_);
            water_shader_->ApplyTexture(loc_normal_map_, 3, normalMapTextureId_);

            //glActiveTexture(GL_TEXTURE4);
            //glBindTexture(GL_TEXTURE_2D, fbos_.getRefractionDepthTexture());
            water_shader_->ApplyTexture(loc_depth_map_, 4, fbos_->getRefractionDepthTexture());

            //glEnable(GL_BLEND);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            rs_blend_reder_water_.SetEnable(GL_TRUE);
            rs_blend_reder_water_.Use();
        }

        void WaterRenderer::unbind()
        {
            rs_blend_reder_water_.SetEnable(GL_FALSE);// glDisable(GL_BLEND);
            rs_blend_reder_water_.Use();
            glDisableVertexAttribArray(0);
            glBindVertexArray(0);
            water_shader_->Finish(); // shader_.stop();
        }
    }
}