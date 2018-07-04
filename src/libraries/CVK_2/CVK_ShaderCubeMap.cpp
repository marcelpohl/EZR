#include "CVK_ShaderCubeMap.h"

CVK::ShaderCubeMap::ShaderCubeMap(GLuint shader_mask, const char** shaderPaths, CVK::CubeMapTexture* cubeMap)
{
	// Texture einbinden
	m_cubeMap = cubeMap;

	// Geometrie der Skybox in den Speicher laden
	glGenVertexArrays(1, &m_skyboxVAO);
	glGenBuffers(1, &m_skyboxVBO);

	glBindVertexArray(m_skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyboxVertices), &m_skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(VERTICES);

	// Shader erstellen
	GenerateShaderProgramm(shader_mask, shaderPaths);

	// Eingänge der Uniform Variablen finden
	m_viewMatrixID = glGetUniformLocation(m_ProgramID, "viewMatrix");
	m_projectionMatrixID = glGetUniformLocation(m_ProgramID, "projectionMatrix");
}

CVK::ShaderCubeMap::~ShaderCubeMap()
{
	if (m_cubeMap != nullptr)
		delete m_cubeMap;
}

void CVK::ShaderCubeMap::render()
{
	glDepthFunc(GL_LEQUAL);

	glBindVertexArray(m_skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	m_cubeMap->bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
}

void CVK::ShaderCubeMap::update()
{
	useProgram();

	CVK::Camera* cam = CVK::State::getInstance()->getCamera();
	glm::mat4 projection, viewmatrix;

	if (cam != nullptr)
	{
		viewmatrix = *cam->getView();
		projection = *cam->getProjection()->getProjMatrix();
	}

	// Ohne verschiebung
	viewmatrix = glm::mat4(glm::mat3(viewmatrix));

	glUniformMatrix4fv(m_viewMatrixID, 1, GL_FALSE, glm::value_ptr(viewmatrix));
	glUniformMatrix4fv(m_projectionMatrixID, 1, GL_FALSE, glm::value_ptr(projection));
}