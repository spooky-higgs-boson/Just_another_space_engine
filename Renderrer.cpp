#include "Renderer.h"
#include <iostream>

Renderer::Renderer() : m_starFieldInitialized(false), 
                       m_lightPos(0.0f, 0.0f, 100.0f), 
                       m_lightColor(1.0f, 1.0f, 0.9f) {}

Renderer::~Renderer() {
    if (m_starFieldInitialized) {
        glDeleteVertexArrays(1, &m_starVAO);
        glDeleteBuffers(1, &m_starVBO);
    }
}

bool Renderer::Initialize() {
    try {
        m_planetShader = std::make_unique<Shader>("shaders/planet_vertex.glsl", "shaders/planet_fragment.glsl");
        m_starShader = std::make_unique<Shader>("shaders/star_vertex.glsl", "shaders/star_fragment.glsl");
        
        std::cout << "Shaders loaded successfully!" << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to initialize renderer: " << e.what() << std::endl;
        return false;
    }
}

void Renderer::RenderPlanet(const Planet& planet, const glm::mat4& view, const glm::mat4& projection) {
    m_planetShader->use();
    
    // Set matrices
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, planet.spec.position);
    model = glm::rotate(model, glm::radians(planet.currentRotation), glm::vec3(0.0f, 1.0f, 0.0f));
    
    m_planetShader->setMat4("uModel", model);
    m_planetShader->setMat4("uView", view);
    m_planetShader->setMat4("uProjection", projection);
    
    // Set lighting
    m_planetShader->setVec3("uLightPos", m_lightPos);
    m_planetShader->setVec3("uLightColor", m_lightColor);
    
    // Extract camera position from view matrix
    glm::mat4 invView = glm::inverse(view);
    glm::vec3 cameraPos = glm::vec3(invView[3]);
    m_planetShader->setVec3("uViewPos", cameraPos);
    
    // Render planet
    glBindVertexArray(planet.VAO);
    glDrawElements(GL_TRIANGLES, planet.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::RenderStarField(const std::vector<glm::vec3>& stars, const glm::mat4& view, const glm::mat4& projection) {
    if (!m_starFieldInitialized) {
        InitializeStarField(stars);
    }
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    m_starShader->use();
    m_starShader->setMat4("uView", view);
    m_starShader->setMat4("uProjection", projection);
    
    glBindVertexArray(m_starVAO);
    glDrawArrays(GL_POINTS, 0, stars.size());
    glBindVertexArray(0);
    
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::InitializeStarField(const std::vector<glm::vec3>& stars) {
    glGenVertexArrays(1, &m_starVAO);
    glGenBuffers(1, &m_starVBO);
    
    glBindVertexArray(m_starVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_starVBO);
    glBufferData(GL_ARRAY_BUFFER, stars.size() * sizeof(glm::vec3), stars.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
    m_starFieldInitialized = true;
}
