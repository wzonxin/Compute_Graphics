#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	unsigned int id;
	string type;
	string path;
};

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	void Draw(Shader shader);

private:
	unsigned int VAO, VBO, EBO;
	void SetupMesh();
};

class Model
{
public:
	Model(char* path)
	{
		loadModel(path);

	}
	void Draw(Shader shader);
private:
	vector<Texture> textures_loaded;
	vector<Mesh> meshs;
	string directory;
	void loadModel(string path);
	void processNode(aiNode *node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName);
};