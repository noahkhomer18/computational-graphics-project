#include "DebugRenderer.h"
#include "ShaderManager.h"
#include <iostream>
#include <cmath>

DebugRenderer::DebugRenderer() 
    : m_lineWidth(1.0f), m_depthTest(true), m_wireframeMode(false) {
    m_lineVAO = m_lineVBO = 0;
    m_boxVAO = m_boxVBO = 0;
}

DebugRenderer::~DebugRenderer() {
    Cleanup();
}

void DebugRenderer::Initialize() {
    std::cout << "Initializing Debug Renderer..." << std::endl;
    
    SetupLineBuffers();
    SetupBoxBuffers();
}

void DebugRenderer::Cleanup() {
    if (m_lineVAO) {
        glDeleteVertexArrays(1, &m_lineVAO);
        glDeleteBuffers(1, &m_lineVBO);
    }
    if (m_boxVAO) {
        glDeleteVertexArrays(1, &m_boxVAO);
        glDeleteBuffers(1, &m_boxVBO);
    }
}

void DebugRenderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color) {
    m_lineVertices.push_back(start);
    m_lineVertices.push_back(end);
    m_lineColors.push_back(color);
    m_lineColors.push_back(color);
}

void DebugRenderer::DrawWireframeBox(const glm::vec3& min, const glm::vec3& max, const glm::vec3& color) {
    // Create 12 lines for a wireframe box
    glm::vec3 vertices[8] = {
        glm::vec3(min.x, min.y, min.z), // 0
        glm::vec3(max.x, min.y, min.z), // 1
        glm::vec3(max.x, max.y, min.z), // 2
        glm::vec3(min.x, max.y, min.z), // 3
        glm::vec3(min.x, min.y, max.z), // 4
        glm::vec3(max.x, min.y, max.z), // 5
        glm::vec3(max.x, max.y, max.z), // 6
        glm::vec3(min.x, max.y, max.z)  // 7
    };
    
    // Bottom face
    DrawLine(vertices[0], vertices[1], color);
    DrawLine(vertices[1], vertices[2], color);
    DrawLine(vertices[2], vertices[3], color);
    DrawLine(vertices[3], vertices[0], color);
    
    // Top face
    DrawLine(vertices[4], vertices[5], color);
    DrawLine(vertices[5], vertices[6], color);
    DrawLine(vertices[6], vertices[7], color);
    DrawLine(vertices[7], vertices[4], color);
    
    // Vertical edges
    DrawLine(vertices[0], vertices[4], color);
    DrawLine(vertices[1], vertices[5], color);
    DrawLine(vertices[2], vertices[6], color);
    DrawLine(vertices[3], vertices[7], color);
}

void DebugRenderer::DrawWireframeSphere(const glm::vec3& center, float radius, const glm::vec3& color) {
    const int segments = 16;
    const float angleStep = 2.0f * M_PI / segments;
    
    // Draw circles around X, Y, Z axes
    for (int i = 0; i < segments; i++) {
        float angle1 = i * angleStep;
        float angle2 = (i + 1) * angleStep;
        
        // XY plane circle
        glm::vec3 p1 = center + glm::vec3(radius * cos(angle1), radius * sin(angle1), 0.0f);
        glm::vec3 p2 = center + glm::vec3(radius * cos(angle2), radius * sin(angle2), 0.0f);
        DrawLine(p1, p2, color);
        
        // XZ plane circle
        p1 = center + glm::vec3(radius * cos(angle1), 0.0f, radius * sin(angle1));
        p2 = center + glm::vec3(radius * cos(angle2), 0.0f, radius * sin(angle2));
        DrawLine(p1, p2, color);
        
        // YZ plane circle
        p1 = center + glm::vec3(0.0f, radius * cos(angle1), radius * sin(angle1));
        p2 = center + glm::vec3(0.0f, radius * cos(angle2), radius * sin(angle2));
        DrawLine(p1, p2, color);
    }
}

