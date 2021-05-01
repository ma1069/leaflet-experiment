


/***************************************************************************
  This is a library for the BMP280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BMEP280 Breakout 
  ----> http://www.adafruit.com/products/2651

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required 
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#include "DHT.h"      ///

#include "SDS011.h"


#include <Wire.h>
// #include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#include <NDIR_I2C.h> ////

// #define BMP_SCK 13
// #define BMP_MISO 12
// #define BMP_MOSI 11 
// #define BMP_CS 10

#define DHTPIN 4     // what pin we're connected to   ///
#define DHTTYPE DHT11   // DHT 11                     ///

float p10, p25;
int error;
SDS011 my_sds;


DHT dht(DHTPIN, DHTTYPE);         ///

Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

NDIR_I2C mySensor(0x4D); //Adaptor's I2C address (7-bit, default: 0x4D) ////

void setup()
{
  Serial.begin(9600);
  /// Serial.println(F("BMP280 test"));
  
  if (!bmp.begin())
  {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  if (mySensor.begin())
  {
    /// Serial.println(F("Wait 10 seconds for MH-Z16 sensor initialization..."));
    delay(10000);
  }
  else
  {
    Serial.println(F("ERROR: Failed to connect to the MH-Z16 sensor."));
    while(1);
  }


  dht.begin();            ///

  my_sds.begin(2, 3); //void begin(uint8_t pin_rx, uint8_t pin_tx); 
                      //             dal punto di vista di Arduino
  
  
}

void loop()
{
/* disabilito proprio il sensore DHT che dà errore ogni tanto
 *  
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(t) || isnan(h)) 
    {
        Serial.println(F("Failed to read from DHT"));
    } 
    else 
    {
        /// Serial.print("*#U"); 
        /// Serial.print(h);
        /// Serial.print("#T1"); 
        /// Serial.print(t);
    }
    /// Serial.print("#T2");


*/
    Serial.print(bmp.readTemperature());
    
  
    /// Serial.print("#P");
    /// Serial.print(bmp.readPressure());

    /// Serial.print("#A");
    /// Serial.print(bmp.readAltitude(1013.25)); // this should be adjusted to your local forcase

    /// Serial.print("#L");
    /// Serial.print(analogRead(0));

    Serial.print("\t");

    if (mySensor.measure())
    {
      /// Serial.print("#C");
      /// Serial.print(mySensor.ppm/1000);
      Serial.print(mySensor.ppm/1000.0);    // divido per 1000 per rapportare la scala agli altri 
                                            // parametri
      /// Serial.println("#*");

      Serial.print("\t");
    }
    else
    {
      Serial.println(F("Sensor communication error."));
    }

  error = my_sds.read(&p25, &p10);
  if (!error)
  {
    /*
    Serial.println("P2.5: " + String(p25));
    Serial.println("P10:  " + String(p10));
    */ 

    Serial.print(p25);
    Serial.print("\t");
    Serial.println(p10);
  }
    delay(1000);
}
