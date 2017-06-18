// 展示一个简单的三角形
#include "frame_buffer_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"

FrameBufferApp::FrameBufferApp() :scene_obj_shader_(nullptr), screen_shader_(nullptr)
{
}

FrameBufferApp::~FrameBufferApp()
{
	KGL_SAFE_DELETE(scene_obj_shader_);
	KGL_SAFE_DELETE(screen_shader_);
	box_.reset();
	floor_.reset();
	floor_texture_.reset();
	box_texture_.reset();
}

void FrameBufferApp::InitScene()
{
	// 初始化shader
	scene_obj_shader_ = new kgl::GPUProgram;
	scene_obj_shader_->CreateFromFile("resources/shader/012_framebuffer_textured_obj_no_lighting_vs.glsl", "resources/shader/012_framebuffer_textured_obj_no_lighting_fs.glsl", nullptr);
	
	screen_shader_ = new kgl::GPUProgram;
	screen_shader_->CreateFromFile("resources/shader/012_framebuffer_textured_ndc_rectangle_vs.glsl", "resources/shader/012_framebuffer_textured_ndc_rectangle_fs.glsl", nullptr);

	// 初始化纹理
	kgl::TextureParams texture_param;
	texture_param.WrapSMode = GL_REPEAT;
	texture_param.WrapTMode = GL_REPEAT;
	texture_param.MagFilterMode = GL_LINEAR;
	texture_param.MinFilterMode = GL_LINEAR;
	texture_param.InternalFormat = GL_RGB;
	texture_param.SrcImgPixelComponentType = GL_UNSIGNED_BYTE;
	texture_param.SrcImgFormat = GL_RGB;
	texture_param.LoadChannel = SOIL_LOAD_RGB;
	texture_param.UseMipmap = false;

	floor_texture_ = std::dynamic_pointer_cast<kgl::SourceTexture>(kgl::KTextureManager::GetInstance()->CreateTextureFromFile(
		"resources/image/floor.jpg", kgl::SOURCE_2D_TEXTURE, texture_param));

	box_texture_ = std::dynamic_pointer_cast<kgl::SourceTexture>(kgl::KTextureManager::GetInstance()->CreateTextureFromFile(
		"resources/image/braynzar.jpg", kgl::SOURCE_2D_TEXTURE, texture_param));

	// 初始化图元
	box_ = kgl::PrimitiveTool::BuildTexturedCube();
	floor_ = kgl::PrimitiveTool::BuildTexturedXZPlane(0.f, 10.f, 10.f,10);
	screen_ = kgl::PrimitiveTool::BuildNDCTexturedRectange(0.5f, 1.0f, 1.0f, 0.5f);

	// 初始化相机
	main_camera_->InitViewProjection(kgl::CameraType::FREE, glm::vec3(0.0f, 3.0f, 8.0f), glm::vec3(0.0f, 0.0f, 0.0f), 120.0f, 0.1f, 50.0f);

	// 初始化渲染状态
	rs_depth_.Enable(true);
	rs_depth_.SetDepthTestFunc(GL_LESS);
	rs_cull_mode_.Enable(false);
}

void FrameBufferApp::RenderFrame()
{
	main_camera_->Update();
	
	// 绘制地板
	glm::mat4 model_matrix;
	const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
	const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();
	
	scene_obj_shader_->Use();
	scene_obj_shader_->ApplyMatrix(glm::value_ptr(model_matrix), "model_matrix");
	scene_obj_shader_->ApplyMatrix(glm::value_ptr(view_matrix), "view_matrix");
	scene_obj_shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "projection_matrix");
	scene_obj_shader_->ApplyTexture(floor_texture_, "source_texture_1", 0);

	floor_->Draw();

	// 绘制箱子
	model_matrix = glm::translate(glm::vec3(0.0f,1.0f,0.0f));
	model_matrix = glm::rotate(model_matrix, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
	scene_obj_shader_->ApplyMatrix(glm::value_ptr(model_matrix), "model_matrix");
	scene_obj_shader_->ApplyTexture(box_texture_, "source_texture_1", 0);
	box_->Draw();

	// 绘制屏幕图
	screen_shader_->Use();
	screen_shader_->ApplyTexture(box_texture_, "source_texture_1", 0);
	screen_->Draw();
}

void FrameBufferApp::ProcessInput()
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

void FrameBufferApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	App::OnKeyAction(window, key, scancode, action, mode);
}

void FrameBufferApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
	App::OnMouseAction(window, xpos, ypos);
}

void FrameBufferApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
	App::OnScrollAction(window, xoffset, yoffset);
}