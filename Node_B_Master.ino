#include <Wire.h>

// Yeh struct Node A ke struct se EXACTLY match hona chahiye
struct SensorData {
  float gasPPM;
  float tempC;
  float lux;
} payload;

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Master ko address ki zaroorat nahi hoti
  Serial.println("Node B Master Initialized. Requesting I2C data...");
}

void loop() {
  // Slave (address 8) se exact struct size ka data mango
  Wire.requestFrom(8, sizeof(payload));

  // Safety Check: Verify karo ki poora data packet receive hua hai ya nahi
  if (Wire.available() == sizeof(payload)) {
    Wire.readBytes((byte*)&payload, sizeof(payload));

    // Serial Monitor par test print
    Serial.print("RECEIVED -> Temp: "); Serial.print(payload.tempC);
    Serial.print(" C | Gas: "); Serial.print(payload.gasPPM);
    Serial.print(" | Lux: "); Serial.println(payload.lux);
    
    // YAHAN TUMHARA WEEK 3 KA LCD LOGIC AAYEGA
    // if (payload.gasPPM > threshold) { ... }
    
  }
  else {
    Serial.println("Error: Incomplete I2C payload received or Node A offline.");
  }

  delay(1000); // 1 second ka polling interval
}
