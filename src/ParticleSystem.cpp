#include "ParticleSystem.h"
#include "ShaderManager.h"
#include <algorithm>
#include <iostream>

ParticleSystem::ParticleSystem() 
    : m_position(0.0f), m_velocityMin(-1.0f), m_velocityMax(1.0f), m_acceleration(0.0f, -9.81f, 0.0f),
      m_particleColor(1.0f, 1.0f, 1.0f, 1.0f), m_emissionRate(10.0f), m_lifeMin(1.0f), m_lifeMax(3.0f),
      m_sizeMin(0.1f), m_sizeMax(0.5f), m_active(false), m_VAO(0), m_VBO(0), m_colorVBO(0),
      m_blending(true), m_depthTest(false), m_rng(std::random_device{}()), m_distribution(0.0f, 1.0f) {
}

ParticleSystem::~ParticleSystem() {
    Cleanup();
}

void ParticleSystem::Initialize() {
    std::cout << "Initializing Particle System..." << std::endl;
    
    SetupBuffers();
    
    // Reserve space for particles
    m_particles.reserve(1000);
    m_vertices.reserve(1000 * 4); // 4 vertices per particle (quad)
    m_colors.reserve(1000 * 4);
}

void ParticleSystem::Cleanup() {
    if (m_VAO) {
        glDeleteVertexArrays(1, &m_VAO);
    }
    if (m_VBO) {
        glDeleteBuffers(1, &m_VBO);
    }
    if (m_colorVBO) {
        glDeleteBuffers(1, &m_colorVBO);
    }
}

void ParticleSystem::Emit(const glm::vec3& position, int count) {
    for (int i = 0; i < count; i++) {
        CreateParticle(position);
    }
}

void ParticleSystem::Update(float deltaTime) {
    if (!m_active) return;
    
    // Emit new particles based on emission rate
    float emissionCount = m_emissionRate * deltaTime;
    int particlesToEmit = static_cast<int>(emissionCount);
    
    if (emissionCount - particlesToEmit > RandomFloat(0.0f, 1.0f)) {
        particlesToEmit++;
    }
    
    for (int i = 0; i < particlesToEmit; i++) {
        Emit(m_position, 1);
    }
    
    // Update existing particles
    for (auto& particle : m_particles) {
        UpdateParticle(particle, deltaTime);
    }
    
    // Remove dead particles
    RemoveDeadParticles();
    
    // Update vertex buffers
    UpdateBuffers();
}

void ParticleSystem::Render(ShaderManager& shader, const glm::mat4& view, const glm::mat4& projection) {
    if (m_particles.empty()) return;
    
    // Set matrices
    shader.setMat4Value("view", view);
    shader.setMat4Value("projection", projection);
    shader.setMat4Value("model", glm::mat4(1.0f));
    
    // Set rendering state
    if (m_blending) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    
    if (!m_depthTest) {
        glDisable(GL_DEPTH_TEST);
    }
    
    // Render particles
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    glBindVertexArray(0);
    
    // Restore state
    if (m_blending) {
        glDisable(GL_BLEND);
    }
    
    if (!m_depthTest) {
        glEnable(GL_DEPTH_TEST);
    }
}

void ParticleSystem::SetParticleLife(float minLife, float maxLife) {
    m_lifeMin = minLife;
    m_lifeMax = maxLife;
}

void ParticleSystem::SetParticleSize(float minSize, float maxSize) {
    m_sizeMin = minSize;
    m_sizeMax = maxSize;
}

void ParticleSystem::SetVelocityRange(const glm::vec3& minVel, const glm::vec3& maxVel) {
    m_velocityMin = minVel;
    m_velocityMax = maxVel;
}

void ParticleSystem::Reset() {
    m_particles.clear();
    m_vertices.clear();
    m_colors.clear();
    UpdateBuffers();
}

