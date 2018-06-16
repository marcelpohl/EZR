#ifndef __CVK_CAMERASIMPLE_H
#define __CVK_CAMERASIMPLE_H

#include "CVK_Camera.h"
#include "CVK_Perspective.h"

namespace CVK
{
/**
 * Implementation of the Camera. Movement is done with varying speed in one direction. Direction can be changed.
 * @brief Camera movement like a pilot
 */
class CameraSimple : public CVK::Camera
{
public:
	/**
	 * Constructor for Pilotview with given parameters
	 * @param width The width of the camera, used for projection
	 * @param height The height of the camera, used for projection
	 * @param projection The corresponding projection matrix
	 */
	CameraSimple(GLFWwindow* window, int width, int height, CVK::Projection *projection);
	/**
	* Constructor for Pilotview with given parameters
	* @param width The width of the camera, used for projection
	* @param height The height of the camera, used for projection
	*/
	CameraSimple(GLFWwindow* window, int width, int height);
	/**
	 * Standard Destructor for Pilotview
	 */
	~CameraSimple();

	/**
	 * Update Function to move camera according to the mouse position and the key controls
	 * @brief Update position and look
	 * @param window The window where OpenGL is running
	 */
	void update(double deltaTime) override;
	/**
	 * @brief Standard Setter for position
	 * @param campos The new position of this object
	 */
	void setCamPos(glm::vec3 *campos);
	/**
	* @brief Setter for position and the look center
	* @param position The new position of this object
	* @param center The new look center of this object
	*/
	void setLookAt( glm::vec3 *position, glm::vec3 *center);
	/**
	 * @brief Standard Setter for up vector 
	 * @param up The new up vector of this object
	 */
	void setUpvector( glm::vec3 *up);

};

}

#endif /* __CVK_CAMERASIMPLE_H */
