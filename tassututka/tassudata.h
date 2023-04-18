#include <SoftwareSerial.h>

#ifndef TASSUDATA_SEND_H
#define TASSUDATA_SEND_H

// AT Command Helper 
// Sends command into the serial and waits for the response
// Returns 1 if command's response contains ATresponse1
// Returns 2 if command's response contains ATresponse2
// Returns 0 if command's response contains something else or doesn't answer in specified time
int atCommandHelper(Stream & serial, char* ATcommand, char* ATresponse1, char* ATresponse2, unsigned long timeout);

// AT Command Response
void atCommandResponse(Stream & serial, char* ATcommand, char* answer);

// Connect to server 
// Returns true if succesfully connected to IP Address. 
// Returns false if some command fails.
bool connect(Stream & serial, char* ipAddress);

// GPS Data Handling
// Returns true if succesfully sends data to server.
// Returns false if sending data to server fails. 
bool sendGPSData(Stream & serial, char* deviceName, char* gpsData);

// Conncection check
// Returns true if connection is still Ok.
// Returns false if connection is not connected.
bool isConnected(Stream & serial);

#endif

