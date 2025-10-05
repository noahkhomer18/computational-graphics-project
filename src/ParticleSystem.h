#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <random>

class ShaderManager;

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec4 color;
    float life;
    float maxLife;
    float size;
    float rotation;
    float rotationSpeed;
    
    Particle() : life(0.0f), maxLife(1.0f), size(1.0f), rotation(0.0f), rotationSpeed(0.0f) {}
};

class ParticleSystem {
public:
    ParticleSystem();
    ~ParticleSystem();

    // Initialization
    void Initialize();
    void Cleanup();

    // Particle management
    void Emit(const glm::vec3& position, int count = 1);
    void Update(float deltaTime);
    void Render(ShaderManager& shader, const glm::mat4& view, const glm::mat4& projection);
    
    // System properties
    void SetEmissionRate(float rate) { m_emissionRate = rate; }
    void SetParticleLife(float minLife, float maxLife);
    void SetParticleSize(float minSize, float maxSize);
    void SetParticleColor(const glm::vec4& color) { m_particleColor = color; }
    void SetVelocityRange(const glm::vec3& minVel, const glm::vec3& maxVel);
    void SetAcceleration(const glm::vec3& accel) { m_acceleration = accel; }
    
    // System state
    void Start() { m_active = true; }
    void Stop() { m_active = false; }
    void Reset();
    void SetPosition(const glm::vec3& position) { m_position = position; }
    
    // Rendering properties
    void SetBlending(bool enabled) { m_blending = enabled; }
    void SetDepthTest(bool enabled) { m_depthTest = enabled; }

private:
    // Particle data
    std::vector<Particle> m_particles;
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec4> m_colors;
    
    // System properties
    glm::vec3 m_position;
    glm::vec3 m_velocityMin;
    glm::vec3 m_velocityMax;
    glm::vec3 m_acceleration;
    glm::vec4 m_particleColor;
    float m_emissionRate;
    float m_lifeMin;
    float m_lifeMax;
    float m_sizeMin;
    float m_sizeMax;
    bool m_active;
    
    // Rendering
    GLuint m_VAO, m_VBO, m_colorVBO;
    bool m_blending;
    bool m_depthTest;
    
    // Random number generation
    std::mt19937 m_rng;
    std::uniform_real_distribution<float> m_distribution;
    
    // Helper methods
    void CreateParticle(const glm::vec3& position);
    void UpdateParticle(Particle& particle, float deltaTime);
    void RemoveDeadParticles();
    void UpdateBuffers();
    void SetupBuffers();
    float RandomFloat(float min, float max);
    glm::vec3 RandomVector(const glm::vec3& min, const glm::vec3& max);
};
