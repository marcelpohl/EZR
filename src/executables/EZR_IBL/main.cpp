#include <CVK_2/CVK_Framework.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#define WIDTH 1024
#define HEIGHT 768

#define SHADOW_RES 1024

GLFWwindow* window = nullptr;

CVK::Node *scene_node = nullptr;
CVK::Node *scene_node2 = nullptr;
CVK::Node *scene_node3 = nullptr;
CVK::Node *scene_node4 = nullptr;
CVK::Node *scene_node5 = nullptr;

//define Camera (Trackball)
int windowWidth = WIDTH;
int windowHeight = HEIGHT;
CVK::Perspective projection(glm::radians(60.0f), WIDTH / (float)HEIGHT, 0.1f, 50.f);
CVK::Trackball* cam_trackball;

//define materials
CVK::Material *pbr_mat_gold = nullptr;
CVK::Material *pbr_mat_iron = nullptr;
CVK::Material *pbr_mat1 = nullptr;
CVK::Material *pbr_mat2 = nullptr;
CVK::Material *pbr_mat3 = nullptr;
CVK::Material *pbr_mat4 = nullptr;

// global shaders
CVK::ShaderMinimal *depthMapShader;
CVK::ShaderShadowCubemap *shadowCubemapShader;

// ImGui 
float metallic = 1.0f;
float roughness = 0.2f;
float ao = 0.0f;
int activeScene = 0;
int lastScene = 0;
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
	windowWidth = w;
	windowHeight = h;
}

