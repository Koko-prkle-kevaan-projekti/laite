/***************************************************************************************/
/**** TASSUTUTKA ***********************************************************************/
/**** TIETOTEKNIIKAN SOVELLUSPROJEKTI, OULUN AMMATTIKORKEAKOULU, TVT23KMO **************/
/**** TEKIJÄT : MIKKO KUJALA / PASI PUHAKKA / REBECCA SOISENNIEMI / NICO HERTOLIN ******/
/***************************************************************************************/

#include <SoftwareSerial.h>
#include "tassudata.h"
#include "settings.h"

#if ((defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)))
  bool wlan = true; // if want to try gprs with mega, change this value from true to false
  Stream & serial = Serial1; 
#else
  bool wlan = false;
  SoftwareSerial gpsSerial(7, 8); 
  Stream & serial = gpsSerial;
#endif

SoftwareSerial ffuzzSerial(-1, 13); 

int Powerkey = 9;

// WORK IN PROGRESS: Trying to automatize AT command functionalities with the device
// so there's no need to apply commands by keyboard after all
// If this goes well, then we might be able to get back to earlier stage with gprs 
void setup()
{

  pinMode(Powerkey, OUTPUT);    // initialize the digital pin as an output.
  Serial.begin(9600);
  #if ((defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)))
    Serial1.begin(9600);
    ffuzzSerial.begin(9600); 
  #else
    gpsSerial.begin(9600);
  #endif

  powerOnSIM808 ();
  Serial.println("Starting now.");
  enableGPS(); 

}

void loop()
{

  if(wlan) {
    if (Serial.available()) {
      char data = Serial.read();
      serial.print(data);      // Send to shield
    }
    if (serial.available()) {
      char data = serial.read();
      Serial.print(data);          // Print to serial monitor
      ffuzzSerial.print(data);     // Send to other device
    }

  } else {
    if (isConnected(serial)) { 
      delay(1000);
      char gdata[100];
      memset(gdata, ' ', 100); 

      int pos = 0;
      while (serial.available()) {
        gdata[pos] = serial.read();
        pos++;
      }

      sendGPSData(serial, DEVICE_NAME, gdata);
    } else {
      connect(serial, SERVER_IP_ADDRESS);
    }
    
  }

}

void powerOnSIM808(void) 
{

  digitalWrite(Powerkey, LOW);
  delay(500);
  digitalWrite(Powerkey, HIGH);
  delay(4000);
  Serial.println("Wait for AT to be sent succesfully.");
  delay(4000);
  while (atCommandHelper(serial, "AT", "OK", "", 2500) == 0);

}

void enableGPS() 
{

// Waiting for phone functionality to be done
  while (atCommandHelper(serial, "AT+CFUN=1", "OK", "ERROR", 3000) != 1);

// Waiting for power on GNSS to be accepted
  while (atCommandHelper(serial, "AT+CGNSPWR=1", "OK", "ERROR", 3500) != 1);

// GPS Status check
  while (atCommandHelper(serial, "AT+CGPSSTATUS?", "2D", "3D", 2500) == 0);
  Serial.println("GPS Status OK");

// Cordinates out
  if (atCommandHelper(serial, "AT+CGPSOUT=32", "OK", "", 2000) != 1) {
    Serial.println("ERROR: No cordinates");
  }

}
