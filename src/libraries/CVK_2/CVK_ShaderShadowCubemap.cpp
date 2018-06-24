#include "CVK_ShaderShadowCubemap.h"

#include <sstream>

CVK::ShaderShadowCubemap::ShaderShadowCubemap(GLuint shader_mask, const char** shaderPaths) : CVK::ShaderMinimal(shader_mask,shaderPaths)
{
	std::stringstream uniformString;
	for (int i = 0; i < 6; i++)
	{
		uniformString.str(""); uniformString << "shadowMatrices[" << i << "]";
		m_shadowMatricesID[i] = glGetUniformLocation(m_ProgramID, uniformString.str().c_str());
	}

	m_lightPosID = glGetUniformLocation(m_ProgramID, "lightPos");
	m_farPlaneID = glGetUniformLocation(m_ProgramID, "farPlane");
}

void CVK::ShaderShadowCubemap::update(CVK::Light *light)
{
	CVK::ShaderMinimal::update();

	glm::vec3 lightPos = glm::vec3(*light->getPosition());
	glm::mat4 *shadowProj = light->getLightCamera()->getProjection()->getProjMatrix();
	
	std::vector<glm::mat4> shadowTransformMatrices;
	shadowTransformMatrices.push_back(*shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransformMatrices.push_back(*shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	shadowTransformMatrices.push_back(*shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f)));
	shadowTransformMatrices.push_back(*shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, -1.0f)));

	shadowTransformMatrices.push_back(*shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransformMatrices.push_back(*shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	glUniform3fv(m_lightPosID, 1, glm::value_ptr(lightPos));
	glUniform1f(m_farPlaneID, 50.0f);

	for (int i = 0; i < 6; i++)
	{
		glUniformMatrix4fv(m_shadowMatricesID[i], 1, GL_FALSE, glm::value_ptr(shadowTransformMatrices[i]));
	}

}

void CVK::ShaderShadowCubemap::update( CVK::Node* node)
{


}