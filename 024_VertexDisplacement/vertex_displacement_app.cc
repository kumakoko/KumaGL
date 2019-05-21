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
#include "vertex_displacement_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"

VertexDisplacementApp::VertexDisplacementApp() :gpu_program_(nullptr), rectangle_primitive_(nullptr)
{
}

VertexDisplacementApp::~VertexDisplacementApp()
{
    KGL_SAFE_DELETE(gpu_program_);
    texture_1_.reset();
	rectangle_primitive_.reset();
}

void VertexDisplacementApp::RenderGUI()
{
	const glm::vec3& camera_pos = main_camera_->GetPosition();
	ImGui::Begin("在顶点着色器中改变顶点位置形成动画效果");
	ImGui::Checkbox("以线框模式显示", &wireframe_mode_);
	ImGui::SliderFloat("旗帜左上角偏移X量", &(flag_left_top_x_), -10.f, 10.f);
	ImGui::SliderFloat("旗帜左上角偏移Y量", &(flag_left_top_y_), -10.f, 10.f);
	ImGui::SliderFloat("旗帜卷动速度", &(speed_of_ratation_), 8.f, 20.f);
	ImGui::End();
}

void VertexDisplacementApp::InitMainCamera()
{
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 20.f);
	float pitch_angle = 0.0f;
	float yaw_angle = 180.0f;
	float fov = 120.f;
	float near_clip_distance = 0.1f; 
	float far_clip_distance = 1200.0f;
	main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, pos,pitch_angle,yaw_angle,fov,near_clip_distance,far_clip_distance);
}

void VertexDisplacementApp::InitModel()
{
    const GLchar* vs_file_path = nullptr;
    const GLchar* fs_file_path = nullptr;
    std::string texture_1_name;
    std::string texture_2_name;

#if defined(WIN32) || defined(_WIN32)
    vs_file_path = "resources/shader/024_vertex_displacement_vs.glsl";
    fs_file_path = "resources/shader/024_vertex_displacement_fs.glsl";
    texture_1_name = "resources/image/hk_flag.png";
#endif
    
    gpu_program_ = new kgl::GPUProgram;
    gpu_program_->CreateFromFile(vs_file_path, fs_file_path, nullptr);

    kgl::TextureParams texture_param;
    texture_param.wrap_s_mode = GL_REPEAT;
    texture_param.wrap_t_mode = GL_REPEAT;
    texture_param.mag_filter_mode = GL_LINEAR;
    texture_param.min_filter_mode = GL_LINEAR;
    texture_param.internal_format = GL_RGBA;
    texture_param.src_img_px_component_type = GL_UNSIGNED_BYTE;
    texture_param.src_img_format = GL_RGBA;
    texture_param.used_mipmap = false;

    texture_1_ = std::make_shared<kgl::SourceTexture>();
    texture_1_->CreateFromFile(texture_1_name, texture_param);

	rectangle_primitive_ = kgl::PrimitiveTool::BuildNormalTexturedRectangle(16, 16, 8.0f, 5.0f);
}

void VertexDisplacementApp::RenderScene()
{
	draw_mode_.SetCurrentDrawMode(wireframe_mode_ ? kgl::DM_LINE : kgl::DM_FILL);

	double current_time = glfwGetTime();
	double elpased_time = current_time - last_frame_time_;
	last_frame_time_ = current_time;
	
	main_camera_->Update();
    gpu_program_->Use();
    gpu_program_->ApplyTexture(texture_1_, "source_texture_1", 0);

	glm::mat4 model_matrix;
	model_matrix = glm::translate(model_matrix, glm::vec3(flag_left_top_x_, flag_left_top_y_, 0.f));
	//model_matrix = glm::rotate(model_matrix, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
	gpu_program_->ApplyMatrix(glm::value_ptr(model_matrix), "u_world_matrix");

	const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
	gpu_program_->ApplyMatrix(glm::value_ptr(view_matrix), "u_view_matrix");

	const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();
	gpu_program_->ApplyMatrix(glm::value_ptr(projection_matrix), "u_projection_matrix");

	current_angle_ -= speed_of_ratation_ * static_cast<float>(elpased_time);

	while (current_angle_ > 360.0f) current_angle_ -= 360.0f;
	while (current_angle_ < 0.0f) current_angle_ += 360.0f;

	gpu_program_->ApplyFloat(current_angle_, "u_current_angle");

	rectangle_primitive_->Draw();
}
