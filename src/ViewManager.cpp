#include "ViewManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

ViewManager::ViewManager() 
    : m_width(1200), m_height(800), m_aspectRatio(1.5f), 
      m_fieldOfView(45.0f), m_nearPlane(0.1f), m_farPlane(100.0f) {
}

ViewManager::~ViewManager() {
    Cleanup();
}

void ViewManager::Initialize(int width, int height) {
    std::cout << "Initializing View Manager..." << std::endl;
    UpdateViewport(width, height);
}

void ViewManager::Cleanup() {
    std::cout << "Cleaning up View Manager..." << std::endl;
}

void ViewManager::UpdateViewport(int width, int height) {
    m_width = width;
    m_height = height;
    m_aspectRatio = (float)width / (float)height;
    
    glViewport(0, 0, width, height);
    UpdateProjectionMatrix();
    
    std::cout << "Viewport updated: " << width << "x" << height << std::endl;
}

void ViewManager::SetViewport(int x, int y, int width, int height) {
    m_width = width;
    m_height = height;
    m_aspectRatio = (float)width / (float)height;
    
    glViewport(x, y, width, height);
    UpdateProjectionMatrix();
}

glm::mat4 ViewManager::GetProjectionMatrix() const {
    return m_projectionMatrix;
}

void ViewManager::SetProjectionMatrix(const glm::mat4& projection) {
    m_projectionMatrix = projection;
}

void ViewManager::SetFieldOfView(float fov) {
    m_fieldOfView = fov;
    UpdateProjectionMatrix();
}

void ViewManager::SetNearPlane(float near) {
    m_nearPlane = near;
    UpdateProjectionMatrix();
}

void ViewManager::SetFarPlane(float far) {
    m_farPlane = far;
    UpdateProjectionMatrix();
}

void ViewManager::SetAspectRatio(float aspect) {
    m_aspectRatio = aspect;
    UpdateProjectionMatrix();
}

void ViewManager::UpdateProjectionMatrix() {
    m_projectionMatrix = glm::perspective(
        glm::radians(m_fieldOfView), 
        m_aspectRatio, 
        m_nearPlane, 
        m_farPlane
    );
}
