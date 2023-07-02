#include <WiFi.h>

#define SSID "dosador_pet"
#define PASSWORD "dosadorpet"

//public functions
void initWifi(void);


/*
overview: Init wifi in mode acess point 
*/
void initWifi(){
  WiFi.softAP(SSID, PASSWORD);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("My ip addres: ");
  Serial.println(myIP);
}