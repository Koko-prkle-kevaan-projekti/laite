#include <HardwareSerial.h>     // for Serial
#include <Arduino.h>       // for millis() 
#include "tassudata.h"
#include "settings.h"

// AT Command Helper 
// sends command into the serial and waits for the response
int atCommandHelper(Stream & serial, char* ATcommand, char* ATresponse1, char* ATresponse2, unsigned long timeout) {
    Serial.println("AT Command:");
    Serial.println(ATcommand);
    while (serial.available() > 0) {
        serial.read();
    }

    uint8_t pos = 0;
    int answer = 0;
    char response[100];
    
    memset(response, ' ', 100);     // sets up a string as empty

    serial.println(ATcommand);
    delay(100);
    unsigned long start = millis();
    
    do {
        if (serial.available() != 0) {
            response[pos] = serial.read();
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

void atCommandResponse(Stream & serial, char *ATcommand, char *answer)
{
    Serial.println("AT Command");
    Serial.println(ATcommand);
    while (serial.available() > 0) {
        serial.read();
    }
}

// Connect to server
bool connect(Stream & serial, char* ipAddress) {

    // Disables cordinates until server connection is done 
    if (atCommandHelper(serial, "AT+CGPSOUT=0", "OK", "", 2000) != 1) {
        Serial.println("ERROR: disabling cordinates failed.");
        return false;
    }

    // checks if PIN code is required
    if (atCommandHelper(serial, "AT+CPIN?", "READY", "ERROR", 10000) != 1) {
        Serial.println("ERROR: cannot initialize SIM card");
        return false;
    }

    if (atCommandHelper(serial, "AT+CREG=1", "OK", "", 1000) != 1) {
        Serial.println("ERROR: Network registration error or timeout.");
    }

    // connecting to the network 
    Serial.println("Network connection");
    while (atCommandHelper(serial, "AT+CREG?", "+CREG: 1,3", "+CREG: 5,3", 1000) == 0);

    // setting up single-IP connection mode
    if (atCommandHelper(serial, "AT+CIPMUX=0", "OK", "ERROR", 1000) != 1) {
        Serial.println("ERROR: Could not set single-IP connection mode");
        return false;
    }
    // gets current connection status IP INITIAL
    while (atCommandHelper(serial, "AT+CIPSTATUS", "INITIAL", "", 500) == 0);

    char connectAPN [40] = {'\0'};
    strcat (connectAPN, "AT+CSTT=\"");
    strcat (connectAPN, APN_CONNECTION);
    strcat (connectAPN, "\",\"");
    strcat (connectAPN, APN_USERNAME);
    strcat (connectAPN, "\",\"");
    strcat (connectAPN, APN_PASSWORD);
    strcat (connectAPN, "\"");
    // sets the APN, username and pw 
    if (atCommandHelper (serial, connectAPN, "OK", "ERROR", 30000) == 1) {
        Serial.println("ERROR: Setting up APN Connection failed.");
    }

    // Waits for status IP START
	while (atCommandHelper(serial, "AT+CIPSTATUS", "START", "", 500) == 0);
	delay(5000);

    // Brings Up Wireless Connection with GPRS
	if (atCommandHelper(serial, "AT+CIICR", "OK", "ERROR", 30000) != 1) {
        Serial.println("ERROR: Did not success with Wireles connection (GPRS)");
        return false;
    }

    // Waits for status. GPRRACT means that it has approved commands before it
    while (atCommandHelper(serial, "AT+CIPSTATUS", "GPRSACT", "", 500) == 0);
    delay(5000);

    // Gets Local IP Address
    if (atCommandHelper(serial, "AT+CIFSR", ".", "ERROR", 10000) != 1) {
        Serial.println("ERROR: Couldn't find local IP Address.");
        return false;
    } 

    // Waits for status IP STATUS
    while (atCommandHelper(serial, "AT+CIPSTATUS", "IP STATUS", "", 500) == 0);
    delay(5000);
    Serial.println("Opening TCP");

    char tcpSocketCommand [100] = {'\0'};
    strcat (tcpSocketCommand, "AT+CIPSTART=\"TCP\",\"");
    strcat (tcpSocketCommand, SERVER_IP_ADDRESS);
    strcat (tcpSocketCommand, "\",\"65000\"");
    
    // Opens a TCP socket
    if (atCommandHelper(serial, tcpSocketCommand, "CONNECT OK", "CONNECT FAIL", 30000) != 1) {
        Serial.println("ERROR: Could'nt open up TCP Socket.");
    }
    
    Serial.println("Connected, well done!");

    // Cordinates out
    if (atCommandHelper(serial, "AT+CGPSOUT=32", "OK", "", 2000) != 1) {
        Serial.println("ERROR: No cordinates");
    }


    return true;
}

// GPS Data Handling
bool sendGPSData(Stream & serial, char* deviceName, char* gpsData) {
    
    char sendDataCommand [50];
    char data [100] = {'\0'};
    strcat (data, deviceName);
    strcat (data, ":");
    strcat (data, gpsData);
    // Sends some data to the TCP socket
    sprintf(sendDataCommand, "AT+CIPSEND=%d", strlen (data));
    if (atCommandHelper(serial, sendDataCommand, ">", "ERROR", 10000) == 1)
    {
        atCommandHelper(serial, data, "SEND OK", "ERROR", 10000);
    } else {
        Serial.println("ERROR: Couldn't send data to socket.");
        return false;
    }
    
    return true;
}

bool isConnected(Stream &serial)
{
    // Waits for status IP STATUS
    if (atCommandHelper(serial, "AT+CIPSTATUS=?", "CONNECT OK", "", 500) != 1) {
        Serial.println("Not connected");
        return false;
    }

    Serial.println("Connection ok.");
    return true;
}
