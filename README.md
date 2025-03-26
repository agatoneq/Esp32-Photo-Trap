
# 📷 ESP32 Photo Trap for Canon EOS 60D

This is a fully functional **motion-triggered wireless shutter system** built using **two ESP32-C3 modules**.  
It allows a Canon EOS 60D DSLR camera to take **single or burst photos** based on motion detection from PIR and Doppler sensors.

Designed for remote wildlife photography or security use cases, the system is battery-powered, weather-resistant, and coded in C++ (PlatformIO).

<p align="center">
  <img src="https://github.com/user-attachments/assets/376fe24e-c449-4daf-a206-ab25bbfc7a72" width="300px"/>
  <img src="https://github.com/user-attachments/assets/83df78c5-dd20-4b3a-bcd1-fc5300bc3fc4" width="300px"/>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/5daa53d7-69e6-4c39-990c-80103ba649e0" height="300px" />
  <img src="https://github.com/user-attachments/assets/c45acc60-9797-44b3-963a-3d7138135b36" height="300px" />
</p>

## 🧠 Features

- Motion detection via **PIR** and **Doppler radar sensors**
- Wireless communication via **WiFi UDP** between modules
- Remote triggering of the **Canon EOS 60D** camera via 2.5mm jack
- **Single shot mode** and **automatic burst mode** (3s or 10s)
- **Interrupt-based manual trigger** button
- **LED indicators** for active state, motion detection, and shot in progress
- Power-efficient design with step-down converters and sleep logic
- PCB and housing designed for ease of use in low-temperature environments

## 🔌 System Architecture

The system is built from two physical modules:
- **Main unit** (camera-side):
  - ESP32, buttons, LEDs, 2-channel relay (shutter & focus)
- **Detector unit** (sensor-side):
  - ESP32, PIR + Doppler sensors, WiFi sender
  - Powered and placed independently

## 📁 Project Structure

- `/src/main/` – Core logic for the camera-side controller
- `/src/sensor/` – Logic for motion detection and UDP data transmission
- `/schematics/` – circuit diagrams and wiring schematics for hardware setup

## 🛠️ Tools & Technologies

- PlatformIO + VS Code
- ESP32-C3 with WiFi
- PIR HC-SR501 + RCWL-0516 Doppler
- 2-channel solid-state relay (G3MB-202P)
- 9V battery + step-down converters
- C++, Arduino core

## ▶️ How to Run

1. Open the project in PlatformIO (VS Code)
2. Connect the appropriate ESP32 to your PC
3. Flash the respective firmware to:
   - `MainPart` (camera unit)
   - `MoveDetector` (sensor unit)
4. Power each unit using a 9V battery or USB
5. Connect the 2.5mm jack to your Canon EOS 60D
6. Position sensor unit and switch on both modules
7. Trigger shooting via motion or manually with buttons

## 💡 Challenges

- BLE was initially considered but replaced by WiFi due to latency
- Manual GPIO mapping and debugging for ESP32-C3
- Mechanical tuning of PIR sensitivity
- UDP-based data synchronization
- GPIO11 reconfiguration using `esptool` to gain extra I/O

## 🧑‍💻 Author

**Agata Sobczyk** – designed, implemented and debugged the system independently

## 📜 License

This is a personal hardware-software project built for individual use and experimentation.
