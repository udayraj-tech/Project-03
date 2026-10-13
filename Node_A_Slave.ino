#include <Wire.h>

// Exact hardware mapping
#define TEMP_PIN A0
#define GAS_PIN A1
#define LDR_PIN A2

// Optimized struct with only required fields
struct SensorData {
  float gasPPM;
  float tempC;
  float lux;
} payload;

void setup() {
  Serial.begin(9600);
  Wire.begin(8); // Node A configured as Slave with address 8
  Wire.onRequest(requestEvent); // Interrupt for Master's request
  
  Serial.println("Node A Slave Initialized. Acquiring real-time data...");
}

void loop() {
  // 1. Raw ADC Data Acquisition (0 - 1023)
  float rawGas = analogRead(GAS_PIN);
  float rawLux = analogRead(LDR_PIN);
  int rawTemp = analogRead(TEMP_PIN);

  // 2. Temperature Conversion for TMP36
  float voltage = rawTemp * (5.0 / 1023.0);
  float tempCelsius = (voltage - 0.5) * 100.0;

  // 3. Update I2C Payload
  payload.gasPPM = rawGas;
  payload.lux = rawLux;
  payload.tempC = tempCelsius;

  // 4. Local Debugging Output
  Serial.print("Temp (C): "); Serial.print(payload.tempC);
  Serial.print(" | Raw Gas: "); Serial.print(payload.gasPPM);
  Serial.print(" | Raw Lux: "); Serial.println(payload.lux);

  delay(500); // Short delay for loop stability
}

// Function that runs whenever Node B (Master) requests data
void requestEvent() {
  Wire.write((byte*)&payload, sizeof(payload));
}