# Project-03
# Dual-Node I2C Distributed SCADA Mesh Architecture

## 📌 Project Overview
This project implements a multi-node Industrial Edge (IoT) system utilizing two Arduino Uno controllers communicating via the Inter-Integrated Circuit (I2C) protocol. It simulates a mission-critical SCADA (Supervisory Control and Data Acquisition) environment where environmental monitoring is hardware-isolated from mechanical actuation to prevent single points of failure.

## ⚙️ System Architecture
The architecture operates on a strict Master-Slave topology:
* **Node A (Edge Sensor Node / I2C Slave - Address 8):** Dedicated to environmental telemetry acquisition. It reads analog data from Gas, Temperature, and Light sensors, packs it into a serialized C++ struct, and transmits it over the I2C bus.
* **Node B (Actuator & Gateway Node / I2C Master):** The central controller. It actively polls Node A, processes the telemetry, handles the local LCD UI, outputs JSON-formatted serial data, and triggers hardware failsafes (RGB alarms, piezo buzzer, and a transistor-driven exhaust motor) when hazard thresholds are breached.

## 🛠️ Hardware Components
* 2x Arduino Uno R3
* 1x Gas Sensor (MQ-2 equivalent)
* 1x Temperature Sensor (TMP36)
* 1x Photoresistor (LDR)
* 1x 16x2 I2C LCD Display
* 1x Common Cathode RGB LED
* 1x Piezo Buzzer
* 1x DC Motor (Ventilation simulation)
* 1x NPN Transistor (2N2222 - for motor overcurrent protection)
* Assorted Resistors (220Ω for LEDs, 1kΩ for Transistor base) & Breadboards

## 🚀 Hazard Mitigation Logic
The system monitors volatile gas concentrations with a hardcoded safety threshold of **400.0 PPM**.
* **Normal State (<= 400 PPM):** Green LED active, system idle, displaying real-time telemetry.
* **Emergency State (> 400 PPM):** Red LED active, 1000Hz Piezo alarm triggered, automated exhaust fan (DC Motor) engaged via NPN transistor. 

## 📂 Repository Structure
* `Node_A_Slave.ino`: C++ source code for the data acquisition node.
* `Node_B_Master.ino`: C++ source code for the actuator and serial gateway node.
* `SCADA_Architecture_Report.pdf`: Full project documentation and schematics.

## 💻 Edge-to-Cloud Telemetry (JSON)
Node B acts as an edge gateway, outputting structured JSON strings via the Serial interface for simulated cloud ingestion:
```json
{"gasPPM": 420.00, "tempC": 25.50, "lux": 180.00}
```

## 🔌 How to Run (Tinkercad / Real Hardware)
1. Flash `Node_A_Slave.ino` to the first Arduino.
2. Flash `Node_B_Master.ino` to the second Arduino.
3. Connect the I2C bus: Node A (A4, A5) to Node B (A4, A5) and establish a common Ground.
4. Open the Serial Monitor (9600 baud) on Node B to view the JSON telemetry stream.