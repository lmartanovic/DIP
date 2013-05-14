#ifndef LIGHT_H
#define LIGHT_H
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define SHADOW_WIDTH 2048
#define SHADOW_HEIGHT 2048

class Light
{
public:
	Light();
	void create();
	void setOrigin(glm::vec3 & o);
	void setTarget(glm::vec3 & t);
	void move() {computeViewMatrix();};

	glm::mat4 & getProjectionMatrix() {return projectionMatrix;};
	glm::mat4 & getViewMatrix() {return viewMatrix;};
	glm::vec3 & getOrigin() {return origin;};

	static glm::mat4 biasMatrix;
private:
	void computeProjectionMatrix();
	void computeViewMatrix();

	float fov;
	float nearPlane;
	float farPlane;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 origin;
	glm::vec3 target;

	glm::vec4 color;

	static const glm::vec3 up;
};

#endif