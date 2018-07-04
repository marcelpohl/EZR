#include "CVK_Light.h"

// parameters like in OpenGL 1.1 (s. spec)
CVK::Light::Light()
{
	m_position = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f);
	m_color = glm::vec3( 1.0f, 1.0f, 1.0f);
	m_spotDirection = glm::vec3( 0.0f, 1.0f, 0.0f);;
	m_spotExponent = 1.0f;
	m_spotCutoff = 0.0f;
}

CVK::Light::Light( glm::vec4 pos, glm::vec3 col, glm::vec3 s_dir, float s_exp, float s_cut)
{
	m_position = pos;
	m_color = col;
	m_spotDirection = s_dir;
	m_spotExponent = s_exp;
	m_spotCutoff = s_cut;
}

CVK::Light::~Light()
{
	//delete m_shadowMapFBO;
	//delete m_lightCamera;
	//delete m_projection;
}

void CVK::Light::setPosition( glm::vec4 pos)
{
	m_position = pos;
} 

glm::vec4 *CVK::Light::getPosition()
{
	return &m_position;
} 

void CVK::Light::setColor( glm::vec3 col)
{
	m_color = col;
} 

glm::vec3 *CVK::Light::getColor()
{
	return &m_color;
} 
	
void CVK::Light::setSpotDirection( glm::vec3 direction)
{
	m_spotDirection = direction;
} 

glm::vec3 *CVK::Light::getSpotDirection() 
{
	return &m_spotDirection;
}

void CVK::Light::setSpotExponent( float spotExponent) 
{
	m_spotExponent = spotExponent;
}

float CVK::Light::getSpotExponent() const
{
	return m_spotExponent;
}

void CVK::Light::setSpotCutoff( float spotCutoff) 
{
	m_spotCutoff = spotCutoff;
}

float CVK::Light::getSpotCutoff() const
{
	return m_spotCutoff;
}

CVK::CameraSimple* CVK::Light::getLightCamera()
{
	return m_lightCamera;
}

void CVK::Light::setCastShadow(bool b, GLFWwindow* window)
{
	m_castShadow = b;

	if (m_castShadow == true)
	{
		if (m_shadowMapFBO == nullptr)
		{
			switch (m_lightType)
			{
			case 0:
				m_shadowMapFBO = new CVK::FBO(m_shadowWidth, m_shadowHeight, 0, true);
				break;
			case 1:
				m_shadowMapFBO = new CVK::FBO(m_shadowWidth, m_shadowHeight, 0, false, false, true);
				break;
			}
		}

		if (m_lightCamera == nullptr)
		{
			m_projection = new CVK::Perspective(glm::radians(90.0f), m_shadowWidth / (float)m_shadowHeight, 0.1f, 500.f);
			m_lightCamera = new CVK::CameraSimple(window, m_shadowWidth, m_shadowHeight, m_projection);

			glm::vec3 *lightPos = new glm::vec3(m_position);
			m_lightCamera->setCamPos(lightPos);
			m_lightCamera->setLookAt(lightPos, &m_spotDirection);
		}
	}

}

bool CVK::Light::castsShadow()
{
	return m_castShadow;
}

void CVK::Light::setType(int type)
{
	if (type != 0 && type != 1)
		return;

	m_lightType = type;

	if (m_castShadow)
	{
		if (m_shadowMapFBO != nullptr)
			delete m_shadowMapFBO;

		switch (m_lightType)
		{
		case 0:
			m_shadowMapFBO = new CVK::FBO(m_shadowWidth, m_shadowHeight, 0, true);
			break;
		case 1:
			m_shadowMapFBO = new CVK::FBO(m_shadowWidth, m_shadowHeight, 0, false, false, true);
			break;
		}
	}
}

int CVK::Light::getType()
{
	return m_lightType;
}


glm::mat4* CVK::Light::getLightVPMatrix()
{
	m_lightVPMatrix =
		*m_lightCamera->getProjection()->getProjMatrix() *
		*m_lightCamera->getView();
	return &m_lightVPMatrix;
}

bool CVK::Light::prepareRenderShadowMap()
{
	if (m_castShadow)
	{
		m_shadowMapFBO->bind();

		glViewport(0, 0, m_shadowWidth, m_shadowHeight);
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::vec3 *lightPos = new glm::vec3(m_position);
		m_lightCamera->setCamPos(lightPos);
		m_lightCamera->setLookAt(lightPos, &m_spotDirection);

		return true;
	}

	return false;
}

void CVK::Light::finishRenderShadowMap()
{
	if (m_castShadow) 
		m_shadowMapFBO->unbind();
}

GLuint CVK::Light::getShadowMap()
{
	if (m_lightType == 0)
		return m_shadowMapFBO->getDepthTexture();
	else
		return m_shadowMapFBO->getDepthCubemapTexture();
}