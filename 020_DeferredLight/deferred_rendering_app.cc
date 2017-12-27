#include "deferred_rendering_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_string_convertor.h"

DeferredRenderingApp::DeferredRenderingApp()
{
}

DeferredRenderingApp::~DeferredRenderingApp()
{
	KGL_SAFE_DELETE(geometry_render_pass_);
	KGL_SAFE_DELETE(point_light_render_pass_);
	KGL_SAFE_DELETE(empty_render_pass_);
	KGL_SAFE_DELETE(m_box);
	KGL_SAFE_DELETE(m_bsphere);
	KGL_SAFE_DELETE(m_quad);
}

void DeferredRenderingApp::InitScene()
{
	App::InitScene();
	rs_depth_.SetEnable(GL_TRUE);

	// 初始化G-buffer
	gbuffer_ = new kgl::GBuffer;
	gbuffer_->Initialise(window_width_, window_height_);

	// 初始化几何渲染阶段
	this->InitGeometryRenderPass();

	// 初始化点光源光照计算阶段
	this->InitPointLightRenderPass();
	this->InitEmptyRenderPass();
	this->InitBoxPositions();
}

void DeferredRenderingApp::InitGeometryRenderPass()
{
	geometry_render_pass_ = new GeometryRenderPass;
	geometry_render_pass_->Init();
}

void DeferredRenderingApp::ExecuteGeometryRenderPass()
{
	float current_time = (GLfloat)glfwGetTime() * 0.5f;
	// geometry pass才更新depth buffer，其他的pass不更新
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
	const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();
	glm::mat4 wvp_matrix;
	glm::mat4 rotation_matrix;
	glm::mat4 translation_matrix;
	glm::mat4 world_matrix;

	for (unsigned int i = 0; i < box_positions_.size(); i++)
	{
		rotation_matrix = glm::rotate(current_time, glm::vec3(0.0f, 1.0f, 0.0f));
		translation_matrix = glm::translate(box_positions_[i]);
		world_matrix = translation_matrix * rotation_matrix;
		wvp_matrix = projection_matrix * view_matrix * world_matrix;
		
		geometry_render_pass_->Enable();
		geometry_render_pass_->SetWVP(wvp_matrix);
		geometry_render_pass_->SetWorldMatrix(world_matrix);
		geometry_render_pass_->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
		m_box->Render();
	}

	// When we get here the depth buffer is already populated and the stencil pass
	// depends on it, but it does not write to it.
	glDepthMask(GL_FALSE);
}

void DeferredRenderingApp::InitModel()
{
	std::string model_path = "resources/model/box2/box.obj";
	m_box = new kgl::BasicStaticMesh;
	m_box->LoadMesh(model_path);

	model_path = "resources/model/quad.obj";
	m_quad = new kgl::BasicStaticMesh;
	m_quad->LoadMesh(model_path);

	model_path = "resources/model/sphere.obj";
	m_bsphere = new kgl::BasicStaticMesh;
	m_bsphere->LoadMesh(model_path);
}

void DeferredRenderingApp::InitShader()
{
}

void DeferredRenderingApp::InitMainCamera()
{
	glm::vec3 camera_pos(0.0f, 2.0f, 36.0f);
	main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, camera_pos);
	main_camera_->SetCameraSpeed(0.1f);
}

void DeferredRenderingApp::InitFont()
{
	kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
	font_renderer->Initialize();
	font_renderer->CreateFontTexture("resources/font/fzkt_sim.ttf", "fzktsim24", 24, 512, 512);
	font_renderer->SetCurrentFont("fzktsim24");

	toggle_help_on_text_ = kgl::StringConvertor::ANSItoUTF16LE("按下H键显示帮助");
	toggle_help_off_text_ = kgl::StringConvertor::ANSItoUTF16LE("按下H键关闭帮助");
	camera_ctrl_text_ = kgl::StringConvertor::ANSItoUTF16LE("持续按下W、S、A、D、U、J键，使得摄像机向前、后、左、右、上、下方向移动");
	material_ctrl_text_ = kgl::StringConvertor::ANSItoUTF16LE("按下左右箭头键，切换模型使用的材质");
}

void DeferredRenderingApp::InitMaterial()
{
}

