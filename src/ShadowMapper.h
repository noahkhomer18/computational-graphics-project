#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

class ShaderManager;

class ShadowMapper {
public:
    ShadowMapper();
    ~ShadowMapper();

    // Initialization
    void Initialize(int shadowWidth = 1024, int shadowHeight = 1024);
    void Cleanup();

    // Shadow mapping
    void BeginShadowPass();
    void EndShadowPass();
    void RenderShadows(ShaderManager& shader, const glm::mat4& view, const glm::mat4& projection);
    
    // Light setup
    void SetLightDirection(const glm::vec3& direction);
    void SetLightPosition(const glm::vec3& position);
    
    // Shadow properties
    void SetShadowBias(float bias) { m_shadowBias = bias; }
    void SetShadowDistance(float distance) { m_shadowDistance = distance; }
    void SetShadowMapSize(int width, int height);
    
    // Getters
    GLuint GetShadowMap() const { return m_shadowMap; }
    glm::mat4 GetLightSpaceMatrix() const { return m_lightSpaceMatrix; }
    float GetShadowBias() const { return m_shadowBias; }

private:
    // Shadow map properties
    GLuint m_shadowFBO;
    GLuint m_shadowMap;
    int m_shadowWidth;
    int m_shadowHeight;
    
    // Light properties
    glm::vec3 m_lightDirection;
    glm::vec3 m_lightPosition;
    glm::mat4 m_lightSpaceMatrix;
    
    // Shadow settings
    float m_shadowBias;
    float m_shadowDistance;
    float m_nearPlane;
    float m_farPlane;
    
    // Shadow shader
    std::unique_ptr<ShaderManager> m_shadowShader;
    
    // Helper methods
    void CalculateLightSpaceMatrix();
    void SetupShadowFramebuffer();
    void RenderSceneToShadowMap(ShaderManager& shader);
};
