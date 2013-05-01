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
//OpenGL
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#define POSITION 0

class Geometry
{
public:
	Geometry();
	void draw();
	void import(const std::string & filename);
	void generateVAOs();
	float getArea();
	int getNumFaces();
	glm::mat4 & getWorldMatrix();
	//TODO compute area
	//TODO compute world matrix
	//TODO point cloud creation
private:
	void recursive_render(const aiNode* node);
	int numFaces;		//triangle count
	float totalArea;	//area of the model
	const aiScene* model;		//whole "scene" composing model
	glm::mat4 worldMatrix;
	//drawable meshes - VAOs
	std::vector<GLuint> VAOs;
	std::vector<GLuint> MeshFaces;

	Assimp::Importer importer;
};

#endif