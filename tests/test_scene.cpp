#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include "../src/SceneManager.h"
#include "../src/Object3D.h"
#include "../src/Light.h"

class SceneTest : public ::testing::Test {
protected:
    void SetUp() override {
        sceneManager = std::make_unique<SceneManager>();
        sceneManager->Initialize();
    }
    
    std::unique_ptr<SceneManager> sceneManager;
};

TEST_F(SceneTest, Initialization) {
    // Scene manager should initialize without errors
    EXPECT_NO_THROW(sceneManager->Initialize());
}

TEST_F(SceneTest, AddObject) {
    auto object = std::make_shared<Object3D>("testObject");
    object->SetPosition(glm::vec3(1.0f, 2.0f, 3.0f));
    
    sceneManager->AddObject(object);
    
    auto retrievedObject = sceneManager->GetObject("testObject");
    EXPECT_NE(retrievedObject, nullptr);
    EXPECT_EQ(retrievedObject->name, "testObject");
}

TEST_F(SceneTest, RemoveObject) {
    auto object = std::make_shared<Object3D>("testObject");
    sceneManager->AddObject(object);
    
    // Object should exist
    EXPECT_NE(sceneManager->GetObject("testObject"), nullptr);
    
    sceneManager->RemoveObject("testObject");
    
    // Object should be removed
    EXPECT_EQ(sceneManager->GetObject("testObject"), nullptr);
}

TEST_F(SceneTest, AddLight) {
    auto light = std::make_shared<Light>("testLight", LightType::DIRECTIONAL);
    light->SetDirection(glm::vec3(-1.0f, -1.0f, -1.0f));
    
    sceneManager->AddLight(light);
    
    // Light should be added (we can't directly retrieve it, but no exception should be thrown)
    EXPECT_NO_THROW(sceneManager->Update(0.016f)); // 60 FPS delta time
}

TEST_F(SceneTest, UpdateObjects) {
    auto object = std::make_shared<Object3D>("testObject");
    glm::vec3 initialPos = object->position;
    
    sceneManager->AddObject(object);
    
    // Update should not throw
    EXPECT_NO_THROW(sceneManager->Update(0.016f));
    
    // Object position should remain the same (no movement in base class)
    EXPECT_EQ(object->position, initialPos);
}

TEST_F(SceneTest, AmbientLight) {
    glm::vec3 testColor(0.2f, 0.3f, 0.4f);
    sceneManager->SetAmbientLight(testColor);
    
    EXPECT_EQ(sceneManager->GetAmbientLight(), testColor);
}

TEST_F(SceneTest, MultipleObjects) {
    // Add multiple objects
    for (int i = 0; i < 5; i++) {
        auto object = std::make_shared<Object3D>("object" + std::to_string(i));
        object->SetPosition(glm::vec3(i, 0.0f, 0.0f));
        sceneManager->AddObject(object);
    }
    
    // All objects should exist
    for (int i = 0; i < 5; i++) {
        auto object = sceneManager->GetObject("object" + std::to_string(i));
        EXPECT_NE(object, nullptr);
        EXPECT_FLOAT_EQ(object->position.x, i);
    }
}

TEST_F(SceneTest, ObjectHierarchy) {
    auto parent = std::make_shared<Object3D>("parent");
    auto child = std::make_shared<Object3D>("child");
    
    parent->AddChild(child);
    sceneManager->AddObject(parent);
    
    // Child should be found through parent
    auto foundChild = parent->GetChild("child");
    EXPECT_NE(foundChild, nullptr);
    EXPECT_EQ(foundChild->name, "child");
}

TEST_F(SceneTest, LightTypes) {
    // Test directional light
    auto dirLight = std::make_shared<Light>("dirLight", LightType::DIRECTIONAL);
    dirLight->SetDirection(glm::vec3(-1.0f, -1.0f, -1.0f));
    sceneManager->AddLight(dirLight);
    
    // Test point light
    auto pointLight = std::make_shared<Light>("pointLight", LightType::POINT);
    pointLight->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
    sceneManager->AddLight(pointLight);
    
    // Test spot light
    auto spotLight = std::make_shared<Light>("spotLight", LightType::SPOT);
    spotLight->SetPosition(glm::vec3(0.0f, 3.0f, 0.0f));
    spotLight->SetDirection(glm::vec3(0.0f, -1.0f, 0.0f));
    sceneManager->AddLight(spotLight);
    
    // All lights should be added without errors
    EXPECT_NO_THROW(sceneManager->Update(0.016f));
}

TEST_F(SceneTest, ObjectVisibility) {
    auto object = std::make_shared<Object3D>("testObject");
    object->visible = false;
    
    sceneManager->AddObject(object);
    
    // Object should exist but be invisible
    EXPECT_NE(sceneManager->GetObject("testObject"), nullptr);
    EXPECT_FALSE(sceneManager->GetObject("testObject")->visible);
    
    // Make visible
    object->visible = true;
    EXPECT_TRUE(sceneManager->GetObject("testObject")->visible);
}

TEST_F(SceneTest, MaterialProperties) {
    auto object = std::make_shared<Object3D>("testObject");
    object->color = glm::vec3(1.0f, 0.0f, 0.0f);
    object->shininess = 64.0f;
    
    sceneManager->AddObject(object);
    
    auto retrievedObject = sceneManager->GetObject("testObject");
    EXPECT_EQ(retrievedObject->color, glm::vec3(1.0f, 0.0f, 0.0f));
    EXPECT_FLOAT_EQ(retrievedObject->shininess, 64.0f);
}
