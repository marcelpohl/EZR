#ifndef __CVK_CAMERA_H
#define __CVK_CAMERA_H

#include "CVK_Defs.h"
#include "CVK_Projection.h"

namespace CVK
{
	/**
	 * Abstract Class for storing the Camera values with movement. It represents
	 * a camera in 3D space and therefore contains a pointer to the corresponding
	 * projection matrix.
	 * @brief Class for Camera with movement
	 */
	class Camera
	{
	public:
		/**
		 * Standard constructor
		 */
		Camera(GLFWwindow* window);
		/**
		* Standard destructor
		*/
		~Camera();

		/**
		 * Virtual function which needs to be implemented in the subclasses. Is used for all movements.
		 * @brief Moves the Camera.
		 * @param window The window of the application. Is used to get the mouse movements.
		 */
		virtual void update(double deltaTime) = 0;

		/**
		 * Returns the view matrix corresponding to this Camera as pointer.
		 * @brief Getter for the view matrix
		 * @return a pointer to the view matrix
		 */
		glm::mat4 *getView();
		/**
		* Returns the view matrix corresponding to this Camera as pointer arguments.
		* @brief Getter for the view matrix
		* @param x pointer to the x Axis
		* @param y pointer to the y Axis
		* @param z pointer to the z Axis
		* @param pos pointer to the position of the camera
		*/
		void getView(glm::vec3 *x, glm::vec3 *y, glm::vec3 *z, glm::vec3 *pos) const;
		/**
		* Sets the view matrix corresponding to this Camera.
		* @brief Setter for the view matrix
		* @param view the new view matrix as pointer
		*/
		void setView( glm::mat4 *view);

		/**
		 * deprecated. 
		 * Used in Raytracer. TODO: Delete... Put it in the Projection class
		 */
		void setWidthHeight(int width, int height);
		/**
		* deprecated.
		* Used in Raytracer. TODO: Delete... Put it in the Projection class
		*/
		void getWidthHeight( int *width, int *height) const;

		/**
		 * Standard lookAt Function for the camera.
		 * @brief Moves Camera and look at the center
		 * @param position the new Position for the camera.
		 * @param center the center point to look at.
		 * @param up the tilt of the camera.
		 */
		void lookAt( glm::vec3 position, glm::vec3 center, glm::vec3 up);

		/**
		 * Sets the projection matrix. 
		 * @brief Setter for the projection matrix
		 * @param projection the new projection matrix as pointer
		 */
		void setProjection( CVK::Projection *projection);

		/**
		* Returns the projection matrix. 
		* @brief Getter for the projection matrix
		* @return the projection matrix as pointer
		*/
		CVK::Projection *getProjection() const;

	protected:
		GLFWwindow* m_window = nullptr; //!< the window which provides keyboard/mouse input.
		int m_width = 0, m_height = 0; //!< deprecated width and height for the projection. TODO remove...
		glm::mat4 m_viewmatrix; //!< the view matrix for this object 
		CVK::Projection *m_projection = nullptr; //!< a pointer to the corresponding projection matrix.
		float m_oldX = 0, m_oldY = 0; //!< Old values for movement
		glm::vec3 m_cameraPos, m_direction, m_up; //!< camera attributes 
	};
}

#endif /* __CVK_CAMERA_H */
