#ifndef __CVK_SHADER_MINIMAL_H
#define __CVK_SHADER_MINIMAL_H

#include "CVK_Defs.h"
#include "CVK_ShaderSet.h"
#include "CVK_State.h"
#include "CVK_Node.h"

namespace CVK
{
/**
 * Minimal shader class implementation using the ShaderSet. The model, view and projection
 * matrices are set. Uses camera bound to State. Has to collect uniform locations for variables from shader first.
 * @brief Minimal shader that sets model, view and projection matrix
 * @see State
 */
class ShaderMinimal : public CVK::ShaderSet
{
public:

	/**
	* Constructor for ShaderMinimal with given parameters. Collects uniform locations for 
	* all used variables from Shader Program.
	* @param shader_mask Describes which shader files are used
	* @param shaderPaths Array of paths to shader files
	*/
	ShaderMinimal(GLuint shader_mask, const char** shaderPaths);
	/**
	 * @brief Standard Setter for model matrix in shader
	 * @param modelmatrix The new model matrix of this object
	 */
	void updateModelMatrix(glm::mat4 modelmatrix) const;
	/**
	 * Sets scene dependent variables in Shader. Namely view and projection matrix by using Camera in State.
	 * @brief Sets scene variables
	 * @see State
	 * @see Camera
	 */
	virtual void update();
	/**
	* Sets node dependent variables in Shader. None used in ShaderMinimal, but needed for subclasses.
	* @brief Sets node variables
	* @see Node
	*/
	virtual void update(CVK::Node* node);

private:
	GLuint m_modelMatrixID;
	GLuint m_viewMatrixID, m_projectionMatrixID;

};

}

#endif /* __CVK_SHADER_MINIMAL_H */
