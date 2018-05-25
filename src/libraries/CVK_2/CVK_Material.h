#ifndef __CVK_MATERIAL_H
#define __CVK_MATERIAL_H

#include "CVK_Defs.h"
#include "CVK_Texture.h"

namespace CVK
{
/**
 * Enum for different texture types. New types might follow.
 * @brief Different texture types
 */
//enum TextureType
//{
//	COLOR_TEXTURE,
//	NORMAL_TEXTURE,
//	METALLIC_TEXTURE,
//	ROUGHNESS_TEXTURE,
//	AO_TEXTURE
//};

/**
 * Material are used for lighting within a shader. For this the objects of this class store
 * the needed values. Furthermore they store all necessary OpenGL texture objects for an
 * object of the scene.
 * @brief Used for storing information for lighting
 */
class Material 
{
public:
	/**
	 * Constructor for Material with given parameters
	 * @param diffuse The color for direct illumination only dependent on angle between normal and light position
	 * @param specular The color for simple reflective illumination dependent on light, normal and camera
	 * @param shininess The exponent for the angle for reflective specular illumination
	 */
	Material(glm::vec3 diffuse, glm::vec3 specular, float shininess);
	/**
	* Constructor for Material with given parameters
	* @param kd The factor of diffuse illumination
	* @param diffuse The color for direct illumination only dependent on angle between normal and light position
	*/
	Material(float kd, glm::vec3 diffuse);
	/**
	* Constructor for Material with given parameters
	* @param kd The factor of diffuse illumination
	* @param diffuse The color for direct illumination only dependent on angle between normal and light position
	* @param ks The factor of specular illumination
	* @param specular The color for simple reflective illumination dependent on light, normal and camera
	* @param shininess The exponent for the angle for reflective specular illumination
	*/
	Material(float kd, glm::vec3 diffuse, float ks, glm::vec3 specular, float shininess);
	/**
	* Constructor for Material with given parameters
	* @param colorTexturePath The path to the color image for this material, used instead of diffuse color. Image is loaded first.
	* @param kd The factor of diffuse illumination
	*/
	Material(const std::string colorTexturePath, float kd);
	/**
	* Constructor for Material with given parameters
	* @param colorTextureID The OpenGL id for the already created OpenGL texture object
	* @param kd The factor of diffuse illumination
	*/
	Material(GLuint colorTextureID, float kd);
	/**
	* Constructor for Material with given parameters
	* @param colorTexturePath The path to the color image for this material, used instead of diffuse color. Image is loaded first.
	* @param kd The factor of diffuse illumination
	* @param ks The factor of specular illumination
	* @param specular The color for simple reflective illumination dependent on light, normal and camera
	* @param shininess The exponent for the angle for reflective specular illumination
	*/
	Material(const std::string colorTexturePath, float kd, float ks, glm::vec3 specular, float shininess);
	/**
	* Constructor for Material with given parameters
	* @param colorTextureID The OpenGL id for the already created OpenGL texture object
	* @param kd The factor of diffuse illumination
	* @param ks The factor of specular illumination
	* @param specular The color for simple reflective illumination dependent on light, normal and camera
	* @param shininess The exponent for the angle for reflective specular illumination
	*/
	Material( GLuint colorTextureID, float kd, float ks, glm::vec3 specular, float shininess);



	/**
	* Constructor for Material with given parameters - PBR
	* @param diffuse The color for direct illumination only dependent on angle between normal and light position
	* @param metallic The metallic factor of the material
	* @param roughness The roughness of the material
	* @param ao The ambient occlusion of the material
	*/
	Material(glm::vec3 diffuse, float metallic, float roughness, float ao);

	/**
	* Constructor for Material with given parameters - PBR
	* @param colorTexturePath The path to the color image for this material
	* @param normalTexturePath The path to the normal image for this material
	* @param metallicTexturePath The path to the metallic image for this material
	* @param roughnessTexturePath The path to the roughness image for this material
	* @param aoTexturePath The path to the ambient occlusion image for this material
	*/
	Material(const std::string colorTexturePath, const std::string normalTexturePath, const std::string metallicTexturePath, const std::string roughnessTexturePath, const std::string aoTexturePath);


	/**
	 * Standard Destructor for Material
	 */
	~Material( );

