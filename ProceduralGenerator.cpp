#include "ProceduralGenerator.h"
#include "FastNoise.h"
#include <GL/glew.h>
#include <cmath>
#include <iostream>

ProceduralGenerator::ProceduralGenerator(unsigned int seed) : m_generator(seed) {}

void ProceduralGenerator::GeneratePlanet(Planet& planet) {
    m_generator.seed(planet.spec.seed);
    
    std::cout << "Generating planet of type " << planet.spec.type << " with radius " << planet.spec.radius << std::endl;
    
    // Generate base sphere mesh
    GenerateSphereMesh(planet, 64); // 64x64 subdivision
    
    // Apply procedural height and color
    ApplyHeightmap(planet);
    
    // Setup OpenGL buffers
    planet.SetupMesh();
    
    std::cout << "Planet generation complete!" << std::endl;
}

void ProceduralGenerator::GenerateSphereMesh(Planet& planet, int subdivisions) {
    planet.vertices.clear();
    planet.indices.clear();
    
    // Generate vertices
    for (int lat = 0; lat <= subdivisions; ++lat) {
        float theta = lat * M_PI / subdivisions;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);
        
        for (int lon = 0; lon <= subdivisions; ++lon) {
            float phi = lon * 2.0f * M_PI / subdivisions;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);
            
            Vertex vertex;
            vertex.position = glm::vec3(
                planet.spec.radius * sinTheta * cosPhi,
                planet.spec.radius * cosTheta,
                planet.spec.radius * sinTheta * sinPhi
            );
            vertex.normal = glm::normalize(vertex.position);
            vertex.texCoord = glm::vec2(
                (float)lon / subdivisions,
                (float)lat / subdivisions
            );
            vertex.color = glm::vec3(0.5f, 0.5f, 0.5f); // Will be modified by heightmap
            
            planet.vertices.push_back(vertex);
        }
    }
    
    // Generate indices
    for (int lat = 0; lat < subdivisions; ++lat) {
        for (int lon = 0; lon < subdivisions; ++lon) {
            int current = lat * (subdivisions + 1) + lon;
            int next = current + subdivisions + 1;
            
            // First triangle
            planet.indices.push_back(current);
            planet.indices.push_back(next);
            planet.indices.push_back(current + 1);
            
            // Second triangle
            planet.indices.push_back(current + 1);
            planet.indices.push_back(next);
            planet.indices.push_back(next + 1);
        }
    }
}

void ProceduralGenerator::ApplyHeightmap(Planet& planet) {
    FastNoise noise(planet.spec.seed);
    noise.SetFrequency(0.02f);
    
    for (auto& vertex : planet.vertices) {
        glm::vec3 normalizedPos = glm::normalize(vertex.position);
        
        // Generate multiple octaves of noise
        float height = 0.0f;
        float amplitude = 1.0f;
        float frequency = 1.0f;
        
        for (int octave = 0; octave < 6; ++octave) {
            height += noise.GetSimplexNoise(
                normalizedPos.x * frequency,
                normalizedPos.y * frequency,
                normalizedPos.z * frequency
            ) * amplitude;
            
            amplitude *= 0.5f;
            frequency *= 2.0f;
        }
        
        // Apply height to vertex position
        float heightMultiplier = planet.spec.radius * 0.1f; // 10% variation
        vertex.position += vertex.normal * (height * heightMultiplier);
        
        // Update normal (simplified - should be recalculated properly)
        vertex.normal = glm::normalize(vertex.position);
        
        // Apply color based on planet type and height
        float temperature = 0.5f + height * 0.5f; // Simple temperature model
        vertex.color = GetPlanetColor(planet.spec.type, height, temperature);
    }
}

glm::vec3 ProceduralGenerator::GetPlanetColor(PlanetType type, float height, float temperature) {
    switch (type) {
        case TERRESTRIAL:
            if (height < -0.2f) return glm::vec3(0.0f, 0.3f, 0.7f); // Ocean
            if (height < 0.0f) return glm::vec3(0.8f, 0.7f, 0.3f);  // Beach
            if (height < 0.3f) return glm::vec3(0.2f, 0.7f, 0.1f);  // Forest
            return glm::vec3(0.6f, 0.4f, 0.2f); // Mountain
            
        case DESERT:
            return glm::vec3(0.9f, 0.7f, 0.3f + height * 0.2f);
            
        case GAS_GIANT:
            return glm::vec3(0.8f + height * 0.2f, 0.6f, 0.4f);
            
        case ICE:
            return glm::vec3(0.9f, 0.9f, 1.0f - height * 0.3f);
            
        case VOLCANIC:
            return glm::vec3(0.8f, 0.3f + height * 0.4f, 0.1f);
            
        default:
            return glm::vec3(0.5f, 0.5f, 0.5f);
    }
}
