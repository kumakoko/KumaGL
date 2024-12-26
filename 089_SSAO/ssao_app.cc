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
#include "ssao_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_debug_tool.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_math_tools.h"

SSAOApp::SSAOApp() : gbuffer_(nullptr), ssao_fb_(nullptr), ssao_blur_fb_(nullptr)
{
	boost::format fmt("samples[%d]");

	for (GLuint i = 0; i < 64; ++i)
	{
		fmt % i;
		uniform_var_name_samples_[i] = fmt.str();
	}
}

SSAOApp::~SSAOApp()
{
	geometry_pass_shader_.reset();
	lighting_pass_shader_.reset();
	ssao_shader_.reset();
	ssao_blur_shader_.reset();
	screen_rectangle_.reset();
	noise_texture_.reset();

	KGL_SAFE_DELETE(gbuffer_);
	KGL_SAFE_DELETE(ssao_fb_);
	KGL_SAFE_DELETE(ssao_blur_fb_);
	KGL_SAFE_DELETE(model_);
}

void SSAOApp::InitMainCamera()
{
	main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, glm::vec3(0.0f, 0.1f, 10.0f));
	main_camera_->SetCameraSpeed(0.5f);
}

void SSAOApp::RenderGUI()
{
	//const glm::vec3& camera_pos = main_camera_->GetPosition();
	//ImGui::Begin("090 PBR -- 基于物理的渲染效果");
	//ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);
	//ImGui::Text("按WSAD键向前后左右移动摄像机");
	//ImGui::Text("摄像机坐标: (%.1f,%.1f,%.1f)", camera_pos.x, camera_pos.y, camera_pos.z);
	//ImGui::Checkbox("使用金属度贴图控制金属度",&use_metallic_texture_);
	//ImGui::Checkbox("使用粗糙度贴图控制粗糙度", &use_roughness_texture_);
	//ImGui::SliderFloat3("用于非金属的基础反射率", glm::value_ptr(base_reflectivity_), 0.001f, 0.17f);
	//ImGui::SliderFloat3("灯光颜色", glm::value_ptr(light_color_), 0.0f, 255.0f);

	//if (!use_metallic_texture_)
	//{
	//	ImGui::SliderFloat("金属度参数", &metallic_roughness_.z, 0.001f,1.0f);
	//}

	//if (!use_roughness_texture_)
	//{
	//	ImGui::SliderFloat("粗糙度参数", &metallic_roughness_.w, 0.001f, 1.0f);
	//}


	//const char* material_name[MATERIAL_COUNT] = 
	//{
	//	"生锈铁球", "花岗岩", "划痕金属", "竹子", "花纹黄铜器"
	//};

	//// 当前选中的那个animation clip的名字
	//static const char* item_current = material_name[0];

	//if (ImGui::BeginCombo("材质名", item_current, 0))
	//{
	//	for (int n = 0; n < MATERIAL_COUNT; n++)
	//	{
	//		bool is_selected = (item_current == material_name[n]);

	//		if (ImGui::Selectable(material_name[n], is_selected))
	//		{
	//			item_current = material_name[n];

	//			current_albedo_texture_ = albedo_textures_[n];
	//			current_normal_texture_ = normal_textures_[n];
	//			current_metallic_texture_ = metallic_textures_[n];
	//			current_roughness_texture_ = roughness_textures_[n];
	//			current_ambient_occlusion_texture_ = ambient_occlusion_textures_[n];
	//		}

	//		if (is_selected)
	//		{
	//			ImGui::SetItemDefaultFocus();
	//		}
	//	}

	//	ImGui::EndCombo();
	//}

	//ImGui::End();
}

void SSAOApp::InitModel()
{
	const char* model_path = "resources/model/box2/box.obj";
	model_ = new kgl::BasicStaticMesh;
	model_->LoadMesh(std::string(model_path));

	// 初始化NDC矩形图元
	screen_rectangle_ = kgl::PrimitiveTool::BuildNDCTexturedRectange();
}

void SSAOApp::InitShaders()
{
	geometry_pass_shader_ = std::make_shared<kgl::GPUProgram>()/*("ssao_geometry.vs", "ssao_geometry.frag")*/;
	geometry_pass_shader_->CreateFromFile("resources/shader/089_ssao_geometry_vs.glsl", "resources/shader/089_ssao_geometry_fs.glsl", nullptr);

	lighting_pass_shader_ = std::make_shared<kgl::GPUProgram>()/*("ssao.vs", "ssao_lighting.frag")*/;
	lighting_pass_shader_->CreateFromFile("resources/shader/089_ssao_vs.glsl", "resources/shader/089_ssao_lighting_fs.glsl", nullptr);

	ssao_shader_ = std::make_shared<kgl::GPUProgram>()/*("ssao.vs", "ssao.frag")*/;
	ssao_shader_->CreateFromFile("resources/shader/089_ssao_vs.glsl", "resources/shader/089_ssao_main_fs.glsl", nullptr);

	ssao_blur_shader_ = std::make_shared<kgl::GPUProgram>()/*("ssao.vs", "ssao_blur.frag")*/;
	ssao_blur_shader_->CreateFromFile("resources/shader/089_ssao_vs.glsl", "resources/shader/089_ssao_blur_fs.glsl", nullptr);
}

