#include "CVK_Environment.h"

CVK::Environment::Environment(CVK::CubeMapTexture* enviromentMap, unsigned int irrMapSize, unsigned int preFilMapSize, unsigned int preFilLevel, unsigned int LUTBRDFMapSize)
{   
	setEnviroment(enviromentMap);

	setIrradianceMapSize(irrMapSize);
	setPreFilteredEnvironmentMapSize(preFilMapSize);
	setPreFilteredEnvironmentLevels(preFilLevel);
	setLUTBRDFMSize(LUTBRDFMapSize);

	setUp();
}

CVK::Environment::Environment(glm::vec3 color, unsigned int irrMapSize, unsigned int preFilMapSize, unsigned int preFilLevel, unsigned int LUTBRDFMapSize)
{
	setUniformEnviroment(color);

	setIrradianceMapSize(irrMapSize);
	setPreFilteredEnvironmentMapSize(preFilMapSize);
	setPreFilteredEnvironmentLevels(preFilLevel);
	setLUTBRDFMSize(LUTBRDFMapSize);

	setUp();
}

CVK::Environment::~Environment()
{
	if (m_irradianceMap != nullptr)
		delete[] m_irradianceMap;

	if (m_preFilteredMap != nullptr)
		delete[] m_preFilteredMap;

	if (m_LUTBRDF != nullptr)
		delete[] m_LUTBRDF;
}

void CVK::Environment::computeMaps()
{
	setUpTextures();

	computeIrradianceMap();
	computePreFilteredEnvironmentMap();
	computeLUTBRDFMap();
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

void CVK::Environment::setEnviroment(CVK::CubeMapTexture* enviromentMap)
{
	m_enviromentMap = enviromentMap;
}

void CVK::Environment::setUniformEnviroment(glm::vec3 color)
{
	m_color = color;

	m_enviromentMap = nullptr;
}

void CVK::Environment::renderSceneToEnvironmentMap(CVK::Node& scene, CVK::ShaderMinimal& shader, CVK::ShaderCubeMap& skyBox, unsigned int passes, unsigned int resolution, glm::vec3 worldPos)
{
	// EnvironmentMap anlegen
	createCubeMapTextureBuffer(resolution, resolution, &m_enviromentMap, false);

	// Werte des alten Viewports speichern
	GLint viewportData[4];
	glGetIntegerv(GL_VIEWPORT, &viewportData[0]);

	// Framebuffer größe festlegen
	m_preComputeFBO->resize(resolution, resolution);

	// Framebuffer binden; setzt auch den Viewport
	m_preComputeFBO->bind();

	// PBR nutzen um uniforms zu finden
	GLuint pbrViewID = glGetUniformLocation(shader.getProgramID(), "viewMatrix");
	GLuint pbrProjectionID = glGetUniformLocation(shader.getProgramID(), "projectionMatrix");
	GLuint pbrCamPos = glGetUniformLocation(shader.getProgramID(), "u_cameraPosition");

	GLuint skyBoxViewID = glGetUniformLocation(skyBox.getProgramID(), "viewMatrix");
	GLuint skyBoxProjectionID = glGetUniformLocation(skyBox.getProgramID(), "projectionMatrix");

	// Information für Shader setzen
	glUniformMatrix4fv(pbrProjectionID, 1, GL_FALSE, glm::value_ptr(m_skyboxCaptureProjection));
	glUniform3fv(pbrCamPos, 1, glm::value_ptr(worldPos));

	// Debug Color
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	for (unsigned int pass = 0; pass < passes; pass++)
	{
		// Die sechs Seiten des Würfeles "Abtasten"
		for (unsigned int face = 0; face < 6; face++)
		{
			// Eine Seite der Irradiance Map zum Rendertarget machen
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, m_enviromentMap->getCubeMapTexture(), 0);

			// Clear
			glClear(GL_COLOR_BUFFER_BIT);

			// View laden
			//glUniformMatrix4fv(pbrViewID, 1, GL_FALSE, glm::value_ptr(m_skyboxCaptureViews[face]));

			// Szene Rendern
			CVK::State::getInstance()->setShader(&shader);
			//shader.update();
			glUniformMatrix4fv(pbrProjectionID, 1, GL_FALSE, glm::value_ptr(m_skyboxCaptureProjection));
			glUniform3fv(pbrCamPos, 1, glm::value_ptr(worldPos));
			glUniformMatrix4fv(pbrViewID, 1, GL_FALSE, glm::value_ptr(m_skyboxCaptureViews[face]));
			//scene.render();

			//skyBox.update();
			glUniformMatrix4fv(skyBoxProjectionID, 1, GL_FALSE, glm::value_ptr(m_skyboxCaptureProjection));
			glUniformMatrix4fv(skyBoxViewID, 1, GL_FALSE, glm::value_ptr(m_skyboxCaptureViews[face]));
			skyBox.render();
		}
	}

	// Aufräumen
	// Standart framebuffer wieder binden
	m_preComputeFBO->unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Viewport zurück zu standart
	glViewport(viewportData[0], viewportData[1], viewportData[2], viewportData[3]);
}

CVK::CubeMapTexture* CVK::Environment::getEnvironmentMap()
{
	return m_enviromentMap;
}

