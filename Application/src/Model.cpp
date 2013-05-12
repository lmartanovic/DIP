#include "Model.h"

Model::Model()
: mainVAO(0),
  worldMatrix(glm::mat4(1.0f))
{}

Model::~Model()
{
	//CLEANUP
}

//import model from file
bool Model::import(const std::string & filename)
{
	//create and bind VAO
	glGenVertexArrays(1, &mainVAO);
	glBindVertexArray(mainVAO);
	//generate index/vertex buffers
	glGenBuffers(NUM_BUFFERS, buffers);
	//init return value
	bool ret = false;
	//create importer
	Assimp::Importer importer;
	//load scene
	const aiScene* scene = importer.ReadFile(filename.c_str(),
											 aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	if(scene)	//on success
	{
		ret = fromScene(scene, filename);
	}else{
		std::cerr << "Error parsing input file - " << filename << std::endl;
		std::cerr << importer.GetErrorString() << std::endl;
	}
	//unbind
	glBindVertexArray(0);

	return ret;
}


//convert assimp scene to VAO
bool Model::fromScene(const aiScene* scene, const std::string & filename)
{
	//resize meshInfos
	std::vector<Vector3f> positions;
	std::vector<Vector3f> normals;
	std::vector<unsigned int> indices;

	unsigned int numVertices = 0;
	unsigned int numIndices = 0;

	MeshInfo info;
	//count indices and vertices
	for(unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		info.numIndices = scene->mMeshes[i]->mNumFaces * 3;
		info.baseVertex = numVertices;
		info.baseIndex = numIndices;
		meshInfos.push_back(info);

		numVertices += scene->mMeshes[i]->mNumVertices;
		numIndices += info.numIndices;
	}

	//resize vectors
	positions.reserve(numVertices);
	normals.reserve(numVertices);
	indices.reserve(numIndices);

	//fill the vectors with mesh data
	for(unsigned int i = 0; i < meshInfos.size(); i++)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		initMesh(mesh, positions, normals, indices);
	}

	//generate VBO and EBO
	//positions
	glBindBuffer(GL_ARRAY_BUFFER, buffers[VBO_POS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0])*positions.size(),
				 &positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);	//enable position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//normals
	glBindBuffer(GL_ARRAY_BUFFER, buffers[VBO_NORM]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0])*normals.size(),
				 &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);	//enable position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[EBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*indices.size(),
				 &indices[0], GL_STATIC_DRAW);
	//TODO: error check
	return true;
}

//fill the data from meshes
void Model::initMesh(const aiMesh* mesh, std::vector<Vector3f> & positions,
				     std::vector<Vector3f> & normals,
				     std::vector<unsigned int> & indices)
{
	Vector3f v;
	//vertices
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		const aiVector3D* pos      = &(mesh->mVertices[i]);
        const aiVector3D* normal   = &(mesh->mNormals[i]);
        v.x = pos->x;
        v.y = pos->y;
        v.z = pos->z;
        positions.push_back(v);
        v.x = normal->x;
        v.y = normal->y;
        v.z = normal->z;
        normals.push_back(v);
	}
	//indices
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
}

//draw the whole model
void Model::draw()
{
	//bind main VAO
	glBindVertexArray(mainVAO);
	//draw meshes
	for(unsigned int i = 0; i < meshInfos.size(); i++)
	{
		glDrawElementsBaseVertex(GL_TRIANGLES, meshInfos[i].numIndices,
								 GL_UNSIGNED_INT,
								 (void*)(sizeof(unsigned int) * meshInfos[i].baseIndex),
								 meshInfos[i].baseVertex);
	}
	//unbind
	glBindVertexArray(0);
}