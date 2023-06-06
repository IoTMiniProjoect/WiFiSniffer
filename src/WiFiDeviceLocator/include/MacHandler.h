#pragma once

#include <vector>
#include <stdint.h>
#include "MacData.h"
#include <stdint.h>
#include "TimerModule.h"

class MacHandler
{
    private:
        std::vector<MacData> m_macDataVector;
        uint32_t m_macTimeoutMs;
        uint32_t m_timeoutCheckIntervalMs;
        Timer m_macValidityCheckTimer = Timer(m_timeoutCheckIntervalMs);
        
        void AddMacInfo(const MacData &macData);
        void UpdateMacInfo(int index, const MacData &newMacData);
        bool RemoveMacInfo(int index);
        void RemoveTimedOutMacs(uint32_t currentTimestamp);
        bool IsValidIndex(int index) const;

    public:
        MacHandler();
        ~MacHandler() = default;

        void Handle();

        void AddOrUpdateMacInfo(const MacData &macData);
        void SetMacTimeout(uint32_t timeoutMs);
        void SetTimeoutCheckInterval(uint32_t checkIntervalMs);

        uint32_t GetMacCount() const;
        int GetMacIndex(const std::vector<uint8_t> &macAddress) const;
        std::string GetDataAsPrettyString() const;
        std::vector<MacData> GetDataAsVector() const;
};
