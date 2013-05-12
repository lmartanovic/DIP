#include "Geometry.h"

Geometry::Geometry()
: numFaces(0),
  totalArea(0.0),
  model(NULL),
  scaleFactor(1.0),
  ry(0.0),
  worldMatrix(glm::mat4(1.0))
{}

void Geometry::draw(SampleShader & shader)
{
	recursive_render(shader, model->mRootNode);
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

	aiVector3D modelMin, modelMax, modelCenter;
	getBoundingBox(model, &modelMin, &modelMax);
	float tmp;
	tmp = modelMax.x-modelMin.x;
	tmp = modelMax.y - modelMin.y > tmp?modelMax.y - modelMin.y:tmp;
	tmp = modelMax.z - modelMin.z > tmp?modelMax.z - modelMin.z:tmp;
	scaleFactor = 1.f / tmp;
}

void Geometry::generateVAOs(shaderAttribs & attribs, SampleShader & sampleShader)
{
	//traverse through model meshes
	for(unsigned int n = 0; n < model->mNumMeshes; n++)
	{
		aiMesh* mesh = model->mMeshes[n];
		//add to triangle count
		numFaces += mesh->mNumFaces;
		//convert from assimp format to array - index data
		unsigned int *faceArray = (unsigned int *)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
		unsigned int faceIndex = 0;
		for(unsigned int m = 0; m < mesh->mNumFaces; m++)
		{
			const struct aiFace* face = &mesh->mFaces[m];
			memcpy(&faceArray[faceIndex], face->mIndices, 3 * sizeof(unsigned int));
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
			glEnableVertexAttribArray(attribs.posAttrib);
			glVertexAttribPointer(attribs.posAttrib, 3, GL_FLOAT,
								  0, 0, 0);
		}

		if(mesh->HasNormals())
		{
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER,
						 sizeof(float)*3*mesh->mNumVertices,
						 mesh->mNormals,
						 GL_STATIC_DRAW);
			glEnableVertexAttribArray(attribs.normAttrib);
			glVertexAttribPointer(attribs.normAttrib, 3, GL_FLOAT,
								  0, 0, 0);
		}
		//store
		VAOs.push_back(vao);
		MeshFaces.push_back(mesh->mNumFaces);
		//unbind
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	std::cout << attribs.posAttrib << " --- " << attribs.normAttrib << std::endl;
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

void Geometry::recursive_render(SampleShader & shader, const aiNode* node)
{
	aiMatrix4x4 m = node->mTransformation;
	m.Transpose();
	//push
	glm::mat4 world = worldMatrix;// = glm::mat4(1.0)
	//set up node transform
	glm::mat4 trans;
	CopyAiMat(&m, trans);
	worldMatrix = worldMatrix * trans;
	setUniform(shader.world, worldMatrix);
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
		recursive_render(shader, node->mChildren[i]);
	}

	//pop
	worldMatrix = world;
}

//-----------------------------------------------------------------------------------------
//STATIC
void Geometry::gbbfn(const aiScene* scene, struct aiNode* node, aiVector3D* min, aiVector3D* max)
{
	for(unsigned int n = 0; n < node->mNumMeshes; n++)
	{
		const struct aiMesh* mesh = scene->mMeshes[node->mMeshes[n]];
		for(unsigned int t = 0; t < mesh->mNumVertices; t++)
		{
			aiVector3D tmp = mesh->mVertices[t];
			min->x = MIN(min->x, tmp.x);
			min->y = MIN(min->y, tmp.y);
			min->z = MIN(min->z, tmp.z);

			max->x = MAX(max->x, tmp.x);
			max->y = MAX(max->y, tmp.y);
			max->z = MAX(max->z, tmp.z);
		}
	}

	for(unsigned int n = 0; n < node->mNumChildren; n++)
	{
		gbbfn(scene, node->mChildren[n], min, max);
	}
}

void Geometry::getBoundingBox(const aiScene* scene, aiVector3D* min, aiVector3D* max)
{
	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;
	gbbfn(scene, scene->mRootNode, min, max);
}