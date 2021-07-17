#include "wifiWrapper.h"
#include <ArduinoJson.h>
#include <Wire.h>
#include <NDIR_I2C.h>


unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 5000L; // delay between updates, in milliseconds

char ssid[] = "FABLAB";
char password[] = "FABLAB01";
char server[] = "progamb.accademialiberaivrea.eu";
char type[] = "POST";
char endpoint[] = "/v0/data/device/AM001";
char payload[] = "values=PM10:11.1|PM25:22.2";
unsigned long value = 0;

String result = "";
StaticJsonDocument<200> doc; //https://arduinojson.org/v6/assistant
NDIR_I2C mySensor(0x4D); //Adaptor's I2C address (7-bit, default: 0x4D) ////

void setup()
{
  wifi.connectESP(ssid, password);
  
  /*if (mySensor.begin())
  {
    Serial.println(F("Wait 10 seconds for MH-Z16 sensor initialization..."));
    delay(10000);
  }
  else
  {
    Serial.println(F("ERROR: Failed to connect to the MH-Z16 sensor."));
    while(1);
  }*/

  
  
}

void loop()
{
  result = wifi.checkForIncomingMessage();
  /*if(result != "") {
    result = result.substring(result.indexOf('{'));
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, result);
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.f_str());
    } else {
      Serial.println();
      String risultato = doc["message"];
      Serial.println(risultato);
    }
  }*/

  if (millis() - lastConnectionTime > postingInterval) {
    
    /*if (mySensor.measure()) {
        Serial.print("CO2 Concentration is ");
        Serial.print(mySensor.ppm);
        value = mySensor.ppm;
        Serial.println("ppm");
    } else {
        Serial.println("Sensor communication error.");
    }*/
    //Serial.println(value);
    //wifi.httpPING(server);
    //wifi.apiCallHTTPS(server, type, endpoint, 443);
    //wifi.apiCallHTTPS("progamb.accademialiberaivrea.eu", "GET", "/v0/data/4", 443);
    //wifi.httpPUT(server, value);
    wifi.apiCallHTTPSpost(payload, server, type, endpoint, 443);
    lastConnectionTime = millis();
  }
}
