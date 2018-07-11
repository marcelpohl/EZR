#include <CVK_2/CVK_Framework.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#define WIDTH 1600
#define HEIGHT 900

GLFWwindow* window = nullptr;

CVK::Node *scene_combined = nullptr;
CVK::Node *scene_node_room = nullptr;
CVK::Node *scene_node_static_obj = nullptr;
CVK::Node *scene_node_dyn_mid = nullptr;
CVK::Node *scene_shadows = nullptr;

//define Camera (Trackball)
int windowWidth = WIDTH;
int windowHeight = HEIGHT;
CVK::Perspective projection(glm::radians(60.0f), WIDTH / (float)HEIGHT, 0.1f, 500.f);
CVK::Pilotview* cam_pilot;

//define materials
CVK::Material *pbr_mat_general = nullptr;
CVK::Material *pbr_mat_floor = nullptr;
CVK::Material *pbr_mat_ceiling = nullptr;
CVK::Material *pbr_mat_wall = nullptr;
CVK::Material *pbr_mat_wallBig = nullptr;
CVK::Material *pbr_mat_pillarGate = nullptr;
CVK::Material *pbr_mat_separator = nullptr;
CVK::Material *pbr_mat_wallUpper = nullptr;
CVK::Material *pbr_mat_midAperature = nullptr;
CVK::Material *pbr_mat_sideAperature = nullptr;

CVK::Material *pbr_mat_matBall = nullptr;
CVK::Material *pbr_mat_box = nullptr;
CVK::Material *pbr_mat_sphere = nullptr;
CVK::Material *pbr_mat_marble = nullptr;
CVK::Material *pbr_mat_chest = nullptr;

// global shaders
CVK::ShaderMinimal *depthMapShader;
CVK::ShaderShadowCubemap *shadowCubemapShader;

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
	cam_pilot->setWidthHeight(w, h);
	cam_pilot->getProjection()->updateRatio(w / (float)h);

	glViewport(0, 0, w, h);
	windowWidth = w;
	windowHeight = h;
}

