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
#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_math_tools.h"
#include "terrain_render_module.h"

const std::size_t MAX_LIGHTS = 4; // 最大的实时灯的数量

TerrainRenderModule::TerrainRenderModule()
{
}

TerrainRenderModule::~TerrainRenderModule()
{
    KGL_SAFE_DELETE(shader_);
}

void TerrainRenderModule::render(const std::vector<wfx::Terrain>& terrains, const std::vector<wfx::Light>& lights, const glm::vec4& clip_plane, kgl::CameraSPtr camera)
{
    const glm::mat4& view_matrix = camera->GetViewMatrix();
    const glm::mat4& project_matrix = camera->GetProjectionMatrix();
    glm::mat4 transform_matrix;

    shader_->Use();

    this->ApplyLights(lights); // 向shader设置灯光参数
    shader_->ApplyVector4(glm::value_ptr(clip_plane), loc_clip_plane_); //  向shader设置裁剪平面参数

    for (const auto& terrain : terrains)
    {
        const wfx::TerrainTexturePack& tex_group = terrain.getTexturePack();

        glBindVertexArray(terrain.getRawModel().getVaoId());
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        shader_->ApplyTexture(loc_bg_texture_, 0, tex_group.getBackgroundTexture().getTextureId());
        shader_->ApplyTexture(loc_red_channel_, 1, tex_group.getRTexture().getTextureId());
        shader_->ApplyTexture(loc_green_channel_, 2, tex_group.getGTexture().getTextureId());
        shader_->ApplyTexture(loc_blue_channel_, 3, tex_group.getBTexture().getTextureId());
        shader_->ApplyTexture(loc_blend_map_, 4, terrain.getBlendMap().getTextureId());
        shader_->ApplyFloat(1, loc_shine_damper_);// shader_->loadShineVariables(1, 0);
        shader_->ApplyFloat(0, loc_reflectivity_);
        shader_->ApplyMatrix(glm::value_ptr(view_matrix), loc_view_matrix_);
        shader_->ApplyMatrix(glm::value_ptr(project_matrix), loc_projection_matrix_);
        shader_->ApplyVector3(glm::value_ptr(glm::vec3(0.5444f, 0.62f, 0.69f)), loc_sky_color_);

        transform_matrix = kgl::MathTools::CreateTransformationMatrix(
            glm::vec3(terrain.getX(), 0.0f, terrain.getZ()),
            glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

        shader_->ApplyMatrix(glm::value_ptr(transform_matrix), loc_transformation_matrix_);

        glDrawElements(GL_TRIANGLES, terrain.getRawModel().getVertexCount(), GL_UNSIGNED_INT, 0);
        unbindTerrain();
    }
}

void TerrainRenderModule::ApplyLights(const std::vector<wfx::Light>& lights)
{
    for (std::size_t i = 0; i < MAX_LIGHTS; i++)
    {
        if (i < (int)lights.size())
        {
            shader_->ApplyVector3(glm::value_ptr(lights[i].getPosition()), loc_light_pos_[i]);//loadVector3(lightPositionLocation_[i], lights[i].getPosition());
            shader_->ApplyVector3(glm::value_ptr(lights[i].getColor()), loc_light_color_[i]);//loadVector3(lightColorLocation_[i], lights[i].getColor());
            shader_->ApplyVector3(glm::value_ptr(lights[i].getAttenuation()), loc_attenuation_[i]);//loadVector3(attenuationLocation_[i], lights[i].getAttenuation());
        }
        else
        {
            shader_->ApplyVector3(glm::value_ptr(glm::vec3(0.f, 0.f, 0.f)), loc_light_pos_[i]); //loaloadVector3(lightPositionLocation_[i], glm::vec3(0, 0, 0));
            shader_->ApplyVector3(glm::value_ptr(glm::vec3(0.f, 0.f, 0.f)), loc_light_color_[i]); //loadloadVector3(lightColorLocation_[i], glm::vec3(0, 0, 0));
            shader_->ApplyVector3(glm::value_ptr(glm::vec3(1.f, 0.f, 0.f)), loc_attenuation_[i]); //loadVector3(attenuationLocation_[i], glm::vec3(1, 0, 0));
        }
    }
}

void TerrainRenderModule::unbindTerrain() const
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

void TerrainRenderModule::InitShader()
{
    memset(loc_light_pos_, 0, sizeof(GLuint) * 4);
    memset(loc_light_color_, 0, sizeof(GLuint) * 4);
    memset(loc_attenuation_, 0, sizeof(GLuint) * 4);

    shader_ = new kgl::GPUProgram();
    shader_->CreateFromFile("resources/shader/080_terrain_vs.glsl", "resources/shader/080_terrain_fs.glsl", nullptr);

    // === fs uniform
    loc_light_pos_[0] = shader_->GetUniformLocation("lightPosition[0]");
    loc_light_pos_[1] = shader_->GetUniformLocation("lightPosition[1]");
    loc_light_pos_[2] = shader_->GetUniformLocation("lightPosition[2]");
    loc_light_pos_[3] = shader_->GetUniformLocation("lightPosition[3]");

    loc_bg_texture_ = shader_->GetUniformLocation("backgroundTexture");	//uniform sampler2D backgroundTexture;
    loc_red_channel_ = shader_->GetUniformLocation("rTexture");			//uniform sampler2D rTexture;
    loc_green_channel_ = shader_->GetUniformLocation("gTexture");		//uniform sampler2D gTexture;
    loc_blue_channel_ = shader_->GetUniformLocation("bTexture");		//uniform sampler2D bTexture;
    loc_blend_map_ = shader_->GetUniformLocation("blendMap");			//uniform sampler2D blendMap;

    loc_light_color_[0] = shader_->GetUniformLocation("lightColor[0]");  // uniform vec3 lightColor[4];
    loc_light_color_[1] = shader_->GetUniformLocation("lightColor[1]");
    loc_light_color_[2] = shader_->GetUniformLocation("lightColor[2]");
    loc_light_color_[3] = shader_->GetUniformLocation("lightColor[3]");

    loc_attenuation_[0] = shader_->GetUniformLocation("attenuation[0]"); // uniform vec3 attenuation[4];
    loc_attenuation_[1] = shader_->GetUniformLocation("attenuation[1]");
    loc_attenuation_[2] = shader_->GetUniformLocation("attenuation[2]");
    loc_attenuation_[3] = shader_->GetUniformLocation("attenuation[3]");

    loc_shine_damper_ = shader_->GetUniformLocation("shineDamper");		//uniform sampler2D shineDamper;
    loc_reflectivity_ = shader_->GetUniformLocation("reflectivity");	//uniform sampler2D reflectivity;
    loc_sky_color_ = shader_->GetUniformLocation("skyColor");			//uniform sampler2D skyColor;


    //== vs uniform
    loc_transformation_matrix_ = shader_->GetUniformLocation("transformationMatrix");
    loc_projection_matrix_ = shader_->GetUniformLocation("projectionMatrix");
    loc_view_matrix_ = shader_->GetUniformLocation("viewMatrix");
    loc_clip_plane_ = shader_->GetUniformLocation("plane"); // uniform vec4 plane;
}