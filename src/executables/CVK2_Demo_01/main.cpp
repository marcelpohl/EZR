#include <CVK_2/CVK_Framework.h>

#define WIDTH 512
#define HEIGHT 512

#define PLIGHT 0 //Point light
#define DLIGHT 1 //Directional Light
#define SLIGHT 2 //Spot Light

GLFWwindow* window = nullptr;

CVK::Node *scene_node = nullptr;

//define Camera (Trackball)
CVK::Perspective projection( glm::radians(60.0f), WIDTH / (float) HEIGHT, 0.1f, 10.f);
CVK::Trackball* cam_trackball;

//define Lights
CVK::Light *plight = nullptr;
CVK::Light *dlight = nullptr;
CVK::Light *slight = nullptr;

//define materials
CVK::Material *mat_bronze_transp = nullptr;
CVK::Material *mat_brick = nullptr;
CVK::Material *mat_cvlogo = nullptr;
CVK::Material *mat_red = nullptr;

int lmode = PLIGHT;
float shininess = 100.0f;
float alpha = 0.1f;

void PrintInfo()
{
	printf("w/W:	Wireframe/Fill\n");
	printf("1:		Point Light\n");
	printf("2:		Directional Light\n");
	printf("3:		Spot Light\n");
	printf("+/-:	decrease/increase Transparency\n\n");
}

void charCallback (GLFWwindow *window, unsigned int key)
{
	switch (key)
	{
		case 'W':
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			break;
		case 'w':
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		case '1':
			lmode = PLIGHT;
			break;
		case '2':
			lmode = DLIGHT;
			break;
		case '3':
			lmode = SLIGHT;
			break;
		case '+':
			alpha += 0.02f;
			if (alpha > 1)
				alpha = 1;
			mat_bronze_transp->setKt( alpha);
			break;
		case '-':
			alpha -= 0.02f;
			if (alpha < 0)
				alpha = 0;	
			mat_bronze_transp->setKt( alpha);
			break;
	}
}

void resizeCallback( GLFWwindow *window, int w, int h)
{
	cam_trackball->setWidthHeight(w, h);
	cam_trackball->getProjection()->updateRatio(w / (float) h);
	glViewport( 0, 0, w, h);
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
	plight = new CVK::Light( glm::vec4( -1, 1, 1, 1), GREY, glm::vec3( 0, 0, 0), 1.0f, 0.0f);
	dlight = new CVK::Light( glm::vec4( -1, 1, 1, 0), GREY, glm::vec3( 0, 0, 0), 1.0f, 0.0f);
	slight = new CVK::Light( glm::vec4( -1, 1, -1, 1), DARKGREY, glm::vec3(-1, -1, -1), 5.0f, glm::radians(35.0f));
	CVK::State::getInstance()->addLight( plight);
	CVK::State::getInstance()->updateSceneSettings( DARKGREY, FOG_LINEAR, WHITE, 1, 10, 1);
}

void init_materials()
{
	mat_bronze_transp = new CVK::Material( BRONZE, WHITE, shininess);
	mat_bronze_transp->setKt( alpha);
	mat_brick = new CVK::Material( RESOURCES_PATH "/brick.bmp", 1.f, 0.2f, WHITE, shininess);
	mat_cvlogo = new CVK::Material( RESOURCES_PATH "/cv_logo.bmp", 1.f, 0.75, WHITE, shininess);
	mat_red = new CVK::Material(RED, WHITE, shininess);
}