void init_lights()
{
	CVK::Light *plight = nullptr;

	//define Light Sources
	/*for (int i = -10; i <= 10; i += 20) {
		for (int j = -10; j <= 10; j += 20) {
			CVK::Light *plight = new CVK::Light(glm::vec4(i, j, 10.0f, 1.0f), glm::vec3(400.0f, 400.0f, 400.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0f);
			CVK::State::getInstance()->addLight(plight);
		}
	}*/
	/*plight = new CVK::Light(glm::vec4(0.1f, 10.0f, 0.1f, 1.0f), glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, glm::radians(20.0f));
	plight->setType(0);
	plight->setCastShadow(true, window);
	CVK::State::getInstance()->addLight(plight);*/

	// mid aperture light
	plight = new CVK::Light(glm::vec4(0.0f, 20.0f, 0.0f, 1.0f), glm::vec3(1000.0f, 1000.0f, 1000.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0f);
	plight->setType(1);
	plight->setCastShadow(true, window);
	CVK::State::getInstance()->addLight(plight);

	// side aperture lights
	plight = new CVK::Light(glm::vec4(43.0f, 23.2f, 28.5f, 1.0f), glm::vec3(100.0f, 100.0f, 100.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0f);
	plight->setType(1);
	plight->setCastShadow(false, window);
	CVK::State::getInstance()->addLight(plight);

	plight = new CVK::Light(glm::vec4(43.0f, 23.2f, -28.5f, 1.0f), glm::vec3(100.0f, 100.0f, 100.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0f);
	plight->setType(1);
	plight->setCastShadow(false, window);
	CVK::State::getInstance()->addLight(plight);

	plight = new CVK::Light(glm::vec4(-43.0f, 23.2f, 28.5f, 1.0f), glm::vec3(100.0f, 100.0f, 100.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0f);
	plight->setType(1);
	plight->setCastShadow(false, window);
	CVK::State::getInstance()->addLight(plight);

	plight = new CVK::Light(glm::vec4(-43.0f, 23.2f, -28.5f, 1.0f), glm::vec3(100.0f, 100.0f, 100.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0f);
	plight->setType(1);
	plight->setCastShadow(false, window);
	CVK::State::getInstance()->addLight(plight);


	plight = new CVK::Light(glm::vec4(43.0f, 23.2f, 28.5f, 1.0f), glm::vec3(750.0f, 750.0f, 750.0f), glm::vec3(43.0f, 20.0f, 28.4f), 1.0f, glm::radians(18.0f));
	plight->setType(0);
	plight->setCastShadow(true, window);
	CVK::State::getInstance()->addLight(plight);

	plight = new CVK::Light(glm::vec4(43.0f, 23.2f, -28.5f, 1.0f), glm::vec3(750.0f, 750.0f, 750.0f), glm::vec3(43.0f, 20.0f, -28.4f), 1.0f, glm::radians(18.0f));
	plight->setType(0);
	plight->setCastShadow(true, window);
	CVK::State::getInstance()->addLight(plight);

	plight = new CVK::Light(glm::vec4(-43.0f, 23.2f, 28.5f, 1.0f), glm::vec3(750.0f, 750.0f, 750.0f), glm::vec3(-43.0f, 20.0f, 28.4f), 1.0f, glm::radians(18.0f));
	plight->setType(0);
	plight->setCastShadow(true, window);
	CVK::State::getInstance()->addLight(plight);

	plight = new CVK::Light(glm::vec4(-43.0f, 23.2f, -28.5f, 1.0f), glm::vec3(750.0f, 750.0f, 750.0f), glm::vec3(-43.0f, 20.0f, -28.4f), 1.0f, glm::radians(18.0f));
	plight->setType(0);
	plight->setCastShadow(true, window);
	CVK::State::getInstance()->addLight(plight);

	CVK::State::getInstance()->updateSceneSettings(DARKGREY, FOG_LINEAR, WHITE, 1, 50, 1);
}


void init_camera()
{
	//Camera
	cam_pilot = new CVK::Pilotview(window, WIDTH, HEIGHT, &projection);
	cam_pilot->setCamPos(new glm::vec3(0.0f, 10.0f, 40.0f));
	CVK::State::getInstance()->setCamera(cam_pilot);
}

void init_materials()
{
	pbr_mat_general = new CVK::Material(RESOURCES_PATH "/textures/Scene/ceiling_BaseColor.png",
										RESOURCES_PATH "/textures/Scene/ceiling_Normal.png",
										RESOURCES_PATH "/textures/Scene/ceiling_Metallic.png",
										RESOURCES_PATH "/textures/Scene/ceiling_Roughness.png",
										RESOURCES_PATH "/textures/Scene/ceiling_ao.png");
	pbr_mat_floor = new CVK::Material(RESOURCES_PATH "/textures/Scene/floor_BaseColor.png",
									  RESOURCES_PATH "/textures/Scene/floor_Normal.png",
									  RESOURCES_PATH "/textures/Scene/floor_Metallic.png",
									  RESOURCES_PATH "/textures/Scene/floor_Roughness.png",
									  RESOURCES_PATH "/textures/Scene/floor_ao.png");
	pbr_mat_ceiling = new CVK::Material(RESOURCES_PATH "/textures/Scene/ceiling_BaseColor.png",
									  RESOURCES_PATH "/textures/Scene/ceiling_Normal.png",
									  RESOURCES_PATH "/textures/Scene/ceiling_Metallic.png",
									  RESOURCES_PATH "/textures/Scene/ceiling_Roughness.png",
									  RESOURCES_PATH "/textures/Scene/ceiling_ao.png");
	pbr_mat_wall = new CVK::Material(RESOURCES_PATH "/textures/Scene/wall_BaseColor.png",
									 RESOURCES_PATH "/textures/Scene/wall_Normal.png",
									 RESOURCES_PATH "/textures/Scene/wall_Metallic.png",
									 RESOURCES_PATH "/textures/Scene/wall_Roughness.png",
									 RESOURCES_PATH "/textures/Scene/wall_AmbientOcclusion.png");	
	pbr_mat_wallBig = new CVK::Material(RESOURCES_PATH "/textures/Scene/wallBig_BaseColor.png",
									    RESOURCES_PATH "/textures/Scene/wallBig_Normal.png",
									    RESOURCES_PATH "/textures/Scene/wallBig_Metallic.png",
									    RESOURCES_PATH "/textures/Scene/wallBig_Roughness.png",
									    RESOURCES_PATH "/textures/Scene/wallBig_AmbientOcclusion.png");	
	pbr_mat_pillarGate = new CVK::Material(RESOURCES_PATH "/textures/Scene/PillarGate_BaseColor.png",
									       RESOURCES_PATH "/textures/Scene/PillarGate_Normal.png",
									       RESOURCES_PATH "/textures/Scene/PillarGate_Metallic.png",
									       RESOURCES_PATH "/textures/Scene/PillarGate_Roughness.png",
									       RESOURCES_PATH "/textures/Scene/PillarGate_AmbientOcclusion.png");
	pbr_mat_separator = new CVK::Material(RESOURCES_PATH "/textures/Scene/separator_BaseColor.png",
										  RESOURCES_PATH "/textures/Scene/separator_Normal.png",
										  RESOURCES_PATH "/textures/Scene/separator_Metallic.png",
										  RESOURCES_PATH "/textures/Scene/separator_Roughness.png",
										  RESOURCES_PATH "/textures/Scene/separator_AmbientOcclusion.png");
	pbr_mat_wallUpper = new CVK::Material(RESOURCES_PATH "/textures/Scene/wallUpper_BaseColor.png",
										  RESOURCES_PATH "/textures/Scene/wallUpper_Normal.png",
										  RESOURCES_PATH "/textures/Scene/wallUpper_Metallic.png",
										  RESOURCES_PATH "/textures/Scene/wallUpper_Roughness.png",
										  RESOURCES_PATH "/textures/Scene/wallUpper_AmbientOcclusion.png");
	pbr_mat_midAperature = new CVK::Material(RESOURCES_PATH "/textures/Scene/midAperature_BaseColor.png",
		RESOURCES_PATH "/textures/Scene/midAperature_Normal.png",
		RESOURCES_PATH "/textures/Scene/midAperature_Metallic.png",
		RESOURCES_PATH "/textures/Scene/midAperature_Roughness.png",
		RESOURCES_PATH "/textures/Scene/midAperature_AmbientOcclusion.png");
	pbr_mat_sideAperature = new CVK::Material(RESOURCES_PATH "/textures/Scene/sideAperature_BaseColor.png",
		RESOURCES_PATH "/textures/Scene/sideAperature_Normal.png",
		RESOURCES_PATH "/textures/Scene/sideAperature_Metallic.png",
		RESOURCES_PATH "/textures/Scene/sideAperature_Roughness.png",
		RESOURCES_PATH "/textures/Scene/sideAperature_AmbientOcclusion.png");

	pbr_mat_matBall = new CVK::Material(RESOURCES_PATH "/textures/matBall/export3dcoat_lambert3SG_color.png",
		RESOURCES_PATH "/textures/matBall/export3dcoat_lambert3SG_nmap.png",
		RESOURCES_PATH "/textures/matBall/export3dcoat_lambert3SG_metalness.png",
		RESOURCES_PATH "/textures/matBall/export3dcoat_lambert3SG_rough.png",
		RESOURCES_PATH "/textures/matBall/materialball_ao.png");
	pbr_mat_box = new CVK::Material(RESOURCES_PATH "/textures/Scene/box_BaseColor.png",
		RESOURCES_PATH "/textures/Scene/box_Normal.png",
		RESOURCES_PATH "/textures/Scene/box_Metallic.png",
		RESOURCES_PATH "/textures/Scene/box_Roughness.png",
		RESOURCES_PATH "/textures/Scene/box_AmbientOcclusion.png");
	pbr_mat_sphere = new CVK::Material(RESOURCES_PATH "/textures/goldScuffed/gold-scuffed_basecolor-boosted.png",
		RESOURCES_PATH "/textures/goldScuffed/gold-scuffed_normal.png",
		RESOURCES_PATH "/textures/goldScuffed/gold-scuffed_metallic.png",
		RESOURCES_PATH "/textures/ironScuffed/Iron-Scuffed_roughness.png",
		RESOURCES_PATH "/textures/goldScuffed/gold-scuffed_ao.png");
	pbr_mat_chest = new CVK::Material(RESOURCES_PATH "/textures/Chest/Chest_Base_Color.png",
		RESOURCES_PATH "/textures/Chest/Chest_Normal_OpenGL.png",
		RESOURCES_PATH "/textures/Chest/Chest_Metallic.png",
		RESOURCES_PATH "/textures/Chest/Chest_Roughness.png",
		RESOURCES_PATH "/textures/Chest/Chest_Mixed_AO.png");
	pbr_mat_marble = new CVK::Material(RESOURCES_PATH "/textures/Scene/marble-speckled-albedo.png",
		RESOURCES_PATH "/textures/Scene/marble-speckled-normal.png",
		RESOURCES_PATH "/textures/Scene/marble-speckled-metalness.png",
		RESOURCES_PATH "/textures/Scene/marble-speckled-roughness.png",
		RESOURCES_PATH "/textures/goldScuffed/gold-scuffed_ao.png");
}

void init_scene()
{
	scene_shadows = new CVK::Node("Shadows");

	/****************************
	* Room
	*****************************/
	scene_node_room = new CVK::Node("Room");
	CVK::Node *node = new CVK::Node(std::string("Floor"), std::string(RESOURCES_PATH "/meshes/Scene/floor.obj"), false);
	node->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	node->setMaterial(pbr_mat_floor);
	scene_node_room->addChild(node);

	node = new CVK::Node(std::string("Ceiling"), std::string(RESOURCES_PATH "/meshes/Scene/ceiling.obj"), false);
	node->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	node->setMaterial(pbr_mat_ceiling);
	scene_node_room->addChild(node);

	node = new CVK::Node(std::string("pillarsGates"), std::string(RESOURCES_PATH "/meshes/Scene/pillars_gates.obj"), false);
	node->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	node->setMaterial(pbr_mat_pillarGate);
	scene_node_room->addChild(node);

	node = new CVK::Node(std::string("separators"), std::string(RESOURCES_PATH "/meshes/Scene/separators.obj"), false);
	node->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	node->setMaterial(pbr_mat_separator);
	scene_node_room->addChild(node);

	node = new CVK::Node(std::string("wallBig"), std::string(RESOURCES_PATH "/meshes/Scene/wallBig.obj"), false);
	node->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	node->setMaterial(pbr_mat_wallBig);
	scene_node_room->addChild(node);

	node = new CVK::Node(std::string("walls"), std::string(RESOURCES_PATH "/meshes/Scene/walls.obj"), false);
	node->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	node->setMaterial(pbr_mat_wall);
	scene_node_room->addChild(node);

	node = new CVK::Node(std::string("wallsUpper"), std::string(RESOURCES_PATH "/meshes/Scene/wallsUpper.obj"), false);
	node->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	node->setMaterial(pbr_mat_wallUpper);
	scene_node_room->addChild(node);


	/****************************
	* Static Objects
	*****************************/
	scene_node_static_obj = new CVK::Node("Static Objects");
	node = new CVK::Node(std::string("MidAperature"), std::string(RESOURCES_PATH "/meshes/Scene/midAperature.obj"), false);
	node->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	node->setMaterial(pbr_mat_midAperature);
	//scene_node_static_obj->addChild(node);
	scene_node_room->addChild(node);
	scene_shadows->addChild(node);

	node = new CVK::Node(std::string("SideAperatures"), std::string(RESOURCES_PATH "/meshes/Scene/sideAparetures.obj"), false);
	node->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	node->setMaterial(pbr_mat_sideAperature);
	//scene_node_static_obj->addChild(node);
	scene_node_room->addChild(node);
	scene_shadows->addChild(node);

	node = new CVK::Node(std::string("SideAperatures"), std::string(RESOURCES_PATH "/meshes/Scene/sphere.obj"), false);
	node->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	node->setMaterial(pbr_mat_sphere);
	//scene_node_static_obj->addChild(node);
	scene_node_room->addChild(node);
	scene_shadows->addChild(node);


	/****************************
	* Dynamic Objects
	*****************************/
	scene_node_dyn_mid = new CVK::Node("Dynamic mid aperature");
	node = new CVK::Node(std::string("Dynamic midAperature"), std::string(RESOURCES_PATH "/meshes/Scene/midAperature_moving.obj"), false);
	node->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	node->setMaterial(pbr_mat_midAperature);
	scene_node_dyn_mid->addChild(node);
	scene_shadows->addChild(node);

	node = new CVK::Node(std::string("Cerberus Gun"), std::string(RESOURCES_PATH "/meshes/export3dcoat.obj"), false);
	node->setModelMatrix(glm::scale( glm::translate(*node->getModelMatrix(), glm::vec3(0.0f, 8.0f, 0.0f)), glm::vec3(0.5f, 0.5f, 0.5f)));
	node->setMaterial(pbr_mat_matBall);
	scene_node_dyn_mid->addChild(node);
	scene_shadows->addChild(node);

	node = new CVK::Node(std::string("Box"), std::string(RESOURCES_PATH "/meshes/Scene/box.obj"), false);
	node->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	node->setMaterial(pbr_mat_box);
	scene_node_dyn_mid->addChild(node);
	scene_shadows->addChild(node);

	node = new CVK::Node(std::string("Chest"), std::string(RESOURCES_PATH "/meshes/chest.obj"), false);
	node->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
	node->setMaterial(pbr_mat_chest);
	scene_node_dyn_mid->addChild(node);
	scene_shadows->addChild(node);


	node = new CVK::Node(std::string("Marble"), std::string(RESOURCES_PATH "/meshes/Scene/sphere_marble.obj"), false);
	node->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	node->setMaterial(pbr_mat_marble);
	scene_node_dyn_mid->addChild(node);
	scene_shadows->addChild(node);

	/****************************
	* Combined Scene
	*****************************/
	scene_combined = new CVK::Node("Combined Scene");
	scene_combined->addChild(scene_node_room);
	scene_combined->addChild(scene_node_static_obj);
	scene_combined->addChild(scene_node_dyn_mid);

}

void clean_up()
{
	delete cam_pilot;
}

void render_scene_cube(CVK::Node *scene, CVK::ShaderMinimal *shader, CVK::ShaderCubeMap *skybox = nullptr, CVK::Camera * cam = cam_pilot, unsigned int width = windowWidth, unsigned int height = windowHeight)
{
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


void render_scene(CVK::Node *scene, CVK::ShaderMinimal *shader, CVK::ShaderCubeMap *skybox = nullptr)
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

			scene_shadows->render();

			light->finishRenderShadowMap();
		}
	}

	// 2nd pass: render scene normally
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CVK::State::getInstance()->setCamera(cam_pilot);

	CVK::State::getInstance()->setShader(shader);
	shader->update();
	scene->render();

	skybox->update();
	skybox->render();
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
	//cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/StPeter/posx.jpg"));
	//cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/StPeter/negx.jpg"));
	//cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/StPeter/posy.jpg"));
	//cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/StPeter/negy.jpg"));
	//cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/StPeter/posz.jpg"));
	//cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/StPeter/negz.jpg"));
	cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/TestCube/scene.png"));
	cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/TestCube/scene.png"));
	cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/TestCube/scene.png"));
	cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/TestCube/scene.png"));
	cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/TestCube/scene.png"));
	cubeMapImages.push_back(std::string(RESOURCES_PATH "/textures/TestCube/scene.png"));

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

	const char *shadernames2[2] = { SHADERS_PATH "/PBR/PBR.vert", SHADERS_PATH "/PBR/PBR.frag" };
	CVK::ShaderPBR pbrShader(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, shadernames2, environment);


	init_lights();
	init_camera();
	init_materials();
	init_scene();

	environment->renderSceneToEnvironmentMap(render_scene_cube, *scene_node_room, pbrShader, skyBoxShader, 1024, glm::vec3(0.0f, 10.0f, 0.0f));
	environment->computeMaps();

	CVK::ShaderCubeMap skyBoxShader2(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, skyBoxShadernames, environment->getEnvironmentMap());

	CVK::State::getInstance()->setShader(&pbrShader);


	double time = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(clear_color.r, clear_color.g, clear_color.b, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update Camera
		double deltaTime = glfwGetTime() - time;
		time = glfwGetTime();

		// light rotations and update cameras
		CVK::Light *light = nullptr;
		for (size_t i = 0; i < CVK::State::getInstance()->getLights()->size(); i++) {
			light = &CVK::State::getInstance()->getLights()->at(i);
			if (light->castsShadow())
				light->getLightCamera()->update(deltaTime);
		}

		// scene animations
		std::vector<CVK::Node*> *nodes = scene_node_dyn_mid->getChildren();
		for (int i = 0; i < nodes->size(); i++) {
			if (*(*nodes)[i]->getName() == "Dynamic midAperature")
				(*nodes)[i]->setModelMatrix(
					glm::rotate(*(*nodes)[i]->getModelMatrix(), glm::radians(20.0f * (float)deltaTime), glm::vec3(0.0f, 1.0f, 0.0f))
				);
			if (*(*nodes)[i]->getName() == "Cerberus Gun")
				(*nodes)[i]->setModelMatrix(
					glm::translate(
						glm::rotate(*(*nodes)[i]->getModelMatrix(), glm::radians(-20.0f * (float)deltaTime), glm::vec3(0.0f, 1.0f, 0.0f)),
						glm::vec3(0.0f, glm::sin(time) / 64.0f, 0.0f))
				);
			if (*(*nodes)[i]->getName() == "Box")
				(*nodes)[i]->setModelMatrix(
					glm::rotate(
						glm::translate(
							glm::mat4(1.0f), 
							glm::vec3(43.0f, glm::sin(time) / 2.0f, 28.5f)
						),
						glm::radians(-20.0f * (float)time), 
						glm::vec3(0.0f, 1.0f, 0.0f)
					)
				);
			if (*(*nodes)[i]->getName() == "Chest")
				(*nodes)[i]->setModelMatrix(
					glm::scale(
						glm::rotate(
							glm::translate(
								glm::mat4(1.0f),
								glm::vec3(-43.0f, glm::sin(time) / 2.0f + 6.0f, -29.0f)
							),
							glm::radians(-20.0f * (float)time),
							glm::vec3(0.0f, 1.0f, 0.0f)
						),
						glm::vec3(2.0f, 2.0f, 2.0f)
					)
				);
			if (*(*nodes)[i]->getName() == "Marble")
				(*nodes)[i]->setModelMatrix(
					glm::rotate(
						glm::translate(
							glm::mat4(1.0f),
							glm::vec3(-43.0f, glm::sin(time) / 2.0f, 29.0f)
						),
						glm::radians(-20.0f * (float)time),
						glm::vec3(0.0f, 1.0f, 0.0f)
					)
				);
		}

		cam_pilot->update(deltaTime);

		render_scene(scene_combined, &pbrShader, &skyBoxShader2);

		//skyBoxShader.update();
		//skyBoxShader.render();
		//
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	clean_up();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}