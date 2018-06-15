#include <CVK_2/CVK_Framework.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#define WIDTH 1024
#define HEIGHT 768

GLFWwindow* window = nullptr;

CVK::Node *scene_node = nullptr;
CVK::Node *scene_node2 = nullptr;
CVK::Node *scene_node3 = nullptr;
CVK::Node *scene_node4 = nullptr;

//define Camera (Trackball)
CVK::Perspective projection(glm::radians(60.0f), WIDTH / (float)HEIGHT, 0.1f, 50.f);
CVK::Trackball* cam_trackball;

//define materials
CVK::Material *pbr_mat_simple = nullptr;
CVK::Material *pbr_mat1 = nullptr;
CVK::Material *pbr_mat2 = nullptr;
CVK::Material *pbr_mat3 = nullptr;

// ImGui
float metallic = 1.0f;
float roughness = 0.2f;
float ao = 0.0f;
int activeScene = 0;
int displayMode = 0;
bool useCamera = false;
glm::vec3 clear_color = glm::vec3(0.45f, 0.55f, 0.60f);

void charCallback(GLFWwindow *window, unsigned int key)
{
	switch (key)
	{
	case 'W':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'w':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void resizeCallback(GLFWwindow *window, int w, int h)
{
	cam_trackball->setWidthHeight(w, h);
	cam_trackball->getProjection()->updateRatio(w / (float)h);
	glViewport(0, 0, w, h);
}

void init_camera()
{
	//Camera
	cam_trackball = new CVK::Trackball(window, WIDTH, HEIGHT, &projection);
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	cam_trackball->setCenter(&center);
	cam_trackball->setRadius(5.0f);
	CVK::State::getInstance()->setCamera(cam_trackball);
}

void init_lights()
{
	//define Light Sources
	/*for (int i = -10; i <= 10; i += 20) {
		for (int j = -10; j <= 10; j += 20) {
			CVK::Light *plight = new CVK::Light(glm::vec4(i, j, 10.0f, 1.0f), glm::vec3(400.0f, 400.0f, 400.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0f);
			CVK::State::getInstance()->addLight(plight);
		}
	}*/
	CVK::Light *plight = new CVK::Light(glm::vec4(-5.0f, 5.0f, 10.0f, 1.0f), glm::vec3(500.0f, 500.0f, 500.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0f);
	CVK::State::getInstance()->addLight(plight);
	CVK::State::getInstance()->updateSceneSettings(DARKGREY, FOG_LINEAR, WHITE, 1, 50, 1);
}

void init_materials()
{
	pbr_mat_simple = new CVK::Material(glm::vec3(0.5f, 0.0f, 0.0f), metallic, roughness, ao);
	pbr_mat1 = new CVK::Material(RESOURCES_PATH "/textures/darkTiles/darktiles1_basecolor.png",
								 RESOURCES_PATH "/textures/darkTiles/darktiles1_normal-OGL.png",
								 RESOURCES_PATH "/textures/darkTiles/darktiles1_metallic.png",
								 RESOURCES_PATH "/textures/darkTiles/darktiles1_roughness.png",
								 RESOURCES_PATH "/textures/darkTiles/darktiles1_AO.png");
	pbr_mat2 = new CVK::Material(RESOURCES_PATH "/textures/Chest/Chest_Base_Color.png",
		                         RESOURCES_PATH "/textures/Chest/Chest_Normal_OpenGL.png",
		                         RESOURCES_PATH "/textures/Chest/Chest_Metallic.png",
		                         RESOURCES_PATH "/textures/Chest/Chest_Roughness.png",
		                         RESOURCES_PATH "/textures/Chest/Chest_Mixed_AO.png");
	pbr_mat3 = new CVK::Material(RESOURCES_PATH "/textures/matBall/export3dcoat_lambert3SG_color.png",
								 RESOURCES_PATH "/textures/matBall/export3dcoat_lambert3SG_nmap.png",
								 RESOURCES_PATH "/textures/matBall/export3dcoat_lambert3SG_metalness.png",
								 RESOURCES_PATH "/textures/matBall/export3dcoat_lambert3SG_rough.png",
								 RESOURCES_PATH "/textures/matBall/materialball_ao.png");
}

void init_scene()
{
	scene_node = new CVK::Node("Scene");
	CVK::Node *sphere_node = new CVK::Node(std::string("Sphere"), std::string(RESOURCES_PATH "/meshes/sphere.obj"), false);
	sphere_node->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f)));
	sphere_node->setMaterial(pbr_mat_simple);
	scene_node->addChild(sphere_node);

	scene_node2 = new CVK::Node("Scene");
	CVK::Node *sphere_node2 = new CVK::Node(std::string("Sphere"), std::string(RESOURCES_PATH "/meshes/sphere.obj"), false);
	sphere_node2->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f)));
	sphere_node2->setMaterial(pbr_mat1);
	scene_node2->addChild(sphere_node2);

	scene_node3 = new CVK::Node("Scene");
	CVK::Node *sphere_node3 = new CVK::Node(std::string("Chest"), std::string(RESOURCES_PATH "/meshes/chest.obj"), false);
	sphere_node3->setModelMatrix(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f)), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	sphere_node3->setMaterial(pbr_mat2);
	scene_node3->addChild(sphere_node3);

	sphere_node3 = new CVK::Node(std::string("Plane"), std::string(RESOURCES_PATH "/meshes/plane.obj"), false);
	sphere_node3->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.1f, 0.0f)));
	sphere_node3->setMaterial(pbr_mat1);
	scene_node3->addChild(sphere_node3);

	scene_node4 = new CVK::Node("Scene");
	CVK::Node *shaderPresenter = new CVK::Node(std::string("Presenter"), std::string(RESOURCES_PATH "/meshes/export3dcoat.obj"), false);
	shaderPresenter->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f)));
	shaderPresenter->setMaterial(pbr_mat3);
	scene_node4->addChild(shaderPresenter);
}

