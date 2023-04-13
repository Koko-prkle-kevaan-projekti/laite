#include <SoftwareSerial.h>

#ifndef TASSUDATA_SEND_H
#define TASSUDATA_SEND_H

// Connect to server
bool connect(SoftwareSerial & swSerial, char* ipAddress);


// GPS Data Handling
bool sendGPSData(SoftwareSerial & swSerial, char* deviceName, char* gpsData);


#endif

