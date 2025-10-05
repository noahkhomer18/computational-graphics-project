#include "PerformanceMonitor.h"
#include <iostream>
#include <algorithm>
#include <numeric>

const float PerformanceMonitor::TARGET_FPS = 60.0f;
const float PerformanceMonitor::MIN_FPS = 30.0f;
const float PerformanceMonitor::MAX_FRAME_TIME = 33.33f; // 30 FPS equivalent

PerformanceMonitor::PerformanceMonitor() 
    : m_fps(0.0f), m_frameTime(0.0f), m_averageFPS(0.0f), m_averageFrameTime(0.0f),
      m_memoryUsage(0), m_peakMemoryUsage(0) {
    m_lastFrameTime = std::chrono::high_resolution_clock::now();
    m_frameTimeHistory.reserve(FRAME_HISTORY_SIZE);
}

PerformanceMonitor::~PerformanceMonitor() {
    // Clean up GPU query objects
    for (auto& pair : m_queryObjects) {
        glDeleteQueries(1, &pair.second);
    }
}

void PerformanceMonitor::BeginFrame() {
    m_currentFrameTime = std::chrono::high_resolution_clock::now();
}

void PerformanceMonitor::EndFrame() {
    auto now = std::chrono::high_resolution_clock::now();
    
    // Calculate frame time
    auto frameDuration = std::chrono::duration_cast<std::chrono::microseconds>(now - m_currentFrameTime);
    m_frameTime = frameDuration.count() / 1000.0f; // Convert to milliseconds
    
    // Calculate FPS
    m_fps = 1000.0f / m_frameTime;
    
    // Update frame history
    m_frameTimeHistory.push_back(m_frameTime);
    if (m_frameTimeHistory.size() > FRAME_HISTORY_SIZE) {
        m_frameTimeHistory.erase(m_frameTimeHistory.begin());
    }
    
    // Calculate averages
    if (!m_frameTimeHistory.empty()) {
        m_averageFrameTime = std::accumulate(m_frameTimeHistory.begin(), m_frameTimeHistory.end(), 0.0f) / m_frameTimeHistory.size();
        m_averageFPS = 1000.0f / m_averageFrameTime;
    }
    
    m_lastFrameTime = now;
}

void PerformanceMonitor::BeginGPUTimer(const std::string& name) {
    if (m_timerActive[name]) {
        std::cout << "Warning: GPU timer '" << name << "' is already active!" << std::endl;
        return;
    }
    
    // Create query object if it doesn't exist
    if (m_queryObjects.find(name) == m_queryObjects.end()) {
        GLuint query;
        glGenQueries(1, &query);
        m_queryObjects[name] = query;
    }
    
    glBeginQuery(GL_TIME_ELAPSED, m_queryObjects[name]);
    m_timerActive[name] = true;
}

void PerformanceMonitor::EndGPUTimer(const std::string& name) {
    if (!m_timerActive[name]) {
        std::cout << "Warning: GPU timer '" << name << "' is not active!" << std::endl;
        return;
    }
    
    glEndQuery(GL_TIME_ELAPSED);
    m_timerActive[name] = false;
    
    // Get the result
    GLint available = 0;
    glGetQueryObjectiv(m_queryObjects[name], GL_QUERY_RESULT_AVAILABLE, &available);
    
    if (available) {
        GLint64 timeElapsed;
        glGetQueryObjecti64v(m_queryObjects[name], GL_QUERY_RESULT, &timeElapsed);
        m_gpuTimes[name] = timeElapsed / 1000000.0f; // Convert to milliseconds
    }
}

float PerformanceMonitor::GetGPUTime(const std::string& name) const {
    auto it = m_gpuTimes.find(name);
    return (it != m_gpuTimes.end()) ? it->second : 0.0f;
}

void PerformanceMonitor::UpdateMemoryUsage() {
    // This is a simplified memory monitoring
    // In a real implementation, you would use platform-specific APIs
    // For now, we'll just track OpenGL memory usage
    GLint totalMemory, availableMemory;
    glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NV, &totalMemory);
    glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NV, &availableMemory);
    
    m_memoryUsage = totalMemory - availableMemory;
    m_peakMemoryUsage = std::max(m_peakMemoryUsage, m_memoryUsage);
}

void PerformanceMonitor::ResetStatistics() {
    m_frameTimeHistory.clear();
    m_fps = 0.0f;
    m_frameTime = 0.0f;
    m_averageFPS = 0.0f;
    m_averageFrameTime = 0.0f;
    m_memoryUsage = 0;
    m_peakMemoryUsage = 0;
    
    for (auto& pair : m_gpuTimes) {
        pair.second = 0.0f;
    }
}

void PerformanceMonitor::PrintStatistics() const {
    std::cout << "\n=== Performance Statistics ===" << std::endl;
    std::cout << "Current FPS: " << m_fps << std::endl;
    std::cout << "Average FPS: " << m_averageFPS << std::endl;
    std::cout << "Frame Time: " << m_frameTime << " ms" << std::endl;
    std::cout << "Average Frame Time: " << m_averageFrameTime << " ms" << std::endl;
    std::cout << "Memory Usage: " << m_memoryUsage / 1024 / 1024 << " MB" << std::endl;
    std::cout << "Peak Memory: " << m_peakMemoryUsage / 1024 / 1024 << " MB" << std::endl;
    
    if (!m_gpuTimes.empty()) {
        std::cout << "\nGPU Times:" << std::endl;
        for (const auto& pair : m_gpuTimes) {
            std::cout << "  " << pair.first << ": " << pair.second << " ms" << std::endl;
        }
    }
    
    std::cout << "=============================" << std::endl;
}

bool PerformanceMonitor::IsPerformanceGood() const {
    return m_fps >= MIN_FPS && m_frameTime <= MAX_FRAME_TIME;
}

std::string PerformanceMonitor::GetPerformanceReport() const {
    std::string report = "Performance Report:\n";
    report += "FPS: " + std::to_string(m_fps) + " (Target: " + std::to_string(TARGET_FPS) + ")\n";
    report += "Frame Time: " + std::to_string(m_frameTime) + " ms\n";
    report += "Memory: " + std::to_string(m_memoryUsage / 1024 / 1024) + " MB\n";
    report += "Status: " + std::string(IsPerformanceGood() ? "Good" : "Needs Optimization");
    return report;
}