void DebugRenderer::DrawWireframeCylinder(const glm::vec3& center, float radius, float height, const glm::vec3& color) {
    const int segments = 16;
    const float angleStep = 2.0f * M_PI / segments;
    
    glm::vec3 topCenter = center + glm::vec3(0.0f, height * 0.5f, 0.0f);
    glm::vec3 bottomCenter = center - glm::vec3(0.0f, height * 0.5f, 0.0f);
    
    // Draw top and bottom circles
    for (int i = 0; i < segments; i++) {
        float angle1 = i * angleStep;
        float angle2 = (i + 1) * angleStep;
        
        glm::vec3 top1 = topCenter + glm::vec3(radius * cos(angle1), 0.0f, radius * sin(angle1));
        glm::vec3 top2 = topCenter + glm::vec3(radius * cos(angle2), 0.0f, radius * sin(angle2));
        DrawLine(top1, top2, color);
        
        glm::vec3 bottom1 = bottomCenter + glm::vec3(radius * cos(angle1), 0.0f, radius * sin(angle1));
        glm::vec3 bottom2 = bottomCenter + glm::vec3(radius * cos(angle2), 0.0f, radius * sin(angle2));
        DrawLine(bottom1, bottom2, color);
        
        // Vertical lines
        DrawLine(top1, bottom1, color);
    }
}

void DebugRenderer::DrawGrid(int size, float spacing, const glm::vec3& color) {
    float halfSize = size * spacing * 0.5f;
    
    // Draw grid lines
    for (int i = -size; i <= size; i++) {
        float pos = i * spacing;
        
        // X-axis lines
        DrawLine(glm::vec3(-halfSize, 0.0f, pos), glm::vec3(halfSize, 0.0f, pos), color);
        // Z-axis lines
        DrawLine(glm::vec3(pos, 0.0f, -halfSize), glm::vec3(pos, 0.0f, halfSize), color);
    }
}

void DebugRenderer::DrawAxis(const glm::vec3& position, float length) {
    // X-axis (red)
    DrawLine(position, position + glm::vec3(length, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // Y-axis (green)
    DrawLine(position, position + glm::vec3(0.0f, length, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // Z-axis (blue)
    DrawLine(position, position + glm::vec3(0.0f, 0.0f, length), glm::vec3(0.0f, 0.0f, 1.0f));
}

void DebugRenderer::DrawBoundingBox(const glm::vec3& min, const glm::vec3& max, const glm::vec3& color) {
    DrawWireframeBox(min, max, color);
}

void DebugRenderer::DrawText(const std::string& text, const glm::vec2& position, float scale, const glm::vec3& color) {
    // Simplified text rendering - in a real implementation you'd use a font atlas
    std::cout << "Debug Text: " << text << " at (" << position.x << ", " << position.y << ")" << std::endl;
}

void DebugRenderer::Render(ShaderManager& shader, const glm::mat4& view, const glm::mat4& projection) {
    if (m_lineVertices.empty()) return;
    
    // Set matrices
    shader.setMat4Value("view", view);
    shader.setMat4Value("projection", projection);
    shader.setMat4Value("model", glm::mat4(1.0f));
    
    // Set line width
    glLineWidth(m_lineWidth);
    
    // Enable/disable depth test
    if (m_depthTest) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    
    // Render lines
    glBindVertexArray(m_lineVAO);
    glBufferData(GL_ARRAY_BUFFER, m_lineVertices.size() * sizeof(glm::vec3), m_lineVertices.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, m_lineVertices.size());
    glBindVertexArray(0);
}

void DebugRenderer::Clear() {
    m_lineVertices.clear();
    m_lineColors.clear();
    m_boxVertices.clear();
    m_boxColors.clear();
}

void DebugRenderer::SetLineWidth(float width) {
    m_lineWidth = width;
}

void DebugRenderer::SetDepthTest(bool enabled) {
    m_depthTest = enabled;
}

void DebugRenderer::SetWireframeMode(bool enabled) {
    m_wireframeMode = enabled;
}

void DebugRenderer::SetupLineBuffers() {
    glGenVertexArrays(1, &m_lineVAO);
    glGenBuffers(1, &m_lineVBO);
    
    glBindVertexArray(m_lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

void DebugRenderer::SetupBoxBuffers() {
    glGenVertexArrays(1, &m_boxVAO);
    glGenBuffers(1, &m_boxVBO);
    
    glBindVertexArray(m_boxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_boxVBO);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}
