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
#include "parallax_mapping_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_debug_tool.h"
#include "../klib/kgl_vertex_type.h"
#include "../klib/kgl_font_renderer.h"

// http://blog.csdn.net/dreamcs/article/details/7691690

ParallaxMappingApp::ParallaxMappingApp()
{
}

ParallaxMappingApp::~ParallaxMappingApp()
{
	parallax_mapping_shader_.reset();
	wall_square_.reset();
	diffuse_map_texture_.reset();
	height_map_texture_.reset();
}

void ParallaxMappingApp::RenderGUI()
{
	const glm::vec3& camera_pos = main_camera_->GetPosition();
	ImGui::Begin("parallax贴图效果演示");
	ImGui::Checkbox("使用视差贴图效果吗", &use_parallax_effect_);
	ImGui::SliderFloat("水平方向纹理坐标偏移值", &(uv_offset_value_.x), 0.01f, 0.04f);
	ImGui::SliderFloat("垂直方向纹理坐标偏移值", &(uv_offset_value_.y), 0.01f, 0.04f);
	ImGui::End();
}

void ParallaxMappingApp::InitScene()
{
	App::InitScene();

	// 初始化渲染状态
	// 繪製數據到frame buffer時的深度模式
	draw_square_depth_mode_.SetEnable(GL_TRUE);
	draw_square_depth_mode_.SetDepthTestFunc(GL_LESS);

	// 繪製數據到frame object時的揀選模式
	draw_square_cull_mode_.SetEnable(GL_FALSE);
	draw_square_cull_mode_.SetCullMode(GL_BACK); // 指定删除背面
	draw_square_cull_mode_.SetFrontFaceMode(GL_CW); // 待渲染的模型，以逆时针为正面

	glDepthMask(GL_TRUE);
}

void ParallaxMappingApp::PreRenderFrame()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ParallaxMappingApp::RenderScene()
{
	main_camera_->Update();

	const glm::mat4& projection = main_camera_->GetProjectionMatrix();
	const glm::mat4& view = main_camera_->GetViewMatrix();
	glm::mat4 model;

	parallax_mapping_shader_->Use();
	parallax_mapping_shader_->ApplyMatrix(glm::value_ptr(projection), "projection_matrix");
	parallax_mapping_shader_->ApplyMatrix(glm::value_ptr(view), "view_matrix");
	parallax_mapping_shader_->ApplyMatrix(glm::value_ptr(model), "world_matrix");
	parallax_mapping_shader_->ApplyVector3(glm::value_ptr(main_camera_->GetPosition()), "camera_pos_in_world");
	parallax_mapping_shader_->ApplyVector2(use_parallax_effect_ ? glm::value_ptr(uv_offset_value_) :
		                                   glm::value_ptr(glm::vec2(0.f, 0.f)), "offset_value");
	parallax_mapping_shader_->ApplyTexture(diffuse_map_texture_, "diffuse_map", 0);
	parallax_mapping_shader_->ApplyTexture(height_map_texture_, "height_map", 1);

	wall_square_->DrawIndexed();
}

void ParallaxMappingApp::InitMaterials()
{
	App::InitMaterials();
}

void ParallaxMappingApp::InitModel()
{
	wall_square_ = kgl::PrimitiveTool::BuildPNTBT1Square(1, true);

	kgl::TextureParams texture_param = kgl::TextureManager::MakeTextureParamsRGB(GL_REPEAT, GL_LINEAR);
	diffuse_map_texture_ = std::make_shared<kgl::SourceTexture>();
	diffuse_map_texture_->CreateFromFile("resources/image/stone_wall_2.png", texture_param);

	height_map_texture_ = std::make_shared<kgl::SourceTexture>();
	height_map_texture_->CreateFromFile("resources/image/stone_wall_2_height_map.png", texture_param);
}

void ParallaxMappingApp::InitShaders()
{
	parallax_mapping_shader_ = std::make_shared<kgl::GPUProgram>();
	const char* vs_file_path = "resources/shader/017_parallax_mapping_vs.glsl";
	const char* fs_file_path = "resources/shader/017_parallax_mapping_fs.glsl";
	parallax_mapping_shader_->CreateFromFile(vs_file_path, fs_file_path, nullptr);
}

void ParallaxMappingApp::InitMainCamera()
{
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 1.75f);
	main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, pos);
}

void ParallaxMappingApp::ProcessInput()
{
	if (key_state_[GLFW_KEY_W])
	{
//		point_light_.Position.y += 0.01f;
	}

	if (key_state_[GLFW_KEY_S])
	{
//		point_light_.Position.y -= 0.01f;
	}

	if (key_state_[GLFW_KEY_A])
	{
//		point_light_.Position.x -= 0.01f;
	}

	if (key_state_[GLFW_KEY_D])
	{
//		point_light_.Position.x += 0.01f;
	}

	if (key_state_[GLFW_KEY_U])
	{
//		shininess_ += 0.1f;

//		if (shininess_ > 32.0f)
//			shininess_ = 32.0f;
	}

	if (key_state_[GLFW_KEY_J])
	{
//		shininess_ -= 0.1f;

//		if (shininess_ < 0.0f)
//			shininess_ = 0.1f;
	}
}

void ParallaxMappingApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	App::OnKeyAction(window, key, scancode, action, mode);

	if (GLFW_KEY_H == key && action == GLFW_RELEASE)
	{
//		is_toggle_on_ = !is_toggle_on_;
	}
}

void ParallaxMappingApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
	App::OnMouseAction(window, xpos, ypos);
}

void ParallaxMappingApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
	App::OnScrollAction(window, xoffset, yoffset);
}