void init_lights()
{
	//define Light Sources
	for (int i = -10; i <= 10; i += 20) {
		for (int j = -10; j <= 10; j += 20) {
			//CVK::Light *plight = new CVK::Light(glm::vec4(i, j, 10.0f, 1.0f), glm::vec3(400.0f, 400.0f, 400.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0f);
			CVK::Light *plight = new CVK::Light(glm::vec4(i, j, 10.0f, 1.0f), glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0f);
			CVK::State::getInstance()->addLight(plight);
		}
	}
	//CVK::Light *plight = new CVK::Light(glm::vec4(-5.0f, 5.0f, 10.0f, 1.0f), glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, glm::radians(15.0f));
	//plight->setType(0);
	//plight->setCastShadow(true, window);
	//CVK::State::getInstance()->addLight(plight);

	//plight = new CVK::Light(glm::vec4(5.0f, 5.0f, 10.0f, 1.0f), glm::vec3(30.0f, 30.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, glm::radians(15.0f));
	//plight->setType(0);
	//plight->setCastShadow(true, window);
	//CVK::State::getInstance()->addLight(plight);

	//plight = new CVK::Light(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0f);
	//plight->setType(1);
	//plight->setCastShadow(true, window);
	//CVK::State::getInstance()->addLight(plight);

	CVK::State::getInstance()->updateSceneSettings(DARKGREY, FOG_LINEAR, WHITE, 1, 50, 1);
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

void init_materials()
{
	pbr_mat_gold = new CVK::Material(RESOURCES_PATH "/textures/goldScuffed/gold-scuffed_basecolor-boosted.png",
									 RESOURCES_PATH "/textures/goldScuffed/gold-scuffed_normal.png",
		                             RESOURCES_PATH "/textures/goldScuffed/gold-scuffed_metallic.png",
		                             RESOURCES_PATH "/textures/ironScuffed/Iron-Scuffed_roughness.png",
		                             RESOURCES_PATH "/textures/goldScuffed/gold-scuffed_ao.png");
	pbr_mat_iron = new CVK::Material(RESOURCES_PATH "/textures/ironScuffed/Iron-Scuffed_basecolor.png",
									 RESOURCES_PATH "/textures/ironScuffed/Iron-Scuffed_normal.png",
									 RESOURCES_PATH "/textures/ironScuffed/Iron-Scuffed_metallic.png",
									 RESOURCES_PATH "/textures/ironScuffed/Iron-Scuffed_roughness.png",
									 RESOURCES_PATH "/textures/ironScuffed/Iron-Scuffed_ao.png");
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
	pbr_mat4 = new CVK::Material(RESOURCES_PATH "/textures/polishMarble/streaked-marble-albedo2.png",
								 RESOURCES_PATH "/textures/polishMarble/streaked-marble-normal.png",
								 RESOURCES_PATH "/textures/polishMarble/streaked-marble-metalness.png",
								 RESOURCES_PATH "/textures/polishMarble/streaked-marble-roughness1.png",
								 RESOURCES_PATH "/textures/polishMarble/streaked-marble-ao.png");
}

void init_scene()
{
	/****************************
	* Scene 1 - IBL
	*****************************/
	scene_node = new CVK::Node("Scene");
	CVK::Node *sphere_node = new CVK::Node(std::string("Sphere"), std::string(RESOURCES_PATH "/meshes/sphere.obj"), false);
	sphere_node->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f)));
	sphere_node->setMaterial(pbr_mat_gold);
	scene_node->addChild(sphere_node);

	/****************************
	* Scene 2 - Chest and directional light
	*****************************/
	scene_node2 = new CVK::Node("Scene");
	CVK::Node *sphere_node2 = new CVK::Node(std::string("Chest"), std::string(RESOURCES_PATH "/meshes/chest.obj"), false);
	sphere_node2->setModelMatrix(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f)), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	sphere_node2->setMaterial(pbr_mat2);
	scene_node2->addChild(sphere_node2);

	sphere_node2 = new CVK::Node(std::string("Plane"), std::string(RESOURCES_PATH "/meshes/plane.obj"), false);
	sphere_node2->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.1f, 0.0f)));
	sphere_node2->setMaterial(pbr_mat4);
	scene_node2->addChild(sphere_node2);


	/****************************
	* Scene 3 - Omnidirectional Shadows
	*****************************/
	scene_node3 = new CVK::Node("Scene");
	CVK::Node *openCube = new CVK::Node(std::string("Cube"), std::string(RESOURCES_PATH "/meshes/openCube.obj"), false);
	openCube->setModelMatrix(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.7f, 0.7f, 0.7f)), glm::vec3(0.0f, 1.0f, 0.0f)));
	openCube->setMaterial(pbr_mat4);
	scene_node3->addChild(openCube);

	CVK::Node *group = new CVK::Node(std::string("Group"));
	scene_node3->addChild(group);
	CVK::Node *sphere_node3 = new CVK::Node(std::string("Sphere"), std::string(RESOURCES_PATH "/meshes/sphere.obj"), false);
	sphere_node3->setModelMatrix(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -2.0f)), glm::vec3(0.1f, 0.1f, 0.1f)));
	sphere_node3->setMaterial(pbr_mat1);
	group->addChild(sphere_node3);
	CVK::Node *sphere_node4 = new CVK::Node(std::string("Sphere"), std::string(RESOURCES_PATH "/meshes/sphere.obj"), false);
	sphere_node4->setModelMatrix(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -0.5f, 2.0f)), glm::vec3(0.1f, 0.1f, 0.1f)));
	sphere_node4->setMaterial(pbr_mat1);
	group->addChild(sphere_node4);
	CVK::Node *sphere_node5 = new CVK::Node(std::string("Sphere"), std::string(RESOURCES_PATH "/meshes/sphere.obj"), false);
	sphere_node5->setModelMatrix(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -0.5f, 2.0f)), glm::vec3(0.1f, 0.1f, 0.1f)));
	sphere_node5->setMaterial(pbr_mat1);
	group->addChild(sphere_node5);


	/****************************
	* Scene 4 - Presenter
	*****************************/
	scene_node4 = new CVK::Node("Scene");
	CVK::Node *shaderPresenter = new CVK::Node(std::string("Presenter"), std::string(RESOURCES_PATH "/meshes/export3dcoat.obj"), false);
	shaderPresenter->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f)));
	shaderPresenter->setMaterial(pbr_mat3);
	scene_node4->addChild(shaderPresenter);

	/****************************
	* Scene 5 - Pre Rendertest
	*****************************/
	scene_node5 = new CVK::Node("Scene");
	CVK::Node *sphere_gold = new CVK::Node(std::string("Sphere"), std::string(RESOURCES_PATH "/meshes/sphere.obj"), false);
	glm::mat4 mm1 = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f));
	mm1 = glm::scale(mm1, glm::vec3(0.5f, 0.5f, 0.5f));
	sphere_gold->setModelMatrix(mm1);
	sphere_gold->setMaterial(pbr_mat_gold);
	scene_node5->addChild(sphere_gold);

	CVK::Node *sphere_iron = new CVK::Node(std::string("Sphere"), std::string(RESOURCES_PATH "/meshes/sphere.obj"), false);
	glm::mat4 mm2 = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 0.0f, 0.0f));
	mm2 = glm::scale(mm2, glm::vec3(0.5f, 0.5f, 0.5f));
	sphere_iron->setModelMatrix(mm2);
	sphere_iron->setMaterial(pbr_mat_iron);
	scene_node5->addChild(sphere_iron);

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

