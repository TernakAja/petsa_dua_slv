#include <Wire.h>
#include <Ticker.h>
#include "sensors.h"
#include "utils.h"
#include "state.h"

#include <ESP8266WiFi.h>
#include <ESP8266WiFiSTA.h>

// Current BPM & Temprature State
float bpmState = 0;
float tempState = 0;

void taskMasterProvider() {
  Serial.printf("Temperature = %.2f\n", tempState);
  if (bpmState) {
    Serial.printf("BPM = %.2f\n", bpmState);
  }
}

Ticker myTicker;

void loop() {
  bpmState = readHeartBeat();
  tempState = readTemperature();
}

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while (!Serial);

  initMLX90614();
  delay(1000);
  initMAX30105();
  Wire.setClock(400000);
  delay(200);

  myTicker.attach(60, taskMasterProvider);
}

void taskMasterProvider() {
  // Kirim Wifi Details Ke IoT HUB
}

void provideCurrentState() {
  // TODO :
  // Ngirim Data ke IoT HUb
}


void getDeviceDetails() {
  Serial.println("=== Device Info ===");
  Serial.print("Chip ID: ");
  Serial.println(ESP.getChipId());

  Serial.print("Flash Chip ID: ");
  Serial.println(ESP.getFlashChipId());

  Serial.print("Flash Chip Size: ");
  Serial.println(ESP.getFlashChipSize());

  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  Serial.print("SDK Version: ");
  Serial.println(ESP.getSdkVersion());

  Serial.print("Core Version: ");
  Serial.println(ESP.getCoreVersion());

  Serial.println("====================");
}



