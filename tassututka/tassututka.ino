/***************************************************************************************/
/**** TASSUTUTKA ***********************************************************************/
/**** TIETOTEKNIIKAN SOVELLUSPROJEKTI, OULUN AMMATTIKORKEAKOULU, TVT23KMO **************/
/**** TEKIJÄT : MIKKO KUJALA / PASI PUHAKKA / REBECCA SOISENNIEMI / NICO HERTOLIN ******/
/***************************************************************************************/

#include <SoftwareSerial.h>
#include "tassudata.h"
#include "settings.h"

SoftwareSerial GSMSerial(7, 8);
int Powerkey = 9;

void setup()
{
  pinMode(Powerkey, OUTPUT);    // initialize the digital pin as an output.
  powerOnSIM808 ();
  GSMSerial.begin(19200);       // the GPRS/GSM baud rate
  Serial.begin(9600);           // the GPRS/GSM baud rate
  enableGPS();
  enableGPRS();


              /* 
              SIM900.begin(19200);  //SIM900 serial communication is starting whit 19200 of baudrate speed
              powerOnSIM808 
              AT+CGNSINF
              AT+CGPSSTATUS?
              AT+CGPSOUT=32
              AT+CGPSRST=0
              AT+CGNSTST=1
              AT+CCLK?
              AT+CGNSPWR=1;+CGNSCMD=0,"$PMTK314,8,8,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0*28";+CGNSTST=1;
              AT+CFUN=0 tai 1 tai 4 0=minimum functionality 1=full functionality defaul 4 = flightmode disable RF

              void sendSMS() {

              // AT command to set SIM900 to SMS mode
              SIM900.print("AT+CMGF=1\r"); 
              delay(100);

              // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
              // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
              SIM900.println("AT+CMGS=\"+358443286650\""); 
              delay(100);

              // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
              SIM900.println("Tutka havaitsi kohteen."); 
              delay(100);

              // End AT command with a ^Z, ASCII code 26
              SIM900.println((char)26); 
              delay(100);
              SIM900.println();
              // Give module time to send SMS
              delay(5000); 
              }
              */
}

void loop()
{
  if(Serial.available())
  GSMSerial.print((char)Serial.read());
  else if(GSMSerial.available())
  Serial.print((char)GSMSerial.read());
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
  GSMSerial.println("AT");
  delay(4000);
  GSMSerial.println("AT+CFUN=0");
  delay(1000);
  GSMSerial.println("AT+CGNSPWR=1");
}

void enableGPRS()
{
  

}