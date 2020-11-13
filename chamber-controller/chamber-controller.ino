// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

#define COOLINGPIN 7 // PIN for input of relay controlling cooling. Relay configured as normally closed.

#define TEMP_TAR 13  // Temperature wanted in chamber
#define TEMP_TOL 1   // Allowed deviation from target

#define HUMID_TAR 70  // Realtive Humidity wanted in chamber
#define HUMID_TOL 5   // Allowed deviation from target

boolean cooling;   // Tracker for cooler on/off
float h_curr;
float t_curr;
float t_diff;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  
  pinMode(DHTPIN, INPUT);
  pinMode(COOLINGPIN, OUTPUT);

  dht.begin();
}

void loop() {
  Serial.println();
  // Wait between measurements, DHT11 sensor is quite slow.
  delay(2000);

  // Read relative humidity
  h_curr = dht.readHumidity();
  
  // Read temperature as Celsius
  t_curr = dht.readTemperature() - 2.5; // DHT11 sensor reporting approx. 2 - 2.5 degrees to high in my operating range of 7-16 degs
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h_curr) || isnan(t_curr)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    //return;
  }

  t_diff = t_curr - TEMP_TAR;
  
  Serial.println("Temp: " + String(t_curr) + " | " + "Humid: " + String(h_curr) );
  Serial.println("tempdiff: " + String(t_diff));
  Serial.println(cooling, DEC);

  if(t_diff > TEMP_TOL && cooling == false){ // Chamber temp over desired temp, turn on cooling
    Serial.println("Turning on cooling");
    coolingOn();
  }

  if(t_diff < -TEMP_TOL && cooling == true){ // Chamber temp under desired temp, turn off cooling.
    Serial.println("Turning off cooling");
    coolingOff();
  }
}

int coolingOn(){
  digitalWrite(COOLINGPIN, LOW); // Relay configured as NC, closed circuit when LOW.
  cooling = true;
  return 0;
}

int coolingOff(){
  digitalWrite(COOLINGPIN, HIGH); // Relay configured as NC, open crcuit when HIGH.
  cooling = false;
  return 0;
}
