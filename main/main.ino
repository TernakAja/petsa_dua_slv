#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <Adafruit_MLX90614.h>

// Packages
#include<Ticker.h>


// Declarations
long lastBeat = 0;
float beatsPerMinute;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
MAX30105 particleSensor;


void initMLX90614() {
  Serial.println("Initializing MLX90614......");

  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };

  Serial.print("Emissivity = "); Serial.println(mlx.readEmissivity());
  Serial.println("MLX90614 Loaded !");
}
void initMAX30105() {
  Serial.println("Initializing MAX30105...");

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
  {
    Serial.println("MAX30105 not found. Check wiring.");
    while (1);
  }

  // Stronger red LED for deeper penetration (Especially Cattle)
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x1F);
  particleSensor.setPulseAmplitudeGreen(0);
  Serial.println("MAX30105 Loaded !");
}

float readHeartBeat() {
    long irValue = particleSensor.getIR();

  // Toleransi Infrared Partikel 
  if (irValue < 20000) {
    delay(100);
    return 0;
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
      return beatsPerMinute;
    } 
  }
  return beatsPerMinute;
}

float readTemprature() {
  Wire.setClock(100000); // MLX90614 prefers 100kHz
  float temp = mlx.readObjectTempC();
  Wire.setClock(400000); // Restore 400kHz for MAX30105
  return temp;
}

Ticker myTicker;
void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial);

  initMLX90614(); 
  delay(1000);
  initMAX30105();

  Wire.setClock(400000);  // Set default I²C speed for MAX30105

  delay(200);

  myTicker.attach(1, taskMasterProvider);
}

float bpmState = 0;
float tempState = 0;
void loop() { // Main Threading
  bpmState = readHeartBeat();
  tempState = readTemprature();
}

void taskMasterProvider() {
  // Serial.println("PETSA | ========================");
  Serial.printf("Temprature = %.2f\n", tempState); 
  if(bpmState) {
    Serial.printf("BPM = %.2f\n", bpmState); 
  }
  
}





