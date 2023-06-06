#pragma once

#include <string>
#include <time.h>

class RealTime
{
    private:
        const char* NTP_SERVER = "europe.pool.ntp.org";
        const long  GMT_OFFSET_SEC = 0;
        const int   DAYLIGHT_OFFSET_SEC = 3600;

        char m_ntpServer[50];   //Arbitrary number
        long m_gmtOffsetSec;
        int m_daylightOffsetSec;
        
    public:
        RealTime();
        RealTime(const char * ntpServer, long gmtOffsetSec, int daylightOffsetSec);
        ~RealTime() {}

        std::string GetLocalTimeAsString();
};