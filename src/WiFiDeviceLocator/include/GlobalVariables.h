#pragma once
#include <mutex>
#include <vector>
#include "MacData.h"


#define DEVICE_NAME "ESP1"
#define WIFI_CHANNEL 1

#warning Insert valid wifi credentials
#define WIFI_SSID "Proftaak"
#define WIFI_PASSWORD "docock123"

extern std::mutex macManipulationgMutex;    //Used for locking macData
extern std::vector<MacData> macData; //Vector containing the most-recently updated mac data