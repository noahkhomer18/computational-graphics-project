#include "SceneManager.h"
#include "ShaderManager.h"
#include <iostream>

// Forward declarations for basic classes
class Object3D {
public:
    std::string name;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec3 color;
    
    Object3D(const std::string& n, const glm::vec3& pos = glm::vec3(0.0f))
        : name(n), position(pos), rotation(0.0f), scale(1.0f), color(1.0f) {}
    
    virtual void Update(float deltaTime) {}
    virtual void Render(ShaderManager& shader) {}
};

class Light {
public:
    std::string name;
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    
    Light(const std::string& n, const glm::vec3& pos, const glm::vec3& col, float intens = 1.0f)
        : name(n), position(pos), color(col), intensity(intens) {}
};

SceneManager::SceneManager() : m_ambientLight(0.1f, 0.1f, 0.1f) {
}

SceneManager::~SceneManager() {
    Cleanup();
}

void SceneManager::Initialize() {
    std::cout << "Initializing Scene Manager..." << std::endl;
    CreateDefaultScene();
    SetupLighting();
}

void SceneManager::Cleanup() {
    m_objects.clear();
    m_lights.clear();
}

void SceneManager::AddObject(std::shared_ptr<Object3D> object) {
    if (object) {
        m_objects.push_back(object);
        std::cout << "Added object: " << object->name << std::endl;
    }
}

void SceneManager::RemoveObject(const std::string& name) {
    m_objects.erase(
        std::remove_if(m_objects.begin(), m_objects.end(),
            [&name](const std::shared_ptr<Object3D>& obj) {
                return obj->name == name;
            }),
        m_objects.end()
    );
}

std::shared_ptr<Object3D> SceneManager::GetObject(const std::string& name) {
    for (auto& obj : m_objects) {
        if (obj->name == name) {
            return obj;
        }
    }
    return nullptr;
}

void SceneManager::AddLight(std::shared_ptr<Light> light) {
    if (light) {
        m_lights.push_back(light);
        std::cout << "Added light: " << light->name << std::endl;
    }
}

void SceneManager::RemoveLight(const std::string& name) {
    m_lights.erase(
        std::remove_if(m_lights.begin(), m_lights.end(),
            [&name](const std::shared_ptr<Light>& light) {
                return light->name == name;
            }),
        m_lights.end()
    );
}

void SceneManager::UpdateLighting(ShaderManager& shader) {
    // Set ambient light
    shader.setVec3Value("ambientLight", m_ambientLight);
    
    // Set directional light (sun)
    if (!m_lights.empty()) {
        auto& mainLight = m_lights[0];
        shader.setVec3Value("lightDirection", -mainLight->position);
        shader.setVec3Value("lightColor", mainLight->color);
    }
}

void SceneManager::Update(float deltaTime) {
    // Update all objects
    for (auto& object : m_objects) {
        object->Update(deltaTime);
    }
    
    // Update lights (if they need animation)
    for (auto& light : m_lights) {
        // Light animation could go here
    }
}

void SceneManager::Render(ShaderManager& shader) {
    // Update lighting uniforms
    UpdateLighting(shader);
    
    // Render all objects
    for (auto& object : m_objects) {
        object->Render(shader);
    }
}

void SceneManager::SetAmbientLight(const glm::vec3& color) {
    m_ambientLight = color;
}

void SceneManager::CreateDefaultScene() {
    std::cout << "Creating default scene..." << std::endl;
    
    // Create a simple floor
    auto floor = std::make_shared<Object3D>("floor", glm::vec3(0.0f, -1.0f, 0.0f));
    floor->scale = glm::vec3(10.0f, 0.1f, 10.0f);
    floor->color = glm::vec3(0.5f, 0.5f, 0.5f);
    AddObject(floor);
    
    // Create a cube
    auto cube = std::make_shared<Object3D>("cube", glm::vec3(0.0f, 0.0f, 0.0f));
    cube->color = glm::vec3(1.0f, 0.0f, 0.0f);
    AddObject(cube);
    
    // Create a laptop object
    auto laptop = std::make_shared<Object3D>("laptop", glm::vec3(2.0f, 0.0f, 0.0f));
    laptop->scale = glm::vec3(1.5f, 0.1f, 1.0f);
    laptop->color = glm::vec3(0.2f, 0.2f, 0.2f);
    AddObject(laptop);
    
    // Create a cylinder
    auto cylinder = std::make_shared<Object3D>("cylinder", glm::vec3(-2.0f, 0.0f, 0.0f));
    cylinder->scale = glm::vec3(0.5f, 1.0f, 0.5f);
    cylinder->color = glm::vec3(0.0f, 1.0f, 0.0f);
    AddObject(cylinder);
}

void SceneManager::SetupLighting() {
    std::cout << "Setting up lighting..." << std::endl;
    
    // Add directional light (sun)
    auto sunLight = std::make_shared<Light>("sun", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.9f), 1.0f);
    AddLight(sunLight);
    
    // Add point light
    auto pointLight = std::make_shared<Light>("pointLight", glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(1.0f, 0.5f, 0.5f), 0.8f);
    AddLight(pointLight);
}