void SSAOApp::InitLights()
{
	//light_position_ = glm::vec3(0.0f, 0.0f, 10.0f);
	//light_color_ = glm::vec3(255.0f, 255.0f, 255.0f);
	lightPos = glm::vec3(2.0, 4.0, -2.0);
	lightColor = glm::vec3(0.2, 0.2, 0.7);
}

// Sample kernel
void SSAOApp::BuildSSAOKernel()
{
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;

	for (GLuint i = 0; i < 64; ++i)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		GLfloat scale = GLfloat(i) / 64.0;

		// Scale samples s.t. they're more aligned to center of kernel
		scale = kgl::MathTools::Lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}
}

void SSAOApp::BuildNoiseTexture()
{
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;

	std::vector<glm::vec3> ssaoNoise;

	for (GLuint i = 0; i < 16; i++)
	{
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}

	kgl::TextureParams tp;
	tp.internal_format = GL_RGB16F;
	tp.src_img_format = GL_RGB;
	tp.src_img_px_component_type = GL_FLOAT;
	tp.mag_filter_mode = GL_LINEAR;
	tp.min_filter_mode = GL_NEAREST;
	tp.wrap_s_mode = GL_REPEAT;
	tp.wrap_t_mode = GL_REPEAT;
	int tex_width = 4;
	int tex_height = 4;
	noise_texture_ = std::make_shared<kgl::SourceTexture>();
	noise_texture_->BuildFromNoise(ssaoNoise.data(), tex_width, tex_height, tp);
}

void SSAOApp::InitScene()
{
	App::InitScene();
	this->BuildSSAOKernel();
	this->BuildNoiseTexture();

	// 设置拣选模式和深度检测模式
	/*cull_mode_.SetEnable(TRUE);
	cull_mode_.SetFrontFaceMode(GL_CW);
	cull_mode_.SetCullMode(GL_BACK);*/
	rs_depth_.SetEnable(GL_TRUE);
	rs_depth_.SetDepthTestFunc(GL_LESS);
	//cull_mode_.Use();
	//depth_test_mode_.Use();

	// 初始化G-buffer
	gbuffer_ = new kgl::GBuffer;
	gbuffer_->Initialise(window_width_, window_height_, true);

	// 初始化帧缓冲对象
	kgl::TextureParams tp;
	tp.wrap_s_mode = GL_CLAMP_TO_BORDER;
	tp.wrap_t_mode = GL_CLAMP_TO_BORDER;
	tp.mag_filter_mode = GL_NEAREST;
	tp.min_filter_mode = GL_NEAREST;
	tp.internal_format = GL_RED; // 纹理在内存（系统或者显卡中）中的存储模式，由于环境遮蔽的结果是一个灰度值，只需要纹理的红色分量，所将颜色缓冲的内部格式设置为GL_RED。
	tp.src_img_format = GL_RGB; // 原始图片中的颜色数据的格式
	tp.src_img_px_component_type = GL_FLOAT; // 原始图片中颜色每一个分量在内存中占用的字节类型
	tp.used_mipmap = false; // 是否使用mipmap

	ssao_fb_ = new kgl::FrameBuffer;
	ssao_fb_->Create(window_width_, window_height_, kgl::RenderedTexelType::RGB, false, &tp);

	ssao_blur_fb_ = new kgl::FrameBuffer;
	ssao_blur_fb_->Create(window_width_, window_height_, kgl::RenderedTexelType::RGB, false, &tp);
}

void SSAOApp::RenderCube()
{
}

void SSAOApp::RenderQuad()
{
	screen_rectangle_->Draw();
}

