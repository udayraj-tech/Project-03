#include <Wire.h>
#include <Adafruit_LiquidCrystal.h> // Tinkercad I2C LCD library

Adafruit_LiquidCrystal lcd(0); // Address 0 for MCP23008 module

// Yeh struct Node A ke struct se EXACTLY match hona chahiye
struct SensorData {
  float gasPPM;
  float tempC;
  float lux;
} payload;

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Master ko address ki zaroorat nahi hoti
  
  // LCD Initialization
  lcd.begin(16, 2);
  lcd.print("System Ready");
  delay(1000);
  
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
    
    // LCD Display Logic
    lcd.clear(); // Har nayi reading se pehle purana text clean karna zaroori hai
    
    if (payload.gasPPM > 400.0) { 
      // DANGER STATE
      lcd.setCursor(0, 0); // Pehli line
      lcd.print("GAS WARNING!");
      lcd.setCursor(0, 1); // Dusri line
      lcd.print("PPM: ");
      lcd.print(payload.gasPPM);
    } else {
      // NORMAL STATE
      lcd.setCursor(0, 0); // Pehli line
      lcd.print("Gas: ");
      lcd.print(payload.gasPPM);
      lcd.setCursor(0, 1); // Dusri line
      lcd.print("Temp: ");
      lcd.print(payload.tempC);
      lcd.print(" C");
    }
  }
  else {
    Serial.println("Error: Incomplete I2C payload received or Node A offline.");
  }

  delay(1000); // 1 second ka polling interval
}