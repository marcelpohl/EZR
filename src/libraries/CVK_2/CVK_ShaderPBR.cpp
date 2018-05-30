#include "CVK_ShaderPBR.h"

#include <sstream>

CVK::ShaderPBR::ShaderPBR(GLuint shader_mask, const char** shaderPaths) : CVK::ShaderMinimal(shader_mask, shaderPaths)
{
	// camera uniforms
	m_camPosID = glGetUniformLocation(m_ProgramID, "u_cameraPosition");

	// material textures uniforms
	m_diffuseMapID = glGetUniformLocation(m_ProgramID, "u_DiffuseMap");
	m_normalMapID = glGetUniformLocation(m_ProgramID, "u_NormalMap");
	m_metallicMapID = glGetUniformLocation(m_ProgramID, "u_MetallicMap");
	m_roughnessMapID = glGetUniformLocation(m_ProgramID, "u_RoughnessMap");
	m_aoMapID = glGetUniformLocation(m_ProgramID, "u_AOMap");

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

	m_displayMode = 0;
	m_displayModeID = glGetUniformLocation(m_ProgramID, "displayMode");
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

	glUniform1i(m_displayModeID, m_displayMode);
}

void CVK::ShaderPBR::update(CVK::Node* node)
{

	if (node->hasMaterial())
	{
		CVK::Material* mat = node->getMaterial();
		CVK::Texture *texture;

		if (mat->hasTexture(COLOR_TEXTURE))
		{
			glUniform1i(m_diffuseMapID, 0);
			glActiveTexture(COLOR_TEXTURE_UNIT);
			texture = mat->getTexture(COLOR_TEXTURE);
			texture->bind();
		}
		if (mat->hasTexture(NORMAL_TEXTURE))
		{
			glUniform1i(m_normalMapID, 1);
			glActiveTexture(NORMAL_TEXTURE_UNIT);
			texture = mat->getTexture(NORMAL_TEXTURE);
			texture->bind();
		}
		if (mat->hasTexture(METALLIC_TEXTURE))
		{
			glUniform1i(m_metallicMapID, 4);
			glActiveTexture(METALLIC_TEXTURE_UNIT);
			texture = mat->getTexture(METALLIC_TEXTURE);
			texture->bind();
		}
		if (mat->hasTexture(ROUGHNESS_TEXTURE))
		{
			glUniform1i(m_roughnessMapID, 5);
			glActiveTexture(ROUGHNESS_TEXTURE_UNIT);
			texture = mat->getTexture(ROUGHNESS_TEXTURE);
			texture->bind();
		}
		if (mat->hasTexture(AO_TEXTURE))
		{
			glUniform1i(m_aoMapID, 6);
			glActiveTexture(AO_TEXTURE_UNIT);
			texture = mat->getTexture(AO_TEXTURE);
			texture->bind();
		}
	}
}

void CVK::ShaderPBR::setDisplayMode(int type)
{
	m_displayMode = type;
}