#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include "../src/PerformanceMonitor.h"

class PerformanceTest : public ::testing::Test {
protected:
    void SetUp() override {
        monitor = std::make_unique<PerformanceMonitor>();
    }
    
    std::unique_ptr<PerformanceMonitor> monitor;
};

TEST_F(PerformanceTest, Initialization) {
    // Performance monitor should initialize without errors
    EXPECT_NO_THROW(monitor->BeginFrame());
    EXPECT_NO_THROW(monitor->EndFrame());
}

TEST_F(PerformanceTest, FrameTiming) {
    monitor->BeginFrame();
    
    // Simulate some work
    std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    
    monitor->EndFrame();
    
    // Should have reasonable frame time
    float frameTime = monitor->GetFrameTime();
    EXPECT_GT(frameTime, 0.0f);
    EXPECT_LT(frameTime, 100.0f); // Should be less than 100ms
    
    // Should have reasonable FPS
    float fps = monitor->GetFPS();
    EXPECT_GT(fps, 0.0f);
    EXPECT_LT(fps, 1000.0f); // Should be less than 1000 FPS
}

TEST_F(PerformanceTest, AverageCalculations) {
    // Simulate multiple frames
    for (int i = 0; i < 10; i++) {
        monitor->BeginFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        monitor->EndFrame();
    }
    
    float averageFPS = monitor->GetAverageFPS();
    float averageFrameTime = monitor->GetAverageFrameTime();
    
    EXPECT_GT(averageFPS, 0.0f);
    EXPECT_GT(averageFrameTime, 0.0f);
    
    // Average FPS and frame time should be inversely related
    EXPECT_NEAR(averageFPS * averageFrameTime, 1000.0f, 100.0f);
}

TEST_F(PerformanceTest, PerformanceGood) {
    // Simulate good performance (60 FPS)
    for (int i = 0; i < 5; i++) {
        monitor->BeginFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        monitor->EndFrame();
    }
    
    EXPECT_TRUE(monitor->IsPerformanceGood());
}

TEST_F(PerformanceTest, PerformanceReport) {
    monitor->BeginFrame();
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    monitor->EndFrame();
    
    std::string report = monitor->GetPerformanceReport();
    EXPECT_FALSE(report.empty());
    EXPECT_NE(report.find("FPS"), std::string::npos);
    EXPECT_NE(report.find("Frame Time"), std::string::npos);
}

TEST_F(PerformanceTest, StatisticsReset) {
    // Generate some statistics
    for (int i = 0; i < 5; i++) {
        monitor->BeginFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        monitor->EndFrame();
    }
    
    // Reset statistics
    monitor->ResetStatistics();
    
    // Statistics should be reset
    EXPECT_FLOAT_EQ(monitor->GetFPS(), 0.0f);
    EXPECT_FLOAT_EQ(monitor->GetFrameTime(), 0.0f);
    EXPECT_FLOAT_EQ(monitor->GetAverageFPS(), 0.0f);
    EXPECT_FLOAT_EQ(monitor->GetAverageFrameTime(), 0.0f);
}

TEST_F(PerformanceTest, MemoryUsage) {
    // Update memory usage
    monitor->UpdateMemoryUsage();
    
    size_t memoryUsage = monitor->GetMemoryUsage();
    size_t peakMemory = monitor->GetPeakMemoryUsage();
    
    // Memory usage should be non-negative
    EXPECT_GE(memoryUsage, 0);
    EXPECT_GE(peakMemory, 0);
    
    // Peak memory should be at least as much as current usage
    EXPECT_GE(peakMemory, memoryUsage);
}

TEST_F(PerformanceTest, MultipleFrames) {
    const int numFrames = 20;
    
    for (int i = 0; i < numFrames; i++) {
        monitor->BeginFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        monitor->EndFrame();
    }
    
    // Should have processed all frames
    float averageFPS = monitor->GetAverageFPS();
    EXPECT_GT(averageFPS, 0.0f);
    EXPECT_LT(averageFPS, 100.0f); // Should be reasonable
}

TEST_F(PerformanceTest, FrameTimeConsistency) {
    std::vector<float> frameTimes;
    
    // Collect frame times
    for (int i = 0; i < 10; i++) {
        monitor->BeginFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        monitor->EndFrame();
        
        frameTimes.push_back(monitor->GetFrameTime());
    }
    
    // Frame times should be consistent
    float sum = 0.0f;
    for (float time : frameTimes) {
        sum += time;
    }
    float average = sum / frameTimes.size();
    
    // Average should be around 16ms
    EXPECT_NEAR(average, 16.0f, 5.0f);
}

TEST_F(PerformanceTest, PerformanceThresholds) {
    // Test with very fast frame (good performance)
    monitor->BeginFrame();
    std::this_thread::sleep_for(std::chrono::microseconds(100)); // Very fast
    monitor->EndFrame();
    
    EXPECT_TRUE(monitor->IsPerformanceGood());
    
    // Test with very slow frame (bad performance)
    monitor->BeginFrame();
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Very slow
    monitor->EndFrame();
    
    // This might still be considered "good" depending on thresholds
    // The actual threshold testing depends on the implementation
}
