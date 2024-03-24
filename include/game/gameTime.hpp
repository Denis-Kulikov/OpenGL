#pragma once
#include "../lib-project/lib.hpp"
#include "../entities/components/motion.hpp"

class GameTime {
private:
    std::chrono::steady_clock::time_point m_prevTime;
    std::chrono::steady_clock::time_point m_currentTime;

public:
    GameTime() {
        m_prevTime = std::chrono::steady_clock::now();
        m_currentTime = m_prevTime;
    }

    void Update() {
        m_prevTime = m_currentTime;
        m_currentTime = std::chrono::steady_clock::now();
        Motion::PushTime(static_cast<float>(m_currentTime.time_since_epoch().count()));
    }

    float GetDeltaTime() const {
        std::chrono::duration<float> deltaTime = m_currentTime - m_prevTime;
        return deltaTime.count();
    }

    float GetCurrentTime() const {
        std::chrono::duration<float> currentTime = m_currentTime.time_since_epoch();
        return currentTime.count();
    }
};
