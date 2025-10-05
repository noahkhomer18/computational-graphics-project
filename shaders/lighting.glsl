#version 330 core

// Advanced lighting shader with multiple light types
// This shader supports directional, point, and spot lights

// Input from vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// Output color
out vec4 FragColor;

// Material structure
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    bool useTexture;
};

// Directional light
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
};

// Point light
struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float intensity;
};

// Spot light
struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutOff;
    float outerCutOff;
    float intensity;
};

// Uniforms
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[4];
uniform SpotLight spotLight;
uniform int numPointLights;
uniform vec3 viewPos;
uniform vec3 ambientLight;

// Texture
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform bool useNormalMap;

// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // Normalize the normal vector
    vec3 norm = normalize(Normal);
    
    // Calculate view direction
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // Initialize result
    vec3 result = vec3(0.0);
    
    // Add directional light
    result += CalcDirLight(dirLight, norm, viewDir);
    
    // Add point lights
    for(int i = 0; i < numPointLights; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    
    // Add spot light
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    
    // Apply texture if available
    if (material.useTexture) {
        vec4 texColor = texture(diffuseTexture, TexCoord);
        result *= texColor.rgb;
    }
    
    // Output final color
    FragColor = vec4(result, 1.0);
}

// Calculate directional light contribution
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
    // Ambient
    vec3 ambient = light.ambient * material.ambient;
    
    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;
    
    return (ambient + diffuse + specular) * light.intensity;
}

// Calculate point light contribution
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));
    
    // Ambient
    vec3 ambient = light.ambient * material.ambient;
    
    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular) * light.intensity;
}

// Calculate spot light contribution
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Check if fragment is in spotlight cone
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    // Ambient
    vec3 ambient = light.ambient * material.ambient;
    
    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;
    
    diffuse *= intensity;
    specular *= intensity;
    
    return (ambient + diffuse + specular) * light.intensity;
}
