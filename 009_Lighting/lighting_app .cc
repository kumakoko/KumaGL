// 简单的纹理应用示例
#include "lighting_app.h"
#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"

LightingApp::LightingApp() :gpu_program_(nullptr)
{
}

LightingApp::~LightingApp()
{
	KGL_SAFE_DELETE(gpu_program_);
	KGL_SAFE_DELETE(lighting_shader_);
	cube_.reset();
}

void LightingApp::InitScene()
{
	this->InitMaterial();

	gpu_program_ = new kgl::GPUProgram;
	gpu_program_->CreateFromFile("resources/shader/002_triangle_vs.glsl", "resources/shader/002_triangle_fs.glsl", nullptr);

	lighting_shader_ = new kgl::GPUProgram;
	lighting_shader_->CreateFromFile("resources/shader/009_lighting_vs.glsl", "resources/shader/009_lighting_fs.glsl", nullptr);

	cube_ = kgl::PrimitiveTool::BuildCube();
}

void LightingApp::InitMaterial()
{
	golden_material_.Ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f);
	golden_material_.Diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f);
	golden_material_.Specular = glm::vec3(0.628281f, 0.555802f, 0.366065f);
	golden_material_.Shininess = 0.4f * 128.f;
}

void LightingApp::RenderFrame()
{
	gpu_program_->Use();

	glm::mat4 model_matrix;
	const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
	const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();
	const glm::vec3& view_pos = main_camera_->GetPosition();

	lighting_shader_->Use();
	lighting_shader_->ApplyMatrix(glm::value_ptr(model_matrix), "model_matrix");
	lighting_shader_->ApplyMatrix(glm::value_ptr(view_matrix), "view_matrix");
	lighting_shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "projection_matrix");

	lighting_shader_->ApplyVector3(glm::value_ptr(view_pos), "view_pos");
	lighting_shader_->ApplyLight(&light_, "light");
	lighting_shader_->ApplyMaterial(&golden_material_, "material");
	
	cube_->Draw();
}