void DeferredRenderingApp::InitLight()
{
	point_lights_[0].Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	point_lights_[0].Diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
	point_lights_[0].Position = glm::vec3(0.0f, 1.5f, 5.0f);
	point_lights_[0].Specular = glm::vec3(0.0f, 0.0f, 0.0f);
	point_lights_[0].AttenuationConstant = 0.0f;
	point_lights_[0].AttenuationLinear = 0.0f;
	point_lights_[0].AttenuationExp = 0.3f;

	point_lights_[1].Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	point_lights_[1].Diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
	point_lights_[1].Position = glm::vec3(2.0f, 0.0f, 5.0f);
	point_lights_[1].Specular = glm::vec3(0.0f, 0.0f, 0.0f);
	point_lights_[1].AttenuationConstant = 0.0f;
	point_lights_[1].AttenuationLinear = 0.0f;
	point_lights_[1].AttenuationExp = 0.3f;

	point_lights_[1].Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	point_lights_[1].Diffuse = glm::vec3(0.0f, 0.0f, 1.0f);
	point_lights_[1].Position = glm::vec3(0.0f, 0.0f, 3.0f);
	point_lights_[1].Specular = glm::vec3(0.0f, 0.0f, 0.0f);
	point_lights_[1].AttenuationConstant = 0.0f;
	point_lights_[1].AttenuationLinear = 0.0f;
	point_lights_[1].AttenuationExp = 0.3f;
}

float DeferredRenderingApp::CalcPointLightBoundSphere(const kgl::PointLight& point_light)
{
	// 常数衰减因子 + 线性衰减因子 * 距离 + 指数衰减因子 * 距离 * 距离 = 256 * 颜色分量中最大的值 * diffuse光的亮度
	// 常数衰减因子：point_light.AttenuationConstant
	// 线性衰减因子：point_light.AttenuationLinear
	// 颜色分量中最大的值： max_channel
	// diffuse光的亮度： diffuse_intensity
	// 解一元二次方程，求得距离值

	float diffuse_intensity = glm::length(point_light.Diffuse);
	float max_channel = fmax(point_light.Diffuse.x, point_light.Diffuse.y);
	max_channel = fmax(max_channel, point_light.Diffuse.z);

	float ret = (-point_light.AttenuationLinear + 
		sqrtf(point_light.AttenuationLinear * point_light.AttenuationLinear - 4.0f * point_light.AttenuationExp * 
		(point_light.AttenuationConstant - 256.f * max_channel * diffuse_intensity))) / (2.0f * point_light.AttenuationExp);

	return ret;
}

void DeferredRenderingApp::InitPointLightRenderPass()
{
	point_light_render_pass_ = new PointLightRenderPass;
	point_light_render_pass_->Init();
}

