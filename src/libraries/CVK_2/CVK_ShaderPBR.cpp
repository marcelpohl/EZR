#include "CVK_ShaderPBR.h"

#include <sstream>

CVK::ShaderPBR::ShaderPBR(GLuint shader_mask, const char** shaderPaths, bool useTextures) : CVK::ShaderMinimal(shader_mask,shaderPaths)
{
	// camera uniforms
	m_camPosID = glGetUniformLocation(m_ProgramID, "camPos");

	// material uniforms
	m_albedoID = glGetUniformLocation(m_ProgramID, "uAlbedo");
	m_metallicID = glGetUniformLocation(m_ProgramID, "uMetallic");
	m_roughnessID = glGetUniformLocation(m_ProgramID, "uRoughness");
	m_aoID = glGetUniformLocation(m_ProgramID, "uAo");

	// material textures uniforms
	m_albedoMapID = glGetUniformLocation(m_ProgramID, "albedoMap");
	m_normalMapID = glGetUniformLocation(m_ProgramID, "normalMap");
	m_metallicMapID = glGetUniformLocation(m_ProgramID, "metallicMap");
	m_roughnessMapID = glGetUniformLocation(m_ProgramID, "roughnessMap");
	m_aoMapID = glGetUniformLocation(m_ProgramID, "aoMap");

	// switcher between global values and textures
	m_useTexturesID = glGetUniformLocation(m_ProgramID, "useTextures");

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

	m_useTextures = useTextures;

}

void CVK::ShaderPBR::update()
{
	int numLights = CVK::State::getInstance()->getLights()->size();
	CVK::ShaderMinimal::update();

	glm::vec3 camPos = CVK::State::getInstance()->getCamera()->getPosition();
	glUniform3fv(m_camPosID, 1, glm::value_ptr(camPos));

	if (m_useTextures)
	{
		glUniform1i(m_useTexturesID, 1);
	}
	else 
	{
		glUniform1i(m_useTexturesID, 0);
	}

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
		CVK::Texture *texture;

		glUniform3fv(m_albedoID, 1, glm::value_ptr(*mat->getdiffColor()));
		glUniform1f(m_metallicID, mat->getMetallic());
		glUniform1f(m_roughnessID, mat->getRoughness());
		glUniform1f(m_aoID, mat->getAO());

		if (m_useTextures)
		{
			if (mat->hasTexture(COLOR_TEXTURE))
			{
				glActiveTexture(COLOR_TEXTURE_UNIT);
				texture = mat->getTexture(COLOR_TEXTURE);
				texture->bind();
			}
			if (mat->hasTexture(NORMAL_TEXTURE))
			{
				glActiveTexture(NORMAL_TEXTURE_UNIT);
				texture = mat->getTexture(NORMAL_TEXTURE);
				texture->bind();
			}
			if (mat->hasTexture(METALLIC_TEXTURE))
			{
				glActiveTexture(METALLIC_TEXTURE_UNIT);
				texture = mat->getTexture(METALLIC_TEXTURE);
				texture->bind();
			}
			if (mat->hasTexture(ROUGHNESS_TEXTURE))
			{
				glActiveTexture(ROUGHNESS_TEXTURE_UNIT);
				texture = mat->getTexture(ROUGHNESS_TEXTURE);
				texture->bind();
			}
			if (mat->hasTexture(AO_TEXTURE))
			{
				glActiveTexture(AO_TEXTURE_UNIT);
				texture = mat->getTexture(AO_TEXTURE);
				texture->bind();
			}
		}
	}

}

void CVK::ShaderPBR::setUseTextures(bool useTextures)
{
	m_useTextures = useTextures;

	if (useTextures)
	{
		glUniform1i(m_albedoMapID, COLOR_TEXTURE_UNIT);
		glUniform1i(m_normalMapID, NORMAL_TEXTURE_UNIT);
		glUniform1i(m_metallicMapID, METALLIC_TEXTURE_UNIT);
		glUniform1i(m_roughnessMapID, ROUGHNESS_TEXTURE_UNIT);
		glUniform1i(m_aoMapID, AO_TEXTURE_UNIT);
	}

}