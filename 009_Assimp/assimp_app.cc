// 简单的纹理应用示例
#include "assimp_app.h"
#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_vertex_type.h"
#include "../klib/kgl_texture_manager.h"

AssimpApp::AssimpApp() :model_(nullptr)
{

}

AssimpApp::~AssimpApp()
{
	KGL_SAFE_DELETE(model_);
	model_shader_.reset();
}

void AssimpApp::InitScene()
{
	App::InitScene();

	rs_depth_.SetEnable(GL_TRUE);
	rs_blend_.SetEnable(GL_TRUE);
	rs_blend_.SetBlendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void AssimpApp::InitMainCamera()
{
	main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, glm::vec3(0.0f, 0.0f, 5.0f));
}

void AssimpApp::InitModel()
{
	const char* model_path = "resources/model/xd_tux_0.3ds";
	model_ = new kgl::StaticModel(kgl::VERTEX_TYPE_PNT1, model_path);
	std::size_t sz = model_->GetMeshCount();

	for (std::size_t s = 0; s < sz; ++s)
	{
		model_->ApplyShaderToMesh(s, model_shader_);
	}
}

void AssimpApp::InitShader()
{
	const GLchar* vs_file_path = "resources/shader/009_assimp_vs.glsl";
	const GLchar* fs_file_path = "resources/shader/009_assimp_fs.glsl";
	const GLchar* gs_file_path = nullptr;
	model_shader_ = std::make_shared<kgl::GPUProgram>();
	model_shader_->CreateFromFile(vs_file_path, fs_file_path,gs_file_path);
}

void AssimpApp::InitFont()
{
	kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
	font_renderer->Initialize();
	font_renderer->CreateFontTexture("resources/font/fzss_gbk.ttf", "fzss24", 24, 512, 512);
	font_renderer->SetCurrentFont("fzss24");
}

void AssimpApp::RenderFrame()
{
	main_camera_->Update();

	glm::mat4 model_matrix;
	model_matrix = glm::rotate(model_matrix, 3.1415926f * 1.5f, glm::vec3(1.0f, 0.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
	
	const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
	const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();

	rs_blend_.Use();
	rs_depth_.Use();

	model_shader_->Use();
	model_shader_->ApplyMatrix(glm::value_ptr(model_matrix), "model_matrix");
	model_shader_->ApplyMatrix(glm::value_ptr(view_matrix), "view_matrix");
	model_shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "projection_matrix");
	model_shader_->ApplyTexture(kgl::KTextureManager::GetInstance()->GetTexture("resources/model/textures/tux_texture.png"), "texture_diffuse_1", 0);

	model_->Draw();
	this->RenderText();
}

void AssimpApp::RenderText()
{
	const glm::vec3& pos = main_camera_->GetPosition();
	boost::wformat f(L"Camea position : (%f,%f,%f)");
	f % pos.x % pos.y % pos.z;
	glm::vec4 font_color(1.0f, 0.0f, 0.0f, 1.0f);

	kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
	font_renderer->AddToRendered(f.str(), 0, 0, font_color, font_color, font_color, font_color, 1.5f);
	font_renderer->Draw();
}

void AssimpApp::ProcessInput()
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