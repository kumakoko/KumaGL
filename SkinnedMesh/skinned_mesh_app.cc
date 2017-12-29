// 蒙皮骨骼动画模型
#include "skinned_mesh_app.h"
#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_vertex_type.h"
#include "../klib/kgl_texture_manager.h"

SkinnedMeshApp::SkinnedMeshApp()
{

}

SkinnedMeshApp::~SkinnedMeshApp()
{
	
}

void SkinnedMeshApp::InitScene()
{
	App::InitScene();

	rs_depth_.SetEnable(GL_TRUE);
	rs_blend_.SetEnable(GL_TRUE);
	rs_blend_.SetBlendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SkinnedMeshApp::InitMainCamera()
{
	main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, glm::vec3(0.0f, 0.0f, 5.0f));
}

void SkinnedMeshApp::InitLights()
{
	
}

void SkinnedMeshApp::InitModel()
{
	
}

void SkinnedMeshApp::InitShaders()
{
	
}

void SkinnedMeshApp::InitFont()
{
	kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
	font_renderer->Initialize();
	font_renderer->CreateFontTexture("resources/font/fzss_gbk.ttf", "fzss24", 24, 512, 512);
	font_renderer->SetCurrentFont("fzss24");
}

void SkinnedMeshApp::RenderFrame()
{
	main_camera_->Update();
	this->RenderText();
}

void SkinnedMeshApp::RenderText()
{
	const glm::vec3& pos = main_camera_->GetPosition();
	boost::wformat f(L"Camera position : (%f,%f,%f)");
	f % pos.x % pos.y % pos.z;
	glm::vec4 font_color(1.0f, 0.0f, 0.0f, 1.0f);

	kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
	font_renderer->AddToRendered(f.str(), 0, 0, font_color, font_color, font_color, font_color, 1.5f);
	font_renderer->Draw();
}

void SkinnedMeshApp::ProcessInput()
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
		main_camera_->Move(kgl::CameraDirection::UP);
	}

	if (key_state_[GLFW_KEY_U])
	{
		main_camera_->Move(kgl::CameraDirection::DOWN);
	}
}