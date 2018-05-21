#include "CVK_State.h"
#include "CVK_ShaderMinimal.h"

CVK::State* CVK::State::g_instance = nullptr;

CVK::State::State()
{
	m_BackgroundColor = SKYBLUE;
}

CVK::State::~State()
{
}

CVK::State* CVK::State::getInstance()
{
	if( g_instance == nullptr)
	{	
		g_instance = new CVK::State();
	}
	return g_instance;
}

void CVK::State::setShader( CVK::ShaderMinimal* shader)
{
	m_shader = shader;
	shader->useProgram();
}

CVK::ShaderMinimal* CVK::State::getShader() const
{
	return m_shader;
}

void CVK::State::setCubeMapTexture( CVK::CubeMapTexture* cubeMap)
{
	m_cubeMap = cubeMap;
}

CVK::CubeMapTexture* CVK::State::getCubeMapTexture() const
{
	return m_cubeMap;
}

void CVK::State::addLight( CVK::Light* light)
{
	m_lights.push_back(*light);
}

void CVK::State::setLight(unsigned int index, CVK::Light* light)
{
	if (index < m_lights.size())
		m_lights[index] = *light;
}

void CVK::State::removeLight(unsigned int indexToRemove)
{
	if (indexToRemove < m_lights.size())
		m_lights.erase(m_lights.begin() + indexToRemove);
}

std::vector<CVK::Light>* CVK::State::getLights()
{
	return &m_lights;
}

void CVK::State::setCamera( CVK::Camera* camera)
{
	m_camera = camera;
}

CVK::Camera* CVK::State::getCamera() const
{
	return m_camera;
}

void CVK::State::updateSceneSettings( glm::vec3 lightAmbient, int fogMode, glm::vec3 fogCol, float fogStart, float fogEnd, float fogDens)
{
	m_lightAmbient = lightAmbient;
	m_fogMode = fogMode;
	m_fogCol = fogCol;
	m_fogStart = fogStart;
	m_fogEnd = fogEnd;
	m_fogDens = fogDens;
}

glm::vec3 CVK::State::getLightAmbient() const
{
	return m_lightAmbient;
}

void CVK::State::setBackgroundColor(glm::vec3 color)
{
	m_BackgroundColor = color;
}

glm::vec3 CVK::State::getBackgroundColor() const
{
	return m_BackgroundColor;
}

int CVK::State::getFogMode() const
{
	return m_fogMode;
}

glm::vec3 CVK::State::getFogCol() const
{
	return m_fogCol;
}

float CVK::State::getFogStart() const
{
	return m_fogStart;
}

float CVK::State::getFogEnd() const
{
	return m_fogEnd;
}

float CVK::State::getFogDens() const
{
	return m_fogDens;
}
