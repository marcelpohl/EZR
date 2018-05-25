#include <CVK_2/CVK_Framework.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#define WIDTH 1024
#define HEIGHT 768

GLFWwindow* window = nullptr;

CVK::Node *scene_node = nullptr;

//define Camera (Trackball)
CVK::Perspective projection(glm::radians(60.0f), WIDTH / (float)HEIGHT, 0.1f, 50.f);
CVK::Trackball* cam_trackball;

//define materials
CVK::Material *mat_red_pbr = nullptr;

float metallic = 1.0f;
float roughness = 0.2f;
float ao = 0.0f;

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
	for (int i = -5; i <= 5; i += 10) {
		for (int j = -5; j <= 5; j += 10) {
			CVK::Light *plight = new CVK::Light(glm::vec4(i, j, 2, 1), glm::vec3(300.0f, 300.0f, 300.0f), glm::vec3(0, 0, 0), 1.0f, 0.0f);
			CVK::State::getInstance()->addLight(plight);
		}
	}
	CVK::State::getInstance()->updateSceneSettings(DARKGREY, FOG_LINEAR, WHITE, 1, 50, 1);
}

void init_materials()
{
	mat_red_pbr = new CVK::Material(RED, metallic, roughness, ao);
}

void init_scene()
{
	scene_node = new CVK::Node("Scene");

	CVK::Sphere *sphere = new CVK::Sphere(0.3f);

	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			CVK::Node *sphere_node = new CVK::Node(std::string("Sphere ") + std::to_string(i) + std::to_string(j));
			sphere_node->setModelMatrix(glm::translate(glm::mat4(1.0), glm::vec3(i, j, 0)));
			sphere_node->setMaterial(mat_red_pbr);
			sphere_node->setGeometry(sphere);
			scene_node->addChild(sphere_node);
		}
	}
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
	// 1. Show a simple window.
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	ImGui::Begin("Debug");
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2(300.0f, 200.0f));

	static float f = 0.0f;
	static int counter = 0;
	ImGui::Text("Hello, world!");
	ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f);
	ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);
	ImGui::SliderFloat("Ambient Occlusion", &ao, 0.0f, 1.0f);
	ImGui::ColorEdit3("clear color", (float*)&clear_color);

	if (ImGui::Button("Button"))
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

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
	const char *shadernames[2] = { SHADERS_PATH "/PBR/PBR.vert", SHADERS_PATH "/PBR/PBR.frag" };
	CVK::ShaderPBR pbrShader(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, shadernames);
	CVK::State::getInstance()->setShader(&pbrShader);

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
		//cam_trackball->update(deltaTime);

		// Use Shader and define camera uniforms
		mat_red_pbr->setMetallic(metallic);
		mat_red_pbr->setRoughness(roughness);
		mat_red_pbr->setAO(ao);
		pbrShader.update();

		scene_node->render();

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