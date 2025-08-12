#include "Planet.h"
#include <GL/glew.h>
#include <iostream>

Planet::Planet(const PlanetSpec& planetSpec) 
    : spec(planetSpec), VAO(0), VBO(0), EBO(0), rotationSpeed(10.0f), 
      currentRotation(0.0f), m_meshSetup(false) {
    
    // Set rotation speed based on planet type
    switch (spec.type) {
        case TERRESTRIAL: rotationSpeed = 15.0f; break;
        case GAS_GIANT: rotationSpeed = 30.0f; break;
        case DESERT: rotationSpeed = 12.0f; break;
        case ICE: rotationSpeed = 8.0f; break;
        case VOLCANIC: rotationSpeed = 20.0f; break;
    }
}

Planet::~Planet() {
    if (m_meshSetup) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
}

void Planet::Update(double deltaTime) {
    currentRotation += rotationSpeed * deltaTime;
    if (currentRotation >= 360.0f) {
        currentRotation -= 360.0f;
    }
}

void Planet::SetupMesh() {
    if (m_meshSetup) return;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    
    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);
    
    // Color attribute
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(3);
    
    glBindVertexArray(0);
    m_meshSetup = true;
    
    std::cout << "Planet mesh setup complete. Vertices: " << vertices.size() << ", Indices: " << indices.size() << std::endl;
}
