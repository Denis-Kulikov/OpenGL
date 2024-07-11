#pragma once
#include "../lib-project/lib.hpp"

class GameTime {
private:
    std::chrono::steady_clock::time_point m_prevTime;
    std::chrono::steady_clock::time_point m_currentTime;
    std::chrono::steady_clock::time_point Start;

public:
    GameTime()
    {
        Start = m_currentTime = m_prevTime = std::chrono::steady_clock::now();
    }

    void Update() {
        m_prevTime = m_currentTime;
        m_currentTime = std::chrono::steady_clock::now();
    }

    float GetDeltaTime() const {
        std::chrono::duration<float> deltaTime = m_currentTime - m_prevTime;
        return deltaTime.count();
    }

    float GetCurrentTime() const {
        std::chrono::duration<float> currentTime = m_currentTime - Start;
        return currentTime.count();
    }
};