void DeferredRenderingApp::ExecutePointLightRenderPass(const kgl::PointLight* point_light, const glm::mat4& world_matrix, const glm::mat4& view_matrix, const glm::mat4& projection_matrix)
{
	gbuffer_->BindForLightPass();
	point_light_render_pass_->Enable();
	point_light_render_pass_->SetViewInWorldPos(main_camera_->GetPosition());

	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glm::mat4 wvp_matrix = projection_matrix * view_matrix * world_matrix;

	point_light_render_pass_->SetWVPMatrix(wvp_matrix);
	point_light_render_pass_->SetPointLight(point_light);
	// 存储着片元的位置，第一层漫反射颜色，法线三种信息的G-buffer，分别对应于texture unit 0，1，2
	point_light_render_pass_->SetPositionTextureUnit(kgl::GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	point_light_render_pass_->SetColorTextureUnit(kgl::GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	point_light_render_pass_->SetNormalTextureUnit(kgl::GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	point_light_render_pass_->SetScreenSize(window_width_, window_height_);
	m_bsphere->Render();

	glCullFace(GL_BACK);
	glDisable(GL_BLEND);
}

void DeferredRenderingApp::ExecuteFinalRenderPass()
{
	gbuffer_->BindForFinalPass();
	glBlitFramebuffer(0, 0, window_width_, window_height_,
		0, 0, window_width_, window_height_, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void DeferredRenderingApp::PreRenderFrame()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DeferredRenderingApp::RenderFrame()
{
	main_camera_->Update();
	const glm::vec3& view_pos = main_camera_->GetPosition();
	const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
	const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();
	glm::mat4 point_light_world_matrix;
	gbuffer_->StartFrame();
	ExecuteGeometryRenderPass();

	glEnable(GL_STENCIL_TEST);

	for (uint32_t i = 0; i < POINT_LIGHT_COUNT; i++) 
	{
		glm::mat4 scale_matrix = glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));
		point_light_world_matrix = glm::translate(point_lights_[i].Position);
		point_light_world_matrix = point_light_world_matrix * scale_matrix;
//		ExecutePointLightStencilPass(&point_lights_[i], point_light_world_matrix, view_matrix, projection_matrix);
//		ExecutePointLightRenderPass(&point_lights_[i], point_light_world_matrix, view_matrix, projection_matrix);
	}

	glDisable(GL_STENCIL_TEST);

//	DSDirectionalLightPass();

	ExecuteFinalRenderPass();

	const std::wstring& help_toggle = is_help_on_ ? toggle_help_off_text_ : toggle_help_on_text_;
	kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
	glm::vec4 text_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	font_renderer->AddToRendered(help_toggle, 0, 0, text_color, 1.0f);

	if (is_help_on_)
	{
		boost::format fmt("摄像机位置坐标： x = %f , y = %f , z = %f");
		fmt % view_pos.x % view_pos.y % view_pos.z;
		font_renderer->AddToRendered(camera_ctrl_text_, 0, 25, text_color, 1.0f);
		font_renderer->AddToRendered(kgl::StringConvertor::ANSItoUTF16LE(fmt.str().c_str()), 0, 100, text_color, 1.0f);
	}

	font_renderer->Draw();
}

void DeferredRenderingApp::ProcessInput()
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
		main_camera_->Move(kgl::CameraDirection::DOWN);
	}

	if (key_state_[GLFW_KEY_U])
	{
		main_camera_->Move(kgl::CameraDirection::UP);
	}
}

void DeferredRenderingApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	App::OnKeyAction(window, key, scancode, action, mode);

	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
	{
		if (cur_mat_index_ == 0)
		{
			cur_mat_index_ = materials_.size() - 1;
			return;
		}
		cur_mat_index_--;
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
	{
		cur_mat_index_++;

		if (cur_mat_index_ >= materials_.size())
			cur_mat_index_ = 0;
	}

	if (GLFW_KEY_F == key && action == GLFW_RELEASE)
	{
		draw_mode_.SetCurrentDrawMode(kgl::DM_FILL);
	}

	if (GLFW_KEY_L == key && action == GLFW_RELEASE)
	{
		draw_mode_.SetCurrentDrawMode(kgl::DM_LINE);
	}

	if (GLFW_KEY_P == key && action == GLFW_RELEASE)
	{
		draw_mode_.SetCurrentDrawMode(kgl::DM_POINT);
	}

	if (GLFW_KEY_H == key && action == GLFW_RELEASE)
	{
		is_help_on_ = !is_help_on_;
	}
}

void DeferredRenderingApp::InitEmptyRenderPass()
{
	empty_render_pass_ = new EmptyRenderPass;
	empty_render_pass_->Init();
}

void DeferredRenderingApp::ExecutePointLightStencilPass(const kgl::PointLight* point_light, const glm::mat4& world_matrix, const glm::mat4& view_matrix, const glm::mat4& projection_matrix)
{
	empty_render_pass_->Enable();

	// 关闭颜色和深度缓冲区的写入，启用模板缓冲区
	gbuffer_->BindForStencilPass();
	glEnable(GL_DEPTH_TEST); // 启动深度测试
	glDisable(GL_CULL_FACE); // 关闭面拣选
	glClear(GL_STENCIL_BUFFER_BIT); // 仅清理模板缓冲区

	// We need the stencil test to be enabled but we want it
	// to succeed always. Only the depth test matters.
	glStencilFunc(GL_ALWAYS, 0, 0);

	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	glm::mat4 wvp_matrix = projection_matrix * view_matrix * world_matrix;
	empty_render_pass_->SetWVPMatrix(wvp_matrix);
	m_bsphere->Render();
}

void DeferredRenderingApp::InitBoxPositions()
{
	box_positions_.resize(5);
	box_positions_[0] = glm::vec3(0.0f, 0.0f, 5.0f);
	box_positions_[1] = glm::vec3(6.0f, 1.0f, 10.0f);
	box_positions_[2] = glm::vec3(-5.0f, -1.0f, 12.0f);
	box_positions_[3] = glm::vec3(4.0f, 4.0f, 15.0f);
	box_positions_[4] = glm::vec3(-4.0f, 2.0f, 20.0f);
}