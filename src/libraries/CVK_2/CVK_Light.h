#ifndef __CVK_LIGHT_H
#define __CVK_LIGHT_H

#include "CVK_Defs.h"
#include "CVK_CameraSimple.h"
#include "CVK_FBO.h"
#include "CVK_Perspective.h"

namespace CVK
{
/**
 * Light is a class for managing different light sources. It can represent the following types:
 * - point lights (with position.w = 1.0f)
 * - directional lights (with position.w = 0.0f)
 * - spot lights (with position.w = 1.0f and spot attributes set
 * Lights are used to store data, which are used in shader programs.
 * @brief Light class for usage in shaders
 */
class Light 
{
public:
	/**
	 * Standard Constructor for Light
	 */
	Light();
	/**
	 * Constructor for Light with given parameters
	 * @param pos The position of the light source
	 * @param col The color for lighting objects
	 * @param s_dir The direction of the lightsource (unnecessary in case of point lights)
	 * @param s_exp The exponent for spot lights
	 * @param s_cut The cutoff angle of spot lights
	 */
	Light( glm::vec4 pos, glm::vec3 col, glm::vec3 s_dir, float s_exp = 1.0f, float s_cut = 0.0f);
	/**
	 * Standard Destructor for Light
	 */
	~Light();

	/**
	 * @brief Standard Setter for position
	 * @param pos the new position of this object
	 */
	void setPosition( glm::vec4 pos);
	/**
	 * @brief Standard Getter for position
	 * @return the position of this object as pointer
	 */
	glm::vec4 *getPosition(); 

	/**
	 * @brief Standard Setter for color
	 * @param col the new color of this object
	 */
	void setColor( glm::vec3 col);
	/**
	 * @brief Standard Getter for color
	 * @return the color of this object as pointer
	 */
	glm::vec3 *getColor(); 

	/**
	 * @brief Standard Setter for direction
	 * @param direction the new direction of this object
	 */
	void setSpotDirection( glm::vec3 direction); 
	/**
	 * @brief Standard Getter for direction
	 * @return the direction of this object as pointer
	 */
	glm::vec3 *getSpotDirection(); 

	/**
	 * @brief Standard Setter for spot exponent
	 * @param spotExponent the new spot exponent of this object
	 */
	void setSpotExponent( float spotExponent); 
	/**
	 * @brief Standard Getter for spot exponent
	 * @return the spot exponent of this object
	 */
	float getSpotExponent() const;

	/**
	 * @brief Standard Setter for spot cutoff angle
	 * @param spotCutoff the new spot cutoff angle of this object
	 */
	void setSpotCutoff( float spotCutoff);
	/**
	 * @brief Standard Getter for spot cutoff angle
	 * @return the spot cutoff angle of this object
	 */
	float getSpotCutoff() const;

	CameraSimple* getLightCamera();

	void setCastShadow(bool b, GLFWwindow* window);
	bool castsShadow();

	void setDirectional(bool b);
	bool isDirectional();

	glm::mat4* getLightVPMatrix();

	bool prepareRenderShadowMap();
	void finishRenderShadowMap();
	GLuint getShadowMap();


private:
	glm::vec4 m_position; //!< the position of the light source in 3D 
	glm::vec3 m_color; //!< the color for lighting 
	glm::vec3 m_spotDirection; //!< the direction of the light  
	float m_spotExponent; //!< the exponent for spot lights 
	float m_spotCutoff; //!< the cutoff angle for spot lights 

	CVK::FBO *m_shadowMapFBO = nullptr;
	CameraSimple *m_lightCamera = nullptr;
	CVK::Perspective *m_projection = nullptr;
	glm::mat4 m_lightVPMatrix = glm::mat4(1.0f);

	bool m_castShadow = false;
	bool m_isDirectional = false;

	int m_shadowWidth = 1024;
	int m_shadowHeight = 1024;
};

}

#endif /* __CVK_LIGHT_H */
