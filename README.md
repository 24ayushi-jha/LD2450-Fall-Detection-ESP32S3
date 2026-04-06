# 24GHz mmWave Fall Detection System
**HLK-LD2450 Radar + 7Semi ESP32-S3-Dev-BoardC-1-N8R8**

A privacy-preserving fall detection system using 24GHz 
FMCW millimeter wave radar. Tracks real-time X/Y coordinates 
and triggers fall alarm on velocity spike above 250 cm/s.

No camera. No PIR. Zero visual data captured.

---

## Project Phases

| Phase | Folder | What it does |
|---|---|---|
| 0 | Phase0_Board_Validation | RGB LED test — confirms board + IDE working |
| 1 | Phase1_UART_Raw_Hex | Raw hex — confirms sensor is alive |
| 2 | Phase2_Coordinate_Parser | Decodes X/Y/Speed values |
| 3 | Phase3_Fall_Detection | Noise filter + fall alarm |

---

## Hardware

| Component | Model |
|---|---|
| Microcontroller | 7Semi ESP32-S3-Dev-BoardC-1-N8R8 |
| Radar Sensor | HI-Link HLK-LD2450 24GHz mmWave |

---

## Wiring

| LD2450 Pin | ESP32-S3 Pin |
|---|---|
| 5V | 5V |
| GND | GND |
| TX | GPIO 18 |
| RX | GPIO 17 |

---

## Datasheet
- [HLK-LD2450 Instruction Manual](https://www.tinytronics.nl/product_files/006000_HLK-LD2450-Instruction-Manual.pdf)
- [Serial Protocol v1.03](https://make.net.za/wp-content/datasheets/HLK%20LD2450%20Serial%20Communication%20Protocol%20v1.03.pdf)

---

## Author
Ayushi Jha
[LinkedIn](https://linkedin.com/in/your-linkedin-here)
