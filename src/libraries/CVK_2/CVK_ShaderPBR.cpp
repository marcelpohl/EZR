#include "CVK_ShaderPBR.h"

#include <sstream>

CVK::ShaderPBR::ShaderPBR(GLuint shader_mask, const char** shaderPaths) : CVK::ShaderMinimal(shader_mask, shaderPaths)
{
	// camera uniforms
	m_camPosID = glGetUniformLocation(m_ProgramID, "camPos");

	// material textures uniforms
	m_albedoMapID = glGetUniformLocation(m_ProgramID, "albedoMap");
	m_normalMapID = glGetUniformLocation(m_ProgramID, "normalMap");
	m_metallicMapID = glGetUniformLocation(m_ProgramID, "metallicMap");
	m_roughnessMapID = glGetUniformLocation(m_ProgramID, "roughnessMap");
	m_aoMapID = glGetUniformLocation(m_ProgramID, "aoMap");

	// light uniforms
	std::stringstream uniformString;
	m_numLightsID = glGetUniformLocation(m_ProgramID, "numLights");
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

	glUniform1i(m_numLightsID, numLights);
	for (auto i = 0; i < numLights; i++)
	{
		CVK::Light *light = &CVK::State::getInstance()->getLights()->at(i);
		glUniform3fv(m_lightPositionsID[i], 1, glm::value_ptr(*light->getPosition()));
		glUniform3fv(m_lightColorsID[i], 1, glm::value_ptr(*light->getColor()));
	}
}

void CVK::ShaderPBR::update(CVK::Node* node)
{
	if (node->hasMaterial())
	{
		CVK::Material* mat = node->getMaterial();
		CVK::Texture *texture;

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