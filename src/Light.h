#pragma once

#include <glm/glm.hpp>
#include <string>

enum class LightType {
    DIRECTIONAL,
    POINT,
    SPOT
};

class Light {
public:
    Light(const std::string& name, LightType type);
    virtual ~Light() = default;

    // Light properties
    std::string name;
    LightType type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float intensity;
    bool enabled;

    // Point light specific
    float constant;
    float linear;
    float quadratic;

    // Spot light specific
    float cutOff;
    float outerCutOff;

    // Methods
    void SetPosition(const glm::vec3& pos);
    void SetDirection(const glm::vec3& dir);
    void SetColor(const glm::vec3& color);
    void SetIntensity(float intens);
    void SetAttenuation(float constant, float linear, float quadratic);
    void SetSpotlightAngles(float cutOff, float outerCutOff);
    
    // Virtual methods for different light types
    virtual void Update(float deltaTime) {}
    virtual glm::vec3 GetContribution(const glm::vec3& worldPos) const;
};
