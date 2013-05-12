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

typedef struct
{
	GLint posAttrib;
	GLint normAttrib;
	GLint texCoordAttrib;
} shaderAttribs;

class Model
{
public:
	Model();
	~Model();

	bool import(const std::string & filename);
	void draw();

	glm::mat4& getWorldMatrix() {return worldMatrix;}

private:
	bool fromScene(const aiScene* scene, const std::string & filename);
	void initMesh(const aiMesh* mesh, std::vector<Vector3f> & positions,
				  std::vector<Vector3f> & normals,
				  std::vector<unsigned int> & indices);

	GLuint mainVAO;
	GLuint buffers[NUM_BUFFERS];
	std::vector<MeshInfo> meshInfos;
	glm::mat4 worldMatrix;
};

#endif