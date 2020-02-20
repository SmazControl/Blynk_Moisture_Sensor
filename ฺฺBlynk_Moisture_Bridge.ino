/**************************************************************
   Blynk Bridge - Communication between ESP8266
   Sketch code for the master module (module which will command others)
   www.geekstips.com
 **************************************************************/
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "------esp8266 with Sensor token -------";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourWifiSSID";
char pass[] = "YourWiFiPassword";

#define DHTPIN 2 // What digital pin we're connected to

//#define DHTTYPE DHT11   // DHT 11
  #define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
WidgetBridge bridge_valve(V1);
WidgetBridge bridge_pump(V2);

BLYNK_CONNECTED() {
  // Place the AuthToken of the second hardware here
  bridge_valve.setAuthToken("----esp8266 with Valve Relay token ------"); 
  bridge_pump.setAuthToken("----esp8266 with Pump Relay token -------"); 
}

void sendSensor(){
  // get readings from the DHT22 sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  int a = analogRead(A0);
  int m = (1024-a)*100/1024;

  Serial.print("Moisture ");
  Serial.println(m);
  
  if (isnan(h) || isnan(t)) {
    // Serial.println("Failed to read from DHT sensor!");
    // return;
    delay(10);
  }

  // Send command to the second ESP
  // WHEN Temperature IS OVER 34 C
  // in order to open the 220V Relay
  // Also update the VIRTUAL port 5 
  // on the second ESP
  if(m < 30){
     Serial.println("Pump-On");
     bridge_valve.digitalWrite(2, 1);
     bridge_valve.virtualWrite(V5, 1); 
     bridge_pump.digitalWrite(2, 1);
     bridge_pump.virtualWrite(V5, 1); 
  }else{
     Serial.println("Pump-Off");    
     bridge_valve.digitalWrite(2, 0);
     bridge_valve.virtualWrite(V5, 0); 
     bridge_pump.digitalWrite(2, 0);
     bridge_pump.virtualWrite(V5, 0); 
  }

  // Send temperature and humidity to Blynk App
  // on VIRTUAL ports 5 and 6 in order to 
  // display on Gauge Widget
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V7, m);
}

void setup(){
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  timer.setInterval(1000L, sendSensor);
}

void loop(){
  Blynk.run();
  timer.run();
}
