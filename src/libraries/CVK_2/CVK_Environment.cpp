#include "CVK_Environment.h"

CVK::Environment::Environment(CVK::CubeMapTexture* enviromentMap, unsigned int irrMapSize, unsigned int preFilMapSize, unsigned int preFilLevel, unsigned int LUTBRDFMapSize)
{   
	setUp(enviromentMap, irrMapSize, preFilMapSize, preFilLevel, LUTBRDFMapSize);
	computeIrradianceMap();
	computePreFilteredEnvironmentMap();
	computeLUTBRDFMap();
}

void CVK::Environment::computeIrradianceMap()
{
	// Werte des alten Viewports speichern
	GLint viewportData[4];
	glGetIntegerv(GL_VIEWPORT, &viewportData[0]);

	// Framebuffer größe festlegen
	m_preComputeFBO->resize(m_irrMapSize, m_irrMapSize);

	// Framebuffer binden; setzt auch den Viewport
	m_preComputeFBO->bind();

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
	glDepthFunc(GL_LESS);

	// Vertexspeicher unbinden
	glBindVertexArray(0);

	// Standart framebuffer wieder binden
	m_preComputeFBO->unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Viewport zurück zu standart
	glViewport(viewportData[0], viewportData[1], viewportData[2], viewportData[3]);
}

void CVK::Environment::computePreFilteredEnvironmentMap()
{
	// Werte des alten Viewports speichern
	GLint viewportData[4];
	glGetIntegerv(GL_VIEWPORT, &viewportData[0]);

	// Shader einbinden
	m_preFilteredShader->useProgram();

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
	glUniformMatrix4fv(m_preFilteredProjectionID, 1, GL_FALSE, glm::value_ptr(m_skyboxCaptureProjection));

	// Mipmaploop
	float roughness = 0.0f;
	float roughnessDelta = 1.0f / static_cast<float>(m_preFilLevel - 1); // Damit der letzte schritt mit roughness = 1 durchgeführt wird
	unsigned int mipmapSize = m_preFilMapSize;
	for (unsigned int level = 0; level < m_preFilLevel; level++)
	{
		// Framebuffer größe festlegen
		m_preComputeFBO->resize(mipmapSize, mipmapSize); // Das die Farbtexturen gelöst werden ist egal die nutzen wir eh nicht! Leider muss der Speicher für diese Texturen aber reserviert werden weil sonst die DrawBuffer nicht stimmen... das evtl. mal umgehen
		// Framebuffer binden; setzt auch den Viewport
		m_preComputeFBO->bind();

		// Rauheit setzten
		glUniform1f(m_roughnessID, roughness);

		// Die sechs Seiten des Würfeles "Abtasten"
		for (unsigned int face = 0; face < 6; face++) {
			// Eine Seite und Level PrefilteredEnviromentMap zum Rendertarget machen
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, m_preFilteredMap->getCubeMapTexture(), level);
			// View laden
			glUniformMatrix4fv(m_preFilteredViewID, 1, GL_FALSE, glm::value_ptr(m_skyboxCaptureViews[face]));

			// Seite Rendern
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Größe der Mipmap ändern
		mipmapSize /= 2;
		// Rauheit ändern
		roughness += roughnessDelta;
	}

	// Aufräumen
	// Normaler Tiefentest
	glDepthFunc(GL_LESS);

	// Vertexspeicher unbinden
	glBindVertexArray(0);

	// Standart framebuffer wieder binden
	m_preComputeFBO->unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Viewport zurück zu standart
	glViewport(viewportData[0], viewportData[1], viewportData[2], viewportData[3]);
}

void CVK::Environment::computeLUTBRDFMap()
{
	// Werte des alten Viewports speichern
	GLint viewportData[4];
	glGetIntegerv(GL_VIEWPORT, &viewportData[0]);

	// Framebuffer größe festlegen
	m_preComputeFBO->resize(m_LUTBRDFMapSize, m_LUTBRDFMapSize);

	// Framebuffer binden; setzt auch den Viewport
	m_preComputeFBO->bind();

	// Shader einbinden
	m_LUTBRDFShader->useProgram();

	// Texturen nutzen
	glActiveTexture(GL_TEXTURE0);
	m_LUTBRDF->bind();

	// VertexDaten einbinden
	glBindVertexArray(m_quadVAO);

	// Debug Clear Farbe
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

	// Tiefentest einstellen
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearDepth(1.0f);

	// Rendern
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_LUTBRDF->getTexture(), 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Aufräumen
	// Vertexspeicher unbinden
	glBindVertexArray(0);

	// Standart framebuffer wieder binden
	m_preComputeFBO->unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Viewport zurück zu standart
	glViewport(viewportData[0], viewportData[1], viewportData[2], viewportData[3]);
}

CVK::CubeMapTexture* CVK::Environment::getIrradianceMap()
{
	return m_irradianceMap;
}

