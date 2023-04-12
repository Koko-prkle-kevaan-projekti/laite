/***************************************************************************************/
/**** TASSUTUTKA ***********************************************************************/
/**** TIETOTEKNIIKAN SOVELLUSPROJEKTI, OULUN AMMATTIKORKEAKOULU, TVT23KMO **************/
/**** TEKIJÄT : MIKKO KUJALA / PASI PUHAKKA / REBECCA SOISENNIEMI / NICO HERTOLIN ******/
/***************************************************************************************/

#include <SoftwareSerial.h>
#include "tassudata.h"
#include "settings.h"

SoftwareSerial gpsSerial(7, 8);
int Powerkey = 9;

void setup()
{
  pinMode(Powerkey, OUTPUT);    // initialize the digital pin as an output.
  powerOnSIM808 ();
  gpsSerial.begin(19200);
  Serial.begin(9600);
  enableGPS();
  connect(gpsSerial, SERVER_IP_ADDRESS);

}

void loop()
{
  if(Serial.available())
  gpsSerial.print((char)Serial.read());

  else if(gpsSerial.available())
  Serial.print((char)gpsSerial.read());

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
  gpsSerial.println("AT");
  delay(4000);
  gpsSerial.println("AT+CFUN=0");
  delay(1000);
  gpsSerial.println("AT+CGNSPWR=1");
}
