#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

class ViewManager {
public:
    ViewManager();
    ~ViewManager();

    // Initialization
    void Initialize(int width, int height);
    void Cleanup();

    // Viewport management
    void UpdateViewport(int width, int height);
    void SetViewport(int x, int y, int width, int height);
    
    // Projection matrix
    glm::mat4 GetProjectionMatrix() const;
    void SetProjectionMatrix(const glm::mat4& projection);
    
    // View properties
    void SetFieldOfView(float fov);
    void SetNearPlane(float near);
    void SetFarPlane(float far);
    void SetAspectRatio(float aspect);
    
    // Viewport properties
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    float GetAspectRatio() const { return m_aspectRatio; }

private:
    int m_width;
    int m_height;
    float m_aspectRatio;
    float m_fieldOfView;
    float m_nearPlane;
    float m_farPlane;
    glm::mat4 m_projectionMatrix;
    
    void UpdateProjectionMatrix();
};
