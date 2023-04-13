#include <HardwareSerial.h>     // for Serial
#include <Arduino.h>     // for millis() 
#include "tassudata.h"
#include "settings.h"

// AT Command Helper 
// sends command into the serial and waits for the response
int atCommandHelper(SoftwareSerial & swSerial, char* ATcommand, char* ATresponse1, char* ATresponse2, unsigned long timeout) {
    Serial.println("AT Command");
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
    Serial.println("ERROR: Timeout");
    Serial.println(response);
    return answer; 
}

// Connect to server
bool connect(SoftwareSerial & swSerial, char* ipAddress) {
    // checks if PIN code is required
    if (atCommandHelper(swSerial, "AT+CPIN?", "READY", "ERROR", 3000) != 1) {
        Serial.println("ERROR: cannot initialize SIM card");
        return false;
    }

    // connecting to the network 
    Serial.println("Network connection");
    while (atCommandHelper(swSerial, "AT+CREG?", "+CREG: 0,1", "+CREG: 0,5", 1000) == 0);

    // setting up single-IP connection mode
    if (atCommandHelper(swSerial, "AT+CIPMUX=0", "OK", "ERROR", 1000) != 1) {
        Serial.println("ERROR: Could not set single-IP connection mode");
        return false;
    }
    // gets current connection status IP INITIAL
    while (atCommandHelper(swSerial, "AT+CIPSTATUS", "INITIAL", "", 500) == 0);

    // sets the APN, username and pw 
    // todo if necessary

    // Waits for status IP START
	while (atCommandHelper(swSerial, "AT+CIPSTATUS", "START", "", 500) == 0);
	delay(5000);

    // Brings Up Wireless Connection with GPRS
	if (atCommandHelper(swSerial, "AT+CIICR", "OK", "ERROR", 30000) != 1) {
        Serial.println("ERROR: Did not success with Wireles connection (GPRS)");
        return false;
    }

    // Waits for status. GPRRACT means that it has approved commands before it
    while (atCommandHelper(swSerial, "AT+CIPSTATUS", "GPRSACT", "", 500) == 0);
    delay(5000);

    // Gets Local IP Address
    if (atCommandHelper(swSerial, "AT+CIFSR", ".", "ERROR", 10000) != 1) {
        Serial.println("ERROR: Couldn't find local IP Address.");
        return false;
    } 

    // Waits for status IP STATUS
    while (atCommandHelper(swSerial, "AT+CIPSTATUS", "IP STATUS", "", 500) == 0);
    delay(5000);
    Serial.println("Openning TCP");


    char tcpSocketCommand [100] = {'\0'};
    strcat (tcpSocketCommand, "AT+CIPSTART=\"TCP\",\"");
    strcat (tcpSocketCommand, SERVER_IP_ADDRESS);
    strcat (tcpSocketCommand, "\",\"65000\"");
    
    // Opens a TCP socket
    if (atCommandHelper(swSerial, tcpSocketCommand, "CONNECT OK", "CONNECT FAIL", 30000) != 1) {
        Serial.println("ERROR: Could'nt open up TCP Socket.");
    }
    
    Serial.println("Connected, well done!");

    return true;
}

// GPS Data Handling
bool sendGPSData(SoftwareSerial & swSerial, char* deviceName, char* gpsData) {
    
    char sendDataCommand [50];
    char data [100] = {'\0'};
    strcat (data, deviceName);
    strcat (data, ":");
    strcat (data, gpsData);
    // Sends some data to the TCP socket
    sprintf(sendDataCommand, "AT+CIPSEND=%d", strlen (data));
    if (atCommandHelper(swSerial, sendDataCommand, ">", "ERROR", 10000) == 1)
    {
        atCommandHelper(swSerial, data, "SEND OK", "ERROR", 10000);
    } else {
        Serial.println("ERROR: Couldn't send data to socket.");
        return false;
    }
    
    return true;
}

/*COMMANDS AND SOME INFO
  Serial1 is using pins 18 and 19 

  GSMSerial.println(" AT+CMEE=1");
  AT+CGNSPWR=1       // power on GPS
  AT+CGNSINF       // asks for gnss info
  AT+CGPSSTATUS?    // connection status
    4 responses; loc unknown, not fixed, 2d, 3d < 2d or 3d response next one 32
  AT+CGPSOUT=32     // cordinates

  AT+CGPSOUT=0    // no  cordinates
  AT+CGPSRST=0    //
  AT+CGNSTST=1    //
  AT+CCLK?       //
  AT+CFUN=0 or 1 or 4 // 0=minimum functionality 1=full functionality defaul 4 = flightmode disable RF
  AT+COPS     //

*/