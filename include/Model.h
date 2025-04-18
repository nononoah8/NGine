#include <string>
#include <vector>
#include <memory>

#include "Mesh.h"
#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
  public:
    Model(std::string& path){
      LoadModel(path);
    }
    void Draw(unsigned int shaderProgram);	
private:
  // model data
  std::vector<std::shared_ptr<Mesh>> meshes;
  std::string directory;
  std::vector<Texture> texturesLoaded;

  void LoadModel(std::string& path);
  void ProcessNode(aiNode* node, const aiScene* scene);
  std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};