void clean_up()
{
	delete cam_trackball;
}

void init_imgui()
{
	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	ImGui_ImplGlfwGL3_Init(window, true);

	// Setup style
	ImGui::StyleColorsDark();
}

void cleanup_imgui()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}

void draw_gui()
{
	ImGui::Begin("Debug");
	ImGui::SetWindowSize(ImVec2(400.0f, 190.0f));

	static float f = 0.0f;
	static int counter = 0;
	ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f);
	ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);
	ImGui::SliderFloat("Ambient Occlusion", &ao, 0.0f, 1.0f);
	ImGui::ColorEdit3("clear color", (float*)&clear_color);
	
	ImGui::InputInt("Active Scene", &activeScene);
	ImGui::InputInt("Display Mode", &displayMode);
	ImGui::Checkbox("Use Camera", &useCamera);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
}

int main()
{
	// Init GLFW and GLEW
	glfwInit();
	CVK::useOpenGL33CoreProfile();
	window = glfwCreateWindow(WIDTH, HEIGHT, "Physical Based Rendering", nullptr, nullptr);
	glfwSetWindowPos(window, 100, 50);
	glfwMakeContextCurrent(window);
	glfwSetCharCallback(window, charCallback);
	glfwSetWindowSizeCallback(window, resizeCallback);
	glfwSwapInterval(1); // vsync
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load, compile and link Shader
	const char *shadernames[2] = { SHADERS_PATH "/PBR/PBRsimple.vert", SHADERS_PATH "/PBR/PBRsimple.frag" };
	CVK::ShaderPBRsimple pbrShaderSimple(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, shadernames);
	const char *shadernames2[2] = { SHADERS_PATH "/PBR/PBR.vert", SHADERS_PATH "/PBR/PBR.frag" };
	CVK::ShaderPBR pbrShader(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, shadernames2);

	CVK::State::getInstance()->setShader(&pbrShaderSimple);


	init_camera();
	init_lights();
	init_materials();
	init_scene();

	init_imgui();

	double time = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(clear_color.r, clear_color.g, clear_color.b, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplGlfwGL3_NewFrame();
		draw_gui();

		// Update Camera
		double deltaTime = glfwGetTime() - time;
		time = glfwGetTime();
		if (useCamera) {
			cam_trackball->update(deltaTime);
		}

		// Use Shader and define camera uniforms
		pbr_mat_simple->setMetallic(metallic);
		pbr_mat_simple->setRoughness(roughness);
		pbr_mat_simple->setAO(ao);

		if (activeScene < 0 || activeScene > 3)
			activeScene = 0;
		if (displayMode < 0 || displayMode > 5)
			displayMode = 0;


		switch (activeScene)
		{
		case 0:
			CVK::State::getInstance()->setShader(&pbrShaderSimple);
			pbrShaderSimple.update();
			scene_node->render();
			break;
		case 1:
			CVK::State::getInstance()->setShader(&pbrShader);
			pbrShader.setDisplayMode(displayMode);
			pbrShader.update();
			scene_node2->render();
			break;
		case 2:
			CVK::State::getInstance()->setShader(&pbrShader);
			pbrShader.setDisplayMode(displayMode);
			pbrShader.update();
			scene_node3->render();
			break;
		case 3:
			CVK::State::getInstance()->setShader(&pbrShader);
			pbrShader.setDisplayMode(displayMode);
			pbrShader.update();
			scene_node4->render();
			break;
		default:
			CVK::State::getInstance()->setShader(&pbrShaderSimple);
			pbrShaderSimple.update();
			scene_node->render();
			break;
		}

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	clean_up();
	cleanup_imgui();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}