#pragma once
#include "../lib-project/lib.hpp"

class GameTime {
private:
    std::chrono::steady_clock::time_point m_prevTime;
    std::chrono::steady_clock::time_point m_currentTime;

public:
    GameTime() {
        m_prevTime = std::chrono::steady_clock::now();
        m_currentTime = m_prevTime;
        Start = static_cast<float>(m_currentTime.time_since_epoch().count());
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
        return static_cast<float>(m_currentTime.time_since_epoch().count()) - Start;
    }

    float Start = 0.0;
};
