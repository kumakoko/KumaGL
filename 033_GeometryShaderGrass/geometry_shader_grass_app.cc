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
#include "geometry_shader_grass_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"
#include "../klib/kgl_debug_tool.h"

// http://blog.csdn.net/dreamcs/article/details/7691690

GeometryShaderGrassApp::GeometryShaderGrassApp()
{
	floor_translation_ = glm::vec3(0.0f, 0.3f, -0.3f);
}

GeometryShaderGrassApp::~GeometryShaderGrassApp()
{
	floor_.reset();
	floor_texture_.reset();
}

void GeometryShaderGrassApp::InitScene()
{
	App::InitScene();

	cull_mode_.SetEnable(GL_TRUE);
	cull_mode_.SetFrontFaceMode(GL_CW);
	cull_mode_.SetCullMode(GL_BACK);

	depth_test_mode_.SetEnable(GL_TRUE);
	depth_test_mode_.SetDepthTestFunc(GL_LESS);

	draw_fb_to_scr_cull_.SetEnable(GL_FALSE);
	draw_fb_to_scr_cull_.SetCullMode(GL_BACK); // 指定删除背面
	draw_fb_to_scr_cull_.SetFrontFaceMode(GL_CCW); // 待渲染的模型，以逆时针为正面

	draw_fb_to_scr_depth_.SetEnable(GL_FALSE); // 关闭深度测试
}

void GeometryShaderGrassApp::PreRenderFrame()
{
	App::PreRenderFrame();
}

void GeometryShaderGrassApp::RenderScene()
{
	main_camera_->Update();
	GLfloat current_time = (GLfloat)glfwGetTime();

	const glm::mat4& view_matrix = main_camera_->GetViewMatrix();
	const glm::mat4& projection_matrix = main_camera_->GetProjectionMatrix();

	cull_mode_.Use();
	depth_test_mode_.Use();

	// 绘制地板
	scene_shader_->Use();
	scene_shader_->ApplyMatrix(glm::value_ptr(view_matrix), "view_matrix");
	scene_shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "projection_matrix");

	glm::mat4 matrix_floor = glm::identity<glm::mat4>();
	matrix_floor = glm::rotate(matrix_floor, -glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
	matrix_floor = glm::scale(matrix_floor, glm::vec3(20.f, 20.f, 20.f));
	matrix_floor = glm::translate(matrix_floor, floor_translation_);
	scene_shader_->ApplyMatrix(glm::value_ptr(matrix_floor), "world_matrix");
	scene_shader_->ApplyTexture(floor_texture_, "source_texture_1", 0);
	floor_->DrawIndexed();
}

void GeometryShaderGrassApp::InitMaterials()
{
	App::InitMaterials();
}

void GeometryShaderGrassApp::InitModel()
{
	auto texture_param = kgl::TextureManager::MakeTextureParamsRGB(GL_REPEAT, GL_LINEAR);
	// 地板图元和贴图
	floor_ = kgl::PrimitiveTool::BuildSquare(10, true);
	floor_texture_ = std::make_shared<kgl::SourceTexture>();
	floor_texture_->CreateFromFile("resources/image/wood_box.jpg", texture_param);
}

void GeometryShaderGrassApp::InitShaders()
{
	const char* gs_file = "resources/shader/033_grass_gs.glsl";
	const char* vs_file = "resources/shader/033_grass_vs.glsl";
	const char* fs_file = "resources/shader/033_grass_fs.glsl";
	grass_shader_ = std::make_shared<kgl::GPUProgram>();
	grass_shader_->CreateFromFile(vs_file, fs_file, gs_file);

	vs_file = "resources/shader/033_floor_draw_vs.glsl";
	fs_file = "resources/shader/033_floor_draw_fs.glsl";
	scene_shader_ = std::make_shared<kgl::GPUProgram>();
	scene_shader_->CreateFromFile(vs_file, fs_file, nullptr);
}

void GeometryShaderGrassApp::InitLights()
{
	App::InitLights();
}

void GeometryShaderGrassApp::InitMainCamera()
{
	glm::vec3 pos = glm::vec3(0.0f, 5.0f, 28.0f);
	glm::vec3 look_at = glm::vec3(0.0f, 0.0f, 0.0f);
	float pitch_angle = -10.0f;
	main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, pos, pitch_angle);
}