void ParticleSystem::CreateParticle(const glm::vec3& position) {
    Particle particle;
    particle.position = position;
    particle.velocity = RandomVector(m_velocityMin, m_velocityMax);
    particle.acceleration = m_acceleration;
    particle.color = m_particleColor;
    particle.maxLife = RandomFloat(m_lifeMin, m_lifeMax);
    particle.life = particle.maxLife;
    particle.size = RandomFloat(m_sizeMin, m_sizeMax);
    particle.rotation = RandomFloat(0.0f, 360.0f);
    particle.rotationSpeed = RandomFloat(-180.0f, 180.0f);
    
    m_particles.push_back(particle);
}

void ParticleSystem::UpdateParticle(Particle& particle, float deltaTime) {
    // Update physics
    particle.velocity += particle.acceleration * deltaTime;
    particle.position += particle.velocity * deltaTime;
    
    // Update rotation
    particle.rotation += particle.rotationSpeed * deltaTime;
    
    // Update life
    particle.life -= deltaTime;
    
    // Update color based on life
    float lifeRatio = particle.life / particle.maxLife;
    particle.color.a = lifeRatio;
    
    // Fade out near end of life
    if (lifeRatio < 0.3f) {
        particle.color.a *= lifeRatio / 0.3f;
    }
}

void ParticleSystem::RemoveDeadParticles() {
    m_particles.erase(
        std::remove_if(m_particles.begin(), m_particles.end(),
            [](const Particle& particle) {
                return particle.life <= 0.0f;
            }),
        m_particles.end()
    );
}

void ParticleSystem::UpdateBuffers() {
    m_vertices.clear();
    m_colors.clear();
    
    for (const auto& particle : m_particles) {
        // Create quad vertices for particle
        float halfSize = particle.size * 0.5f;
        
        // Calculate rotation matrix
        float cosRot = cos(glm::radians(particle.rotation));
        float sinRot = sin(glm::radians(particle.rotation));
        
        // Create 4 vertices for quad
        glm::vec3 v1 = particle.position + glm::vec3(-halfSize, -halfSize, 0.0f);
        glm::vec3 v2 = particle.position + glm::vec3(halfSize, -halfSize, 0.0f);
        glm::vec3 v3 = particle.position + glm::vec3(halfSize, halfSize, 0.0f);
        glm::vec3 v4 = particle.position + glm::vec3(-halfSize, halfSize, 0.0f);
        
        // Apply rotation
        v1 = glm::vec3(v1.x * cosRot - v1.y * sinRot, v1.x * sinRot + v1.y * cosRot, v1.z);
        v2 = glm::vec3(v2.x * cosRot - v2.y * sinRot, v2.x * sinRot + v2.y * cosRot, v2.z);
        v3 = glm::vec3(v3.x * cosRot - v3.y * sinRot, v3.x * sinRot + v3.y * cosRot, v3.z);
        v4 = glm::vec3(v4.x * cosRot - v4.y * sinRot, v4.x * sinRot + v4.y * cosRot, v4.z);
        
        // Add vertices (two triangles)
        m_vertices.push_back(v1);
        m_vertices.push_back(v2);
        m_vertices.push_back(v3);
        
        m_vertices.push_back(v1);
        m_vertices.push_back(v3);
        m_vertices.push_back(v4);
        
        // Add colors
        for (int i = 0; i < 6; i++) {
            m_colors.push_back(particle.color);
        }
    }
    
    // Update OpenGL buffers
    if (!m_vertices.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), 
                     m_vertices.data(), GL_DYNAMIC_DRAW);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
        glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec4), 
                     m_colors.data(), GL_DYNAMIC_DRAW);
    }
}

void ParticleSystem::SetupBuffers() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_colorVBO);
    
    glBindVertexArray(m_VAO);
    
    // Position buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

float ParticleSystem::RandomFloat(float min, float max) {
    return min + m_distribution(m_rng) * (max - min);
}

glm::vec3 ParticleSystem::RandomVector(const glm::vec3& min, const glm::vec3& max) {
    return glm::vec3(
        RandomFloat(min.x, max.x),
        RandomFloat(min.y, max.y),
        RandomFloat(min.z, max.z)
    );
}
