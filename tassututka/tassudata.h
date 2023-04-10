#ifndef TASSUDATA_SEND_H
#define TASSUDATA_SEND_H

// Connect to server
bool connect(char* ipAddress, int ipAddressLength);


// GPS Data Handling
bool sendGPSData(char* deviceName, int deviceNameLength, char* gpsData, int gpsDataLength);


#endif

