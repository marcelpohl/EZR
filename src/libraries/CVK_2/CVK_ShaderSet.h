#ifndef __CVK_SHADERSET_H
#define __CVK_SHADERSET_H

#include "CVK_Defs.h"
#include "CVK_State.h"

namespace CVK
{
/**
 * Base class for Shader. Loads the shader files and compiles them with OpenGL to shader programs.
 * @brief Class for loading and compiling shader files
 */
class ShaderSet
{
public:
	/**
	 * Standard Constructor for ShaderSet. Normally not used.
	 */
	ShaderSet();
	/**
	 * Constructor for ShaderSet with given parameters
	 * @param shader_mask Describes which shader files are used
	 * @param ShaderNames Array of paths to shader files
	 */
	ShaderSet( GLuint shader_mask, const char** ShaderNames);
	/**
	 * Standard Destructor for ShaderSet
	 */
	~ShaderSet();

	/**
	 * Loads the shader files to strings and compiles them with OpenGL
	 * @brief Loading and compiling shader files
	 * @param shader_mask Describes which shader files are used
	 * @param ShaderNames Array of paths to shader files
	 */
	void GenerateShaderProgramm( GLuint shader_mask, const char** ShaderNames);
	/**
	 * @brief Standard Getter for OpenGL program id
	 * @return The OpenGL program id of this object
	 */
	GLuint getProgramID() const;
	/**
	 * Binds the OpenGL shader program with OpenGL so that it can be used for rendering
	 * @brief Binds shader program
	 */
	void useProgram() const;
	/**
	 * Convenience method to bind an OpenGL texture object and use it in the shader at given location. 
	 * Has to be used in shader implementation
	 * @brief Binds the given texture to use in shader
	 * @param num The location, where to bind the texture
	 * @param texture The OpenGL texture object to bind
	 */
	void setTextureInput(unsigned int num, GLuint texture);
	/**
	* Convenience method to set a float variable to the given variable name within a shader
	* @brief Sets the value of the float variable in the shader
	* @param variableName The name of the variable in the shader
	* @param value The float value of the variable
	*/
	void setValue(const char* variableName, float value) const;

private: 
	/**
	 * Checks if the shader can be compiled without errors. Prints result via standard output.
	 * @brief Checks shader for errors
	 * @param shaderID The shader to check
	 */
	void checkShader(GLuint shaderID);
	/**
	* Checks if the shader program can be compiled without errors. Prints result via standard output.
	* @brief Checks shader program for errors
	* @param programID The shader program to check
	*/
	void checkProgram(GLuint programID);
	/**
	* Loads the shader file and compiles it with OpenGL
	* @brief Loading and compiling of one shader file
	* @param shaderID OpenGL shader object id
	* @param fileName Path to shader source code file
	*/
	void loadShaderSource(GLint shaderID, const char* fileName) const;

	GLuint m_shader_mask; //!< Which shaders does the program contain 

protected:
	GLuint m_ProgramID; //!< The OpenGL shader program id 
	std::vector<GLuint> m_textures; //!< Convenience list of OpenGL texture objects to set in shader program
};

}

#endif /* __CVK_SHADERSET_H */
