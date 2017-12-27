// 演示使用深度测试的效果
#include "normal_mapping_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_debug_tool.h"
#include "../klib/kgl_font_renderer.h"
#include "../klib/kgl_string_convertor.h"

// http://blog.csdn.net/dreamcs/article/details/7691690

NormalMappingApp::NormalMappingApp()
{
}

NormalMappingApp::~NormalMappingApp()
{
	normal_mapping_shader_.reset();
	wall_square_.reset();
	diffuse_map_texture_.reset();
	normal_map_texture_.reset();
}

void NormalMappingApp::InitScene()
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

void NormalMappingApp::PreRenderFrame()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void NormalMappingApp::RenderFrame()
{
	main_camera_->Update();

	const glm::mat4& projection = main_camera_->GetProjectionMatrix();
	const glm::mat4& view = main_camera_->GetViewMatrix();
	glm::mat4 model;

	normal_mapping_shader_->Use();
	normal_mapping_shader_->ApplyMatrix(glm::value_ptr(projection), "projection_matrix");
	normal_mapping_shader_->ApplyMatrix(glm::value_ptr(view), "view_matrix");
	normal_mapping_shader_->ApplyMatrix(glm::value_ptr(model), "world_matrix");
	normal_mapping_shader_->ApplyVector3(glm::value_ptr(main_camera_->GetPosition()),"camera_world_position");
	normal_mapping_shader_->ApplyVector3(glm::value_ptr(point_light_.Position), "light_world_position");
	normal_mapping_shader_->ApplyTexture(diffuse_map_texture_, "diffuse_map", 0);
	normal_mapping_shader_->ApplyTexture(normal_map_texture_, "normal_map", 1);
	normal_mapping_shader_->ApplyFloat(shininess_,"shininess");
	normal_mapping_shader_->ApplyPointLight(&point_light_, "point_light");
	
	wall_square_->DrawIndexed();

// ========================================================================
	this->RenderText();
}

void NormalMappingApp::RenderText()
{
	const glm::vec3& main_camera_pos = main_camera_->GetPosition();
	glm::vec4 text_color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();

	std::wstring text_1 = is_toggle_on_ ? toggle_help_off_text_ : toggle_help_on_text_;

	font_renderer->AddToRendered(text_1, 10, 0, text_color, 1.f);

	if (is_toggle_on_)
	{
		font_renderer->AddToRendered(kgl::StringConvertor::ANSItoUTF16LE("按WSAD键上下左右移动点光源，按UJ键增大减小镜面反射辉光值"), 10, 20, text_color, 1.f);

		boost::format wf;
		wf.clear();
		wf.parse("点光源的世界坐标值为%2f,%2f,%2f，镜面反射的辉光值为：%2f");
		wf % point_light_.Position.x % point_light_.Position.y % point_light_.Position.z % shininess_;
		std::wstring text_2 = kgl::StringConvertor::ANSItoUTF16LE(wf.str().c_str());

		font_renderer->AddToRendered(text_2, 10, 40, text_color, 1.f);
	}

	font_renderer->Draw();
}

void NormalMappingApp::InitMaterial()
{
	App::InitMaterial();
}

void NormalMappingApp::InitModel()
{
	wall_square_ = kgl::PrimitiveTool::BuildPNTBT1Square(1,true);

	kgl::TextureParams texture_param = kgl::TextureManager::MakeTextureParamsRGB(GL_REPEAT, GL_LINEAR);
	diffuse_map_texture_ = std::make_shared<kgl::SourceTexture>();
	diffuse_map_texture_->CreateFromFile("resources/image/stone_wall.png", texture_param);

	normal_map_texture_ = std::make_shared<kgl::SourceTexture>();
	normal_map_texture_->CreateFromFile("resources/image/stone_wall_normal_map.png", texture_param);
}

void NormalMappingApp::InitShader()
{
	normal_mapping_shader_ = std::make_shared<kgl::GPUProgram>();
	std::vector<std::string> vs_file_paths;
	std::vector<std::string> fs_file_paths;
	std::vector<std::string> gs_file_path;
	vs_file_paths.push_back("resources/shader/lighting_struct.glsl");
	vs_file_paths.push_back("resources/shader/normal_mapping_vs.glsl");
	fs_file_paths.push_back("resources/shader/normal_mapping_fs.glsl");
	normal_mapping_shader_->CreateFromFile(vs_file_paths, fs_file_paths, gs_file_path, 3, 3, 0);
}

void NormalMappingApp::InitLight()
{
	point_light_.Ambient = glm::vec3(1.0f, 1.0f, 1.0f); // 环境光分量
	point_light_.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f); // 漫反射分量
	point_light_.Specular = glm::vec3(1.0f, 1.0f, 1.0f); // 镜面反射分量
	point_light_.Position = glm::vec3(0.0f, 0.0f, 0.7f);  // 点光源在世界空间中的坐标位置
	point_light_.AttenuationConstant = 1.0f; // 常数衰减值
	point_light_.AttenuationLinear = 1.0f;  // 线性衰减值
	point_light_.AttenuationExp = 1.0f; // 指数衰减值
}

void NormalMappingApp::InitMainCamera()
{
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 1.75f);
	main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, pos);
}

void NormalMappingApp::InitFont()
{
	kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
	font_renderer->Initialize();
	font_renderer->CreateFontTexture("resources/font/wqy_wmh.ttf", "wqywmh24", 24, 512, 512);
	font_renderer->SetCurrentFont("wqywmh24");

	toggle_help_on_text_ = kgl::StringConvertor::ANSItoUTF16LE("使用法线贴图实现凹凸效果，按下H键显示帮助");
	toggle_help_off_text_ = kgl::StringConvertor::ANSItoUTF16LE("使用法线贴图实现凹凸效果，按下H键关闭帮助");
}

void NormalMappingApp::ProcessInput()
{
	if (key_state_[GLFW_KEY_W])
	{
		point_light_.Position.y += 0.01f;
	}

	if (key_state_[GLFW_KEY_S])
	{
		point_light_.Position.y -= 0.01f;
	}

	if (key_state_[GLFW_KEY_A])
	{
		point_light_.Position.x -= 0.01f;
	}

	if (key_state_[GLFW_KEY_D])
	{
		point_light_.Position.x += 0.01f;
	}

	if (key_state_[GLFW_KEY_U])
	{
		shininess_ += 0.1f;

		if (shininess_ > 32.0f)
			shininess_ = 32.0f;
	}

	if (key_state_[GLFW_KEY_J])
	{
		shininess_ -= 0.1f;

		if (shininess_ < 0.0f)
			shininess_ = 0.1f;
	}
}

void NormalMappingApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	App::OnKeyAction(window, key, scancode, action, mode);

	if (GLFW_KEY_H == key && action == GLFW_RELEASE)
	{
		is_toggle_on_ = !is_toggle_on_;
	}
}

void NormalMappingApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
	App::OnMouseAction(window, xpos, ypos);
}

void NormalMappingApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
	App::OnScrollAction(window, xoffset, yoffset);
}