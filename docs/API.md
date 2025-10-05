# API Documentation

## Core Classes

### Camera Class

The Camera class provides first-person camera controls for 3D navigation.

#### Constructor
```cpp
Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
       glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
       float yaw = -90.0f, 
       float pitch = 0.0f)
```

#### Public Methods
- `glm::mat4 GetViewMatrix() const` - Returns the view matrix
- `void ProcessKeyboard(Camera_Movement direction, float deltaTime)` - Handle keyboard input
- `void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)` - Handle mouse input
- `void ProcessMouseScroll(float yoffset)` - Handle scroll wheel input

#### Public Properties
- `glm::vec3 Position` - Camera position
- `glm::vec3 Front` - Camera front direction
- `glm::vec3 Up` - Camera up direction
- `float MovementSpeed` - Movement speed multiplier
- `float MouseSensitivity` - Mouse sensitivity
- `float Zoom` - Field of view zoom

### ShaderManager Class

Manages OpenGL shaders and uniform variables.

#### Constructor
```cpp
ShaderManager()
```

#### Public Methods
- `GLuint LoadShaders(const char* vertexPath, const char* fragmentPath)` - Load and compile shaders
- `void use()` - Activate the shader program
- `void setBoolValue(const std::string& name, bool value)` - Set boolean uniform
- `void setIntValue(const std::string& name, int value)` - Set integer uniform
- `void setFloatValue(const std::string& name, float value)` - Set float uniform
- `void setVec3Value(const std::string& name, const glm::vec3& value)` - Set vec3 uniform
- `void setMat4Value(const std::string& name, const glm::mat4& value)` - Set mat4 uniform

### SceneManager Class

Manages 3D objects and lighting in the scene.

#### Constructor
```cpp
SceneManager()
```

#### Public Methods
- `void Initialize()` - Initialize the scene manager
- `void AddObject(std::shared_ptr<Object3D> object)` - Add object to scene
- `void RemoveObject(const std::string& name)` - Remove object from scene
- `void AddLight(std::shared_ptr<Light> light)` - Add light to scene
- `void Update(float deltaTime)` - Update scene objects
- `void Render(ShaderManager& shader)` - Render all objects

### Object3D Class

Base class for all 3D objects with transform hierarchy.

#### Constructor
```cpp
Object3D(const std::string& name)
```

#### Public Properties
- `glm::vec3 position` - Object position
- `glm::vec3 rotation` - Object rotation (Euler angles)
- `glm::vec3 scale` - Object scale
- `glm::vec3 color` - Object color
- `float shininess` - Material shininess
- `bool visible` - Object visibility

#### Public Methods
- `void SetPosition(const glm::vec3& pos)` - Set object position
- `void SetRotation(const glm::vec3& rot)` - Set object rotation
- `void SetScale(const glm::vec3& scl)` - Set object scale
- `glm::mat4 GetModelMatrix() const` - Get model matrix
- `void AddChild(std::shared_ptr<Object3D> child)` - Add child object
- `virtual void Update(float deltaTime)` - Update object (override in derived classes)
- `virtual void Render(ShaderManager& shader)` - Render object (override in derived classes)

### Light Class

Represents different types of lights in the scene.

#### Constructor
```cpp
Light(const std::string& name, LightType type)
```

#### Light Types
- `LightType::DIRECTIONAL` - Directional light (sun)
- `LightType::POINT` - Point light (bulb)
- `LightType::SPOT` - Spot light (flashlight)

#### Public Properties
- `std::string name` - Light name
- `LightType type` - Light type
- `glm::vec3 position` - Light position
- `glm::vec3 direction` - Light direction
- `glm::vec3 ambient` - Ambient color
- `glm::vec3 diffuse` - Diffuse color
- `glm::vec3 specular` - Specular color
- `float intensity` - Light intensity
- `bool enabled` - Light enabled state

### PerformanceMonitor Class

Monitors application performance and provides statistics.

#### Constructor
```cpp
PerformanceMonitor()
```

#### Public Methods
- `void BeginFrame()` - Start frame timing
- `void EndFrame()` - End frame timing
- `float GetFPS() const` - Get current FPS
- `float GetFrameTime() const` - Get current frame time
- `void BeginGPUTimer(const std::string& name)` - Start GPU timing
- `void EndGPUTimer(const std::string& name)` - End GPU timing
- `void PrintStatistics() const` - Print performance statistics
- `bool IsPerformanceGood() const` - Check if performance is acceptable

### DebugRenderer Class

Provides debugging visualization tools.

#### Constructor
```cpp
DebugRenderer()
```

#### Public Methods
- `void Initialize()` - Initialize debug renderer
- `void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color)` - Draw line
- `void DrawWireframeBox(const glm::vec3& min, const glm::vec3& max, const glm::vec3& color)` - Draw wireframe box
- `void DrawWireframeSphere(const glm::vec3& center, float radius, const glm::vec3& color)` - Draw wireframe sphere
- `void DrawGrid(int size, float spacing, const glm::vec3& color)` - Draw grid
- `void DrawAxis(const glm::vec3& position, float length)` - Draw coordinate axes
- `void Render(ShaderManager& shader, const glm::mat4& view, const glm::mat4& projection)` - Render debug objects
- `void Clear()` - Clear debug objects

## Shader System

### Vertex Shader (vertex.glsl)
- Handles vertex transformations
- Calculates world position and normal
- Passes texture coordinates to fragment shader

### Fragment Shader (fragment.glsl)
- Implements Phong lighting model
- Supports material properties
- Handles texture mapping

### Advanced Lighting Shader (lighting.glsl)
- Supports multiple light types
- Implements attenuation for point lights
- Supports spotlight cones
- Advanced material properties

## Usage Examples

### Basic Camera Setup
```cpp
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
camera.ProcessKeyboard(FORWARD, deltaTime);
glm::mat4 view = camera.GetViewMatrix();
```

### Loading Shaders
```cpp
ShaderManager shader;
shader.LoadShaders("shaders/vertex.glsl", "shaders/fragment.glsl");
shader.use();
shader.setVec3Value("lightColor", 1.0f, 1.0f, 1.0f);
```

### Creating Objects
```cpp
auto object = std::make_shared<Object3D>("myObject");
object->SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));
object->color = glm::vec3(1.0f, 0.0f, 0.0f);
sceneManager->AddObject(object);
```

### Adding Lights
```cpp
auto light = std::make_shared<Light>("sun", LightType::DIRECTIONAL);
light->SetDirection(glm::vec3(-1.0f, -1.0f, -1.0f));
light->SetColor(glm::vec3(1.0f, 1.0f, 0.9f));
sceneManager->AddLight(light);
```
