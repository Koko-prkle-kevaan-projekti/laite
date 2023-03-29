/************* TASSUTUTKA ****************/
/**** TIETOTEKNIIKAN SOVELLUSPROJEKTI ****/
/********************************* OAMK **/
/**** ARDUINOKOODIT **********************/
/****************** PASI PUHAKKA *********/
/****************** MIKKO KUJALA *********/
/****************** NICO HERTOLIN ********/
/****************** REBECCA SOISENNIEMI***/

#include <SoftwareSerial.h>
SoftwareSerial GSMSerial(7, 8);
int Powerkey = 9;
void setup()
{
pinMode(Powerkey, OUTPUT);   // initialize the digital pin as an output.
virratPaalle();
GSMSerial.begin(19200);               // the GPRS/GSM baud rate
Serial.begin(9600);                 // the GPRS/GSM baud rate
gpsPaalle();

// AT+CGNSPWR=1
// AT+CGNSINF
// AT+CGPSSTATUS?
// AT+CGPSOUT=32
// AT+CGPSRST=0
// AT+CGNSTST=1
// AT+CCLK?
// AT+CGNSPWR=1;+CGNSCMD=0,"$PMTK314,8,8,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0*28";+CGNSTST=1;
// AT+CFUN=0 tai 1 tai 4 0=minimum functionality 1=full functionality defaul 4 = flightmode disable RF

}

void loop()
{
if(Serial.available())

GSMSerial.print((char)Serial.read());

else  if(GSMSerial.available())

Serial.print((char)GSMSerial.read());

}

void virratPaalle(void)
{
  digitalWrite(Powerkey, LOW); 
  delay(500);               // odottaa hetke
  digitalWrite(Powerkey, HIGH);
}

void gpsPaalle()
{
delay(1000);
GSMSerial.println("AT");
delay(4000);
GSMSerial.println("AT+CFUN=0");
delay(1000);
GSMSerial.println("AT+CGNSPWR=1");
}