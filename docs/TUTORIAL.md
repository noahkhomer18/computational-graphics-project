# Tutorial: Getting Started with Computational Graphics Project

## Prerequisites

Before starting, ensure you have:
- Visual Studio 2022 or compatible IDE
- OpenGL drivers installed
- Git for version control
- Basic C++ knowledge

## Installation

### Step 1: Clone the Repository
```bash
git clone https://github.com/noahkhomer18/computational-graphics-project.git
cd computational-graphics-project
```

### Step 2: Install Dependencies

#### Windows (Visual Studio)
1. Download GLFW from [glfw.org](https://www.glfw.org/)
2. Download GLEW from [glew.sourceforge.net](https://glew.sourceforge.net/)
3. Download GLM from [glm.g-truc.net](https://glm.g-truc.net/)
4. Place libraries in `Libraries/` directory

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get install libglfw3-dev libglew-dev libglm-dev
```

#### macOS
```bash
brew install glfw glew glm
```

### Step 3: Build the Project

#### Using CMake (Recommended)
```bash
mkdir build
cd build
cmake ..
make
```

#### Using Visual Studio
1. Open `7-1_FinalProjectMilestones.sln`
2. Build the solution (Ctrl+Shift+B)

## Basic Usage

### Running the Application
```bash
./ComputationalGraphics
```

### Controls
- **WASD**: Move camera
- **Mouse**: Look around
- **Scroll**: Adjust movement speed
- **ESC**: Exit

## Creating Your First Scene

### Step 1: Basic Setup
```cpp
#include "SceneManager.h"
#include "ShaderManager.h"
#include "camera.h"

// Initialize managers
auto sceneManager = std::make_unique<SceneManager>();
auto shaderManager = std::make_unique<ShaderManager>();

// Load shaders
shaderManager->LoadShaders("shaders/vertex.glsl", "shaders/fragment.glsl");
```

### Step 2: Add Objects
```cpp
// Create a cube
auto cube = std::make_shared<Object3D>("myCube");
cube->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
cube->color = glm::vec3(1.0f, 0.0f, 0.0f); // Red
sceneManager->AddObject(cube);

// Create a sphere
auto sphere = std::make_shared<Object3D>("mySphere");
sphere->SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));
sphere->color = glm::vec3(0.0f, 1.0f, 0.0f); // Green
sceneManager->AddObject(sphere);
```

### Step 3: Add Lighting
```cpp
// Add directional light (sun)
auto sunLight = std::make_shared<Light>("sun", LightType::DIRECTIONAL);
sunLight->SetDirection(glm::vec3(-1.0f, -1.0f, -1.0f));
sunLight->SetColor(glm::vec3(1.0f, 1.0f, 0.9f));
sceneManager->AddLight(sunLight);

// Add point light
auto pointLight = std::make_shared<Light>("lamp", LightType::POINT);
pointLight->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
pointLight->SetColor(glm::vec3(1.0f, 0.5f, 0.5f));
sceneManager->AddLight(pointLight);
```

### Step 4: Render Loop
```cpp
while (!glfwWindowShouldClose(window)) {
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Update scene
    sceneManager->Update(deltaTime);
    
    // Render scene
    shaderManager->use();
    sceneManager->Render(*shaderManager);
    
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}
```

## Advanced Features

### Performance Monitoring
```cpp
PerformanceMonitor monitor;
monitor.BeginFrame();
// ... render code ...
monitor.EndFrame();

if (!monitor.IsPerformanceGood()) {
    std::cout << monitor.GetPerformanceReport() << std::endl;
}
```

### Debug Rendering
```cpp
DebugRenderer debug;
debug.Initialize();

// Draw wireframe box
debug.DrawWireframeBox(glm::vec3(-1.0f), glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));

// Draw coordinate axes
debug.DrawAxis(glm::vec3(0.0f), 2.0f);

// Render debug objects
debug.Render(*shaderManager, view, projection);
```

### Custom Objects
```cpp
class MyCustomObject : public Object3D {
public:
    MyCustomObject(const std::string& name) : Object3D(name) {}
    
    void Update(float deltaTime) override {
        // Custom update logic
        rotation.y += 45.0f * deltaTime; // Rotate around Y axis
    }
    
    void Render(ShaderManager& shader) override {
        // Custom rendering logic
        Object3D::Render(shader); // Call base class render
    }
};
```

## Shader Customization

### Creating Custom Shaders
1. Create new `.glsl` files in `shaders/` directory
2. Use `ShaderManager::LoadShaders()` to load them
3. Set uniforms using `ShaderManager::set*Value()` methods

### Example Custom Fragment Shader
```glsl
#version 330 core
in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;

void main() {
    // Simple lighting calculation
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(normalize(Normal), lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    FragColor = vec4((objectColor + diffuse), 1.0);
}
```

## Troubleshooting

### Common Issues

#### Black Screen
- Check if shaders compiled successfully
- Verify OpenGL context is created
- Ensure depth testing is enabled

#### Build Errors
- Verify all dependencies are installed
- Check include paths in project settings
- Ensure OpenGL drivers are up to date

#### Performance Issues
- Use PerformanceMonitor to identify bottlenecks
- Reduce number of objects or lights
- Optimize shader complexity

### Getting Help
- Check the API documentation in `docs/API.md`
- Review the design decisions in `Design Decisions.docx`
- Open an issue on GitHub for bugs or questions

## Next Steps

1. **Experiment with Materials**: Try different shininess and color values
2. **Add Textures**: Load image files and apply them to objects
3. **Create Animations**: Use the Update() method to animate objects
4. **Optimize Performance**: Use the performance monitoring tools
5. **Extend Functionality**: Create custom object types and behaviors

## Resources

- [OpenGL Documentation](https://docs.gl/)
- [GLM Documentation](https://glm.g-truc.net/)
- [GLFW Documentation](https://www.glfw.org/docs/)
- [Learn OpenGL](https://learnopengl.com/)
