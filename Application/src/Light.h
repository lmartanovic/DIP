/******************************************************************************
* DIP - Real-Time Illumination of a Scene - Light.h                           *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - Light - Basic light.                                                      *
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

#ifndef LIGHT_H
#define LIGHT_H
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define SHADOW_WIDTH 2048
#define SHADOW_HEIGHT 2048
 
/*!
 * \class Light
 * \brief Basic movable light
*/
class Light
{
public:
	//! Default constructor
	Light();
	//! Default initialisation
	void create();
	//! Set light point of origin
	/*!
	  \param o - world-space coordinates of origin point
	*/
	void setOrigin(glm::vec3 & o);
	//! Set target point
	/*!
	  \param t - world-space coordinates of target point to look at
	*/
	void setTarget(glm::vec3 & t);
	//! Move the light
	void move() {computeViewMatrix(); moved = true;};
	//! Get movement indicating flag
	bool hasMoved() {return moved;};
	//! Reset movement indicating flag
	void resetMoveFlag() {moved = false;};

	//! Get light projection matrix
	glm::mat4 & getProjectionMatrix() {return projectionMatrix;};
	//! Get light view matrix
	glm::mat4 & getViewMatrix() {return viewMatrix;};
	//! Get light point of origin
	glm::vec3 & getOrigin() {return origin;};

	static glm::mat4 biasMatrix;	/*!< Bias matrix for shadow mapping */
private:
	//! Projection matrix setup/recomputation
	void computeProjectionMatrix();
	//! View matrix setup/recomputation
	void computeViewMatrix();

	bool moved;					/*!< Movement indicating flag */
	float fov;					/*!< Field of view */
	float nearPlane;			/*!< Near clipping plane */
	float farPlane;				/*!< Far clipping plane */
	glm::mat4 projectionMatrix; /*!< Projection matrix */
	glm::mat4 viewMatrix;		/*!< View matrix */
	glm::vec3 origin;			/*!< Point of origin in world-space */
	glm::vec3 target;			/*!< Target point in world-space */

	glm::vec4 color;			/*!< Light color */

	static const glm::vec3 up;	/*!< y up vector */
};

#endif