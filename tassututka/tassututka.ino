/************* TASSUTUTKA ****************/
/**** TIETOTEKNIIKAN SOVELLUSPROJEKTI ****/
/********************************* OAMK **/
/**** ARDUINOKOODIT **********************/
/****************** PASI PUHAKKA *********/
/****************** MIKKO KUJALA *********/
/****************** NICO HERTOLIN ********/
/****************** REBECCA SOISENNIEMI***/

#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(10, 11); // GPS-moduulin RX ja TX -pinnit
String gpsData;

void setup() {
  Serial.begin(9600); // Sarjaportti tietokoneelle
  gpsSerial.begin(9600); // Sarjaportti GPS-moduulille
}

void loop() {
  gpsSerial.println("AT+CGPSPWR=1"); // Laita GPS-moduuli päälle
  delay(1000);
  gpsSerial.println("AT+CGPSSTATUS?"); // Tarkista GPS-moduulin tila
  delay(1000);
  while (gpsSerial.available() > 0) {
    gpsData = gpsSerial.readString();
    Serial.println(gpsData);
  }
  gpsSerial.println("AT+CGPSINF=0"); // Hae GPS-sijaintitiedot
  delay(1000);
  while (gpsSerial.available() > 0) {
    gpsData = gpsSerial.readString();
    Serial.println(gpsData);
  }
  delay(5000);
  