#include "CVK_Environment.h"

CVK::Environment::Environment(CVK::CubeMapTexture* enviromentMap, unsigned int irrMapSize)
{   
	setUp(enviromentMap, irrMapSize);
	computeIrradianceMap();
}

void CVK::Environment::computeIrradianceMap()
{
	// Werte des alten Viewports speichern
	GLint viewportData[4];
	glGetIntegerv(GL_VIEWPORT, &viewportData[0]);

	// Framebuffer binden; setzt auch den Viewport
	m_irradianceFBO->bind();

	// Shader einbinden
	m_irradianceShader->useProgram();

	// Texturen nutzen
	glActiveTexture(GL_TEXTURE0);
	m_enviromentMap->bind();

	// VertexDaten einbinden
	glBindVertexArray(m_skyboxVAO);

	// Debug Clear Farbe
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

	// Tiefentest einstellen
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);

	// Information für Shader setzen
	glUniformMatrix4fv(m_irradianceProjectionID, 1, GL_FALSE, glm::value_ptr(m_skyboxCaptureProjection));

	// Die sechs Seiten des Würfeles "Abtasten"
	for (unsigned int face = 0; face < 6; face++) {
		// Eine Seite der Irradiance Map zum Rendertarget machen
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, m_irradianceMap->getCubeMapTexture(), 0);
		// View laden
		glUniformMatrix4fv(m_irradianceViewID, 1, GL_FALSE, glm::value_ptr(m_skyboxCaptureViews[face]));

		// Seite Rendern
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Aufräumen
	// Normaler Tiefentest
	glDepthFunc(GL_EQUAL);

	// Vertexspeicher unbinden
	glBindVertexArray(0);

	// Standart framebuffer wieder binden
	m_irradianceFBO->unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Viewport zurück zu standart
	glViewport(viewportData[0], viewportData[1], viewportData[2], viewportData[3]);
}

CVK::CubeMapTexture* CVK::Environment::getIrradianceMap()
{
	return m_irradianceMap;
}

void CVK::Environment::setUp(CVK::CubeMapTexture* enviromentMap, unsigned int irrMapSize)
{
	// Irradiance Map dimensionen festlegen
	m_irrMapSize = irrMapSize;

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

	// Shaderladen und anlegen
	const char *shadernames[2] = { SHADERS_PATH "/PBR/Skybox.vert", SHADERS_PATH "/PBR/Irradiance.frag" };
	m_irradianceShader = new ShaderSet(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, shadernames);

	// Uniforms des Shaders finden
	m_irradianceViewID = glGetUniformLocation(m_irradianceShader->getProgramID(), "viewMatrix");
	m_irradianceProjectionID = glGetUniformLocation(m_irradianceShader->getProgramID(), "projectionMatrix");

	//CVK FBO anlegen mit einer Color Texture die gegen die Seitend des Cubs ausgetauscht werden kann.
	m_irradianceFBO = new FBO(m_irrMapSize, m_irrMapSize, 1, true, false);

	// Umgebungstexture einbinden
	m_enviromentMap = enviromentMap;

	// Irradiance Map anlegen
	createCubeMapTextureBuffer(m_irrMapSize, m_irrMapSize, &m_irradianceMap);
}

void CVK::Environment::createCubeMapTextureBuffer(unsigned int sizeX, unsigned int sizeY, CVK::CubeMapTexture** texture )
{
	// Setup CubeMap
	GLuint map;
	glGenTextures(1, &map);
	glBindTexture(GL_TEXTURE_CUBE_MAP, map);

	// Filtereinstellungen
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	GLint types[6] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	// Speicher reservieren
	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(types[i], 0, GL_RGB16F, sizeX, sizeY, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	// CubeMap in CubeMapTextureObject kapseln
	if (*texture != nullptr)
		delete[] *texture;

	*texture = new CVK::CubeMapTexture(map);
}