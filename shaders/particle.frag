#version 330 core

// Particle fragment shader
in vec4 Color;
out vec4 FragColor;

uniform sampler2D particleTexture;
uniform bool useTexture;

void main()
{
    if (useTexture) {
        // Use texture for particle
        vec4 texColor = texture(particleTexture, gl_PointCoord);
        FragColor = Color * texColor;
    } else {
        // Use solid color
        FragColor = Color;
    }
    
    // Discard transparent pixels
    if (FragColor.a < 0.01) {
        discard;
    }
}
