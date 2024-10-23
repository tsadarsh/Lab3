#include "custom_objloader.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <common/texture.hpp>

bool c_loadAssImp(
	const char * path, 
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals,
    int meshIndex,
	objDetails& objDetails,
	unsigned int pp_flags
){

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_JoinIdenticalVertices | aiProcess_SortByPType); // aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
	if( !scene) {
		fprintf( stderr, importer.GetErrorString());
		getchar();
		return false;
	}
	std::cout << "Total Meshes: " << scene->mNumMeshes << std::endl;
	const aiMesh* mesh = scene->mMeshes[meshIndex]; // In this simple example code we always use the 1rst mesh (in OBJ files there is often only one anyway)

	// Fill vertices positions
	vertices.reserve(mesh->mNumVertices);
	for(unsigned int i=0; i<mesh->mNumVertices; i++){
		aiVector3D pos = mesh->mVertices[i];
		vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
	}

	// Fill vertices texture coordinates
	uvs.reserve(mesh->mNumVertices);
	for(unsigned int i=0; i<mesh->mNumVertices; i++){
		aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
		uvs.push_back(glm::vec2(UVW.x, UVW.y));
	}

	// Fill vertices normals
	normals.reserve(mesh->mNumVertices);
	for(unsigned int i=0; i<mesh->mNumVertices; i++){
		aiVector3D n = mesh->mNormals[i];
		normals.push_back(glm::vec3(n.x, n.y, n.z));
	}


	// Fill face indices
	indices.reserve(3*mesh->mNumFaces);
	for (unsigned int i=0; i<mesh->mNumFaces; i++){
		// Assume the model has only triangles.
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		
		// Now we can process the material and load textures
		if (objDetails.use_texture_format == "jpg")
		{
			loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", objDetails);
		}
		if (objDetails.use_texture_format == "bmp")
		{
			aiString str;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
			std::string fullPath = "assets/textures/" + std::string(str.C_Str());
			objDetails.textureID = loadBMP_custom(fullPath.c_str());
		}
	}
	
	// The "scene" pointer will be deleted automatically by "importer"
	return true;
}

void loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, objDetails& objDetails) {

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        // Convert the aiString to std::string for easier handling
        std::string fullPath = "assets/textures/" + std::string(str.C_Str());

		std::cout << "Finding texture at: " << fullPath << std::endl;
        
        // Load the texture file using an image loader (e.g., stb_image.h)
        int width, height, nrChannels;
        unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);
        
        if (data) {
            GLuint textureID;
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);

			objDetails.textureID = textureID;
			objDetails.width = width;
			objDetails.height = height;
			objDetails.data = data;
            
            // Load the texture data into OpenGL
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            
            // Set texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            stbi_image_free(data); // Free the image memory after uploading to GPU
        } else {
            std::cerr << "Failed to load texture: " << fullPath << std::endl;
        }
    }
}