void init_scene()
{
	scene_node = new CVK::Node( "Scene");

	CVK::Teapot *teapot = new CVK::Teapot(); 

	CVK::Node *teapot_node_up = new CVK::Node("Teapot_up");
	teapot_node_up->setModelMatrix( glm::translate(glm::mat4( 1.0f), glm::vec3( 0, 0.72, 0)));
	teapot_node_up->setMaterial( mat_cvlogo);
	teapot_node_up->setGeometry( teapot);
	scene_node->addChild( teapot_node_up);
	CVK::Node *teapot_node_down = new CVK::Node("Teapot_down");
	teapot_node_down->setModelMatrix( glm::rotate( glm::translate(glm::mat4( 1.0f), glm::vec3( 0, -0.72, 0)), glm::radians(180.0f), glm::vec3( 1, 0, 0)));
	teapot_node_down->setMaterial( mat_cvlogo);
	teapot_node_down->setGeometry( teapot);
	scene_node->addChild( teapot_node_down);

	CVK::Sphere *sphere = new CVK::Sphere( 0.3f);
	
	CVK::Node *sphere_node_up = new CVK::Node("sphere_up");
	sphere_node_up->setModelMatrix(glm::translate(glm::mat4( 1.0f), glm::vec3( 2, 0.4, -2)));
	sphere_node_up->setMaterial( mat_brick);
	sphere_node_up->setGeometry( sphere);
	scene_node->addChild( sphere_node_up);
	CVK::Node *sphere_node_down = new CVK::Node("sphere_down");
	sphere_node_down->setModelMatrix( glm::rotate( glm::translate(glm::mat4( 1.0f), glm::vec3( 2,-0.4, -2)), glm::radians(180.0f), glm::vec3( 1, 0, 0)));
	sphere_node_down->setMaterial( mat_brick);
	sphere_node_down->setGeometry( sphere);
	scene_node->addChild( sphere_node_down);

	CVK::Cone *cone = new CVK::Cone( 0.2f, 0.2f, 0.5f, 20);
	
	CVK::Node *cone_node_up = new CVK::Node("cone_up");
	cone_node_up->setModelMatrix( glm::translate(glm::mat4( 1.0f), glm::vec3( -2, 0, -2)));
	cone_node_up->setMaterial( mat_red);
	cone_node_up->setGeometry( cone);
	scene_node->addChild( cone_node_up);
	CVK::Node *cone_node_down = new CVK::Node("cone_down");
	cone_node_down->setModelMatrix( glm::rotate( glm::translate(glm::mat4( 1.0f), glm::vec3( -2, 0, -2)), glm::radians(180.0f), glm::vec3( 1, 0, 0)));
	cone_node_down->setMaterial( mat_red);
	cone_node_down->setGeometry( cone);
	scene_node->addChild( cone_node_down);
	
	CVK::Plane *plane = new CVK::Plane();
	
	CVK::Node *plane_node_up = new CVK::Node("plane_up");
	plane_node_up->setModelMatrix( glm::scale( glm::mat4( 1.0f), glm::vec3( 5, 5, 5)));
	plane_node_up->setMaterial( mat_bronze_transp);
	plane_node_up->setGeometry( plane);
	scene_node->addChild( plane_node_up);
}

void clean_up() {
  delete cam_trackball;
}

int main() 
{
	PrintInfo();

	// Init GLFW and GLEW
	glfwInit();
	CVK::useOpenGL33CoreProfile();
	window = glfwCreateWindow(WIDTH, HEIGHT, "CVK_2 Demo_01", nullptr, nullptr);
	glfwSetWindowPos( window, 100, 50);
	glfwMakeContextCurrent(window);
	glfwSetCharCallback (window, charCallback);
	glfwSetWindowSizeCallback( window, resizeCallback);
	glfwSwapInterval(1); // vsync
	glewInit();

	CVK::State::getInstance()->setBackgroundColor(WHITE);
	glm::vec3 BgCol = CVK::State::getInstance()->getBackgroundColor();
	glClearColor( BgCol.r, BgCol.g, BgCol.b, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);         
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

	// Load, compile and link Shader
	const char *shadernames[2] = {SHADERS_PATH "/CVK2_Demos/Phong.vert", SHADERS_PATH "/CVK2_Demos/Phong.frag"};
	CVK::ShaderPhong phongShader( VERTEX_SHADER_BIT|FRAGMENT_SHADER_BIT, shadernames);
	CVK::State::getInstance()->setShader( &phongShader);

	init_camera();
	init_lights();
	init_materials();
	init_scene();
	
	double time = glfwGetTime();
	while(!glfwWindowShouldClose( window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update Camera
		double deltaTime = glfwGetTime() - time;
		time = glfwGetTime();
		cam_trackball->update(deltaTime);

		// Use Shader and define camera uniforms
		phongShader.update();

		// Define Light uniforms
		if (lmode == PLIGHT)
			CVK::State::getInstance()->setLight( 0, plight);
		else if (lmode == DLIGHT)
			CVK::State::getInstance()->setLight( 0, dlight);
		else if (lmode == SLIGHT)
			CVK::State::getInstance()->setLight( 0, slight);

		scene_node->render();

		glfwSwapBuffers( window);
		glfwPollEvents();
	}

	clean_up();

	glfwDestroyWindow( window);
	glfwTerminate();
	return 0;
}

