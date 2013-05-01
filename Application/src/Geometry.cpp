#include "Geometry.h"

Geometry::Geometry()
: numFaces(0),
  totalArea(0.0),
  model(NULL),
  worldMatrix(glm::mat4(1.0))
{}

void Geometry::draw()
{
	recursive_render(model->mRootNode);
}

void Geometry::import(const std::string & filename)
{
	model = importer.ReadFile(filename,
							  aiProcessPreset_TargetRealtime_Quality);
	std::cout << "loaded meshes - ";
	std::cout << model->mNumMeshes << std::endl;
	if(!model)
	{
		std::cerr << "File loading failed - " << filename << std::endl;
		std::cout << importer.GetErrorString() << std::endl;
	}
}

void Geometry::generateVAOs()
{
	//traverse through model meshes
	for(unsigned int n = 0; n < model->mNumMeshes; n++)
	{
		aiMesh* mesh = model->mMeshes[n];
		//add to triangle count
		numFaces += mesh->mNumFaces;
		//convert from assimp format to array
		unsigned int *faceArray = (unsigned int *)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
		unsigned int faceIndex = 0;
		for(unsigned int m = 0; m < mesh->mNumFaces; m++)
		{
			aiFace* face = &mesh->mFaces[m];
			memcpy(&faceArray[faceIndex], face->mIndices, 3 * sizeof(float));
			faceIndex += 3;
		}

		//generate vao
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		GLuint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					 sizeof(unsigned int) * mesh->mNumFaces * 3,	//3 per triangle
					 faceArray,
					 GL_STATIC_DRAW);
		GLuint vbo;
		if(mesh->HasPositions())
		{
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER,
						 sizeof(float)*3*mesh->mNumVertices,	//position - 3 floats per vertex
						 mesh->mVertices,
						 GL_STATIC_DRAW);
			glEnableVertexAttribArray(POSITION);
			glVertexAttribPointer(POSITION, 3, GL_FLOAT,
								  GL_FALSE, 0, 0);
		}
		//store
		VAOs.push_back(vao);
		MeshFaces.push_back(mesh->mNumFaces);
		//unbind
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

float Geometry::getArea()
{
	return totalArea;
}

int Geometry::getNumFaces()
{
	return numFaces;
}

glm::mat4 & Geometry::getWorldMatrix()
{
	return worldMatrix;
}

void Geometry::recursive_render(const aiNode* node)
{
	//TODO: push, recompute world matrix
	//draw this node
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		//TODO setup material
		glBindVertexArray(VAOs[node->mMeshes[i]]);
		glDrawElements(GL_TRIANGLES,
					   MeshFaces[node->mMeshes[i]]*3,
					   GL_UNSIGNED_INT, 0);
	}
	//draw children
	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		recursive_render(node->mChildren[i]);
	}
	//TODO: pop matrix
}