void GeometryShaderGrassApp::InitFont()
{
	App::InitFont();
}

void GeometryShaderGrassApp::ProcessInput()
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

	if (key_state_[GLFW_KEY_LEFT])
	{
		main_camera_->ChangeYaw(-0.1f);
	}

	if (key_state_[GLFW_KEY_RIGHT])
	{
		main_camera_->ChangeYaw(0.1f);
	}

	if (key_state_[GLFW_KEY_UP])
	{
		main_camera_->ChangePitch(0.1f);
	}

	if (key_state_[GLFW_KEY_DOWN])
	{
		main_camera_->ChangePitch(-0.1f);
	}
}

void GeometryShaderGrassApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	App::OnKeyAction(window, key, scancode, action, mode);
}

void GeometryShaderGrassApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
	App::OnMouseAction(window, xpos, ypos);
}

void GeometryShaderGrassApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
	App::OnScrollAction(window, xoffset, yoffset);
}

void GeometryShaderGrassApp::CreateGrassData()
{
	vboGrassData.Create();

	float fGrassPatchOffsetMin = 1.5f;
	float fGrassPatchOffsetMax = 2.5f;
	float fGrassPatchHeight = 5.0f;

	glm::vec3 vCurPatchPos(-vRenderScale.x * 0.5f + fGrassPatchOffsetMin, 0.0f, vRenderScale.z * 0.5f - fGrassPatchOffsetMin);

	iNumGrassTriangles = 0;

	while (vCurPatchPos.x < vRenderScale.x * 0.5f)
	{
		vCurPatchPos.z = vRenderScale.z * 0.5f - fGrassPatchOffsetMin;

		while (vCurPatchPos.z > -vRenderScale.z * 0.5f)
		{
			vCurPatchPos.y = 0;// GetHeightFromRealVector(vCurPatchPos) - 0.3f;
			vboGrassData.AddData(&vCurPatchPos, sizeof(glm::vec3));

			iNumGrassTriangles += 1;

			vCurPatchPos.z -= fGrassPatchOffsetMin + (fGrassPatchOffsetMax - fGrassPatchOffsetMin) * float(rand() % 1000) * 0.001f;
		}

		vCurPatchPos.x += fGrassPatchOffsetMin + (fGrassPatchOffsetMax - fGrassPatchOffsetMin) * float(rand() % 1000) * 0.001f;
	}

	//生成和绑定草的vertex array object
	glGenVertexArrays(1, &uiGrassVAO);
	glBindVertexArray(uiGrassVAO);
	// Attach vertex data to this VAO
	vboGrassData.Bind();
	vboGrassData.UploadDataToGPU(GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
}

/*
scene_shader_->Use();
scene_shader_->ApplyMatrix(glm::value_ptr(view_matrix), "view_matrix");
scene_shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "projection_matrix");

glm::mat4 matrix_floor = glm::identity<glm::mat4>();
matrix_floor = glm::rotate(matrix_floor, -glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
matrix_floor = glm::scale(matrix_floor, glm::vec3(20.f, 20.f, 20.f));
matrix_floor = glm::translate(matrix_floor, floor_translation_);
scene_shader_->ApplyMatrix(glm::value_ptr(matrix_floor), "world_matrix");
scene_shader_->ApplyTexture(floor_texture_, "source_texture_1", 0);
floor_->DrawIndexed();
*/

void GeometryShaderGrassApp::RenderGrass()
{
	grass_shader_->Use();

	grass_shader_->ApplyTexture(grass_texture_, "source_texture_1", 0);//tTextures[7].BindTexture(0);
	grass_shader_->ApplyInt(0, "gSampler");//spGrass.SetUniform("gSampler", 0);
	grass_shader_->ApplyFloat(fTimePassed, "fTimePassed");//spGrass.SetUniform("fTimePassed", fTimePassed);
	//fTimePassed += appMain.sof(1.0f);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

	glBindVertexArray(uiGrassVAO);
	glDrawArrays(GL_POINTS, 0, iNumGrassTriangles);

	glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	glDisable(GL_MULTISAMPLE);
}