/******************************************************************************
* DIP - Real-Time Illumination of a Scene - Camera.cpp                        *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - Camera class implementation.                                              *
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

#include "Camera.h"
#include <iostream>

//! Default constructor
Camera::Camera()
: origin(glm::vec3(-19.0, 2.0, 0.0)),
  target(glm::vec3(0.0, -10.0, 0.0)),
  moved(true),
  fov(60.0),
  winWidth(800.0),
  winHeight(600.0),
  nearPlane(0.1),
  farPlane(1000.0),
  horizAngle(90.0),
  vertAngle(0.0)
{}
//! Custom constructor
Camera::Camera(float FoV, float wWidth, float wHeight, float nPlane, float fPlane)
: origin(glm::vec3(0.0, 0.0, 100.0)),
  target(glm::vec3(0.0, 0.0, 0.0)),
  moved(false),
  fov(FoV),
  winWidth(wWidth),
  winHeight(wHeight),
  nearPlane(nPlane),
  farPlane(fPlane)
{}
//! Custom camera initialisation
void Camera::create(float FoV, float wWidth, float wHeight, float nPlane, float fPlane)
{
	fov = FoV;
	winWidth = wWidth;
	winHeight = wHeight;
	nearPlane = nPlane;
	farPlane = fPlane;
	computeProjectionMatrix();
	computeViewMatrix();
}
//! Default camera initialisation
void Camera::create()
{
	computeProjectionMatrix();
	computeViewMatrix();
}
//! Force camera to recompute projection matrix on window resize
void Camera::recomputeProjection(float wWidth, float wHeight)
{
	winWidth = wWidth;
	winHeight = wHeight;
	computeProjectionMatrix();
}
//! Set camera origin
void Camera::setOrigin(glm::vec3 & o)
{
	origin = o;
	computeViewMatrix();
}
//! Set camera target
void Camera::setTarget(glm::vec3 & t)
{
	target = t;
	computeViewMatrix();
}
//! Projection matrix setup/recomputation
void Camera::computeProjectionMatrix()
{
	projectionMatrix = glm::perspective(fov,
										winWidth/winHeight,
										nearPlane,
										farPlane);
}
//! View matrix setup/recomputation
void Camera::computeViewMatrix()
{
	//compute direction and right based on mouse movement
	direction = glm::vec3(cos(vertAngle) * sin(horizAngle),
                          sin(vertAngle),
                          cos(vertAngle) * cos(horizAngle));
	right = glm::vec3(sin(horizAngle - 3.14f/2.0f),
                      0,
                      cos(horizAngle - 3.14f/2.0f));
	//compute up vector
	up = glm::cross(right, direction);
	//setup view matrix
	viewMatrix = glm::lookAt(origin,
							 origin+direction,
							 up);
}
//! Mouse movement handler
void Camera::look(int x, int y)
{
	horizAngle += 0.001*float(winWidth/2 - x);
	vertAngle += 0.001*float(winHeight/2 - y);
	computeViewMatrix();
	moved = true;
}