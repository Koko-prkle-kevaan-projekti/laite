/***************************************************************************************/
/**** TASSUTUTKA ***********************************************************************/
/**** TIETOTEKNIIKAN SOVELLUSPROJEKTI, OULUN AMMATTIKORKEAKOULU, TVT23KMO **************/
/**** TEKIJÄT : MIKKO KUJALA / PASI PUHAKKA / REBECCA SOISENNIEMI / NICO HERTOLIN ******/
/***************************************************************************************/

#include <SoftwareSerial.h>
#include "tassudata.h"
#include "settings.h"

SoftwareSerial gpsSerial(12, 13); // RX, TX *UNO 7,8 *MEGA 12, 13
//SoftwareSerial ffuzzSerial(3, -1); // feather fuzz RX, TX (3, -1 OR 3, 1)

// Compiles with MEGA. If you want to use UNO instead, uncomment next line: 
// SoftwareSerial Serial1(10, 11); // RX, TX

int Powerkey = 9;

// WORK IN PROGRESS: Trying to automatize AT command functionalities with the device
// so there's no need to apply commands by keyboard after all
// If this goes well, then we might be able to get back to earlier stage with gprs 
void setup()
{
  pinMode(Powerkey, OUTPUT);    // initialize the digital pin as an output.
  powerOnSIM808 ();
  Serial.begin(115200);
  Serial1.begin(9600);
  gpsSerial.begin(9600);
  //ffuzzSerial.begin(9600);
  Serial.println("Starting now.");
  enableGPS(); // <<
  connect(gpsSerial, SERVER_IP_ADDRESS);

}

void loop()
{
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
}

void enableGPS() 
{
  delay(1000);

// Serial printlines: with MEGA Serial1 instead of gpsSerial
  Serial1.println("AT");
  delay(4000);

// FLIGHT MODE: for stabilizing the power. 
// GPS works normally and doesnt break all the time because of all shit
  Serial1.println("AT+CFUN=4"); 
  delay(1000);

  Serial1.println("AT+CGNSPWR=1");
  delay(1000);

}