void CVK::Environment::setIrradianceMapSize(unsigned int size)
{
	m_irrMapSize = size;
}

void CVK::Environment::setPreFilteredEnvironmentMapSize(unsigned int size)
{
	m_preFilMapSize = size;
}

void CVK::Environment::setPreFilteredEnvironmentLevels(unsigned int levels)
{
	m_preFilLevel = levels;
}

void CVK::Environment::setLUTBRDFMSize(unsigned int size)
{
	m_LUTBRDFMapSize = size;
}

unsigned int CVK::Environment::getIrradianceMapSize()
{
	return m_irrMapSize;
}

unsigned int CVK::Environment::getPreFilteredEnvironmentMapSize()
{
	return m_preFilMapSize;
}

unsigned int CVK::Environment::getPreFilteredEnvironmentLevels()
{
	return m_preFilLevel;
}

unsigned int CVK::Environment::getLUTBRDFMSize()
{
	return m_LUTBRDFMapSize;
}

unsigned int CVK::Environment::getAccIrradianceMapSize()
{
	return m_accIrrMapSize;
}

unsigned int CVK::Environment::getAccPreFilteredEnvironmentMapSize()
{
	return m_accPreFilMapSize;
}

unsigned int CVK::Environment::getAccPreFilteredEnvironmentLevels()
{
	return m_accPreFilLevel;
}

unsigned int CVK::Environment::getAccLUTBRDFMSize()
{
	return m_accLUTBRDFMapSize;
}

void CVK::Environment::setUp()
{
	//CVK FBO anlegen mit einer Color Texture die gegen die Seitend des Cubs ausgetauscht werden kann.
	m_preComputeFBO = new FBO(16, 16, 1, true, false);

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
	// Shader laden und anlegen
	const char *irrShadernames[2] = { SHADERS_PATH "/PBR/Skybox.vert", SHADERS_PATH "/PBR/Irradiance.frag" };
	m_irradianceShader = new ShaderSet(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, irrShadernames);

	// Uniforms des Shaders finden
	m_irradianceViewID = glGetUniformLocation(m_irradianceShader->getProgramID(), "viewMatrix");
	m_irradianceProjectionID = glGetUniformLocation(m_irradianceShader->getProgramID(), "projectionMatrix");

	// --- PreFiltered Environement ---
	// Shader laden und anlegen
	const char *preFilShadernames[2] = { SHADERS_PATH "/PBR/Skybox.vert", SHADERS_PATH "/PBR/PreFiltered.frag" };
	m_preFilteredShader = new ShaderSet(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, preFilShadernames);

	// Uniforms finden
	m_preFilteredViewID = glGetUniformLocation(m_preFilteredShader->getProgramID(), "viewMatrix");
	m_preFilteredProjectionID = glGetUniformLocation(m_preFilteredShader->getProgramID(), "projectionMatrix");
	m_roughnessID = glGetUniformLocation(m_preFilteredShader->getProgramID(), "roughness");

	// --- LUT BRDF ---
	// Shader laden und anlegen
	const char *LUTBRDFShadernames[2] = { SHADERS_PATH "/PBR/Quad.vert", SHADERS_PATH "/PBR/Quad.frag" };
	m_LUTBRDFShader = new ShaderSet(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, LUTBRDFShadernames);
}

void CVK::Environment::setUpTextures()
{
	if (m_enviromentMap == nullptr)
	{
		createCubeMapTextureBuffer(16, 16, &m_enviromentMap, false);

		// Clear EnviromentMap
		// Werte des alten Viewports speichern
		GLint viewportData[4];
		glGetIntegerv(GL_VIEWPORT, &viewportData[0]);

		// Framebuffer größe festlegen
		m_preComputeFBO->resize(m_irrMapSize, m_irrMapSize);

		// Framebuffer binden; setzt auch den Viewport
		m_preComputeFBO->bind();

		// Clear Farbe
		glClearColor(m_color.r, m_color.g, m_color.b, 1.0f);

		// Die sechs Seiten des Würfeles "Abtasten"
		for (unsigned int face = 0; face < 6; face++) {
			// Eine Seite der Irradiance Map zum Rendertarget machen
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, m_enviromentMap->getCubeMapTexture(), 0);

			// Seite Rendern
			glClear(GL_COLOR_BUFFER_BIT);
		}

		// Aufräumen
		// Standart framebuffer wieder binden
		m_preComputeFBO->unbind();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//Viewport zurück zu standart
		glViewport(viewportData[0], viewportData[1], viewportData[2], viewportData[3]);
	}

	// Irradiance Map anlegen
	m_accIrrMapSize = m_irrMapSize;
	createCubeMapTextureBuffer(m_irrMapSize, m_irrMapSize, &m_irradianceMap, false);

	// preFilteredMap anlegen
	m_accPreFilMapSize = m_preFilMapSize;
	m_accPreFilLevel = m_preFilLevel;
	createCubeMapTextureBuffer(m_preFilMapSize, m_preFilMapSize, &m_preFilteredMap, true);

	// LUTBRDFMap anlegen
	m_accLUTBRDFMapSize = m_LUTBRDFMapSize;
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
		delete *texture;

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
		delete *texture;

	*texture = new CVK::Texture(map);

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
