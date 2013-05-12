#ifndef GEOMETRY_H
#define GEOMETRY_H
//standard library
#include <iostream>
#include <string>
#include <vector>
//assimp
#include <Assimp/Importer.hpp>
#include <Assimp/postprocess.h>
#include <Assimp/scene.h>
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//OpenGL
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include "SampleShader.h"

#define POSITION 0

#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)

typedef struct
{
	GLint posAttrib;
	GLint normAttrib;
	GLint texCoordAttrib;
} shaderAttribs;

class Geometry
{
public:
	Geometry();
	void draw(SampleShader & shader);
	void import(const std::string & filename);
	void generateVAOs(shaderAttribs & attribs, SampleShader & sampleShader);
	float getArea();
	int getNumFaces();
	glm::mat4 & getWorldMatrix();
	//TODO compute area
	//TODO compute world matrix
	//TODO point cloud creation
private:
	static void gbbfn(const aiScene* scene, aiNode* node, aiVector3D* min, aiVector3D* max);
	static void getBoundingBox(const aiScene* scene, aiVector3D* min, aiVector3D* max);
	//render
	void recursive_render(SampleShader & shader, const aiNode* node);
	//make a glm mat from aiMat
	void CopyAiMat(aiMatrix4x4 *from, glm::mat4 &to) {
		to[0][0] = from->a1; to[1][0] = from->a2;
		to[2][0] = from->a3; to[3][0] = from->a4;
		to[0][1] = from->b1; to[1][1] = from->b2;
		to[2][1] = from->b3; to[3][1] = from->b4;
		to[0][2] = from->c1; to[1][2] = from->c2;
		to[2][2] = from->c3; to[3][2] = from->c4;
		to[0][3] = from->d1; to[1][3] = from->d2;
		to[2][3] = from->d3; to[3][3] = from->d4;
	}

	int numFaces;		//triangle count
	float totalArea;	//area of the model
	const aiScene* model;		//whole "scene" composing model
	float scaleFactor;
	float ry;
	glm::mat4 worldMatrix;
	//drawable meshes - VAOs
	std::vector<GLuint> VAOs;
	std::vector<GLuint> MeshFaces;

	Assimp::Importer importer;
};

#endif