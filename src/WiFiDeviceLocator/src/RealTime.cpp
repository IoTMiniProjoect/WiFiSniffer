#include "RealTime.h"
#include <sstream>  //Note: Remove if too beefy
#include "DebugPrint.h"
#include <time.h>
#include <cstring>

#define NTP_SERVER "europe.pool.ntp.org"
#define GMT_OFFSET_SEC 0
#define DAYLIGHT_OFFSET_SEC 3600

RealTime::RealTime() : RealTime(NTP_SERVER, GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC) { }

RealTime::RealTime(const char *ntpServer, long gmtOffsetSec, int daylightOffsetSec)
{
    strcpy(m_ntpServer, ntpServer);
    m_gmtOffsetSec = gmtOffsetSec;
    m_daylightOffsetSec = daylightOffsetSec;

    configTime(m_gmtOffsetSec, m_daylightOffsetSec, m_ntpServer);
    GetLocalTimeAsString(); //Call to get initial time
}

/// @brief Get the local time as a string. Format: hh:mm:ss
/// @return The time string on success, empty string on failure
std::string RealTime::GetLocalTimeAsString()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        DEBUG_PRINTLN("Failed to obtain time");
        return "";
    }

    std::stringstream timeStream;
    timeStream << timeinfo.tm_hour + 1
               << ':'
               << timeinfo.tm_min
               << ':'
               << timeinfo.tm_sec;
    
    return timeStream.str();
}