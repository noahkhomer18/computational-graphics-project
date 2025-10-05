#include "Light.h"
#include <algorithm>

Light::Light(const std::string& name, LightType type) 
    : name(name), type(type), position(0.0f), direction(0.0f, -1.0f, 0.0f),
      ambient(0.1f), diffuse(1.0f), specular(1.0f), intensity(1.0f), enabled(true),
      constant(1.0f), linear(0.09f), quadratic(0.032f),
      cutOff(12.5f), outerCutOff(17.5f) {
}

void Light::SetPosition(const glm::vec3& pos) {
    position = pos;
}

void Light::SetDirection(const glm::vec3& dir) {
    direction = glm::normalize(dir);
}

void Light::SetColor(const glm::vec3& color) {
    ambient = color * 0.1f;
    diffuse = color;
    specular = color;
}

void Light::SetIntensity(float intens) {
    intensity = std::max(0.0f, intens);
}

void Light::SetAttenuation(float constant, float linear, float quadratic) {
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
}

void Light::SetSpotlightAngles(float cutOff, float outerCutOff) {
    this->cutOff = cutOff;
    this->outerCutOff = outerCutOff;
}

glm::vec3 Light::GetContribution(const glm::vec3& worldPos) const {
    if (!enabled) return glm::vec3(0.0f);
    
    switch (type) {
        case LightType::DIRECTIONAL:
            return diffuse * intensity;
            
        case LightType::POINT: {
            float distance = glm::length(position - worldPos);
            float attenuation = 1.0f / (constant + linear * distance + quadratic * (distance * distance));
            return diffuse * intensity * attenuation;
        }
        
        case LightType::SPOT: {
            glm::vec3 lightDir = glm::normalize(position - worldPos);
            float theta = glm::dot(lightDir, glm::normalize(-direction));
            float epsilon = cutOff - outerCutOff;
            float intensity_factor = glm::clamp((theta - outerCutOff) / epsilon, 0.0f, 1.0f);
            return diffuse * intensity * intensity_factor;
        }
        
        default:
            return glm::vec3(0.0f);
    }
}
