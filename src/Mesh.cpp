#include "Mesh.h"

#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
  : vertices(vertices), indices(indices), vertexCount(vertices.size() / 9), indexCount(indices.size()) {
  
  // Create buffers
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  
  // Bind the VAO
  glBindVertexArray(VAO);
  
  // Bind and set the VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
  
  // Bind the EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
  
  // Position attribute (3 floats)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  
  // Color attribute (3 floats)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  
  // Normal attribute (3 floats)
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Unbind the VBO and VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

// Constructor for textured meshes
Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures, Material material)
    : vertices(vertices), indices(indices), textures(textures), vertexCount(vertices.size() / 11), indexCount(indices.size()), hasTextureCoords(true) {

  // Create buffers
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // Bind the VAO
  glBindVertexArray(VAO);

  // Bind and set the VBO/EBO (same as your current code)
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  int stride = 11 * sizeof(float);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
  glEnableVertexAttribArray(0);

  // Color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Normal attribute
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Texture coordinate attribute
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)(9 * sizeof(float)));
  glEnableVertexAttribArray(3);

  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  this->material = material;
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void Mesh::Draw(unsigned int shaderProgram) const {
  // Set material properties
  glUniform3fv(glGetUniformLocation(shaderProgram, "material.ambient"), 1, glm::value_ptr(material.ambient));
  glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1, glm::value_ptr(material.diffuse));
  glUniform3fv(glGetUniformLocation(shaderProgram, "material.specular"), 1, glm::value_ptr(material.specular));
  glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), material.shininess);
  glUniform1i(glGetUniformLocation(shaderProgram, "material.useTexture"), material.useTexture);

  // Handle textures based on what's available
  if (material.useTexture) {
    if (!textures.empty()) {
      // Use textures from the textures array (for models)
      unsigned int diffuseNr = 1;
      unsigned int specularNr = 1;
      
      for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].type;
        
        if (name == "texture_diffuse")
          number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
          number = std::to_string(specularNr++);
          
        std::string uniformName = name + number;
        glUniform1i(glGetUniformLocation(shaderProgram, uniformName.c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
      }
    } 
    else {
      // Use material.diffuseMap and material.specularMap
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, material.diffuseMap);
      glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuseMap"), 0);
      
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, material.specularMap);
      glUniform1i(glGetUniformLocation(shaderProgram, "material.specularMap"), 1);
    }
  }

  // Draw the mesh
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  
  // Clean up textures
  if (material.useTexture) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}
