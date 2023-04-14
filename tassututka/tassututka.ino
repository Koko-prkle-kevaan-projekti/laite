/***************************************************************************************/
/**** TASSUTUTKA ***********************************************************************/
/**** TIETOTEKNIIKAN SOVELLUSPROJEKTI, OULUN AMMATTIKORKEAKOULU, TVT23KMO **************/
/**** TEKIJÄT : MIKKO KUJALA / PASI PUHAKKA / REBECCA SOISENNIEMI / NICO HERTOLIN ******/
/***************************************************************************************/

#include <SoftwareSerial.h>
#include "tassudata.h"
#include "settings.h"

#if ((defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)))
  SoftwareSerial gpsSerial(12, 13); // MEGA 12, 13
#else
  SoftwareSerial gpsSerial(7, 8); // RX, TX *UNO 7,8 tai 10, 11 ? 
#endif

//SoftwareSerial ffuzzSerial(3, -1); // feather fuzz RX, TX (3, -1 OR 3, 1)

int Powerkey = 9;

// WORK IN PROGRESS: Trying to automatize AT command functionalities with the device
// so there's no need to apply commands by keyboard after all
// If this goes well, then we might be able to get back to earlier stage with gprs 
void setup()
{

  pinMode(Powerkey, OUTPUT);    // initialize the digital pin as an output.
  powerOnSIM808 ();
  Serial.begin(9600);
  #if ((defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)))
    Serial1.begin(9600);
  #endif
  gpsSerial.begin(9600);
  //ffuzzSerial.begin(9600); 
  Serial.println("Starting now.");
  //enableGPS();  // commented off for a moment 

}

void loop()
{

  connect(gpsSerial, SERVER_IP_ADDRESS);

 /* if(Serial.available())
  gpsSerial.print((char)Serial.read());

  else if(gpsSerial.available())
  Serial.print((char)gpsSerial.read());*/

// Sends/prints to serial monitor / shield / other device etc.

/*
    if (Serial.available()) {
      char srl = Serial.read();

      Serial1.print(srl); 
      gpsSerial.print(srl); 
      ffuzzSerial.print(srl); 
    }

    if (Serial1.available()) {
      char srl = Serial1.read();

      Serial.print(srl); 
      gpsSerial.print(srl); 
      ffuzzSerial.print(srl); 
    }
      
    if (gpsSerial.available()) {
      char srl = gpsSerial.read();

      Serial.print(srl);     
      Serial1.print(srl); 
      ffuzzSerial.print(srl); 
    }

    if (ffuzzSerial.available()) {
      char srl = ffuzzSerial.read();

      Serial.print(srl);   
      Serial1.print(srl); 
      gpsSerial.print(srl); 
    }
    */
      
}

void powerOnSIM808(void) 
{
  digitalWrite(Powerkey, LOW);
  delay(500);
  digitalWrite(Powerkey, HIGH);

  Serial.println("Wait for AT to be sent succesfully.");
  while (atCommandHelper(gpsSerial, "AT", "OK", "", 2400) == 0);

}

void enableGPS() 
{
  
// FLIGHT MODE: 4 - for stabilizing the power. 
// GPS works normally and doesnt break all the time because of all shit

// Phone functionality setup
  if (atCommandHelper(gpsSerial, "AT+CFUN=1", "OK", "ERROR", 3000) != 1) {
    Serial.println("ERROR: Phone functionality setup failed.");
  }

// Power on GNSS 
    if (atCommandHelper(gpsSerial, "AT+CGNSPWR=1", "OK", "ERROR", 3600) != 1) {
    Serial.println("ERROR: GNSS didn't start");
  }

//while (atCommandHelper(gpsSerial, "AT+CGNSINF", "OK", "", 1000) == 0);


}

