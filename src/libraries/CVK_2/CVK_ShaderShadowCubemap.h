#ifndef __CVK_SHADER_SHADOW_CUBEMAP_H
#define __CVK_SHADER_SHADOW_CUBEMAP_H

#include "CVK_Defs.h"
#include "CVK_Camera.h"
#include "CVK_Light.h"
#include "CVK_Material.h"
#include "CVK_Geometry.h"
#include "CVK_ShaderMinimal.h"

namespace CVK
{
/**
* PBR shader class implementation using the ShaderSet and ShaderMinimal. The model, view and projection
* matrices are set using ShaderMinimal. Sets additional variables (f.e. light and fog informations).
* Uses values set in State. Has to collect uniform locations for variables from shader first.
* @brief PBR shader that sets light and fog informations
* @see State
*/
class ShaderShadowCubemap : public CVK::ShaderMinimal
{
public:
	/**
	* Constructor for ShaderPhong with given parameters. Collects uniform locations for 
	* all used variables from Shader Program.
	* @param shader_mask Describes which shader files are used
	* @param shaderPaths Array of paths to shader files
	*/
	ShaderShadowCubemap( GLuint shader_mask, const char** shaderPaths);

	/**
	* Sets scene dependent variables in Shader. Namely light and fog informations set in State.
	* @brief Sets scene variables
	* @see State
	* @see Light
	*/
	void update(CVK::Light *light);
	/**
	* Sets node dependent variables in Shader, like Material information.
	* @brief Sets node variables
	* @see Material
	*/
	void update( CVK::Node* node) override;

private:
	GLuint m_shadowMatricesID[6];
	GLuint m_lightPosID, m_farPlaneID;
};

}

#endif /* __CVK_SHADER_SHADOW_CUBEMAP_H */
