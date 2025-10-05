#include "ShadowMapper.h"
#include "ShaderManager.h"
#include <iostream>

ShadowMapper::ShadowMapper() 
    : m_shadowFBO(0), m_shadowMap(0), m_shadowWidth(1024), m_shadowHeight(1024),
      m_lightDirection(0.0f, -1.0f, 0.0f), m_lightPosition(0.0f, 10.0f, 0.0f),
      m_shadowBias(0.005f), m_shadowDistance(50.0f), m_nearPlane(1.0f), m_farPlane(100.0f) {
}

ShadowMapper::~ShadowMapper() {
    Cleanup();
}

void ShadowMapper::Initialize(int shadowWidth, int shadowHeight) {
    std::cout << "Initializing Shadow Mapper..." << std::endl;
    
    m_shadowWidth = shadowWidth;
    m_shadowHeight = shadowHeight;
    
    SetupShadowFramebuffer();
    
    // Initialize shadow shader
    m_shadowShader = std::make_unique<ShaderManager>();
    // Note: In a real implementation, you would load shadow-specific shaders
    // For now, we'll use the existing shader system
    
    CalculateLightSpaceMatrix();
}

void ShadowMapper::Cleanup() {
    if (m_shadowFBO) {
        glDeleteFramebuffers(1, &m_shadowFBO);
    }
    if (m_shadowMap) {
        glDeleteTextures(1, &m_shadowMap);
    }
}

void ShadowMapper::BeginShadowPass() {
    // Bind shadow framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
    glViewport(0, 0, m_shadowWidth, m_shadowHeight);
    
    // Clear depth buffer
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Cull front faces to reduce shadow acne
    glCullFace(GL_FRONT);
}

void ShadowMapper::EndShadowPass() {
    // Restore cull face
    glCullFace(GL_BACK);
    
    // Unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapper::RenderShadows(ShaderManager& shader, const glm::mat4& view, const glm::mat4& projection) {
    // Set light space matrix for shadow sampling
    shader.setMat4Value("lightSpaceMatrix", m_lightSpaceMatrix);
    shader.setFloatValue("shadowBias", m_shadowBias);
    
    // Bind shadow map texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_shadowMap);
    shader.setIntValue("shadowMap", 1);
}

void ShadowMapper::SetLightDirection(const glm::vec3& direction) {
    m_lightDirection = glm::normalize(direction);
    CalculateLightSpaceMatrix();
}

void ShadowMapper::SetLightPosition(const glm::vec3& position) {
    m_lightPosition = position;
    CalculateLightSpaceMatrix();
}

void ShadowMapper::SetShadowMapSize(int width, int height) {
    m_shadowWidth = width;
    m_shadowHeight = height;
    
    // Recreate shadow map with new size
    if (m_shadowMap) {
        glDeleteTextures(1, &m_shadowMap);
    }
    if (m_shadowFBO) {
        glDeleteFramebuffers(1, &m_shadowFBO);
    }
    
    SetupShadowFramebuffer();
}

void ShadowMapper::CalculateLightSpaceMatrix() {
    // Calculate light space matrix for directional light
    glm::mat4 lightProjection = glm::ortho(-m_shadowDistance, m_shadowDistance, 
                                          -m_shadowDistance, m_shadowDistance, 
                                          m_nearPlane, m_farPlane);
    
    glm::mat4 lightView = glm::lookAt(m_lightPosition, 
                                     m_lightPosition + m_lightDirection, 
                                     glm::vec3(0.0f, 1.0f, 0.0f));
    
    m_lightSpaceMatrix = lightProjection * lightView;
}

void ShadowMapper::SetupShadowFramebuffer() {
    // Create shadow map texture
    glGenTextures(1, &m_shadowMap);
    glBindTexture(GL_TEXTURE_2D, m_shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                 m_shadowWidth, m_shadowHeight, 0, 
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    
    // Set border color to white (no shadow)
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    // Create framebuffer
    glGenFramebuffers(1, &m_shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
    
    // Attach depth texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);
    
    // No color buffer needed for depth-only rendering
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    // Check framebuffer completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR: Shadow framebuffer not complete!" << std::endl;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapper::RenderSceneToShadowMap(ShaderManager& shader) {
    // This would be called during shadow pass to render scene from light's perspective
    // Implementation depends on the scene rendering system
}
