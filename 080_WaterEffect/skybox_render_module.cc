#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_defines.h"
#include "skybox_render_module.h"

static const float SKY_BOX_SIZE = 500.0f;

static const float VERTICES[] =
{
    -SKY_BOX_SIZE, SKY_BOX_SIZE, -SKY_BOX_SIZE,
    -SKY_BOX_SIZE, -SKY_BOX_SIZE, -SKY_BOX_SIZE,
    SKY_BOX_SIZE, -SKY_BOX_SIZE, -SKY_BOX_SIZE,
    SKY_BOX_SIZE, -SKY_BOX_SIZE, -SKY_BOX_SIZE,
    SKY_BOX_SIZE, SKY_BOX_SIZE, -SKY_BOX_SIZE,
    -SKY_BOX_SIZE, SKY_BOX_SIZE, -SKY_BOX_SIZE,

    -SKY_BOX_SIZE, -SKY_BOX_SIZE, SKY_BOX_SIZE,
    -SKY_BOX_SIZE, -SKY_BOX_SIZE, -SKY_BOX_SIZE,
    -SKY_BOX_SIZE, SKY_BOX_SIZE, -SKY_BOX_SIZE,
    -SKY_BOX_SIZE, SKY_BOX_SIZE, -SKY_BOX_SIZE,
    -SKY_BOX_SIZE, SKY_BOX_SIZE, SKY_BOX_SIZE,
    -SKY_BOX_SIZE, -SKY_BOX_SIZE, SKY_BOX_SIZE,

    SKY_BOX_SIZE, -SKY_BOX_SIZE, -SKY_BOX_SIZE,
    SKY_BOX_SIZE, -SKY_BOX_SIZE, SKY_BOX_SIZE,
    SKY_BOX_SIZE, SKY_BOX_SIZE, SKY_BOX_SIZE,
    SKY_BOX_SIZE, SKY_BOX_SIZE, SKY_BOX_SIZE,
    SKY_BOX_SIZE, SKY_BOX_SIZE, -SKY_BOX_SIZE,
    SKY_BOX_SIZE, -SKY_BOX_SIZE, -SKY_BOX_SIZE,

    -SKY_BOX_SIZE, -SKY_BOX_SIZE, SKY_BOX_SIZE,
    -SKY_BOX_SIZE, SKY_BOX_SIZE, SKY_BOX_SIZE,
    SKY_BOX_SIZE, SKY_BOX_SIZE, SKY_BOX_SIZE,
    SKY_BOX_SIZE, SKY_BOX_SIZE, SKY_BOX_SIZE,
    SKY_BOX_SIZE, -SKY_BOX_SIZE, SKY_BOX_SIZE,
    -SKY_BOX_SIZE, -SKY_BOX_SIZE, SKY_BOX_SIZE,

    -SKY_BOX_SIZE, SKY_BOX_SIZE, -SKY_BOX_SIZE,
    SKY_BOX_SIZE, SKY_BOX_SIZE, -SKY_BOX_SIZE,
    SKY_BOX_SIZE, SKY_BOX_SIZE, SKY_BOX_SIZE,
    SKY_BOX_SIZE, SKY_BOX_SIZE, SKY_BOX_SIZE,
    -SKY_BOX_SIZE, SKY_BOX_SIZE, SKY_BOX_SIZE,
    -SKY_BOX_SIZE, SKY_BOX_SIZE, -SKY_BOX_SIZE,

    -SKY_BOX_SIZE, -SKY_BOX_SIZE, -SKY_BOX_SIZE,
    -SKY_BOX_SIZE, -SKY_BOX_SIZE, SKY_BOX_SIZE,
    SKY_BOX_SIZE, -SKY_BOX_SIZE, -SKY_BOX_SIZE,
    SKY_BOX_SIZE, -SKY_BOX_SIZE, -SKY_BOX_SIZE,
    -SKY_BOX_SIZE, -SKY_BOX_SIZE, SKY_BOX_SIZE,
    SKY_BOX_SIZE, -SKY_BOX_SIZE, SKY_BOX_SIZE
};

const char * const texture_files[] =
{
    "resources/image/080_water_effect/skybox/right.png",
    "resources/image/080_water_effect/skybox/left.png",
    "resources/image/080_water_effect/skybox/top.png",
    "resources/image/080_water_effect/skybox/bottom.png",
    "resources/image/080_water_effect/skybox/back.png",
    "resources/image/080_water_effect/skybox/front.png"
};

