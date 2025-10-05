#pragma once

#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Object3D;
class Light;
class ShaderManager;

class SceneManager {
public:
    SceneManager();
    ~SceneManager();

    // Initialization
    void Initialize();
    void Cleanup();

    // Scene management
    void AddObject(std::shared_ptr<Object3D> object);
    void RemoveObject(const std::string& name);
    std::shared_ptr<Object3D> GetObject(const std::string& name);
    
    // Lighting
    void AddLight(std::shared_ptr<Light> light);
    void RemoveLight(const std::string& name);
    void UpdateLighting(ShaderManager& shader);

    // Update and render
    void Update(float deltaTime);
    void Render(ShaderManager& shader);

    // Scene properties
    void SetAmbientLight(const glm::vec3& color);
    glm::vec3 GetAmbientLight() const { return m_ambientLight; }

private:
    std::vector<std::shared_ptr<Object3D>> m_objects;
    std::vector<std::shared_ptr<Light>> m_lights;
    glm::vec3 m_ambientLight;
    
    // Scene setup
    void CreateDefaultScene();
    void SetupLighting();
};
