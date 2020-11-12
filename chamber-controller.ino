// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

#define RELAYPIN 7

#define TEMP_TAR 12  // Temperature wanted in chamber
#define TEMP_TOL 2   // Allowed deviation from target

#define HUMID_TAR 70  // Realtive Humidity wanted in chamber
#define HUMID_TOL 5   // Allowed deviation from target



void setup() {
  Serial.begin(9600);
  Serial.println("Setup");

  dht.begin();

  // Read relative humidity
  float h = dht.readHumidity();
  
  // Read temperature as Celsius
  float t = dht.readTemperature();

}

void loop() {
  
  // Wait between measurements, DHT11 sensor is quite slow.
  delay(2000);

  // Read relative humidity
  float h = dht.readHumidity();
  
  // Read temperature as Celsius
  float t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }



  Serial.println("Temp: " + String(t) + " | " + "Humid: " + String(h) );
}
