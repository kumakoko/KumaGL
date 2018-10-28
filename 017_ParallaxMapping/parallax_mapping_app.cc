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

ParallaxMappingApp::ParallaxMappingApp()
{
}

ParallaxMappingApp::~ParallaxMappingApp()
{
    KGL_SAFE_DELETE(model_);
    model_shader_.reset();
}

void ParallaxMappingApp::InitNDCScreeenRectangle()
{
    // NDC矩形的四个顶点坐标
    glm::vec3 pos1(-1.0f, 1.0f, 0.0f); // 左上
    glm::vec3 pos2(-1.0f, -1.0f, 0.0f); // 左下
    glm::vec3 pos3(1.0f, -1.0f, 0.0f); // 右下
    glm::vec3 pos4(1.0f, 1.0f, 0.0f); // 右上

    // NDC矩形的四个顶点的纹理贴图坐标
    glm::vec2 uv1(0.0f, 1.0f);
    glm::vec2 uv2(0.0f, 0.0f);
    glm::vec2 uv3(1.0f, 0.0f);
    glm::vec2 uv4(1.0f, 1.0f);

    // 顶点的法线，四个顶点都一样
    glm::vec3 nm(0.0f, 0.0f, 1.0f);

    //计算两个三角形的切线和副法线 calculate tangent/bitangent vectors of both triangles

    glm::vec3 tangent1, bitangent1;
    glm::vec3 tangent2, bitangent2;

    // 三角形1
    // ----------
    glm::vec3 edge1 = pos2 - pos1; // 边1 
    glm::vec3 edge2 = pos3 - pos1; // 边2
    glm::vec2 deltaUV1 = uv2 - uv1; // 纹理空间的两个基
    glm::vec2 deltaUV2 = uv3 - uv1;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent1 = glm::normalize(tangent1);

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent1 = glm::normalize(bitangent1);

    // 三角形2
    // ----------
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent2 = glm::normalize(tangent2);

    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent2 = glm::normalize(bitangent2);

    kgl::VertexPNTBT1 vtx[6];

    vtx[0].Position = glm::vec3(pos1.x, pos1.y, pos1.z);
    vtx[0].Normal = glm::vec3(nm.x, nm.y, nm.z);
    vtx[0].Tangent = glm::vec3(tangent1.x, tangent1.y, tangent1.z);
    vtx[0].Binormal = glm::vec3(bitangent1.x, bitangent1.y, bitangent1.z);
    vtx[0].TextureCoord1 = glm::vec2(uv1.x, uv1.y);

    vtx[1].Position = glm::vec3(pos2.x, pos2.y, pos2.z);
    vtx[1].Normal = glm::vec3(nm.x, nm.y, nm.z);
    vtx[1].Tangent = glm::vec3(tangent1.x, tangent1.y, tangent1.z);
    vtx[1].Binormal = glm::vec3(bitangent1.x, bitangent1.y, bitangent1.z);
    vtx[1].TextureCoord1 = glm::vec2(uv2.x, uv2.y);

    vtx[2].Position = glm::vec3(pos3.x, pos3.y, pos3.z);
    vtx[2].Normal = glm::vec3(nm.x, nm.y, nm.z);
    vtx[2].Tangent = glm::vec3(tangent1.x, tangent1.y, tangent1.z);
    vtx[2].Binormal = glm::vec3(bitangent1.x, bitangent1.y, bitangent1.z);
    vtx[2].TextureCoord1 = glm::vec2(uv3.x, uv3.y);

    vtx[3].Position = glm::vec3(pos1.x, pos1.y, pos1.z);
    vtx[3].Normal = glm::vec3(nm.x, nm.y, nm.z);
    vtx[3].Tangent = glm::vec3(tangent2.x, tangent2.y, tangent2.z);
    vtx[3].Binormal = glm::vec3(bitangent2.x, bitangent2.y, bitangent2.z);
    vtx[3].TextureCoord1 = glm::vec2(uv1.x, uv1.y);

    vtx[4].Position = glm::vec3(pos3.x, pos3.y, pos3.z);
    vtx[4].Normal = glm::vec3(nm.x, nm.y, nm.z);
    vtx[4].Tangent = glm::vec3(tangent2.x, tangent2.y, tangent2.z);
    vtx[4].Binormal = glm::vec3(bitangent2.x, bitangent2.y, bitangent2.z);
    vtx[4].TextureCoord1 = glm::vec2(uv3.x, uv3.y);

    vtx[5].Position = glm::vec3(pos4.x, pos4.y, pos4.z);
    vtx[5].Normal = glm::vec3(nm.x, nm.y, nm.z);
    vtx[5].Tangent = glm::vec3(tangent2.x, tangent2.y, tangent2.z);
    vtx[5].Binormal = glm::vec3(bitangent2.x, bitangent2.y, bitangent2.z);
    vtx[5].TextureCoord1 = glm::vec2(uv4.x, uv4.y);

    kgl::VertexAttribute va_position;
    va_position.index = 0;
    va_position.normalized = GL_FALSE;
    va_position.type = GL_FLOAT;
    va_position.size = 3;
    va_position.stride = sizeof(kgl::VertexPNTBT1);
    va_position.pointer = nullptr;

    kgl::VertexAttribute va_normal;
    va_normal.index = 1;
    va_normal.normalized = GL_FALSE;
    va_normal.type = GL_FLOAT;
    va_normal.size = 3;
    va_normal.stride = sizeof(kgl::VertexPNTBT1); // 每个位置的步长为 
    va_normal.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat));

    kgl::VertexAttribute va_tangent;
    va_tangent.index = 1;
    va_tangent.normalized = GL_FALSE;
    va_tangent.type = GL_FLOAT;
    va_tangent.size = 3;
    va_tangent.stride = sizeof(kgl::VertexPNTBT1);
    va_tangent.pointer = reinterpret_cast<GLvoid*> (6 * sizeof(GLfloat));

    kgl::VertexAttribute va_binormal;
    va_binormal.index = 1;
    va_binormal.normalized = GL_FALSE;
    va_binormal.type = GL_FLOAT;
    va_binormal.size = 3;
    va_binormal.stride = sizeof(kgl::VertexPNTBT1);
    va_binormal.pointer = reinterpret_cast<GLvoid*> (9 * sizeof(GLfloat));

    kgl::VertexAttribute va_texture_coord_1;
    va_texture_coord_1.index = 1;
    va_texture_coord_1.normalized = GL_FALSE;
    va_texture_coord_1.type = GL_FLOAT;
    va_texture_coord_1.size = 2;
    va_texture_coord_1.stride = sizeof(kgl::VertexPNTBT1);
    va_texture_coord_1.pointer = reinterpret_cast<GLvoid*> (12 * sizeof(GLfloat));

    std::vector<kgl::VertexAttribute> vtx_attri_array;
    vtx_attri_array.clear();
    vtx_attri_array.push_back(va_position);
    vtx_attri_array.push_back(va_normal);
    vtx_attri_array.push_back(va_tangent);
    vtx_attri_array.push_back(va_binormal);
    vtx_attri_array.push_back(va_texture_coord_1);

    screen_quad_ = std::make_shared<kgl::Primitive>();
    screen_quad_->Create(GL_TRIANGLES, vtx, sizeof(kgl::VertexPNTBT1) * 6, 6, GL_STATIC_DRAW, vtx_attri_array);
}