void render_scene(CVK::Node *scene, CVK::ShaderMinimal *shader, CVK::ShaderCubeMap *skybox = nullptr , CVK::Camera * cam = cam_trackball ,unsigned int width = windowWidth, unsigned int height = windowHeight)
{
	// 1st pass: render shadow maps
	CVK::Light *light = nullptr;
	for (size_t i = 0; i < CVK::State::getInstance()->getLights()->size(); i++) {
		light = &CVK::State::getInstance()->getLights()->at(i);
		if (light->prepareRenderShadowMap())
		{
			CVK::State::getInstance()->setCamera(light->getLightCamera());
			if (light->getType() == 0) {
				CVK::State::getInstance()->setShader(depthMapShader);
				depthMapShader->update();
			}
			else {
				CVK::State::getInstance()->setShader(shadowCubemapShader);
				shadowCubemapShader->update(light);
			}
			
			scene->render();

			light->finishRenderShadowMap();
		}
	}

	// 2nd pass: render scene normally
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CVK::State::getInstance()->setCamera(cam);

	CVK::State::getInstance()->setShader(shader);
	shader->update();
	scene->render();

	if (skybox != nullptr)
	{
		skybox->update();
		skybox->render();
	}
}

int main()
{
	// Init GLFW and GLEW
	glfwInit();
	CVK::useOpenGL45CoreProfile();
	window = glfwCreateWindow(WIDTH, HEIGHT, "Physical Based Rendering", nullptr, nullptr);
	glfwSetWindowPos(window, 100, 50);
	glfwMakeContextCurrent(window);
	glfwSetCharCallback(window, charCallback);
	glfwSetWindowSizeCallback(window, resizeCallback);
	glfwSwapInterval(0); // vsync
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // Bessere CubeMaps

	std::vector<std::string> cubeMapImages;
	cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/StPeter/posx.jpg"));
	cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/StPeter/negx.jpg"));
	cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/StPeter/posy.jpg"));
	cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/StPeter/negy.jpg"));
	cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/StPeter/posz.jpg"));
	cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/StPeter/negz.jpg"));	
	//cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/TestCube/white.png"));
	//cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/TestCube/green.png"));
	//cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/TestCube/red.png"));
	//cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/TestCube/white.png"));
	//cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/TestCube/white.png"));
	//cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/TestCube/white.png"));

	CVK::CubeMapTexture* cubeMap = new CVK::CubeMapTexture(cubeMapImages);
	CVK::Environment* environment = new CVK::Environment(cubeMap, 32u, 512u, 5u, 1024u);
	environment->computeMaps();

	// Skybox
	const char *skyBoxShadernames[2] = { SHADERS_PATH "/PBR/Skybox.vert", SHADERS_PATH "/PBR/Skybox.frag" };
	CVK::ShaderCubeMap skyBoxShader(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, skyBoxShadernames, cubeMap);

	// Load, compile and link Shader
	const char *depthShadernames[2] = { SHADERS_PATH "/PBR/depthMap.vert", SHADERS_PATH "/PBR/depthMap.frag" };
	depthMapShader = new CVK::ShaderMinimal(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, depthShadernames);

	const char *cubeShadowsShadernames[3] = { SHADERS_PATH "/PBR/shadowCube.vert", SHADERS_PATH "/PBR/shadowCube.geom", SHADERS_PATH "/PBR/shadowCube.frag" };
	shadowCubemapShader = new CVK::ShaderShadowCubemap(VERTEX_SHADER_BIT | GEOMETRY_SHADER_BIT | FRAGMENT_SHADER_BIT, cubeShadowsShadernames);

	//const char *shadernames[2] = { SHADERS_PATH "/PBR/PBRsimple.vert", SHADERS_PATH "/PBR/PBRsimple.frag" };
	//CVK::ShaderPBRsimple pbrShaderSimple(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, shadernames);
	const char *shadernames2[2] = { SHADERS_PATH "/PBR/PBR.vert", SHADERS_PATH "/PBR/PBR.frag" };
	CVK::ShaderPBR pbrShader(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, shadernames2, environment);

	init_lights();
	init_camera();
	init_materials();
	init_scene();
	init_imgui();

	environment->renderSceneToEnvironmentMap(render_scene,*scene_node5, pbrShader, skyBoxShader, 1024, glm::vec3(0.0, 0.0, 0.0));
	environment->computeMaps();

	CVK::ShaderCubeMap skyBoxShader2(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, skyBoxShadernames, environment->getEnvironmentMap());

	//CVK::State::getInstance()->setShader(&pbrShaderSimple);
	CVK::State::getInstance()->setShader(&pbrShader);

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

		// light rotations and update cameras
		CVK::Light *light = nullptr;
		for (size_t i = 0; i < CVK::State::getInstance()->getLights()->size(); i++) {
			light = &CVK::State::getInstance()->getLights()->at(i);
			if (activeScene != 2) {
				glm::vec4 lightPos = *(light->getPosition());
				lightPos = glm::rotate(glm::mat4(1.0f), glm::radians(20.0f * (float)deltaTime), glm::vec3(0.0f, 1.0f, 0.0f)) * lightPos;
				light->setPosition(lightPos);
			}
			if (light->castsShadow())
				light->getLightCamera()->update(deltaTime);
		}

		// set active lights
		if (activeScene != lastScene) {
			if (activeScene == 2)
			{
				CVK::State::getInstance()->getLights()->at(0).setColor(glm::vec3(0.0f, 0.0f, 0.0f));
				//CVK::State::getInstance()->getLights()->at(1).setColor(glm::vec3(0.0f, 0.0f, 0.0f));
				CVK::State::getInstance()->getLights()->at(1).setColor(glm::vec3(20.0f, 15.0f, 15.0f));
			}
			else {
				CVK::State::getInstance()->getLights()->at(0).setColor(glm::vec3(50.0f, 50.0f, 50.0f));
				//CVK::State::getInstance()->getLights()->at(1).setColor(glm::vec3(30.0f, 30.0f, 30.0f));
				CVK::State::getInstance()->getLights()->at(1).setColor(glm::vec3(0.0f, 0.0f, 0.0f));
			}
			lastScene = activeScene;
		}

		// rotate spheres in scene 2
		if (activeScene == 2)
		{
			std::vector<CVK::Node*> *nodes = scene_node3->getChildren();
			for (int i = 0; i < nodes->size(); i++) {
				if (*(*nodes)[i]->getName() == "Group")
					(*nodes)[i]->setModelMatrix(glm::rotate(*(*nodes)[i]->getModelMatrix(), glm::radians(20.0f * (float)deltaTime), glm::vec3(0.0f, 1.0f, 0.0f)));
			}
		}

		if (useCamera) {
			cam_trackball->update(deltaTime);
		}

		// Use Shader and define camera uniforms
		pbr_mat_gold->setMetallic(metallic);
		pbr_mat_gold->setRoughness(roughness);
		pbr_mat_gold->setAO(ao);

		if (activeScene < 0 || activeScene > 3)
			activeScene = 0;
		if (displayMode < 0 || displayMode > 5)
			displayMode = 0;

		switch (activeScene)
		{
		case 0:
			pbrShader.setDisplayMode(displayMode);
			render_scene(scene_node, &pbrShader, &skyBoxShader2);
			break;
		case 1:
			pbrShader.setDisplayMode(displayMode);
			render_scene(scene_node2, &pbrShader, &skyBoxShader2);
			break;
		case 2:
			pbrShader.setDisplayMode(displayMode);
			render_scene(scene_node3, &pbrShader);
			break;
		case 3:
			pbrShader.setDisplayMode(displayMode);
			render_scene(scene_node4, &pbrShader, &skyBoxShader2);
			break;
		default:
			render_scene(scene_node, &pbrShader);
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