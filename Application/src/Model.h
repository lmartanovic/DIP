/******************************************************************************
* DIP - Real-Time Illumination of a Scene - Model.h                           *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - Model - 3D model loading and rendering.                                   *
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

#ifndef MODEL_H
#define MODEL_H
//std
#include <iostream>
#include <map>
#include <vector>
//assimp
#include <Assimp/Importer.hpp>
#include <Assimp/postprocess.h>
#include <Assimp/scene.h>
//devIL
#include <IL/il.h>
//OpenGL
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define NUM_BUFFERS 4
#define EBO 0
#define VBO_POS 1
#define VBO_NORM 2
#define VBO_TC 3

#define NUM_VPLS 64
#define VPL_SQRT 8
#define POINTS_PER_VPL 16000

//! 3D vector
struct Vector3f
{
	float x;
	float y;
	float z;
};

//! 2D vector
struct Vector2f
{
	float x;
	float y;
};

//! Mesh attributes struct
struct MeshInfo
{
	unsigned int numIndices;	/*!< Number of indices */
	unsigned int baseIndex;		/*!< First corresponding index in EBO */
	unsigned int baseVertex;	/*!< First corresponding vertex in VBOs */
	unsigned int texture;		/*!< Associated diffuse color texture */
};

//! Point vertex struct
struct PointVertex
{
	float position[3];			/*!< 3D position */
	int VPLindex;				/*!< Corresponding VPL */
	float barycentric[3];		/*!< Barycentric coordinates */
	unsigned int triangleIndex;	/*!< Corresponding triangle */
};

//! Triangle struct
struct Triangle
{
	unsigned int ABCind[3];		/*!< Vertex indices */
	float area;					/*!< Triangle area */
	unsigned int meshIndex;		/*!< Corresponding mesh */
};

/*!
 * \class Model
 * \brief 3D model loading and rendering class
*/
class Model
{
public:
	//! Default constructor
	Model();
	//! Default destructor
	~Model();

	//! Import model/scene from file
	/*!
	  \param filename - file containing the model
	*/
	bool import(const std::string & filename);
	//! Draw the model
	void draw();
	//! Draw corresponding point cloud
	void drawPointCloud();
	//! Scale the model
	/*!
	  \param factor - scale factor
	*/
	void scale(float factor) {worldMatrix = glm::scale(worldMatrix, glm::vec3(factor));}
	//! Translate the model
	/*!
	  \param direction - vector in which to move the model
	*/
	void moveBy(glm::vec3 direction) {worldMatrix = glm::translate(worldMatrix,
																	direction);}
	//! Rotate the model
	/*!
	  \param angle - rotation angle in radians
	  \param axis - rotation axis
	*/
	void rotate(float angle, glm::vec3 axis) {worldMatrix = glm::rotate(worldMatrix,
																		angle,
																		axis);}
	//! Get the model transformation matrix
	glm::mat4& getWorldMatrix() {return worldMatrix;}
	//! Get geometrical center
	glm::vec3& getCenter();

private:
	//! Load meshes from assimp scene object
	/*!
	  \param scene - assimp scene object
	  \param filename - file containing the scene
	*/
	bool fromScene(const aiScene* scene, const std::string & filename);
	//! Generate point cloud from loaded meshes
	/*!
	  \param positions - vector of vertex positions
	  \param indices - vector of triangle indices
	*/
	void generatePointCloud(std::vector<Vector3f> & positions,
							std::vector<unsigned int> & indices);
	//! Load vector attributes for each individual mesh in a scene
	/*!
	  \param mesh - assimp mesh object
	  \param positions - vector to hold vertex positions
	  \param normals - vector to hold vertex normals
	  \param texCoords - vector to hold vertex texture coordinates
	  \param indices - vector to hold triangle indices
	*/
	void initMesh(const aiMesh* mesh, std::vector<Vector3f> & positions,
				  std::vector<Vector3f> & normals,
				  std::vector<Vector2f> & texCoords,
				  std::vector<unsigned int> & indices);
	//! Set models geometrical center based on vertex positions
	/*!
	  \param positions - vector of model vertex positions
	*/
	void setCenter(std::vector<Vector3f>& positions);
	//! Sample random point from a triangle
	/*!
	  \param positions - vector of vertex positions
	  \param t - triangle to be sampled
	  \param p - structure to hold sampled point attributes
	*/
	void samplePoint(std::vector<Vector3f> & positions,
					 Triangle & t, PointVertex & p);
	//! Load textures corresponding with the model
	/*!
	  \param scene - assimp scene object
	*/
	void loadTextures(const aiScene* scene);
	//! Compute triangle area based on vertex positions
	/*!
	  \param A - first vertex
	  \param B - second vertex
	  \param C - third vertex
	*/
	static float computeArea(Vector3f & A, Vector3f & B, Vector3f & C);
	//! Compute vector dot product
	/*!
	  \param a - first vector
	  \param b - second vector
	*/
	static float dot(Vector3f & a, Vector3f & b);
	//geometry
	GLuint mainVAO;					/*!< Polygonal geometry VAO */
	GLuint buffers[NUM_BUFFERS];	/*!< Polygonal geometry buffers (VBOs, EBO) */
	//point cloud
	GLuint pointsVAO;				/*!< Point cloud VAO */
	GLuint pointsVBO;				/*!< Point cloud VBO */
	std::vector<Triangle> triangles;	/*!< Vector of model triangles */
	//common properties
	std::vector<MeshInfo> meshInfos;	/*!< Mesh attributes */
	std::map<std::string, GLuint> textureMap;	/*!< Picture file -> OpenGL object map */
	glm::mat4 worldMatrix;			/*! Model transformation matrix */
	glm::vec3 center;				/*! Model center point */
};

#endif