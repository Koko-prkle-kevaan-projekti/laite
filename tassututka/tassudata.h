#include <SoftwareSerial.h>

#ifndef TASSUDATA_SEND_H
#define TASSUDATA_SEND_H

// AT Command Helper 
// sends command into the serial and waits for the response
int atCommandHelper(SoftwareSerial & swSerial, char* ATcommand, char* ATresponse1, char* ATresponse2, unsigned long timeout);

// Connect to server
bool connect(SoftwareSerial & swSerial, char* ipAddress);

// GPS Data Handling
bool sendGPSData(SoftwareSerial & swSerial, char* deviceName, char* gpsData);

#endif

