#pragma once

#include <string>
#include <time.h>

class RealTime
{
    private:
        char m_ntpServer[50];   //Arbitrary number
        long m_gmtOffsetSec;
        int m_daylightOffsetSec;
        
    public:
        RealTime();
        RealTime(const char * ntpServer, long gmtOffsetSec, int daylightOffsetSec);
        ~RealTime() {}

        std::string GetLocalTimeAsString();
};