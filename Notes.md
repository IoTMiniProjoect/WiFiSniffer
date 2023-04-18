# Research paper
1. Research question
2. Steps taken
3. 

## IMRaD

Introduction Methods Results analysis Discussion

Scrum + research = maybe not

## Next week

- Simple WiFi sniffer application

- Scan every ~1 second for devices

- Random MAC handling (most phones past 2005)
    - Once the phone connects, it maintains the MAC address

- Scrape some data to analyze

## ESP32
- APSTA mode
    - Have other devices probe it
    - Connect it to another network to get the data off
        - BL can be used
    - Promiscuous mode to capture EVERY packet
        - Filter on probe requests 
- Examples can be found
- Huge amount of data from probing requests
    - Has to be filtered
    - Colerate to # of people

- What is the scan range

Diff channels, same SSID, map  to the same device from multiple stations -> research

At least 3 WiFi sniffers
Assume device identification based on previous location