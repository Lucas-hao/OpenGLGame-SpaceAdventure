#pragma once

#ifdef _WIN32
#include "Dependencies/assimp/Importer.hpp"
#include "Dependencies/assimp/scene.h"
#endif

#ifdef linux
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb/stb_image.h>
#endif

#include <string>
#include <iostream>
#include <vector>

#include "mesh.h"
#include "Shader.h"



using namespace std;
unsigned int textureFromFile(const char* path, const string& directory, bool gamma = false);

class Model
{
public:
	// Constructor
	Model() = default;
	~Model() = default;

	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Texture> textures_loaded;
	vector<Mesh>    meshes;
	string directory;
	bool gammaCorrection;

	void initialize(string const& path, bool gamma = false);
	void draw(Shader& shader);

private:
	void loadModel(string const& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

