#version 330 core

// Input from vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// Output color
out vec4 FragColor;

// Material properties
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// Light properties
struct Light {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
};

// Uniforms
uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform vec3 ambientLight;

// Texture
uniform sampler2D diffuseTexture;
uniform bool useTexture;

void main()
{
    // Normalize the normal vector
    vec3 norm = normalize(Normal);
    
    // Calculate ambient lighting
    vec3 ambient = ambientLight * material.ambient;
    
    // Calculate diffuse lighting
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    
    // Calculate specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;
    
    // Combine lighting
    vec3 result = (ambient + diffuse + specular) * light.intensity;
    
    // Apply texture if available
    if (useTexture) {
        vec4 texColor = texture(diffuseTexture, TexCoord);
        result *= texColor.rgb;
    }
    
    // Output final color
    FragColor = vec4(result, 1.0);
}
