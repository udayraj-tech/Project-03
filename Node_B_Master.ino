#include <Wire.h>
#include <Adafruit_LiquidCrystal.h> // Tinkercad I2C LCD library

Adafruit_LiquidCrystal lcd(0); // Address 0 for MCP23008 module

// Hardware Mapping for Hazard System (RGB LED, Buzzer, Motor)
#define RED_PIN 11
#define BLUE_PIN 10
#define GREEN_PIN 6
#define BUZZER_PIN 12
#define MOTOR_PIN 9 // Exhaust fan/motor (optional backup)

// Yeh struct Node A ke struct se EXACTLY match hona chahiye
struct SensorData {
  float gasPPM;
  float tempC;
  float lux;
} payload;

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Master ko address ki zaroorat nahi hoti
  
  // Safety Components Initialize karo
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  
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
      // === DANGER STATE (Threshold Breached) ===
      lcd.setCursor(0, 0);
      lcd.print("GAS WARNING!");
      lcd.setCursor(0, 1);
      lcd.print("PPM: ");
      lcd.print(payload.gasPPM);
      
      // Trigger Visual and Audio Alarms
      analogWrite(RED_PIN, 255); // Red ON
      analogWrite(BLUE_PIN, 0);  // Blue OFF
      analogWrite(GREEN_PIN, 0); // Green OFF
      tone(BUZZER_PIN, 1000);    // Buzzer ON (1000Hz frequency)
      digitalWrite(MOTOR_PIN, HIGH); // Ventilation ON
      
    } else {
      // === NORMAL STATE (Environment Safe) ===
      lcd.setCursor(0, 0); 
      lcd.print("Gas: ");
      lcd.print(payload.gasPPM);
      lcd.setCursor(0, 1); 
      lcd.print("Temp: ");
      lcd.print(payload.tempC);
      lcd.print(" C");
      
      // Stop all alarms, switch to Green Light
      analogWrite(RED_PIN, 0);   // Red OFF
      analogWrite(BLUE_PIN, 0);  // Blue OFF
      analogWrite(GREEN_PIN, 255); // Green ON
      noTone(BUZZER_PIN);        // Buzzer OFF
      digitalWrite(MOTOR_PIN, LOW);  // Ventilation OFF
    }
  }
  else {
    Serial.println("Error: Incomplete I2C payload received or Node A offline.");
  }

  delay(1000); // 1 second ka polling interval
}