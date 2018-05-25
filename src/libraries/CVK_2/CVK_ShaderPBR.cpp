#include "CVK_ShaderPBR.h"

#include <sstream>

CVK::ShaderPBR::ShaderPBR(GLuint shader_mask, const char** shaderPaths) : CVK::ShaderMinimal(shader_mask,shaderPaths)
{
	// camera uniforms
	m_camPosID = glGetUniformLocation(m_ProgramID, "camPos");

	// material uniforms
	m_albedoID = glGetUniformLocation(m_ProgramID, "albedo");
	m_metallicID = glGetUniformLocation(m_ProgramID, "metallic");
	m_roughnessID = glGetUniformLocation(m_ProgramID, "roughness");
	m_aoID = glGetUniformLocation(m_ProgramID, "ao");

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

void CVK::ShaderPBR::update()
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

void CVK::ShaderPBR::update( CVK::Node* node)
{

	if( node->hasMaterial())
	{
		CVK::Material* mat = node->getMaterial();
		//CVK::Texture *color_texture;

		glUniform3fv(m_albedoID, 1, glm::value_ptr( *mat->getdiffColor()));
		glUniform1f(m_metallicID, mat->getMetallic());
		glUniform1f(m_roughnessID, mat->getRoughness());
		glUniform1f(m_aoID, mat->getAO());

		//bool colorTexture = mat->hasTexture(COLOR_TEXTURE);
		//glUniform1i( m_useColorTexture, colorTexture);

		//if (colorTexture)
		//{	
		//	//TODO: COLOR_TEXTURE_UNIT
		//	glUniform1i( m_colorTextureID, 0);

		//	glActiveTexture(COLOR_TEXTURE_UNIT);
		//	color_texture = mat->getTexture(COLOR_TEXTURE);
		//	color_texture->bind();
		//}
	}
}