const char* const night_texture_files[] =
{
    "resources/image/080_water_effect/skybox/nightRight.png",
    "resources/image/080_water_effect/skybox/nightLeft.png",
    "resources/image/080_water_effect/skybox/nightTop.png",
    "resources/image/080_water_effect/skybox/nightBottom.png",
    "resources/image/080_water_effect/skybox/nightBack.png",
    "resources/image/080_water_effect/skybox/nightFront.png"
};

SkyboxRenderModule::SkyboxRenderModule(wfx::LoaderSPtr loader, kgl::CameraSPtr camera)
{
    cube_ = loader->loadToVao(std::vector<float>(std::begin(VERTICES), std::end(VERTICES)), 3);
    texture_id_ = loader->loadCubeMap(std::vector<std::string>(std::begin(texture_files), std::end(texture_files)));
    night_texture_id_ = loader->loadCubeMap(std::vector<std::string>(std::begin(night_texture_files), std::end(night_texture_files)));
    this->InitShader();
}

void SkyboxRenderModule::InitShader()
{
    const char* vs_file = "resources/shader/080_skybox_vs.glsl";
    const char* fs_file = "resources/shader/080_skybox_fs.glsl";
    shader_ = new kgl::GPUProgram;
    shader_->CreateFromFile(vs_file, fs_file, nullptr);
    loc_projection_matrix_ = shader_->GetUniformLocation("projectionMatrix");//uniform mat4 projectionMatrix;
    loc_view_matrix_ = shader_->GetUniformLocation("viewMatrix");//uniform mat4 viewMatrix;
    loc_cube_map_ = shader_->GetUniformLocation("cubeMap");//uniform samplerCube cubeMap;
    loc_cube_map_2_ = shader_->GetUniformLocation("cubeMap2");//uniform samplerCube cubeMap2;
    loc_blend_factor_ = shader_->GetUniformLocation("blendFactor");//uniform float blendFactor;
    loc_fog_color_ = shader_->GetUniformLocation("fogColor");//uniform vec3 fogColor;
}

void SkyboxRenderModule::render(kgl::CameraSPtr camera, float r, float g, float b)
{
    shader_->Use();
    shader_->ApplyMatrix(glm::value_ptr(camera->GetViewMatrix()), loc_view_matrix_);//shader.load_view_matrix(cam);
    shader_->ApplyMatrix(glm::value_ptr(camera->GetProjectionMatrix()), loc_projection_matrix_);//shader.load_view_matrix(cam);
    shader_->ApplyVector3(glm::value_ptr(glm::vec3(r, g, b)), loc_fog_color_);//shader.load_fog_colour(r, g, b);

    glBindVertexArray(cube_.getVaoId());
    glEnableVertexAttribArray(0);
    bind_textures();
    glDrawArrays(GL_TRIANGLES, 0, cube_.getVertexCount());
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

SkyboxRenderModule::~SkyboxRenderModule()
{
    KGL_SAFE_DELETE(shader_);
}

void SkyboxRenderModule::bind_textures()
{
    //	time += get_frame_time_seconds().count() * 1000;
    //	time = std::fmod(time, 24000);
    GLuint texture1;
    GLuint texture2;
    float blendFactor = 0.0;
#if 0
    if (time >= 0 && time < 5000){
        texture1 = night_texture_id;
        texture2 = night_texture_id;
        blendFactor = (time - 0) / (5000 - 0);
    }
    else if (time >= 5000 && time < 8000){
        texture1 = night_texture_id;
        texture2 = texture_id;
        blendFactor = (time - 5000) / (8000 - 5000);
    }
    else if (time >= 8000 && time < 21000){
        texture1 = texture_id;
        texture2 = texture_id;
        blendFactor = (time - 8000) / (21000 - 8000);
    }
    else{
        texture1 = texture_id;
        texture2 = night_texture_id;
        blendFactor = (time - 21000) / (24000 - 21000);
    }
#endif
    texture1 = texture_id_;
    texture2 = texture_id_;
    shader_->ApplyTexture(loc_cube_map_, 0, texture1, GL_TEXTURE_CUBE_MAP);
    shader_->ApplyTexture(loc_cube_map_2_, 1, texture2, GL_TEXTURE_CUBE_MAP);
    shader_->ApplyFloat(loc_blend_factor_, blendFactor);//shader.load_blend_factor(blendFactor);
}
