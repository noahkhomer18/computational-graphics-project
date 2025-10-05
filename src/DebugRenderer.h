#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <GL/glew.h>

class ShaderManager;

class DebugRenderer {
public:
    DebugRenderer();
    ~DebugRenderer();

    // Initialization
    void Initialize();
    void Cleanup();

    // Debug drawing
    void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color = glm::vec3(1.0f));
    void DrawWireframeBox(const glm::vec3& min, const glm::vec3& max, const glm::vec3& color = glm::vec3(1.0f));
    void DrawWireframeSphere(const glm::vec3& center, float radius, const glm::vec3& color = glm::vec3(1.0f));
    void DrawWireframeCylinder(const glm::vec3& center, float radius, float height, const glm::vec3& color = glm::vec3(1.0f));
    void DrawGrid(int size, float spacing, const glm::vec3& color = glm::vec3(0.5f));
    void DrawAxis(const glm::vec3& position, float length = 1.0f);
    void DrawBoundingBox(const glm::vec3& min, const glm::vec3& max, const glm::vec3& color = glm::vec3(1.0f, 0.0f, 0.0f));

    // Text rendering (simplified)
    void DrawText(const std::string& text, const glm::vec2& position, float scale = 1.0f, const glm::vec3& color = glm::vec3(1.0f));

    // Rendering
    void Render(ShaderManager& shader, const glm::mat4& view, const glm::mat4& projection);
    void Clear();

    // Settings
    void SetLineWidth(float width);
    void SetDepthTest(bool enabled);
    void SetWireframeMode(bool enabled);

private:
    // Line rendering
    std::vector<glm::vec3> m_lineVertices;
    std::vector<glm::vec3> m_lineColors;
    GLuint m_lineVAO, m_lineVBO;
    
    // Box rendering
    std::vector<glm::vec3> m_boxVertices;
    std::vector<glm::vec3> m_boxColors;
    GLuint m_boxVAO, m_boxVBO;
    
    // Settings
    float m_lineWidth;
    bool m_depthTest;
    bool m_wireframeMode;
    
    // Helper methods
    void SetupLineBuffers();
    void SetupBoxBuffers();
    void CreateBoxVertices(const glm::vec3& min, const glm::vec3& max);
    void CreateSphereVertices(const glm::vec3& center, float radius, int segments = 16);
    void CreateCylinderVertices(const glm::vec3& center, float radius, float height, int segments = 16);
};
