#include "Model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <iostream>

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

void Model::Draw(unsigned int shaderProgram) {
  for(auto& mesh : meshes) {
    mesh->Draw(shaderProgram);
  }
}

void Model::LoadModel(std::string& path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return;
  }

  size_t lastSlash = path.find_last_of("/\\");
  if (lastSlash != std::string::npos) {
    directory = path.substr(0, lastSlash);
  } else {
    // No slashes found - assume current directory
    directory = ".";
  }

  ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
  // process all the node's meshes (if any)
  for(unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
    meshes.push_back(ProcessMesh(mesh, scene));			
  }
  // then do the same for each of its children
  for(unsigned int i = 0; i < node->mNumChildren; i++) {
    ProcessNode(node->mChildren[i], scene);
  }
}

std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  std::vector<float> vertexData;

  // Reserve space for efficiency
  vertices.reserve(mesh->mNumVertices * 11);

  for(unsigned int i = 0; i < mesh->mNumVertices; ++i) {
    // Create a vertex
    Vertex vertex;

    // Set the position of the vertex
    vertex.position.x = mesh->mVertices[i].x;
    vertex.position.y = mesh->mVertices[i].y;
    vertex.position.z = mesh->mVertices[i].z;
    
    // Normal
    if(mesh->HasNormals()) {
      vertex.normal.x = mesh->mNormals[i].x;
      vertex.normal.y = mesh->mNormals[i].y;
      vertex.normal.z = mesh->mNormals[i].z;
    } else {
      // Set the default normal to be up.
      vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    // Texture coords
    if(mesh->mTextureCoords[0]) {
      vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
      vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
    } else {
      vertex.texCoords = glm::vec2(0.0f, 0.0f);
    }

    vertices.push_back(vertex);
  }

  // Process indices
  for(unsigned int i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];
    for(unsigned int j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // Process the material texture(s)
  if(mesh->mMaterialIndex >= 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // Load diffuse texture(s)
    std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // Load specular textures
    std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  vertexData.reserve(vertices.size() * 11);  // 11 floats per vertex
  
  for(const auto& vertex : vertices) {
    // Position
    vertexData.push_back(vertex.position.x);
    vertexData.push_back(vertex.position.y);
    vertexData.push_back(vertex.position.z);
    
    // Color (default white)
    vertexData.push_back(1.0f);
    vertexData.push_back(1.0f);
    vertexData.push_back(1.0f);
    
    // Normal
    vertexData.push_back(vertex.normal.x);
    vertexData.push_back(vertex.normal.y);
    vertexData.push_back(vertex.normal.z);
    
    // Texture Coordinates
    vertexData.push_back(vertex.texCoords.x);
    vertexData.push_back(vertex.texCoords.y);
  }
  
  return std::make_shared<Mesh>(vertexData, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
  std::vector<Texture> textures;
  for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);
    // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
    bool skip = false;
    for(unsigned int j = 0; j < texturesLoaded.size(); j++) {
      if(std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0) {
        textures.push_back(texturesLoaded[j]);
        skip = true; 
        break;
      }
    }

    // if texture hasn't been loaded already, load it
    if(!skip) {
      Texture texture;
      texture.id = TextureFromFile(str.C_Str(), directory);
      texture.type = typeName;
      texture.path = str.C_Str();
      textures.push_back(texture);
      texturesLoaded.push_back(texture);
    }
  }
  return textures;
}



unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma) {
  std::string filename = std::string(path);
  filename = directory + "/" + filename;

  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format = 0;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}