CVK::CubeMapTexture* CVK::Environment::getPreFilteredEnvironmentMap()
{
	return m_preFilteredMap;
}

CVK::Texture* CVK::Environment::getLUTBRDFMap()
{
	return m_LUTBRDF;
}

void CVK::Environment::setUp(CVK::CubeMapTexture* enviromentMap, unsigned int irrMapSize, unsigned int preFilMapSize, unsigned int preFilLevel, unsigned int LUTBRDFMapSize)
{
	// --- Environment ---

	//CVK FBO anlegen mit einer Color Texture die gegen die Seitend des Cubs ausgetauscht werden kann.
	m_preComputeFBO = new FBO(16, 16, 1, true, false);

	// Umgebungstexture einbinden
	m_enviromentMap = enviromentMap;

	// --- Skybox ---

	// Geometrie der Skybox in den Speicher laden
	glGenVertexArrays(1, &m_skyboxVAO);
	glGenBuffers(1, &m_skyboxVBO);

	glBindVertexArray(m_skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyboxVertices), &m_skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(VERTICES);
	glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(VERTICES);

	// --- Quad ---

	// Geometrie des Screenfilling quads in den Speicher laden
	glGenVertexArrays(1, &m_quadVAO);
	glGenBuffers(1, &m_quadVBO);

	glBindVertexArray(m_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_quadVertices), &m_quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(VERTICES);
	glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(TEXTURECOORDS);
	glVertexAttribPointer(TEXTURECOORDS, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(TEXTURECOORDS);

	// --- Irradiance ---

	// Irradiance Map dimensionen festlegen
	m_irrMapSize = irrMapSize;

	// Shader laden und anlegen
	const char *irrShadernames[2] = { SHADERS_PATH "/PBR/Skybox.vert", SHADERS_PATH "/PBR/Irradiance.frag" };
	m_irradianceShader = new ShaderSet(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, irrShadernames);

	// Uniforms des Shaders finden
	m_irradianceViewID = glGetUniformLocation(m_irradianceShader->getProgramID(), "viewMatrix");
	m_irradianceProjectionID = glGetUniformLocation(m_irradianceShader->getProgramID(), "projectionMatrix");

	// Irradiance Map anlegen
	createCubeMapTextureBuffer(m_irrMapSize, m_irrMapSize, &m_irradianceMap, false);

	// --- PreFiltered Environement ---

	// Dimensionen festlegen
	m_preFilMapSize = preFilMapSize;
	m_preFilLevel = preFilLevel;

	// Shader laden und anlegen
	const char *preFilShadernames[2] = { SHADERS_PATH "/PBR/Skybox.vert", SHADERS_PATH "/PBR/PreFiltered.frag" };
	m_preFilteredShader = new ShaderSet(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, preFilShadernames);

	// Uniforms finden
	m_preFilteredViewID = glGetUniformLocation(m_preFilteredShader->getProgramID(), "viewMatrix");
	m_preFilteredProjectionID = glGetUniformLocation(m_preFilteredShader->getProgramID(), "projectionMatrix");
	m_roughnessID = glGetUniformLocation(m_preFilteredShader->getProgramID(), "roughness");

	//preFilteredMap anlegen
	createCubeMapTextureBuffer(m_preFilMapSize, m_preFilMapSize, &m_preFilteredMap, true);

	// --- LUT BRDF ---
	m_LUTBRDFMapSize = LUTBRDFMapSize;

	// Shader laden und anlegen
	const char *LUTBRDFShadernames[2] = { SHADERS_PATH "/PBR/Quad.vert", SHADERS_PATH "/PBR/Quad.frag" };
	m_LUTBRDFShader = new ShaderSet(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, LUTBRDFShadernames);

	// Texture anlegen
	createQuadTextureBuffer(m_LUTBRDFMapSize, m_LUTBRDFMapSize, &m_LUTBRDF);
}

void CVK::Environment::createCubeMapTextureBuffer(unsigned int sizeX, unsigned int sizeY, CVK::CubeMapTexture** texture, bool mipmap)
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

	// Mit oder Mipmap
	if (mipmap)
	{
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Trilinear filtern!
	}

	// CubeMap in CubeMapTextureObject kapseln
	if (*texture != nullptr)
		delete[] *texture;

	*texture = new CVK::CubeMapTexture(map);
}

void CVK::Environment::createQuadTextureBuffer(unsigned int sizeX, unsigned int sizeY, CVK::Texture** texture)
{
	// Setup Texture
	GLuint map;
	glGenTextures(1, &map);
	glBindTexture(GL_TEXTURE_2D, map);

	// Filtereinstellungen
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Wichtig !!!!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Wichtig !!!!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Speicher reservieren
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, sizeX, sizeY, 0, GL_RG, GL_FLOAT, nullptr); // Nur RG !!!!

	// In Object kapseln
	if (*texture != nullptr)
		delete[] * texture;

	*texture = new CVK::Texture(map);

}