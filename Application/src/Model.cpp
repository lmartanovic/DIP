/******************************************************************************
* DIP - Real-Time Illumination of a Scene - Model.cpp                         *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - Model loading and rendering implementation.                               *
*                                                                             *
*******************************************************************************
* Disclaimer                                                                  *
* ----------                                                                  *
* - Some parts of this module using Assimp and DevIL are based on             *
*	http://ogldev.atspace.co.uk and http://www.lighthouse3d.com tutorials.    *
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

#include "Model.h"

//! Default constructor
Model::Model()
: mainVAO(0),
  worldMatrix(glm::mat4(1.0f)),
  center(glm::vec3(0.0))
{}
//! Default destructor
Model::~Model()
{
	//CLEANUP
}
//! Import model/scene from file
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
		loadTextures(scene);
		ret = fromScene(scene, filename);
	}else{
		std::cerr << "Error parsing input file - " << filename << std::endl;
		std::cerr << importer.GetErrorString() << std::endl;
	}
	//unbind
	glBindVertexArray(0);

	return ret;
}


//! Load meshes from assimp scene object
bool Model::fromScene(const aiScene* scene, const std::string & filename)
{
	//resize meshInfos
	std::vector<Vector3f> positions;
	std::vector<Vector3f> normals;
	std::vector<Vector2f> texCoords;
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
	texCoords.reserve(numVertices);
	indices.reserve(numIndices);

	//fill the vectors with mesh data
	for(unsigned int i = 0; i < meshInfos.size(); i++)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		//get material info
		struct aiMaterial *mtl = scene->mMaterials[mesh->mMaterialIndex];
		aiString texPath;
		std::string file = "./Application/data/";
		if(mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
		{
			meshInfos[i].texture = textureMap[file+texPath.data];
		}
		initMesh(mesh, positions, normals, texCoords, indices);
	}

	//generate point cloud from complete model/scene
	generatePointCloud(positions, indices);
	//set object center based on geometry
	setCenter(positions);
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
	glEnableVertexAttribArray(1);	//enable normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//texCoords
	glBindBuffer(GL_ARRAY_BUFFER, buffers[VBO_TC]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0])*texCoords.size(),
				 &texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);	//enable normal
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[EBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*indices.size(),
				 &indices[0], GL_STATIC_DRAW);
	//TODO: error check
	return true;
}

//! Load vector attributes for each individual mesh in a scene
void Model::initMesh(const aiMesh* mesh, std::vector<Vector3f> & positions,
				     std::vector<Vector3f> & normals,
				     std::vector<Vector2f> & texCoords,
				     std::vector<unsigned int> & indices)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	Vector3f v;
	Vector2f t;
	//vertices
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		const aiVector3D* pos      = &(mesh->mVertices[i]);
        const aiVector3D* normal   = &(mesh->mNormals[i]);
        const aiVector3D* texCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;
        v.x = pos->x;
        v.y = pos->y;
        v.z = pos->z;
        positions.push_back(v);
        v.x = normal->x;
        v.y = normal->y;
        v.z = normal->z;
        normals.push_back(v);
        t.x = texCoord->x;
        t.y = texCoord->y;
        texCoords.push_back(t);
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
//! Get geometrical center
glm::vec3& Model::getCenter()
{
	glm::vec3 c;
	glm::vec4 cc = glm::vec4(center, 1.0);
	cc = worldMatrix*cc;
	center.x = cc.x;
	center.y = cc.y;
	center.z = cc.z;
	return center;
}
//! Set models geometrical center based on vertex positions
void Model::setCenter(std::vector<Vector3f>& positions)
{
	float minX = 1e30f;
	float minY = 1e30f;
	float minZ = 1e30f;
	float maxX = -1e30f;
	float maxY = -1e30f;
	float maxZ = -1e30f;
	float x, y, z;
	for(unsigned int i = 0; i < positions.size(); i++)
	{
		x = positions[i].x;
		minX = minX > x ? x : minX;
		maxX = maxX > x ? maxX : x;
		y = positions[i].y;
		minY = minY > y ? y : minY;
		maxY = maxY > y ? maxY : y;
		z = positions[i].z;
		minZ = minZ > z ? z : minZ;
		maxZ = maxZ > z ? maxZ : z;
	}
	center.x = minX + (maxX - minX)/2;
	center.y = minY + (maxY - minY)/2;
	center.z = minZ + (maxZ - minZ)/2;
}
//! Draw the model
void Model::draw()
{
	//bind main VAO
	glBindVertexArray(mainVAO);
	//draw meshes
	for(unsigned int i = 0; i < meshInfos.size(); i++)
	{
		//bind diffuse texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, meshInfos[i].texture);
		//std::cout << "texture - " << meshInfos[i].texture << std::endl;
		glDrawElementsBaseVertex(GL_TRIANGLES, meshInfos[i].numIndices,
								 GL_UNSIGNED_INT,
								 (void*)(sizeof(unsigned int) * meshInfos[i].baseIndex),
								 meshInfos[i].baseVertex);
	}
	//unbind
	glBindVertexArray(0);
}
//! Draw corresponding point cloud
void Model::drawPointCloud()
{
	//bind point cloud VAO
	glBindVertexArray(pointsVAO);
	//draw
	glDrawArrays(GL_POINTS, 0, NUM_VPLS*POINTS_PER_VPL);
	//unbind VAO
	glBindVertexArray(0);
}
//! Generate point cloud from loaded meshes
void Model::generatePointCloud(std::vector<Vector3f> & positions,
							   std::vector<unsigned int> & indices)
{
	std::cout << "generating point cloud..." << std::endl;
	if(indices.size() % 3) std::cerr << "Invalid index count " << std::endl;
	//compute area
	Vector3f A,B,C;	//triangle vertices
	Triangle t;
	float totalArea = 0.0;
	unsigned int parsed = 0;
	//create triangle structures to be sampled
	for(unsigned int k = 0; k < meshInfos.size(); k++)
	{
		t.meshIndex = k;
		unsigned int bi = meshInfos[k].baseIndex;
		unsigned int mi = meshInfos[k].baseIndex + meshInfos[k].numIndices - 3;
		for(unsigned int i = bi; i < mi; i+=3)
		{
			//get vertex indices
			t.ABCind[0] = indices[i];
			t.ABCind[1] = indices[i+1];
			t.ABCind[2] = indices[i+2];
			//get vertices
			A = positions[indices[i]];
			B = positions[indices[i+1]];
			C = positions[indices[i+2]];
			//compute area
			t.area = computeArea(A,B,C);
			totalArea += t.area;
			//push in place based on position
			std::vector<Triangle>::iterator j = triangles.begin();
			while(j != triangles.end())
			{
				if((*j).area > t.area)
				{
					j++;
					continue;
				}else{
					triangles.insert(j, t);	//put into vector
					break;
				}
			}
			if(j == triangles.end()) triangles.push_back(t);
			parsed+=3;
		}//for indices		
		//std::cout << (float)parsed*100/indices.size() << "%" << std::endl;
	}//for meshInfos
	//divide the triangles into groups based on their area
	float maxGroupArea = totalArea/15;
	std::vector<unsigned int> groups;
	float areaAccumulator = 0.0;
	unsigned int baseIndex = 0;
	//init - first group begins at index 0 (first triangle)
	std::cout << "\nDividing groups..." << std::endl;
	groups.push_back(0);
	for(unsigned int i = 0; i < triangles.size(); i++)
	{
		areaAccumulator += triangles[i].area;
		//if total area of triangles in group is bigger then thrash
		if(areaAccumulator > maxGroupArea)
		{
			groups.push_back(baseIndex);
			areaAccumulator = 0.0;
		}
		baseIndex++;
	}
	groups.push_back(triangles.size()-1);
	//sample points
	int groupIndex = 1;
	std::vector<PointVertex> points;
	PointVertex point;
	//sample as many points as necessary
	std::cout << "sampling..." << std::endl;
	for(int i = 0; i < NUM_VPLS*POINTS_PER_VPL; i++)
	{
		//get group bounds
		unsigned int end =  groups.at(groupIndex);
    	unsigned int front = groups.at(groupIndex-1);
    	unsigned int var = end - front;
    	//randomly select a triangle from given group
    	unsigned int trIndex = (unsigned int)(((float)rand()/RAND_MAX)*var)+front;
    	//move
    	if(var == 1) trIndex +=1;
    		groupIndex++;
    	if(groupIndex > (groups.size() - 1))
      		groupIndex = 1;
      	//sample
      	point.VPLindex = i % (int)NUM_VPLS;
      	point.triangleIndex = trIndex;
      	samplePoint(positions, triangles[trIndex], point);
      	points.push_back(point);
	}
	//generate and bind VAO
	glGenVertexArrays(1, &pointsVAO);
	glBindVertexArray(pointsVAO);
	//generate VBO
	glGenBuffers(1, &pointsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
	//fill it with point data
	glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(PointVertex),
				 &points[0], GL_STATIC_DRAW);
	//enable attributes
	glEnableVertexAttribArray(0);	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PointVertex),
                          (void*)offsetof(PointVertex, position));
	glEnableVertexAttribArray(1);	//VPLindex
	glVertexAttribIPointer(1, 1, GL_INT, sizeof(PointVertex),
                          (void*)offsetof(PointVertex, VPLindex));
	//glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, sizeof(PointVertex),
	//					  (void*)offsetof(PointVertex, VPLindex));
	//unbind and bind the main VAO back
	glBindVertexArray(mainVAO);
}
//! Compute triangle area based on vertex positions
float Model::computeArea(Vector3f & A, Vector3f & B, Vector3f & C)
{
	float a = sqrt(pow(A.x - B.x, 2) +
	               pow(A.y - B.y, 2) +
	               pow(A.z - B.z, 2));
	float b = sqrt(pow(C.x - A.x, 2) +
	               pow(C.y - A.y, 2) +
	               pow(C.z - A.z, 2));
	float c = sqrt(pow(B.x - C.x, 2) +
	               pow(B.y - C.y, 2) +
	               pow(B.z - C.z, 2));
	float p = (a+b+c)/2;
	return sqrt(p*(p-a)*(p-b)*(p-c));
}
//! Sample random point from a triangle
void Model::samplePoint(std::vector<Vector3f> & positions,
						Triangle & t, PointVertex & p)
{
	float a1 = (float)rand()/RAND_MAX;
	float a2 = (float)rand()/RAND_MAX;;
	float sa1 = sqrt(a1);
	float x;

	float A[3];
	float B[3];
	float C[3];
	//get vertex coordinates
	A[0] = positions[meshInfos[t.meshIndex].baseVertex + t.ABCind[0]].x;
	A[1] = positions[meshInfos[t.meshIndex].baseVertex + t.ABCind[0]].y;
	A[2] = positions[meshInfos[t.meshIndex].baseVertex + t.ABCind[0]].z;
	B[0] = positions[meshInfos[t.meshIndex].baseVertex + t.ABCind[1]].x;
	B[1] = positions[meshInfos[t.meshIndex].baseVertex + t.ABCind[1]].y;
	B[2] = positions[meshInfos[t.meshIndex].baseVertex + t.ABCind[1]].z;
	C[0] = positions[meshInfos[t.meshIndex].baseVertex + t.ABCind[2]].x;
	C[1] = positions[meshInfos[t.meshIndex].baseVertex + t.ABCind[2]].y;
	C[2] = positions[meshInfos[t.meshIndex].baseVertex + t.ABCind[2]].z;
	//compute point position
	for(int j = 0; j < 3; j++)
	{
		x = (1-sa1)*A[j] +
			(sa1*(1-a2))*B[j]+
			(a2*sa1)*C[j];
		p.position[j] = x;
	}
	//compute barycentric
	Vector3f v0, v1, v2;
	v0.x = B[0] - A[0];
	v0.y = B[1] - A[1];
	v0.z = B[2] - A[2];
	v1.x = C[0] - A[0];
	v1.y = C[1] - A[1];
	v1.z = C[2] - A[2];
	v2.x = p.position[0] - A[0];
	v2.y = p.position[1] - A[1];
	v2.z = p.position[2] - A[2];
	float d00 = dot(v0, v0);
	float d01 = dot(v0, v1);
	float d11 = dot(v1, v1);
	float d20 = dot(v2, v0);
	float d21 = dot(v2, v1);
	float denom = d00*d11 - d01*d01;
	p.barycentric[0] = (d11 * d20 - d01 * d21) / denom;
	p.barycentric[1] = (d00 * d21 - d01 * d20) / denom;
	p.barycentric[2] = 1.0f - p.barycentric[0] - p.barycentric[1];
}
//! Compute vector dot product
float Model::dot(Vector3f & a, Vector3f & b)
{
	return (a.x*b.x + a.y*b.y + a.z*b.z);
}
//! Load textures corresponding with the model
void Model::loadTextures(const aiScene* scene)
{
	std::string fPath = "./Application/data/";
	ILboolean success;
	//init devIL
	ilInit();
	//find materials contained in scene
	for(unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		int texIndex = 0;
		aiString path;
		aiReturn texFound = scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE,
															 texIndex, &path);
		while(texFound == AI_SUCCESS)
		{
			textureMap[fPath+path.data] = 0;
			texIndex++;
			texFound = scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE,
													    texIndex, &path);
		}
	}

	//get number of found textures
	int numTextures = textureMap.size();
	ILuint* imageIds = new ILuint[numTextures];
	ilGenImages(numTextures, imageIds);
	GLuint* textureIds = new GLuint[numTextures];
	glGenTextures(numTextures, textureIds);

	//fill the map
	std::map<std::string, GLuint>::iterator it;
	unsigned int i = 0;
	for(it = textureMap.begin(); it != textureMap.end(); it++, i++)
	{
		std::string fileName = (*it).first;
		(*it).second = textureIds[i];

		ilBindImage(imageIds[i]);
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
		success = ilLoadImage((ILstring)fileName.c_str());

		if(success) 
		{
			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); 

			//Create and load textures to OpenGL
			glBindTexture(GL_TEXTURE_2D, textureIds[i]); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH),
				ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE,
				ilGetData()); 
		}else printf("Couldn't load Image: %s\n", fileName.c_str());
	}

	ilDeleteImages(numTextures, imageIds);

	delete[] imageIds;
	delete[] textureIds;
}