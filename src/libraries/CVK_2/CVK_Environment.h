#pragma once
#include "CVK_ShaderCubeMap.h"
#include "CVK_Texture.h"
#include "CVK_FBO.h"

//TODO: Pre Filter AntiAlias
//TODO: Precompute BRDF parameters

namespace CVK 
{
class Environment
{
public:
	Environment(CVK::CubeMapTexture* enviromentMap, unsigned int irrMapSize, unsigned int preFilMapSize, unsigned int preFilLevel, unsigned int m_LUTBRDFMapSize);

	void computeIrradianceMap();
	void computePreFilteredEnvironmentMap();
	void computeLUTBRDFMap();

	CVK::CubeMapTexture* getIrradianceMap();
	CVK::CubeMapTexture* getPreFilteredEnvironmentMap();
	CVK::Texture* getLUTBRDFMap();

private:
	void setUp(CVK::CubeMapTexture* enviromentMap, unsigned int irrMapSize, unsigned int preFilMapSize, unsigned int preFilLevel, unsigned int m_LUTBRDFMapSize);

	void createCubeMapTextureBuffer(unsigned int sizeX, unsigned int sizeY, CVK::CubeMapTexture** texture, bool mipmap);
	void createQuadTextureBuffer(unsigned int sizeX, unsigned int sizeY, CVK::Texture** texture);

	CVK::FBO* m_preComputeFBO;

	// --- Environment ---
	CVK::CubeMapTexture* m_enviromentMap = nullptr;

	// --- Irradiance ---
	unsigned int m_irrMapSize;

	CVK::CubeMapTexture* m_irradianceMap = nullptr;

	CVK::ShaderSet* m_irradianceShader = nullptr;

	GLuint m_irradianceViewID; // TODO: rausfinden ob die Positionen sich unterscheiden wenn der Vertex shader der gleiche bleibt ANTWORT: NEIN!!!!!!!!
	GLuint m_irradianceProjectionID;

	// --- PreFilteredEnvironement ---
	unsigned int m_preFilMapSize;
	unsigned int m_preFilLevel;

	CVK::CubeMapTexture* m_preFilteredMap = nullptr;

	CVK::ShaderSet* m_preFilteredShader = nullptr;

	GLuint m_roughnessID;
	GLuint m_preFilteredViewID;
	GLuint m_preFilteredProjectionID;

	// --- LUT-BRDF ---
	unsigned int m_LUTBRDFMapSize;

	CVK::Texture* m_LUTBRDF = nullptr;

	CVK::ShaderSet* m_LUTBRDFShader = nullptr;

	// --- Skybox ---
	GLuint m_skyboxVAO;
	GLuint m_skyboxVBO;

	glm::mat4 m_skyboxCaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);;
	glm::mat4 m_skyboxCaptureViews[6] = // All Blickrichtungen
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	const float m_skyboxVertices[6 * 3 * 6] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// --- Quad ---
	GLuint m_quadVAO;
	GLuint m_quadVBO;

	float m_quadVertices[5 * 4] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
};
}