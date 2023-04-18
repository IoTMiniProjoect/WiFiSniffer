#pragma once

//Uncomment or define somewhere else to see debug messages
#define DEBUG_MODE

#ifdef DEBUG_MODE
    #define DEBUG_PRINT(message) Serial.print(message)
    #define DEBUG_PRINTLN(message) Serial.println(message)
#else
    #define DEBUG_PRINT(message)
    #define DEBUG_PRINTLN(message)
#endif