#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "Shader.h"
#include "Planet.h"

class Renderer {
private:
    std::unique_ptr<Shader> m_planetShader;
    std::unique_ptr<Shader> m_starShader;
    
    // Star field rendering
    unsigned int m_starVAO, m_starVBO;
    bool m_starFieldInitialized;
    
    // Lighting
    glm::vec3 m_lightPos;
    glm::vec3 m_lightColor;

public:
    Renderer();
    ~Renderer();
    
    bool Initialize();
    void RenderPlanet(const Planet& planet, const glm::mat4& view, const glm::mat4& projection);
    void RenderStarField(const std::vector<glm::vec3>& stars, const glm::mat4& view, const glm::mat4& projection);
    
    void SetLightPosition(const glm::vec3& position) { m_lightPos = position; }
    void SetLightColor(const glm::vec3& color) { m_lightColor = color; }

private:
    void InitializeStarField(const std::vector<glm::vec3>& stars);
};

#endif