void SSAOApp::RenderScene()
{
	float current_time = (GLfloat)glfwGetTime() * 0.5f;

	rs_depth_.Use();
	main_camera_->Update();

	const glm::mat4& projection = main_camera_->GetProjectionMatrix();//glm::perspective(camera.Zoom, (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 50.0f);
	const glm::mat4& view = main_camera_->GetViewMatrix();

	// 1 开始几何阶段的处理 =====================================================================
	gbuffer_->StartGeometryRenderPass();
	{
		geometry_pass_shader_->Use();
		geometry_pass_shader_->ApplyMatrix(glm::value_ptr(projection), "projection");//glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		geometry_pass_shader_->ApplyMatrix(glm::value_ptr(projection), "view");//glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// 渲染表示地板的cubeFloor cube
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
		geometry_pass_shader_->ApplyMatrix(glm::value_ptr(projection), "model");//glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	//	RenderCube();

		// 渲染模型
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0));
		model = glm::rotate(model, -90.0f, glm::vec3(1.0, 0.0, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));

		geometry_pass_shader_->ApplyMatrix(glm::value_ptr(projection), "model"); //glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		model_->Render();
	}
	gbuffer_->EndGeometryRenderPass(); // glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ========== 2. Create SSAO texture ==========

	ssao_fb_->StartWriting(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), GL_COLOR_BUFFER_BIT);
	{
		ssao_shader_->Use();
		ssao_shader_->ApplyTexture("gPositionDepth", 0, gbuffer_->position_info_texture());
		ssao_shader_->ApplyTexture("gNormal", 1, gbuffer_->normal_info_texture());
		ssao_shader_->ApplyTexture(noise_texture_, "texNoise", 2);

		for (GLuint i = 0; i < 64; ++i)
		{
			ssao_shader_->ApplyVector3(glm::value_ptr(ssaoKernel[i]), uniform_var_name_samples_[i].c_str());
		}

		ssao_shader_->ApplyMatrix(glm::value_ptr(projection), "projection"); // glUniformMatrix4fv(glGetUniformLocation(shaderSSAO.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		this->RenderQuad(); // 把内容往四边形上面绘制
	}
	ssao_fb_->EndWriting();

	// ========== 3. 对SSAO纹理进行一次柔化，移除噪声 ==========
	ssao_blur_fb_->StartWriting(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), GL_COLOR_BUFFER_BIT);
	{
		ssao_blur_shader_->Use();
		ssao_blur_shader_->ApplyTexture(ssao_fb_->GetTexture(), "ssaoInput", 0);
		this->RenderQuad();
	}
	ssao_blur_fb_->EndWriting();

	// ========== 4. 照明用的pass，使用Blinn-Phong光照模型，且加上SSAO ==========

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	lighting_pass_shader_->Use();
	lighting_pass_shader_->ApplyTexture("gPositionDepth", 0, gbuffer_->position_info_texture()); // glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D, gPositionDepth);
	lighting_pass_shader_->ApplyTexture("gNormal", 1, gbuffer_->normal_info_texture()); // glActiveTexture(GL_TEXTURE1);glBindTexture(GL_TEXTURE_2D, gNormal);
	lighting_pass_shader_->ApplyTexture("gAlbedo", 2, gbuffer_->albedo_info_texture()); // glActiveTexture(GL_TEXTURE2);glBindTexture(GL_TEXTURE_2D, gAlbedo);
	lighting_pass_shader_->ApplyTexture(ssao_blur_fb_->GetTexture(), "ssao", 3);// glActiveTexture(GL_TEXTURE3) glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);

	// Also send light relevant uniforms
	glm::vec3 lightPosView = glm::vec3(view * glm::vec4(lightPos, 1.0));
	lighting_pass_shader_->ApplyVector3(glm::value_ptr(lightPosView), "light.Position"); //glUniform3fv(glGetUniformLocation(shaderLightingPass.Program, "light.Position"), 1, &lightPosView[0]);
	lighting_pass_shader_->ApplyVector3(glm::value_ptr(lightColor), "light.Color"); //glUniform3fv(glGetUniformLocation(shaderLightingPass.Program, "light.Color"), 1, &lightColor[0]);

	// Update attenuation parameters
	const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
	const GLfloat linear = 0.09;
	const GLfloat quadratic = 0.032;
	lighting_pass_shader_->ApplyFloat(linear, "light.Linear");// glUniform1f(glGetUniformLocation(shaderLightingPass.Program, "light.Linear"), linear);
	lighting_pass_shader_->ApplyFloat(quadratic, "light.Quadratic");//glUniform1f(glGetUniformLocation(shaderLightingPass.Program, "light.Quadratic"), quadratic);
	lighting_pass_shader_->ApplyFloat(draw_mode, "draw_mode");//glUniform1i(glGetUniformLocation(shaderLightingPass.Program, "draw_mode"), draw_mode);
	RenderQuad();
}

void SSAOApp::ProcessInput()
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
		//	light_position_.x += 0.1f;
	}

	if (key_state_[GLFW_KEY_K])
	{
		//	light_position_.x -= 0.1f;
	}
}
