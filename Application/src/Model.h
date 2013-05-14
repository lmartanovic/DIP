#ifndef MODEL_H
#define MODEL_H
//std
#include <iostream>
#include <vector>
//assimp
#include <Assimp/Importer.hpp>
#include <Assimp/postprocess.h>
#include <Assimp/scene.h>
//OpenGL
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define NUM_BUFFERS 3
#define EBO 0
#define VBO_POS 1
#define VBO_NORM 2

#define NUM_VPLS 64
#define POINTS_PER_VPL 400

struct Vector3f
{
	float x;
	float y;
	float z;
};

struct MeshInfo
{
	unsigned int numIndices;
	unsigned int baseIndex;
	unsigned int baseVertex;
};

struct PointVertex
{
	float position[3];
	float barycentric[3];
	unsigned int triangleIndex;
	unsigned int VPLindex;
};

struct Triangle
{
	unsigned int ABCind[3];
	float area;
	unsigned int meshIndex;
};

class Model
{
public:
	Model();
	~Model();

	bool import(const std::string & filename);
	void draw();
	void drawPointCloud();
	void scale(float factor) {worldMatrix = glm::scale(worldMatrix, glm::vec3(factor));}
	void moveBy(glm::vec3 direction) {worldMatrix = glm::translate(worldMatrix,
																	direction);}
	void rotate(float angle, glm::vec3 axis) {worldMatrix = glm::rotate(worldMatrix,
																		angle,
																		axis);}

	glm::mat4& getWorldMatrix() {return worldMatrix;}
	glm::vec3& getCenter();

private:
	bool fromScene(const aiScene* scene, const std::string & filename);
	void generatePointCloud(std::vector<Vector3f> & positions,
							std::vector<unsigned int> & indices);
	void initMesh(const aiMesh* mesh, std::vector<Vector3f> & positions,
				  std::vector<Vector3f> & normals,
				  std::vector<unsigned int> & indices);
	void setCenter(std::vector<Vector3f>& positions);
	void samplePoint(std::vector<Vector3f> & positions,
					 Triangle & t, PointVertex & p);
	static float computeArea(Vector3f & A, Vector3f & B, Vector3f & C);
	static float dot(Vector3f & a, Vector3f & b);
	//geometry
	GLuint mainVAO;
	GLuint buffers[NUM_BUFFERS];
	//point cloud
	GLuint pointsVAO;
	GLuint pointsVBO;
	std::vector<Triangle> triangles;
	//common properties
	std::vector<MeshInfo> meshInfos;
	glm::mat4 worldMatrix;
	glm::vec3 center;
};

#endif