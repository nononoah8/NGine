#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "Mesh.h"
#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb/stb_image.h>

class Model {
public:
  Model(std::string& path){
    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);
    LoadModel(path);
  }
  void Draw(unsigned int shaderProgram);	
  // model data
  std::vector<std::shared_ptr<Mesh>> meshes;
  std::string directory;
  std::vector<Texture> texturesLoaded;

private:
  void LoadModel(std::string& path);
  void ProcessNode(aiNode* node, const aiScene* scene);
  std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif  // MODEL_H