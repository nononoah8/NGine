#include "Mesh.h"

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
  : vertices(vertices), indices(indices), vertexCount(vertices.size() / 6), indexCount(indices.size()) {
  
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  
  // Color attribute (3 floats)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  
  // Unbind the VBO and VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void Mesh::Draw() const {
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

// Move constructor
Mesh::Mesh(Mesh&& other) noexcept
  : vertices(std::move(other.vertices)),
    indices(std::move(other.indices)),
    vertexCount(other.vertexCount),
    indexCount(other.indexCount),
    VAO(other.VAO),
    VBO(other.VBO),
    EBO(other.EBO) {
  
  // Invalidate other's handles
  other.VAO = 0;
  other.VBO = 0;
  other.EBO = 0;
}

// Move assignment operator
Mesh& Mesh::operator=(Mesh&& other) noexcept {
  if (this != &other) {
    // Free current resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    // Transfer ownership
    vertices = std::move(other.vertices);
    indices = std::move(other.indices);
    vertexCount = other.vertexCount;
    indexCount = other.indexCount;
    VAO = other.VAO;
    VBO = other.VBO;
    EBO = other.EBO;
    
    // Invalidate other's handles
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
  }
  return *this;
}