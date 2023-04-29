#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ArduinoJson.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT11     // DHT 11 sensor

const int PIN_SENSOR = A0; // Pin pre pripojenie snímača MH 3326
float distance = 0.0; // Premenná pre uchovanie vzdialenosti

DHT_Unified dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); // Inicializácia sériovej komunikácie
  dht.begin();
}

void loop() {
  
  // Čítanie analógového signálu z pinu snímača
  int sensorValue = analogRead(PIN_SENSOR);
  
  // Výpočet vzdialenosti na základe času potrebného na odrážanie laserového impulzu
  float time = sensorValue / 1000000.0; // Prevod na sekundy
  distance = time * 17150.0; // Vzorec pre vzdialenosť
  
  // Vytvorenie JSON objektu
  StaticJsonDocument<100> jsonDoc;
  jsonDoc["distance"] = distance;

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    jsonDoc["temperature"] = "Error";
  }
  else {
    jsonDoc["temperature"] = event.temperature;
  }

  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    jsonDoc["humidity"] = "Error";
  }
  else {
    jsonDoc["humidity"] = event.relative_humidity;
  }

  // Konvertovanie JSON objektu na reťazec
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  // Výpis meraných hodnôt do sériovej konzoly v JSON formáte
  Serial.println(jsonString);

  delay(1000); // Čakanie pred ďalším čítaním snímača
}