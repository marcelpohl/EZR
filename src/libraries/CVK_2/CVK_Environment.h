#pragma once
#include "CVK_ShaderCubeMap.h"
#include "CVK_Texture.h"
#include "CVK_FBO.h"
#include "CVK_Node.h"
#include "CVK_ShaderMinimal.h"

//TODO: Pre Filter AntiAlias
//TODO: Precompute BRDF parameters

namespace CVK 
{
class Environment
{
public:
	Environment(CVK::CubeMapTexture* enviromentMap, unsigned int irrMapSize, unsigned int preFilMapSize, unsigned int preFilLevel, unsigned int m_LUTBRDFMapSize);
	Environment(glm::vec3 color, unsigned int irrMapSize, unsigned int preFilMapSize, unsigned int preFilLevel, unsigned int m_LUTBRDFMapSize);

	/**
	* Make sure, that the old maps are no longer referenced anywhere because this deletes them all!!!
	*/
	~Environment();

	/**
	* Make sure, that the old maps are no longer referenced anywhere because this deletes them all!!!
	*/
	void computeMaps();

	CVK::CubeMapTexture* getIrradianceMap();
	CVK::CubeMapTexture* getPreFilteredEnvironmentMap();
	CVK::Texture* getLUTBRDFMap();

	void setEnviroment(CVK::CubeMapTexture* enviromentMap);
	void setUniformEnviroment(glm::vec3 color);
	// Ein ich hätte das lieber über eine Funktionszeiger geregelt aber da alle nötigen Variablen im Rahmen der main-funktion bestehen ist der transfer aufwand so geringer auch wenn das ganze dann nicht ganz fexibel ist.
	void renderSceneToEnvironmentMap(CVK::Node &scene, CVK::ShaderMinimal &shader, CVK::ShaderCubeMap &skyBox, unsigned int passes, unsigned int resolution, glm::vec3 worldPos);

	CVK::CubeMapTexture* getEnvironmentMap();

	void setIrradianceMapSize(unsigned int size);
	void setPreFilteredEnvironmentMapSize(unsigned int size);
	void setPreFilteredEnvironmentLevels(unsigned int levels);
	void setLUTBRDFMSize(unsigned int size);

	unsigned int getIrradianceMapSize();
	unsigned int getPreFilteredEnvironmentMapSize();
	unsigned int getPreFilteredEnvironmentLevels();
	unsigned int getLUTBRDFMSize();

	unsigned int getAccIrradianceMapSize();
	unsigned int getAccPreFilteredEnvironmentMapSize();
	unsigned int getAccPreFilteredEnvironmentLevels();
	unsigned int getAccLUTBRDFMSize();

private:
	void setUp();
	void setUpTextures(); // Ohne LUTBRDF, die ist teil von Setup!

	void createCubeMapTextureBuffer(unsigned int sizeX, unsigned int sizeY, CVK::CubeMapTexture** texture, bool mipmap);
	void createQuadTextureBuffer(unsigned int sizeX, unsigned int sizeY, CVK::Texture** texture);

	void computeIrradianceMap();
	void computePreFilteredEnvironmentMap();
	void computeLUTBRDFMap();

	CVK::FBO* m_preComputeFBO;

	// --- Environment ---
	CVK::CubeMapTexture* m_enviromentMap = nullptr;
	glm::vec3 m_color = glm::vec3(0.0f, 0.0f, 0.0f);

	// --- Irradiance ---
	unsigned int m_irrMapSize = 0;
	unsigned int m_accIrrMapSize = 0;

	CVK::CubeMapTexture* m_irradianceMap = nullptr;

	CVK::ShaderSet* m_irradianceShader = nullptr;

	GLuint m_irradianceViewID; // TODO: rausfinden ob die Positionen sich unterscheiden wenn der Vertex shader der gleiche bleibt ANTWORT: NEIN!!!!!!!!
	GLuint m_irradianceProjectionID;

	// --- PreFilteredEnvironement ---
	unsigned int m_preFilMapSize = 0;
	unsigned int m_preFilLevel = 0;
	unsigned int m_accPreFilMapSize = 0;
	unsigned int m_accPreFilLevel = 0;

	CVK::CubeMapTexture* m_preFilteredMap = nullptr;

	CVK::ShaderSet* m_preFilteredShader = nullptr;

	GLuint m_roughnessID;
	GLuint m_preFilteredViewID;
	GLuint m_preFilteredProjectionID;

	// --- LUT-BRDF ---
	unsigned int m_LUTBRDFMapSize = 0;
	unsigned int m_accLUTBRDFMapSize = 0;

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