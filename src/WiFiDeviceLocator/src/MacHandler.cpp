#include "MacHandler.h"
#include "VectorManipulator.h"
#include "DebugPrint.h"

MacHandler::MacHandler()
{
    SetMacTimeout(10 MINUTES);
    SetTimeoutCheckInterval(10 SECONDS);
    m_macValidityCheckTimer = Timer(m_timeoutCheckIntervalMs);

    m_macValidityCheckTimer.Start();
}

//Note: Handle timers in a separate thread to avoid having to have .Handle()'s?
/// @brief To be called every cycle
void MacHandler::Handle()
{   
    if (m_macValidityCheckTimer.Elapsed())
    {
        RemoveTimedOutMacs(millis());

        m_macValidityCheckTimer.Start();
    }
}

/// @brief Updates the mac inside macData or adds it if it does not exist
/// @param macData The mac data to add or update
void MacHandler::AddOrUpdateMacInfo(const MacData &macData)
{
    int macIndex = GetMacIndex(macData.macAddress);

    if (macIndex == -1)
    {
        AddMacInfo(macData);
    }
    else
    {
        UpdateMacInfo(macIndex, macData);
    }
}

/// @brief Set the amount of time to keep a MAC in the list before it is deleted
/// @param timeoutMs The timeout, in milliseconds
void MacHandler::SetMacTimeout(uint32_t timeoutMs)
{
    m_macTimeoutMs = timeoutMs;
}

/// @brief Sets the interval between checking for MAC validity and restarts the timer
/// @param checkIntervalMs The interval, in milliseconds
void MacHandler::SetTimeoutCheckInterval(uint32_t checkIntervalMs)
{
    m_timeoutCheckIntervalMs = checkIntervalMs;
    m_macValidityCheckTimer.Disable();
    m_macValidityCheckTimer.SetDuration(m_timeoutCheckIntervalMs);
    m_macValidityCheckTimer.Start();
}

/// @brief Gets the found macs' count
/// @return The count of the macs found
uint32_t MacHandler::GetMacCount() const
{
    return m_macDataVector.size();
}

/// @brief Removes the MAC info with the given index
/// @param index The index of the mac address to remove
/// @return True on success, false otherwise
bool MacHandler::RemoveMacInfo(int index)
{
    if (!IsValidIndex(index))
    {
        return false;
    }

    m_macDataVector.erase(m_macDataVector.begin() + index);

    return true;
}

/// @brief Removes timed-out macs from the list
void MacHandler::RemoveTimedOutMacs(uint32_t currentTimestamp)
{
    std::vector<int> indeciesToRemove;

    int currentIndex = 0;

    for (MacData data : m_macDataVector)
    {
        uint32_t differenceOfDataTimeToMaxValue = UINT32_MAX - data.timestamp;
        uint32_t actualDifference = differenceOfDataTimeToMaxValue + currentTimestamp + 1; //+1 because 0 counts as a number too when overflowing

        if (actualDifference > m_macTimeoutMs)
        {
            //Timeout
            indeciesToRemove.push_back(currentIndex);
        }

        currentIndex++;
    }
    
    VectorManipulator::DeleteIndecies(m_macDataVector, indeciesToRemove.begin(), indeciesToRemove.end());
}

/// @brief Adds mac data to the known mac addresses list
/// @param macData The mac data to add
void MacHandler::AddMacInfo(const MacData &macData)
{
    m_macDataVector.push_back(macData);
}

/// @brief Updates mac data on position index
/// @param index The index of the item to update
/// @param newMacData The new data for the data at position index
void MacHandler::UpdateMacInfo(int index, const MacData &newMacData)
{
    if (!IsValidIndex(index))
    {
        //Invalid index
        return;
    }

    m_macDataVector[index] = newMacData;
}

/// @brief Checks if an index is valid or not
/// @param index The index to check
/// @return True on valid, false on invalid
bool MacHandler::IsValidIndex(int index) const
{
    return index >= 0 && index < m_macDataVector.size();
}

/// @brief Finds the mac address' index in the storage container
/// @param macAddress The mac address to check
/// @return The index of the mac if it exists, -1 otherwise
int MacHandler::GetMacIndex(const std::vector<uint8_t> &macAddress) const
{
    for (int i = 0; i < m_macDataVector.size(); i++)
    {
        if (m_macDataVector[i].macAddress == macAddress)
        {
            return i;
        }
    }
    
    return -1;
}

/// @brief Gets all data a pretty string
/// @return The output string
std::string MacHandler::GetDataAsPrettyString() const
{
    std::string result = "";

    for (MacData currentData : m_macDataVector)
    {
        result += currentData.ToString() + "\n";
    }
    
    result.pop_back();  //Remove trailing \n
    return result;
}
