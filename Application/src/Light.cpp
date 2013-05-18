/******************************************************************************
* DIP - Real-Time Illumination of a Scene - Light.cpp                         *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - Light class implementation.                                               *
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

#include "Light.h"

//! Default constructor
Light::Light()
: moved(true),
  fov(60.0),
  nearPlane(1.0f),
  farPlane(100.0f),
  origin(glm::vec3(-19.0, 2.0, 0.0)),
  target(glm::vec3(0.0, -10.0, 0.0)),
  color(glm::vec4(1.0, 1.0, 1.0, 1.0))
 {}
//! Default initialisation
void Light::create()
{
	computeProjectionMatrix();
	computeViewMatrix();
}
//! Set light point of origin
void Light::setOrigin(glm::vec3 & o)
{
	origin = o;
	computeViewMatrix();
}
//! Set target point
void Light::setTarget(glm::vec3 & t)
{
	target = t;
	computeViewMatrix();
}
//! Projection matrix setup/recomputation
void Light::computeProjectionMatrix()
{
	projectionMatrix = glm::perspective(fov,
										(float)SHADOW_WIDTH/(float)SHADOW_HEIGHT,
										nearPlane,
										farPlane);
}
//! View matrix setup/recomputation
void Light::computeViewMatrix()
{
	viewMatrix = glm::lookAt(origin,
							 target,
							 up);
}

//static variable initialisation
const glm::vec3 Light::up = glm::vec3(0.0, 1.0, 0.0);
glm::mat4 Light::biasMatrix = glm::mat4(0.5, 0.0, 0.0, 0.0,
					  				    0.0, 0.5, 0.0, 0.0,
    						 			0.0, 0.0, 0.5, 0.0,
    						 			0.5, 0.5, 0.5, 1.0);