#ifndef CAMERA_H
#define CAMERA_H
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	Camera(float FoV, float wWidth, float wHeight, float nPlane, float fPlane);
	void create(float FoV, float wWidth, float wHeight, float nPlane, float fPlane);
	void create();
	void recomputeProjection(float wWidth, float wHeight);
	void setOrigin(glm::vec3 & o);
	void setTarget(glm::vec3 & t);
	void move() {computeViewMatrix();};

	glm::mat4 & getProjectionMatrix();
	glm::mat4 & getViewMatrix();
	glm::vec3 & getOrigin();
private:
	void computeProjectionMatrix();
	void computeViewMatrix();

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 origin;
	glm::vec3 target;
	float fov;
	float winWidth;
	float winHeight;
	float nearPlane;
	float farPlane;

	static const glm::vec3 up;
};

#endif