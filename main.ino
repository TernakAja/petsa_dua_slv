#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "MAX30105.h"
#include "heartRate.h"

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
MAX30105 particleSensor;

long lastBeat = 0;

void initMax() {
  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("MAX30102 tidak terdeteksi. Periksa koneksi!");
    while (1);
  }

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);
}

void setup() {
  Serial.begin(115200);
  mlx.begin();
  initMax();
}

// Infrared Sensor
void readTemprature(float offset) {
  float suhuAsli = mlx.readObjectTempC();
  float suhuTerkalibrasi = suhuAsli + offset;

  Serial.print("Suhu Asli: ");
  Serial.print(suhuAsli);
  Serial.print(" °C | Suhu Terkalibrasi: ");
  Serial.print(suhuTerkalibrasi);
  Serial.println(" °C");
  delay(1000);
}

// PPG Sensor
void readSpO2(float heartRateGain, float heartRateOffset) {
  long irValue = particleSensor.getIR();

  if (irValue > 50000) {
    int beatDetected = checkForBeat(irValue);

    if (beatDetected) {
      long currentTime = millis();
      long timeDifference = currentTime - lastBeat;
      lastBeat = currentTime;

      float beatsPerMinute = 60.0 / (timeDifference / 1000.0);
      float bpmTerkalibrasi = (beatsPerMinute + heartRateOffset) * heartRateGain;

      Serial.print("BPM Asli: ");
      Serial.print(beatsPerMinute);
      Serial.print(" | BPM Terkalibrasi: ");
      Serial.println(bpmTerkalibrasi);
    }
  } else {
    Serial.println("Harap letakkan jari di sensor.");
  }
}

void loop() {
  readTemprature(1.5);
  readSpO2(1.05, -2);
}