void ParallaxMappingApp::InitModel()
{
    const char* diffuse_map_file = "resources/image/bricks2.jpg";
    const char* normal_map_file = "resources/image/bricks2_normal.jpg";
    const char* height_map_file = "resources/image/bricks2_disp.jpg";

    kgl::TextureParams tp = kgl::TextureManager::MakeTextureParamsRGB(GL_REPEAT, GL_LINEAR);

    diffuse_map_texture_ = std::make_shared<kgl::SourceTexture>();
    diffuse_map_texture_->CreateFromFile(diffuse_map_file, tp);

    normal_map_texture_ = std::make_shared<kgl::SourceTexture>();
    normal_map_texture_->CreateFromFile(normal_map_file, tp);

    height_map_texture_ = std::make_shared<kgl::SourceTexture>();
    height_map_texture_->CreateFromFile(height_map_file, tp);

    this->InitNDCScreeenRectangle();
}

void ParallaxMappingApp::InitShaders()
{
    const char* vs_file = "parallax_mapping_vs.glsl";
    const char* fs_file = "parallax_mapping_fs.glsl";
    model_shader_ = std::make_shared<kgl::GPUProgram>();
    model_shader_->CreateFromFile(vs_file, fs_file,nullptr);
}

void ParallaxMappingApp::InitScene()
{
    App::InitScene();
}

void ParallaxMappingApp::InitMainCamera()
{
    main_camera_->InitViewProjection(kgl::CameraType::PERSPECTIVE, glm::vec3(0.0f, 0.0f, -400.0f));
}

void ParallaxMappingApp::InitLights()
{
    directional_light_.Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    directional_light_.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    directional_light_.Direction = glm::vec3(0.0f, 0.0f, -1.0f);
    directional_light_.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
}

void ParallaxMappingApp::InitMaterials()
{
    // 白银材质
    material_.Ambient = glm::vec3(0.19225f, 0.19225f, 0.19225f);
    material_.Diffuse = glm::vec3(0.50754f, 0.50754f, 0.50754f);
    material_.Specular = glm::vec3(0.508273f, 0.508273f, 0.508273f);
    material_.Shininess = 0.4f * 128.f;
}

