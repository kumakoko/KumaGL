// 展示一个简单的三角形
#include "frame_buffer_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_primitive_tool.h"
#include "../klib/kgl_texture_manager.h"

// http://blog.csdn.net/dreamcs/article/details/7691690

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
	frame_buffer_texture_.reset();
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
	texture_param.wrap_s_mode = GL_REPEAT;
	texture_param.wrap_t_mode = GL_REPEAT;
	texture_param.mag_filter_mode = GL_LINEAR;
	texture_param.min_filter_mode = GL_LINEAR;
	texture_param.internal_format = GL_RGB;
	texture_param.src_img_px_component_type = GL_UNSIGNED_BYTE;
	texture_param.src_img_format = GL_RGB;
	texture_param.load_channel = SOIL_LOAD_RGB;
	texture_param.used_mipmap = false;

	floor_texture_ = std::dynamic_pointer_cast<kgl::SourceTexture>(kgl::KTextureManager::GetInstance()->CreateTextureFromFile(
		"resources/image/floor.jpg", kgl::SOURCE_2D_TEXTURE, texture_param));

	box_texture_ = std::dynamic_pointer_cast<kgl::SourceTexture>(kgl::KTextureManager::GetInstance()->CreateTextureFromFile(
		"resources/image/braynzar.jpg", kgl::SOURCE_2D_TEXTURE, texture_param));

	frame_buffer_texture_ = std::make_shared<kgl::RenderedTexture>(kgl::RenderedTextureUsedType::COLOR_RGB_ONLY);
	frame_buffer_texture_->Create(512, 512);

	// 初始化图元
	box_ = kgl::PrimitiveTool::BuildTexturedCube();
	floor_ = kgl::PrimitiveTool::BuildTexturedXZPlane(0.f, 10.f, 10.f,10);
	screen_ = kgl::PrimitiveTool::BuildNDCTexturedRectange(0.0f, 1.0f, 1.0f, 0.0f);

	// 初始化相机
	main_camera_->InitViewProjection(kgl::CameraType::FREE, glm::vec3(0.0f, 3.0f, 8.0f), glm::vec3(0.0f, 0.0f, 0.0f), 120.0f, 0.1f, 50.0f);

	// 初始化渲染状态
	rs_depth_.SetEnable(true);
	rs_depth_.SetDepthTestFunc(GL_LESS);
	rs_cull_mode_.SetEnable(GL_FALSE);
}

void FrameBufferApp::RenderFrame()
{
	main_camera_->Update();

	frame_buffer_texture_->StartWriting();

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	frame_buffer_texture_->EndWritting();

	// 绘制屏幕图

	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);



	// 绘制地板

	scene_obj_shader_->Use();
	scene_obj_shader_->ApplyMatrix(glm::value_ptr(model_matrix), "model_matrix");
	scene_obj_shader_->ApplyMatrix(glm::value_ptr(view_matrix), "view_matrix");
	scene_obj_shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "projection_matrix");
	scene_obj_shader_->ApplyTexture(floor_texture_, "source_texture_1", 0);

	floor_->Draw();

	// 绘制箱子
	model_matrix = glm::translate(glm::vec3(0.0f, 1.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
	scene_obj_shader_->ApplyMatrix(glm::value_ptr(model_matrix), "model_matrix");
	scene_obj_shader_->ApplyTexture(box_texture_, "source_texture_1", 0);
	box_->Draw();



	screen_shader_->Use();
	screen_shader_->ApplyTexture(frame_buffer_texture_, "source_texture_1", 0);
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
/*
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader shader("5.1.framebuffers.vs", "5.1.framebuffers.fs");
	Shader screenShader("5.1.framebuffers_screen.vs", "5.1.framebuffers_screen.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float cubeVertices[] = {
		// positions          // texture Coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};
	float planeVertices[] = {
		// positions          // texture Coords 
		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

		5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
		5.0f, -0.5f, -5.0f, 2.0f, 2.0f
	};
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};
	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	// plane VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	// screen quad VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// load textures
	// -------------
	unsigned int cubeTexture = loadTexture(FileSystem::getPath("resources/textures/marble.jpg").c_str());
	unsigned int floorTexture = loadTexture(FileSystem::getPath("resources/textures/metal.png").c_str());

	// shader configuration
	// --------------------
	shader.use();
	shader.setInt("texture1", 0);

	screenShader.use();
	screenShader.setInt("screenTexture", 0);

	// framebuffer configuration
	// -------------------------
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// create a color attachment texture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// draw as wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);


		// render
		// ------
		// bind to framebuffer and draw scene as we normally would to color texture 
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

		// make sure we clear the framebuffer's content
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glm::mat4 model;
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		// cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// floor
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		shader.setMat4("model", glm::mat4());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.use();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &quadVBO);

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
*/