// 演示使用深度测试的效果
#include "depth_test_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_debug_tool.h"
#include "../klib/kgl_font_renderer.h"
#include "../klib/kgl_string_convertor.h"

// http://blog.csdn.net/dreamcs/article/details/7691690

DepthTestApp::DepthTestApp()
{
}

DepthTestApp::~DepthTestApp()
{
	KGL_SAFE_DELETE(square_shader_);
	coordinate_square_.reset();
	wood_box_square_.reset();
	yellow_square_translation_ = glm::vec3(0.0f, 0.0f, 0.0f);
}

void DepthTestApp::InitScene()
{
	App::InitScene();

	// 初始化渲染状态
	// 繪製數據到frame buffer時的深度模式
	draw_square_depth_mode_.SetEnable(GL_TRUE);
	draw_square_depth_mode_.SetDepthTestFunc(draw_square_depth_func_);

	// 繪製數據到frame object時的揀選模式
	draw_square_cull_mode_.SetEnable(GL_TRUE);
	draw_square_cull_mode_.SetCullMode(GL_BACK); // 指定删除背面
	draw_square_cull_mode_.SetFrontFaceMode(GL_CW); // 待渲染的模型，以逆时针为正面

	glDepthMask(GL_TRUE);
}

void DepthTestApp::RenderFrame()
{
	main_camera_->Update();

	draw_square_cull_mode_.Use();
	draw_square_depth_mode_.Use();

	const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
	const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();

	glm::mat4 mat_square_wood_box;
	mat_square_wood_box = glm::translate(mat_square_wood_box, yellow_square_translation_);

	square_shader_->Use();
	square_shader_->ApplyMatrix(glm::value_ptr(view_matrix), "view_matrix");
	square_shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "projection_matrix");
	square_shader_->ApplyMatrix(glm::value_ptr(mat_square_wood_box), "world_matrix");
	square_shader_->ApplyTexture(texture_wood_box_, "source_texture_1", 0);
	wood_box_square_->DrawIndexed(); 

// ========================================================================
	
	glm::mat4 square_mat_coordinate;
	square_shader_->ApplyMatrix(glm::value_ptr(square_mat_coordinate), "world_matrix");
	square_shader_->ApplyTexture(texture_coordinate_, "source_texture_1", 0);
	coordinate_square_->DrawIndexed();

	this->RenderText();
}

void DepthTestApp::RenderText()
{
	const glm::vec3& main_camera_pos = main_camera_->GetPosition();
	glm::vec4 text_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();

	boost::format wf("木箱面片的世界坐标值为%4f,%4f,%4f");
	wf % yellow_square_translation_.x % yellow_square_translation_.y % yellow_square_translation_.z;
	std::string text_1 = wf.str();

	wf.clear();
	wf.parse("摄像机的世界坐标值为%4f,%4f,%4f");
	wf % main_camera_pos.x % main_camera_pos.y % main_camera_pos.z;
	std::string text_2 = wf.str();

	wf.clear();
	wf.parse("绘制面片时是否已经开启深度测试: %s 深度测试函数为： %s");
	wf % (draw_square_depth_mode_.GetEnable() ? "Yes" : "No")
	   % kgl::RenderStateDepth::GetTestFuncName(draw_square_depth_mode_.GetDepthTestFunc());
	std::string text_3 = wf.str();

	font_renderer->AddToRendered(
		kgl::StringConvertor::ANSItoUTF16LE(text_1.c_str()), 10, 0, text_color, 1.f);

	font_renderer->AddToRendered(
		kgl::StringConvertor::ANSItoUTF16LE(text_2.c_str()), 10, 24, text_color, 1.f);

	font_renderer->AddToRendered(
		kgl::StringConvertor::ANSItoUTF16LE(text_3.c_str()), 10, 48, text_color, 1.f);

	font_renderer->Draw();
}

void DepthTestApp::InitMaterial()
{
	App::InitMaterial();
}

void DepthTestApp::InitModel()
{
	glm::vec4 red_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 yellow_color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	coordinate_square_ = kgl::PrimitiveTool::BuildSquare(1, true, red_color);
	wood_box_square_ = kgl::PrimitiveTool::BuildSquare(1, true, yellow_color);

	kgl::TextureParams texture_param = kgl::TextureManager::MakeTextureParamsRGB(GL_REPEAT, GL_LINEAR);
	texture_coordinate_ = std::make_shared<kgl::SourceTexture>();
	texture_coordinate_->CreateFromFile("resources/image/coordinate.bmp", texture_param);

	texture_wood_box_ = std::make_shared<kgl::SourceTexture>();
	texture_wood_box_->CreateFromFile("resources/image/wood_box.jpg", texture_param);
}

void DepthTestApp::InitShader()
{
	square_shader_ = new kgl::GPUProgram;	
	const char* vs_file = "resources/shader/depth_test_draw_square_vs.glsl";
	const char* fs_file = "resources/shader/depth_test_draw_square_fs.glsl";
	square_shader_->CreateFromFile(vs_file,fs_file , nullptr);
}

void DepthTestApp::InitLight()
{
	App::InitLight();
}

void DepthTestApp::InitMainCamera()
{
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 look_at = glm::vec3(0.0f, 0.0f, -10.0f);
	float fov = 120.0f;
	float far_z = 100.0f;
	float near_z = 0.1f;
	main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, pos);
}

void DepthTestApp::InitFont()
{
	kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
	font_renderer->Initialize();
	font_renderer->CreateFontTexture("resources/font/wqy_wmh.ttf", "wqywmh24", 24, 512, 512);
	font_renderer->SetCurrentFont("wqywmh24");
}

void DepthTestApp::ProcessInput()
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


	if (key_state_[GLFW_KEY_DOWN])
	{
		yellow_square_translation_.y -= 0.01f;
		kgl::DebugTool::PrintVector3("floor translation", yellow_square_translation_);
	}

	if (key_state_[GLFW_KEY_UP])
	{
		yellow_square_translation_.y += 0.01f;
		kgl::DebugTool::PrintVector3("floor translation", yellow_square_translation_);
	}

	if (key_state_[GLFW_KEY_LEFT])
	{
		yellow_square_translation_.x -= 0.01f;
		kgl::DebugTool::PrintVector3("floor translation", yellow_square_translation_);
	}

	if (key_state_[GLFW_KEY_RIGHT])
	{
		yellow_square_translation_.x += 0.01f;
		kgl::DebugTool::PrintVector3("floor translation", yellow_square_translation_);
	}

	if (key_state_[GLFW_KEY_HOME])
	{
		yellow_square_translation_.z -= 0.01f;
		kgl::DebugTool::PrintVector3("floor translation", yellow_square_translation_);
	}

	if (key_state_[GLFW_KEY_END])
	{
		yellow_square_translation_.z += 0.01f;
		kgl::DebugTool::PrintVector3("floor translation", yellow_square_translation_);
	}
}

void DepthTestApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	App::OnKeyAction(window, key, scancode, action, mode);

	if (key == GLFW_KEY_TAB && action == GLFW_RELEASE)
	{
		draw_square_depth_func_++;

		if (draw_square_depth_func_ > GL_ALWAYS)
		{
			draw_square_depth_func_ = GL_LESS;
		}

		draw_square_depth_mode_.SetDepthTestFunc(draw_square_depth_func_);
	}
}

void DepthTestApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
	App::OnMouseAction(window, xpos, ypos);
}

void DepthTestApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
	App::OnScrollAction(window, xoffset, yoffset);
}