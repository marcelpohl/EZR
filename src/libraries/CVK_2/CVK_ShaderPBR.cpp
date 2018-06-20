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
	m_lightSSBOID = GL_INVALID_VALUE;

	m_lightTransformMatrixID = glGetUniformLocation(m_ProgramID, "lightTransformMatrix");
	m_shadowMapID = glGetUniformLocation(m_ProgramID, "u_shadowMap");

	m_lightViewportMatrix = glm::mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	m_displayMode = 0;
	m_displayModeID = glGetUniformLocation(m_ProgramID, "displayMode");
}

void CVK::ShaderPBR::update()
{

	CVK::ShaderMinimal::update();

	updateLights();

	glm::mat4 lightTransformMatrix = m_lightViewportMatrix * m_lightProjMatrix * m_lightViewMatrix;
	glUniformMatrix4fv(m_lightTransformMatrixID, 1, GL_FALSE, glm::value_ptr(lightTransformMatrix));
	glUniform1i(m_shadowMapID, 2);
	glActiveTexture(SHADOW_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, m_textures[0]);
	
	glm::vec3 camPos = CVK::State::getInstance()->getCamera()->getPosition();
	glUniform3fv(m_camPosID, 1, glm::value_ptr(camPos));

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

void CVK::ShaderPBR::setLightViewMatrix(glm::mat4 *m)
{
	m_lightViewMatrix = *m;
}

void CVK::ShaderPBR::setLightProjMatrix(glm::mat4 *m)
{
	m_lightProjMatrix = *m;
}

void CVK::ShaderPBR::updateLights()
{
	int numLights = CVK::State::getInstance()->getLights()->size();
	glUniform1i(m_numLightsID, numLights);

	if (m_lightSSBOID == GL_INVALID_VALUE)
	{
		for (int i = 0; i < numLights; i++)
			m_lightSSBO.push_back(lightSSBO());

		glGenBuffers(1, &m_lightSSBOID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_lightSSBOID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, m_lightSSBO.size() * sizeof(lightSSBO), m_lightSSBO.data(), GL_STATIC_COPY);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
	
	for (auto i = 0; i < numLights; i++)
	{
		CVK::Light *light = &CVK::State::getInstance()->getLights()->at(i);
		m_lightSSBO[i].position = glm::vec3(*light->getPosition());
		m_lightSSBO[i].color = *light->getColor();
		m_lightSSBO[i].castShadow = light->castsShadow();
		m_lightSSBO[i].directional = light->isDirectional();
	}
	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_lightSSBOID);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, m_lightSSBO.size() * sizeof(lightSSBO), m_lightSSBO.data());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_lightSSBOID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	
}
