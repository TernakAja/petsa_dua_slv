#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#define BEAT_THRESHOLD  4000   // tune between 3000–6000 for your clip test

MAX30105 particleSensor;

long lastBeat = 0;
float beatsPerMinute;

void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing...");

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
  {
    Serial.println("MAX30105 not found. Check wiring.");
    while (1);
  }

  // Stronger red LED for deeper penetration (Especially Cattle)
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x1F);
  particleSensor.setPulseAmplitudeGreen(0);
}

void loop()
{
  long irValue = particleSensor.getIR();

  // Toleransi Infrared Partikel 
  if (irValue < 20000) {
    Serial.println("No ear contact");
    delay(100);
    return;
  }

  // optional high‑pass to remove baseline drift
  static float prevIR = 0;
  float filteredIR = irValue - 0.99 * prevIR;
  prevIR = irValue;

  if (checkForBeat(filteredIR))
  {
    // compute instantaneous BPM
    unsigned long now = millis();
    unsigned long delta = now - lastBeat;
    lastBeat = now;

    beatsPerMinute = 60.0 / (delta / 1000.0);

    // only valid bovine HR
    if (beatsPerMinute > 30 && beatsPerMinute < 100)
    {
      Serial.print("Live BPM = ");
      Serial.println(beatsPerMinute, 1);
    }
  }
  delay(20);
}
