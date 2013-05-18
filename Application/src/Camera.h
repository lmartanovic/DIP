/******************************************************************************
* DIP - Real-Time Illumination of a Scene - Camera.h                          *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - Camera - Camera class.                                                    *
*                                                                             *
*******************************************************************************
* Author                                                                      *
* ------                                                                      *
* Lukáš Martanovič (xmarta00@stud.fit.vutbr.cz)                               *
*                                                                             *
* 18.05.2013                                                                  *
*                                                                             *
*******************************************************************************
* This software is not copyrighted.                                           *
*                                                                             *
* This source code is offered for use in the public domain.                   *
* You may use, modify or distribute it freely.                                *
*                                                                             *
******************************************************************************/

#ifndef CAMERA_H
#define CAMERA_H
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*!
 * \class Camera
 * \brief Free roaming camera class
*/
class Camera
{
public:
	//! Default constructor
	Camera();
	//! Custom constructor
	/*!
	  \param FoV - camera field of view
	  \param wWidth - rendering window width
	  \param wHeight - rendering window height
	  \param nPlane - near clipping plane
	  \param fPlane - far clipping plane
	*/
	Camera(float FoV, float wWidth, float wHeight, float nPlane, float fPlane);
	//! Custom camera initialisation
	/*!
	  \param FoV - camera field of view
	  \param wWidth - rendering window width
	  \param wHeight - rendering window height
	  \param nPlane - near clipping plane
	  \param fPlane - far clipping plane
	*/
	void create(float FoV, float wWidth, float wHeight, float nPlane, float fPlane);
	//! Default camera initialisation
	void create();
	//! Force camera to recompute projection matrix on window resize
	/*!
	  \param wWidth - new rendering window width
	  \param wHeight - new rendering window height
	*/
	void recomputeProjection(float wWidth, float wHeight);
	//! Set camera origin
	/*!
	  \param o - world-space point to be set as camera origin
	*/
	void setOrigin(glm::vec3 & o);
	//! Set camera target
	/*!
	  \param t - point in world-space for camera to look at
	*/
	void setTarget(glm::vec3 & t);
	//! Set camera direction
	/*!
	  \param d - vector from origin for camera to look
	*/
	void setDirection(glm::vec3 & d);
	//! Mouse movement handler
	/*!
	  \param x - mouse movement in pixels in x-axis relative to window center
	  \param y - mouse movement in pixels in y-axis relative to window center
	*/
	void look(int x, int y);
	//! Move the camera
	void move() {computeViewMatrix(); moved = true;};
	//! Get camera movement flag
	bool hasMoved() {return moved;};
	//! Reset camera movement flag
	void resetMoveFlag() {moved = false;};

	//! Get camera projection matrix
	glm::mat4 & getProjectionMatrix() {return projectionMatrix;};
	//! Get camera view matrix
	glm::mat4 & getViewMatrix() {return viewMatrix;};
	//! Get camera point of origin
	glm::vec3 & getOrigin() {return origin;};
	//! Get camera direction vector
	glm::vec3 & getDirection() {return direction;};
	//! Get camera right-hand vector
	glm::vec3 & getRight() {return right;};
private:
	//! Projection matrix setup/recomputation
	void computeProjectionMatrix();
	//! View matrix setup/recomputation
	void computeViewMatrix();

	glm::mat4 projectionMatrix;	/*!< Projection Matrix */
	glm::mat4 viewMatrix;		/*!< View Matrix */
	glm::vec3 origin;			/*!< Point of origin */
	glm::vec3 target;			/*!< Point to look at */
	glm::vec3 direction;		/*!< Direction */
	glm::vec3 right;			/*!< Right hand vector */
	glm::vec3 up;				/*!< Up vector */
	bool moved;					/*!< Movement indicator flag */
	float fov;					/*!< Field of view */
	float winWidth;				/*!< Rendering window width */
	float winHeight;			/*!< Rendering window height */
	float nearPlane;			/*!< Near clipping plane */
	float farPlane;				/*!< Far clipping plane */
	float horizAngle;			/*!< Horizontal angle */
	float vertAngle;			/*!< Vertical angle */
};

#endif