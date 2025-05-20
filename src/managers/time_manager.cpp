#include <managers/time_manager.hpp>

void TimeManager::Initialize() {
    Start = m_currentTime = m_prevTime = std::chrono::steady_clock::now();
}

void TimeManager::Update() {
    m_prevTime = m_currentTime;
    m_currentTime = std::chrono::steady_clock::now();
}

float TimeManager::GetDeltaTime() {
    std::chrono::duration<float> deltaTime = m_currentTime - m_prevTime;
    return deltaTime.count();
}

float TimeManager::GetCurrentTime() {
    std::chrono::duration<float> currentTime = m_currentTime - Start;
    return currentTime.count();
}
