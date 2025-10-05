#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <memory>

class ShaderManager;

class Object3D {
public:
    Object3D(const std::string& name);
    virtual ~Object3D() = default;

    // Transform properties
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    
    // Material properties
    glm::vec3 color;
    float shininess;
    bool useTexture;
    
    // Object identification
    std::string name;
    bool visible;
    
    // Transform methods
    void SetPosition(const glm::vec3& pos);
    void SetRotation(const glm::vec3& rot);
    void SetScale(const glm::vec3& scl);
    void Translate(const glm::vec3& translation);
    void Rotate(const glm::vec3& rotation);
    void Scale(const glm::vec3& scaling);
    
    // Matrix calculations
    glm::mat4 GetModelMatrix() const;
    glm::mat4 GetWorldMatrix() const;
    
    // Rendering
    virtual void Update(float deltaTime);
    virtual void Render(ShaderManager& shader);
    virtual void Render(ShaderManager& shader, const glm::mat4& parentMatrix);
    
    // Object hierarchy
    void AddChild(std::shared_ptr<Object3D> child);
    void RemoveChild(const std::string& childName);
    std::shared_ptr<Object3D> GetChild(const std::string& childName);
    
    // Parent relationship
    void SetParent(std::shared_ptr<Object3D> parent);
    std::shared_ptr<Object3D> GetParent() const { return m_parent.lock(); }
    
    // Bounding box
    glm::vec3 GetBoundingBoxMin() const { return m_boundingBoxMin; }
    glm::vec3 GetBoundingBoxMax() const { return m_boundingBoxMax; }
    void SetBoundingBox(const glm::vec3& min, const glm::vec3& max);

protected:
    std::vector<std::shared_ptr<Object3D>> m_children;
    std::weak_ptr<Object3D> m_parent;
    glm::vec3 m_boundingBoxMin;
    glm::vec3 m_boundingBoxMax;
    
    void UpdateChildren(float deltaTime);
    void RenderChildren(ShaderManager& shader, const glm::mat4& parentMatrix);
};
