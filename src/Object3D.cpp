#include "Object3D.h"
#include "ShaderManager.h"
#include <algorithm>

Object3D::Object3D(const std::string& name) 
    : name(name), position(0.0f), rotation(0.0f), scale(1.0f), 
      color(1.0f), shininess(32.0f), useTexture(false), visible(true),
      m_boundingBoxMin(-0.5f), m_boundingBoxMax(0.5f) {
}

void Object3D::SetPosition(const glm::vec3& pos) {
    position = pos;
}

void Object3D::SetRotation(const glm::vec3& rot) {
    rotation = rot;
}

void Object3D::SetScale(const glm::vec3& scl) {
    scale = scl;
}

void Object3D::Translate(const glm::vec3& translation) {
    position += translation;
}

void Object3D::Rotate(const glm::vec3& rotation) {
    this->rotation += rotation;
}

void Object3D::Scale(const glm::vec3& scaling) {
    scale *= scaling;
}

glm::mat4 Object3D::GetModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    
    // Apply transformations in order: Scale, Rotate, Translate
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);
    
    return model;
}

glm::mat4 Object3D::GetWorldMatrix() const {
    glm::mat4 worldMatrix = GetModelMatrix();
    
    // Apply parent transformations
    if (auto parent = m_parent.lock()) {
        worldMatrix = parent->GetWorldMatrix() * worldMatrix;
    }
    
    return worldMatrix;
}

void Object3D::Update(float deltaTime) {
    // Update children first
    UpdateChildren(deltaTime);
    
    // Override in derived classes for custom update logic
}

void Object3D::Render(ShaderManager& shader) {
    if (!visible) return;
    
    // Set model matrix
    glm::mat4 modelMatrix = GetWorldMatrix();
    shader.setMat4Value("model", modelMatrix);
    
    // Set material properties
    shader.setVec3Value("material.ambient", color * 0.1f);
    shader.setVec3Value("material.diffuse", color);
    shader.setVec3Value("material.specular", color * 0.5f);
    shader.setFloatValue("material.shininess", shininess);
    shader.setBoolValue("useTexture", useTexture);
    
    // Render children
    RenderChildren(shader, modelMatrix);
}

void Object3D::Render(ShaderManager& shader, const glm::mat4& parentMatrix) {
    if (!visible) return;
    
    // Calculate world matrix
    glm::mat4 worldMatrix = parentMatrix * GetModelMatrix();
    shader.setMat4Value("model", worldMatrix);
    
    // Set material properties
    shader.setVec3Value("material.ambient", color * 0.1f);
    shader.setVec3Value("material.diffuse", color);
    shader.setVec3Value("material.specular", color * 0.5f);
    shader.setFloatValue("material.shininess", shininess);
    shader.setBoolValue("useTexture", useTexture);
    
    // Render children
    RenderChildren(shader, worldMatrix);
}

void Object3D::AddChild(std::shared_ptr<Object3D> child) {
    if (child) {
        child->SetParent(shared_from_this());
        m_children.push_back(child);
    }
}

void Object3D::RemoveChild(const std::string& childName) {
    m_children.erase(
        std::remove_if(m_children.begin(), m_children.end(),
            [&childName](const std::shared_ptr<Object3D>& child) {
                return child->name == childName;
            }),
        m_children.end()
    );
}

std::shared_ptr<Object3D> Object3D::GetChild(const std::string& childName) {
    for (auto& child : m_children) {
        if (child->name == childName) {
            return child;
        }
    }
    return nullptr;
}

void Object3D::SetParent(std::shared_ptr<Object3D> parent) {
    m_parent = parent;
}

void Object3D::SetBoundingBox(const glm::vec3& min, const glm::vec3& max) {
    m_boundingBoxMin = min;
    m_boundingBoxMax = max;
}

void Object3D::UpdateChildren(float deltaTime) {
    for (auto& child : m_children) {
        child->Update(deltaTime);
    }
}

void Object3D::RenderChildren(ShaderManager& shader, const glm::mat4& parentMatrix) {
    for (auto& child : m_children) {
        child->Render(shader, parentMatrix);
    }
}
