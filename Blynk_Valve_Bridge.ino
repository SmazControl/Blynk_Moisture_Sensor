/**************************************************************
   Blynk Bridge - Communication between ESP8266
   Sketch code for the receiver module (module which is controlled)
   www.geekstips.com
 **************************************************************/

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxx";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "XXXXXXXX";
char pass[] = "********";

// This code will update the virtual port 5
BLYNK_WRITE(V5) {
  int pinData = param.asInt();
  digitalWrite(D1,pinData);   // Relay
}

void setup(){
  Serial.begin(115200);
  pinMode(D1,OUTPUT);
  Blynk.begin(auth, ssid, pass);
}

void loop(){
  Blynk.run();
}
