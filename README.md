# 🌦️ Weather Monitoring System using STM32 & ESP32

## 📘 Overview

This project is a **weather monitoring system** designed to measure and report environmental data such as:

- 🌡️ Temperature  
- 💧 Humidity  
- 🌬️ Atmospheric Pressure  
- 💡 Light Intensity  

The system uses an **STM32** microcontroller as the main processor. It reads environmental data from multiple sensors, displays it on an **I2C 16x2 LCD**, and sends the data to **Adafruit IO** via an **ESP32 module** running in **AT command mode**.

---

## 🧩 Hardware Components

| Component         | Description                                 |
|-------------------|---------------------------------------------|
| STM32 MCU         | Main microcontroller (e.g., STM32F103C8T6)  |
| ESP32             | Wi-Fi module (AT Command firmware)          |
| BH1750            | Light intensity sensor (lux)                |
| BMP180            | Barometric pressure + temperature sensor    |
| DHT20             | Temperature & humidity sensor               |
| LCD 16x2 (I2C)    | Display environmental values                |
| Power Supply      | 5V or 3.3V depending on board configuration |

---

## 🔁 System Workflow

1. Initialize peripherals: sensors and LCD.
2. Read data from sensors:
   - **BH1750**: Light intensity (lux)
   - **BMP180**: Temperature and pressure (°C, hPa)
   - **DHT20**: Temperature and humidity (°C, %RH)
3. Display the data on **LCD 16x2 (via I2C)**.
4. Send AT commands from **STM32** to **ESP32** via UART to:
   - Connect to Wi-Fi
   - Publish data to **Adafruit IO** (MQTT/REST API)
5. Monitor data on the Adafruit dashboard in real-time.

---

## 🔌 Communication Interfaces

| Interface        | Devices                                    |
|------------------|--------------------------------------------|
| UART             | STM32 ↔ ESP32 (AT Commands)                |
| I2C              | STM32 ↔ BH1750, BMP180, DHT20, LCD         |

---

## 📊 Output & Visualization

- Data is **displayed locally** on the 16x2 LCD.
- Data is **published remotely** to **Adafruit IO**, viewable in a web dashboard.

---

## 🛠️ Tools Used

- **STM32CubeIDE** (for STM32 development)
- **Adafruit IO** (cloud dashboard and data visualization)
- **Logic Analyzer** (debugging the cooporative scheduler)
- **Serial Monitor** (debugging UART communication)

---
