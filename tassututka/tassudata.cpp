#include <HardwareSerial.h>     // for Serial
#include <Arduino.h>     // for millis() 
#include "tassudata.h"

// AT Command Helper 
// sends command into the serial and waits for the response
int atCommandHelper(SoftwareSerial & swSerial, char* ATcommand, char* ATresponse1, char* ATresponse2, unsigned long timeout) {
    while (swSerial.available() > 0) {
        swSerial.read();
    }

    uint8_t pos = 0;
    int answer = 0;
    char response[100];
    
    memset(response, ' ', 100);     // sets up a string as empty

    swSerial.println(ATcommand);
    unsigned long start = millis();
    
    do {
        if (swSerial.available() != 0) {
            response[pos] = swSerial.read();
            pos++;
            
            // strstr checks if response contains ATresponse1 
            if (strstr(response, ATresponse1) != NULL)
            {
                answer = 1;
            }
            else if (strstr (response, ATresponse2) != NULL)
            {
                answer = 2;
            }
        }

    } while ((answer == 0) && ((millis() - start) < timeout));

    return answer; 
}

// Connect to server
bool connect(SoftwareSerial & swSerial, char* ipAddress) {
    // connecting to the network 
    while (atCommandHelper(swSerial, "AT+CREG?", "+CREG: 0,1", "+CREG: 0,5", 1000) == 0);

    // setting up single-IP connection mode
    if (atCommandHelper(swSerial, "AT+CIPMUX=0", "OK", "ERROR", 1000) == 2) {
        Serial.println("ERROR: Could not set single-IP connection mode");
        return false;
    }

    // gets current connection status
    while (atCommandHelper(swSerial, "AT+CIPSTATUS", "INITIAL", "", 500) == 0);

    return true;
}

// GPS Data Handling
bool sendGPSData(char* deviceName, int deviceNameLength, char* gpsData, int gpsDataLength) {
    return false;
}