#include "TimerModule.h"

Timer::Timer() : Timer(0) {}

Timer::Timer(uint32_t durationMs)
{
    m_durationMs = durationMs;
    m_isEnabled = false;
}

/// @brief Determines if the timer is elapsed or not
/// @return True if the timer has elapsed OR if it is disabled, false if it is still running
bool Timer::Elapsed()
{
    if (!IsEnabled())
    {
        // TODO: Further discussion on true or false if timer is disabled
        //  Timer not running, therefore is elapsed
        return true;
    }

    if (millis() - m_timeStartedMs > m_durationMs)
    {
        Disable();
        return true;
    }

    return false;
}