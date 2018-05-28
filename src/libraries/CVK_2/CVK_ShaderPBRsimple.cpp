#include "CVK_ShaderPBRsimple.h"

#include <sstream>

CVK::ShaderPBRsimple::ShaderPBRsimple(GLuint shader_mask, const char** shaderPaths) : CVK::ShaderMinimal(shader_mask,shaderPaths)
{
	// camera uniforms
	m_camPosID = glGetUniformLocation(m_ProgramID, "camPos");

	// material uniforms
	m_albedoID = glGetUniformLocation(m_ProgramID, "uAlbedo");
	m_metallicID = glGetUniformLocation(m_ProgramID, "uMetallic");
	m_roughnessID = glGetUniformLocation(m_ProgramID, "uRoughness");
	m_aoID = glGetUniformLocation(m_ProgramID, "uAo");

	
	// light uniforms
	std::stringstream uniformString;
	m_numLightsID = glGetUniformLocation( m_ProgramID, "numLights");
	for (auto i = 0; i < MAX_LIGHTS; ++i)
	{
		uniformString.str(""); uniformString << "lightPositions[" << i << "]";
		m_lightPositionsID[i] = glGetUniformLocation(m_ProgramID, uniformString.str().c_str());
		uniformString.str(""); uniformString << "lightColors[" << i << "]";
		m_lightColorsID[i] = glGetUniformLocation(m_ProgramID, uniformString.str().c_str());
	}

}

void CVK::ShaderPBRsimple::update()
{
	int numLights = CVK::State::getInstance()->getLights()->size();
	CVK::ShaderMinimal::update();

	glm::vec3 camPos = CVK::State::getInstance()->getCamera()->getPosition();
	glUniform3fv(m_camPosID, 1, glm::value_ptr(camPos));
	
	glUniform1i( m_numLightsID, numLights);
	for (auto i = 0 ; i < numLights; i++)
	{
		CVK::Light *light = &CVK::State::getInstance()->getLights()->at(i);
		glUniform3fv(m_lightPositionsID[i], 1, glm::value_ptr(*light->getPosition()));
		glUniform3fv(m_lightColorsID[i], 1, glm::value_ptr(*light->getColor()));
	}
}

void CVK::ShaderPBRsimple::update( CVK::Node* node)
{

	if( node->hasMaterial())
	{
		CVK::Material* mat = node->getMaterial();
		CVK::Texture *texture;

		glUniform3fv(m_albedoID, 1, glm::value_ptr(*mat->getdiffColor()));
		glUniform1f(m_metallicID, mat->getMetallic());
		glUniform1f(m_roughnessID, mat->getRoughness());
		glUniform1f(m_aoID, mat->getAO());
	}

}