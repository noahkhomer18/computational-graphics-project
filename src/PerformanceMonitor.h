#pragma once

#include <chrono>
#include <vector>
#include <string>
#include <map>
#include <GL/glew.h>

class PerformanceMonitor {
public:
    PerformanceMonitor();
    ~PerformanceMonitor();

    // Frame timing
    void BeginFrame();
    void EndFrame();
    float GetFPS() const { return m_fps; }
    float GetFrameTime() const { return m_frameTime; }
    float GetAverageFPS() const { return m_averageFPS; }
    float GetAverageFrameTime() const { return m_averageFrameTime; }

    // GPU timing
    void BeginGPUTimer(const std::string& name);
    void EndGPUTimer(const std::string& name);
    float GetGPUTime(const std::string& name) const;

    // Memory monitoring
    void UpdateMemoryUsage();
    size_t GetMemoryUsage() const { return m_memoryUsage; }
    size_t GetPeakMemoryUsage() const { return m_peakMemoryUsage; }

    // Performance statistics
    void ResetStatistics();
    void PrintStatistics() const;
    
    // Performance warnings
    bool IsPerformanceGood() const;
    std::string GetPerformanceReport() const;

private:
    // Frame timing
    std::chrono::high_resolution_clock::time_point m_lastFrameTime;
    std::chrono::high_resolution_clock::time_point m_currentFrameTime;
    float m_fps;
    float m_frameTime;
    float m_averageFPS;
    float m_averageFrameTime;
    
    // Frame history for averaging
    std::vector<float> m_frameTimeHistory;
    static const size_t FRAME_HISTORY_SIZE = 60;
    
    // GPU timing
    std::map<std::string, GLuint> m_queryObjects;
    std::map<std::string, bool> m_timerActive;
    std::map<std::string, float> m_gpuTimes;
    
    // Memory monitoring
    size_t m_memoryUsage;
    size_t m_peakMemoryUsage;
    
    // Performance thresholds
    static const float TARGET_FPS;
    static const float MIN_FPS;
    static const float MAX_FRAME_TIME;
};
