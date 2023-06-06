#pragma once

#include <Arduino.h>

#define SECONDS * 1000
#define MINUTES * 60 SECONDS

class Timer
{
private:
    uint32_t m_durationMs = 0;
    uint32_t m_timeStartedMs = 0;
    bool m_isEnabled;

public:
    Timer();
    explicit Timer(uint32_t durationMs);
    ~Timer() = default;

    inline void Start();
    inline void Start(uint32_t durationMs);
    inline void Disable();
    inline void SetDuration(uint32_t durationMs);

    bool Elapsed();

    inline bool IsEnabled();
};

inline void Timer::Start()
{
    m_isEnabled = true;
    m_timeStartedMs = millis();
}

inline void Timer::Start(uint32_t durationMs)
{
    SetDuration(durationMs);
    Start();
}

inline void Timer::Disable()
{
    m_isEnabled = false;
}

inline void Timer::SetDuration(uint32_t durationMs)
{
    m_durationMs = durationMs;
}

inline bool Timer::IsEnabled()
{
    return m_isEnabled;
}