#include "CVK_CameraSimple.h"

CVK::CameraSimple::CameraSimple(GLFWwindow* window, int width, int height, CVK::Projection *projection) : Camera(window)
{
	m_cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	m_direction = glm::vec3( 0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);

	m_width = width;
	m_height = height;

	m_viewmatrix = glm::lookAt( m_cameraPos, m_cameraPos + m_direction, m_up);   
 
	m_oldX = width/2.f;
	m_oldY = height/2.f;

	setProjection( projection);
}

CVK::CameraSimple::CameraSimple(GLFWwindow* window, int width, int height) : Camera(window)
{
	m_cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	m_direction = glm::vec3( 0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);

	m_width = width;
	m_height = height;

	m_viewmatrix = glm::lookAt( m_cameraPos, m_cameraPos + m_direction, m_up);
 
	m_oldX = width/2.f;
	m_oldY = height/2.f;

	CVK::Perspective* projection = new CVK::Perspective( width / (float) height);

	setProjection( projection);
}

CVK::CameraSimple::~CameraSimple()
{
}

void CVK::CameraSimple::update(double deltaTime)
{
	m_viewmatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_direction, m_up);
}

void CVK::CameraSimple::setCamPos( glm::vec3 *campos)
{
	m_cameraPos = *campos;
}

void CVK::CameraSimple::setLookAt( glm::vec3 *position, glm::vec3 *center)
{
	m_cameraPos = *position;
	glm::vec3 diff = glm::normalize( *center - *position);
	m_direction = diff;
}

void CVK::CameraSimple::setUpvector( glm::vec3 *up)
{
	m_up = *up;
}
