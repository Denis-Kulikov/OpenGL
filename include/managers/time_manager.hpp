#pragma once
#include <chrono>

class TimeManager {
public:
    static void Initialize();
    static void Update();
    static float GetDeltaTime();
    static float GetCurrentTime();

private:
    static inline std::chrono::steady_clock::time_point m_prevTime;
    static inline std::chrono::steady_clock::time_point m_currentTime;
    static inline std::chrono::steady_clock::time_point Start;
};