	/**
	* This function is called by the constructors. Normally it is not necessary to call it manually.
	* @brief Initializes Phong Material parameters
	* @param kd The factor of diffuse illumination
	* @param diffuse The color for direct illumination only dependent on angle between normal and light position
	* @param ks The factor of specular illumination
	* @param specular The color for simple reflective illumination dependent on light, normal and camera
	* @param shininess The exponent for the angle for reflective specular illumination
	*/
	void init( float kd, glm::vec3 diffuse, float ks, glm::vec3 specular, float shininess);

	/**
	 * @brief Standard Setter for factor of diffuse illumination
	 * @param kd the new factor of diffuse illumination of this object
	 */
	void setKd ( float kd);
	/**
	 * @brief Standard Getter for factor of diffuse illumination
	 * @return the factor of diffuse illumination of this object
	 */
	float getKd() const;
	/**
	* @brief Standard Setter for factor of specular illumination
	* @param ks the new factor of specular illumination of this object
	*/
	void setKs(float ks);
	/**
	* @brief Standard Getter for factor of specular illumination
	* @return the factor of specular illumination of this object
	*/
	float getKs() const;
	/**
	* @brief Standard Setter for factor of transparency
	* @param kt the new factor of transparency of this object
	*/
	void setKt(float kt);
	/**
	* @brief Standard Getter for factor of transparency
	* @return the factor of transparency of this object
	*/
	float getKt() const;

	/**
	 * @brief Standard Setter for diffuse color
	 * @param col the new diffuse color of this object
	 */
	void setdiffColor( glm::vec3 col); 
	/**
	 * @brief Standard Getter for diffuse color
	 * @return the diffuse color of this object
	 */
	glm::vec3* getdiffColor();

	/**
	* @brief Standard Setter for specular color
	* @param col the new specular color of this object
	*/
	void setspecColor(glm::vec3 col);
	/**
	* @brief Standard Getter for specular color
	* @return the specular color of this object
	*/
	glm::vec3* getspecColor();
	/**
	 * @brief Standard Setter for shininess
	 * @param shininess the new shininess of this object
	 */
	void setShininess( float shininess); 
	/**
	 * @brief Standard Getter for shininess
	 * @return the shininess of this object
	 */
	float getShininess() const;
	/**
	 * @brief Standard Setter for factor of refraction
	 * @param ior the new factor of refraction of this object
	 */
	void setIor ( float ior);
	/**
	 * @brief Standard Getter for factor of refraction
	 * @return the factor of refraction of this object
	 */
	float getIor() const;


	void setMetallic(float metallic);
	float getMetallic() const;

	void setRoughness(float roughness);
	float getRoughness() const;

	void setAO(float ao);
	float getAO() const;


	/**
	 * @brief Setter for texture of given texture type. Image is loaded first.
	 * @param type the type of the texture
	 * @param fileName the type of the texture
	 */
	void setTexture(TextureType type, const std::string fileName);
	/**
	* @brief Setter for texture of given texture type. 
	* @param type the type of the texture
	* @param textureID the OpenGL ID of the OpenGL texture object
	*/
	void setTexture( TextureType type, GLuint textureID);
	/**
	 * @brief Getter for determining, if texture of given type is set
	 * @return true, if such a texture is set, false otherwise
	 */
	bool hasTexture( TextureType type) const;
	/**
	 * @brief Standard Getter for texture of given type
	 * @param type the type of the texture
	 * @return The texture of given type of this object
	 */
	CVK::Texture* getTexture( TextureType type) const;


private:

	float m_kd, m_ks, m_kt;
	glm::vec3 m_diffColor; //!< diffuse color of material 
	glm::vec3 m_specColor; //!< specular color of material 
	float m_shininess; //!< shininess exponent for specular illumination 
	float m_ior; //!< index of refraction 

	CVK::Texture* m_colorTexture = nullptr; //!< color texture for this material
	CVK::Texture* m_normalTexture = nullptr; //!< normal texture for this material


	// material attributes by global values
	float m_metallic;		//!< is the material metallic or not
	float m_roughness;		//!< roughness of the material
	float m_ao;				//!< ambient occlusion of the material

	CVK::Texture* m_metallicTexture = nullptr;	//!< metallic texture for this material
	CVK::Texture* m_roughnessTexture = nullptr; //!< roughness texture for this material
	CVK::Texture* m_aoTexture = nullptr;		//!< ambient occlusion texture for this material

};

}

#endif /* __CVK_MATERIAL_H */
