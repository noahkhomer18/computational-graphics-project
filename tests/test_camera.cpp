#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../include/camera.h"

class CameraTest : public ::testing::Test {
protected:
    void SetUp() override {
        camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
    }
    
    std::unique_ptr<Camera> camera;
};

TEST_F(CameraTest, InitialPosition) {
    EXPECT_FLOAT_EQ(camera->Position.x, 0.0f);
    EXPECT_FLOAT_EQ(camera->Position.y, 0.0f);
    EXPECT_FLOAT_EQ(camera->Position.z, 3.0f);
}

TEST_F(CameraTest, MovementForward) {
    float deltaTime = 1.0f;
    glm::vec3 initialPos = camera->Position;
    
    camera->ProcessKeyboard(FORWARD, deltaTime);
    
    // Should move forward (negative Z direction by default)
    EXPECT_LT(camera->Position.z, initialPos.z);
}

TEST_F(CameraTest, MovementBackward) {
    float deltaTime = 1.0f;
    glm::vec3 initialPos = camera->Position;
    
    camera->ProcessKeyboard(BACKWARD, deltaTime);
    
    // Should move backward (positive Z direction by default)
    EXPECT_GT(camera->Position.z, initialPos.z);
}

TEST_F(CameraTest, MovementLeft) {
    float deltaTime = 1.0f;
    glm::vec3 initialPos = camera->Position;
    
    camera->ProcessKeyboard(LEFT, deltaTime);
    
    // Should move left (negative X direction)
    EXPECT_LT(camera->Position.x, initialPos.x);
}

TEST_F(CameraTest, MovementRight) {
    float deltaTime = 1.0f;
    glm::vec3 initialPos = camera->Position;
    
    camera->ProcessKeyboard(RIGHT, deltaTime);
    
    // Should move right (positive X direction)
    EXPECT_GT(camera->Position.x, initialPos.x);
}

TEST_F(CameraTest, MouseMovement) {
    float initialYaw = camera->Yaw;
    float initialPitch = camera->Pitch;
    
    camera->ProcessMouseMovement(10.0f, 5.0f);
    
    // Yaw should increase (positive X offset)
    EXPECT_GT(camera->Yaw, initialYaw);
    
    // Pitch should increase (positive Y offset)
    EXPECT_GT(camera->Pitch, initialPitch);
}

TEST_F(CameraTest, PitchConstraints) {
    // Test pitch constraint at 89 degrees
    camera->ProcessMouseMovement(0.0f, 1000.0f); // Large Y offset
    EXPECT_LE(camera->Pitch, 89.0f);
    
    // Reset and test negative constraint
    camera->Pitch = 0.0f;
    camera->ProcessMouseMovement(0.0f, -1000.0f); // Large negative Y offset
    EXPECT_GE(camera->Pitch, -89.0f);
}

TEST_F(CameraTest, ViewMatrix) {
    glm::mat4 view = camera->GetViewMatrix();
    
    // View matrix should be 4x4
    EXPECT_EQ(view.length(), 4);
    EXPECT_EQ(view[0].length(), 4);
    
    // View matrix should be invertible
    glm::mat4 invView = glm::inverse(view);
    glm::mat4 identity = view * invView;
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                EXPECT_NEAR(identity[i][j], 1.0f, 0.001f);
            } else {
                EXPECT_NEAR(identity[i][j], 0.0f, 0.001f);
            }
        }
    }
}

TEST_F(CameraTest, MovementSpeed) {
    float initialSpeed = camera->MovementSpeed;
    
    // Test speed increase
    camera->ProcessMouseScroll(-1.0f);
    EXPECT_GT(camera->MovementSpeed, initialSpeed);
    
    // Test speed decrease
    camera->ProcessMouseScroll(1.0f);
    EXPECT_LT(camera->MovementSpeed, initialSpeed);
}

TEST_F(CameraTest, SpeedBounds) {
    // Test minimum speed
    camera->MovementSpeed = 0.1f;
    camera->ProcessMouseScroll(10.0f);
    EXPECT_GE(camera->MovementSpeed, 0.1f);
    
    // Test maximum speed
    camera->MovementSpeed = 45.0f;
    camera->ProcessMouseScroll(-10.0f);
    EXPECT_LE(camera->MovementSpeed, 45.0f);
}