void ParallaxMappingApp::RenderFrame()
{
    main_camera_->Update();

    

    // Set texture units 
    model_shader_->Use();
    
    // Light position
    glm::vec3 lightPos(0.5f, 1.0f, 0.3f);


        // Set frame time
    GLfloat currentFrame = static_cast<float>(glfwGetTime());
    //  deltaTime = currentFrame - lastFrame;
    //  lastFrame = currentFrame;

        // Check and call events
        //glfwPollEvents();
        //Do_Movement();

        // Clear the colorbuffer
        //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Configure view/projection matrices
        model_shader_->Use();
        glm::mat4 view = main_camera_->GetViewMatrix();
        glm::mat4 projection = main_camera_->GetProjectionMatrix();//glm::perspective(camera.Zoom, (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);
        model_shader_->ApplyMatrix(glm::value_ptr(view), "view");//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        model_shader_->ApplyMatrix(glm::value_ptr(projection), "projection");//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        // Render normal-mapped quad
        glm::mat4 model;
        model = glm::rotate(model, (GLfloat)glfwGetTime() * -10, glm::normalize(glm::vec3(1.0, 0.0, 1.0))); // Rotates the quad to show parallax mapping works in all directions
        model_shader_->ApplyMatrix(glm::value_ptr(model), "model");//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        model_shader_->ApplyVector3(glm::value_ptr(lightPos), "lightPos");//glUniform3fv(glGetUniformLocation(shader.Program, "lightPos"), 1, &lightPos[0]);
        model_shader_->ApplyVector3(glm::value_ptr(main_camera_->GetPosition()), "viewPos");//glUniform3fv(glGetUniformLocation(shader.Program, "viewPos"), 1, &camera.Position[0]);
        model_shader_->ApplyFloat(height_scale, "height_scale");//glUniform1f(glGetUniformLocation(shader.Program, "height_scale"), height_scale);
        // glUniform1i(glGetUniformLocation(shader.Program, "parallax"), parallax_mapping);
        
        model_shader_->ApplyTexture(diffuse_map_texture_, "", 0);
        model_shader_->ApplyTexture(normal_map_texture_, "", 1);
        model_shader_->ApplyTexture(height_map_texture_, "", 2);
        /**
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, heightMap);*/
        screen_quad_->Draw();

        // render light source (simply renders a smaller plane at the light's position for debugging/visualization)
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.1f));
		model_shader_->ApplyMatrix(glm::value_ptr(model), "model");
		// glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		screen_quad_->Draw();
}

// renders a 1x1 quad in NDC with manually calculated tangent vectors
// ------------------------------------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void  ParallaxMappingApp::renderQuad()
{
	if (quadVAO == 0)
	{
		// positions
		glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
		glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
		glm::vec3 pos3(1.0f, -1.0f, 0.0f);
		glm::vec3 pos4(1.0f, 1.0f, 0.0f);
		// texture coordinates
		glm::vec2 uv1(0.0f, 1.0f);
		glm::vec2 uv2(0.0f, 0.0f);
		glm::vec2 uv3(1.0f, 0.0f);
		glm::vec2 uv4(1.0f, 1.0f);
		// normal vector
		glm::vec3 nm(0.0f, 0.0f, 1.0f);

		// calculate tangent/bitangent vectors of both triangles
		glm::vec3 tangent1, bitangent1;
		glm::vec3 tangent2, bitangent2;
		// triangle 1
		// ----------
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = glm::normalize(bitangent1);

		// triangle 2
		// ----------
		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent2 = glm::normalize(tangent2);


		bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent2 = glm::normalize(bitangent2);


		float quadVertices[] = {
			// positions            // normal         // texcoords  // tangent                          // bitangent
			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
		};
		// configure plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void ParallaxMappingApp::ProcessInput()
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

	if (key_state_[GLFW_KEY_R])
	{
		main_camera_->ChangeYaw(0.01f);
	}

	if (key_state_[GLFW_KEY_UP])
	{
		main_camera_->ChangePitch(0.01f);
	}

	if (key_state_[GLFW_KEY_DOWN])
	{
		main_camera_->ChangePitch(-0.01f);
	}

	if (key_state_[GLFW_KEY_LEFT])
	{
		main_camera_->ChangeYaw(0.01f);
	}

	if (key_state_[GLFW_KEY_RIGHT])
	{
		main_camera_->ChangeYaw(-0.01f);
	}
}

void ParallaxMappingApp::OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	App::OnKeyAction(window, key, scancode, action, mode);
}

void ParallaxMappingApp::OnMouseAction(GLFWwindow* window, double xpos, double ypos)
{
	App::OnMouseAction(window, xpos, ypos);
}

void ParallaxMappingApp::OnScrollAction(GLFWwindow* window, double xoffset, double yoffset)
{
	App::OnScrollAction(window, xoffset, yoffset);
}