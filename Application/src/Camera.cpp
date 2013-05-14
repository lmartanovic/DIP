#include "Camera.h"
#include <iostream>
Camera::Camera()
: origin(glm::vec3(0.0, 100.0, 150.0)),
  target(glm::vec3(0.0, 0.0, 0.0)),
  fov(60.0),
  winWidth(800.0),
  winHeight(600.0),
  nearPlane(0.1),
  farPlane(1000.0)
{}

Camera::Camera(float FoV, float wWidth, float wHeight, float nPlane, float fPlane)
: origin(glm::vec3(0.0, 0.0, 100.0)),
  target(glm::vec3(0.0, 0.0, 0.0)),
  fov(FoV),
  winWidth(wWidth),
  winHeight(wHeight),
  nearPlane(nPlane),
  farPlane(fPlane)
{}

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

void Camera::create()
{
	computeProjectionMatrix();
	computeViewMatrix();
}

void Camera::recomputeProjection(float wWidth, float wHeight)
{
	winWidth = wWidth;
	winHeight = wHeight;
	computeProjectionMatrix();
}

void Camera::setOrigin(glm::vec3 & o)
{
	origin = o;
	computeViewMatrix();
}

void Camera::setTarget(glm::vec3 & t)
{
	target = t;
	computeViewMatrix();
}

glm::mat4 & Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

glm::mat4 & Camera::getViewMatrix()
{
	return viewMatrix;
}

glm::vec3 & Camera::getOrigin()
{
	return origin;
}

void Camera::computeProjectionMatrix()
{
	projectionMatrix = glm::perspective(fov,
										winWidth/winHeight,
										nearPlane,
										farPlane);
}

void Camera::computeViewMatrix()
{
	viewMatrix = glm::lookAt(origin,
							 target,
							 up);
}

const glm::vec3 Camera::up = glm::vec3(0.0, 1.0, 0.0);