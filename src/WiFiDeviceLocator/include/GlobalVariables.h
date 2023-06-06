#pragma once
#include <mutex>
#include <vector>
#include "MacData.h"


#define DEVICE_NAME "ESP-1"
#define WIFI_CHANNEL 1

#warning Insert valid wifi credentials
#define SSID "insert"
#define PASSWORD "password"

extern std::mutex macManipulationgMutex;    //Used for locking macData
extern std::vector<MacData> macData; //Vector containing the most-recently updated mac data