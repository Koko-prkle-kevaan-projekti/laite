#include <SoftwareSerial.h>

#ifndef TASSUDATA_SEND_H
#define TASSUDATA_SEND_H

// AT Command Helper 
// sends command into the serial and waits for the response
int atCommandHelper(Stream & serial, char* ATcommand, char* ATresponse1, char* ATresponse2, unsigned long timeout);

// AT Command Response
void atCommandResponse(Stream & serial, char* ATcommand, char* answer);

// Connect to server
bool connect(Stream & serial, char* ipAddress);

// GPS Data Handling
bool sendGPSData(Stream & serial, char* deviceName, char* gpsData);

// Conncection check
bool isConnected(Stream & serial);



#endif

