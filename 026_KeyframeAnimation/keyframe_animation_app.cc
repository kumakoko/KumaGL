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
#include "keyframe_animation_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_debug_tool.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"

const int MAX_ANIMATIONS = 21;

// 每一个animation的起始帧编号，结束帧编号和帧数的数组
kgl::MD2AnimClipInfo s_anim_clip_info[MAX_ANIMATIONS] =
{
	// first, last, fps
	{ 0, 39, 9 },		// STAND
	{ 40, 45, 10 },		// RUN
	{ 46, 53, 10 },		// ATTACK
	{ 54, 57, 7 },		// PAIN_A
	{ 58, 61, 7 },		// PAIN_B
	{ 62, 65, 7 },		// PAIN_C
	{ 66, 71, 7 },		// JUMP
	{ 72, 83, 7 },		// FLIP
	{ 84, 94, 7 },		// SALUTE
	{ 95, 111, 10 },	// FALLBACK
	{ 112, 122, 7 },	// WAVE
	{ 123, 134, 6 },	// POINTIING
	{ 135, 153, 10 },	// CROUCH_STAND
	{ 154, 159, 7 },	// CROUCH_WALK
	{ 160, 168, 10 },	// CROUCH_ATTACK
	{ 196, 172, 7 },	// CROUCH_PAIN
	{ 173, 177, 5 },	// CROUCH_DEATH
	{ 178, 183, 7 },	// DEATH_FALLBACK
	{ 184, 189, 7 },	// DEATH_FALLFORWARD
	{ 190, 197, 7 },	// DEATH_FALLBACKSLOW
	{ 198, 198, 5 },	// BOOM
};

// 每一个动作帧的名字
const char* anim_clip_name[MAX_ANIMATIONS] =
{
	"Stand",
	"Run",
	"Attack",
	"Pain A",
	"Pain B",
	"Pain C",
	"Jump",
	"Flip",
	"Salute",
	"Fallback",
	"Wave",
	"Pointing",
	"Crouch Stand",
	"Crouch Walk",
	"Crouch Attack",
	"Crouch Pain",
	"Crouch Death",
	"Death Fallback",
	"Death Fall Forward",
	"Death Fallback Slow",
	"Boom"
};

KeyframeAnimationApp::KeyframeAnimationApp() :
model_(nullptr), model_rotation_around_y_axis_(-1.09f), model_rotation_around_x_axis_(-89.54f),
animation_play_speed_(0.005f)
{
}

KeyframeAnimationApp::~KeyframeAnimationApp()
{
	model_shader_.reset();
	KGL_SAFE_DELETE(model_);
}

void KeyframeAnimationApp::InitMainCamera()
{
	main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, glm::vec3(-13.0f, 0.1f, 120.0f));
	main_camera_->SetCameraSpeed(0.5f);
}

void KeyframeAnimationApp::RenderGUI()
{
	const glm::vec3& camera_pos = main_camera_->GetPosition();
	ImGui::Begin("026 KeyframeAnimation -- 顶点帧动画");
	ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);
	ImGui::Text("按WSAD键向前后左右移动摄像机");
	ImGui::Text("按UI键顺逆时针绕世界坐标X轴转动模型");
	ImGui::Text("按JK键顺逆时针绕世界坐标Y轴转动模型");
	ImGui::Text("摄像机坐标: (%.1f,%.1f,%.1f)", camera_pos.x, camera_pos.y, camera_pos.z);
	ImGui::Text("模型绕Y轴旋转的角度：%.2f", model_rotation_around_y_axis_);
	ImGui::Text("模型绕X轴旋转的角度：%.2f", model_rotation_around_x_axis_);
	ImGui::SliderFloat("动画播放速率", &animation_play_speed_, 0.005f, 0.03f);

	// 当前选中的那个animation clip的名字
	static const char* item_current = anim_clip_name[0];

	if (ImGui::BeginCombo("动作序列名", item_current, 0))
	{
		for (int n = 0; n < MAX_ANIMATIONS; n++)
		{
			bool is_selected = (item_current == anim_clip_name[n]);

			if (ImGui::Selectable(anim_clip_name[n], is_selected))
			{
				item_current = anim_clip_name[n];
				playing_animation_state_ = model_->StartAnimation(item_current);
			}

			if (is_selected)
			{
				// Set the initial focus when opening the combo (scrolling + for keyboard 
				// navigation support in the upcoming navigation branch)
				ImGui::SetItemDefaultFocus();   
			}
		}

		ImGui::EndCombo();
	}

	ImGui::End();
}

void KeyframeAnimationApp::InitScene()
{
	App::InitScene();

	model_shader_ = std::make_shared<kgl::GPUProgram>();
	model_shader_->CreateFromFile(
		"resources/shader/026_md2_keyframe_anim_vs.glsl",
		"resources/shader/026_md2_keyframe_anim_fs.glsl", nullptr);
	model_ = new kgl::MD2Model(model_shader_);
	model_->LoadFromFile("resources/model/samourai/Samourai.md2");

	for (int i = 0; i < MAX_ANIMATIONS; ++i)
	{
		model_->AddKeyframeInfo(anim_clip_name[i], s_anim_clip_info[i]);
	}

	//设置初始状态为stand
	playing_animation_state_ = model_->StartAnimation("Stand");

	// 设置拣选模式和深度检测模式
	cull_mode_.SetEnable(TRUE);
	cull_mode_.SetFrontFaceMode(GL_CW);
	cull_mode_.SetCullMode(GL_BACK);
	depth_test_mode_.SetEnable(GL_TRUE);
	depth_test_mode_.SetDepthTestFunc(GL_LESS);

	cull_mode_.Use();
	depth_test_mode_.Use();
}

void KeyframeAnimationApp::RenderScene()
{;
	main_camera_->Update();

	glm::mat4 model_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 0));
	model_matrix = glm::rotate(model_matrix, model_rotation_around_y_axis_, glm::vec3(0, 1, 0));
	model_matrix = glm::rotate(model_matrix, model_rotation_around_x_axis_, glm::vec3(1, 0, 0));

	const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
	const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();

	model_->Render(&playing_animation_state_, model_matrix, view_matrix, projection_matrix);
	model_->UpdateAnimation(&playing_animation_state_, animation_play_speed_);
}

void KeyframeAnimationApp::ProcessInput()
{
	if (key_state_[GLFW_KEY_W])
	{
		main_camera_->Move(kgl::CameraDirection::FORWARD);
	}

	if (key_state_[GLFW_KEY_S])
	{
		main_camera_->Move(kgl::CameraDirection::BACK);
	}

	if (key_state_[GLFW_KEY_A])
	{
		main_camera_->Move(kgl::CameraDirection::LEFT);
	}

	if (key_state_[GLFW_KEY_D])
	{
		main_camera_->Move(kgl::CameraDirection::RIGHT);
	}

	if (key_state_[GLFW_KEY_J])
	{
		model_rotation_around_y_axis_ += 0.01f;
	}

	if (key_state_[GLFW_KEY_K])
	{
		model_rotation_around_y_axis_ -= 0.01f;
	}

	if (key_state_[GLFW_KEY_U])
	{
		model_rotation_around_x_axis_ += 0.01f;
	}

	if (key_state_[GLFW_KEY_I])
	{
		model_rotation_around_x_axis_ -= 0.01f;
	}
}
