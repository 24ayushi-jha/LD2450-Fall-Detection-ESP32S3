/*
 * Phase 2 — UART Handshake / Raw Hex Passthrough
 * Project: 24GHz mmWave Fall Detection System
 * Board:   7Semi ESP32-S3-Dev-BoardC-1-N8R8
 * Sensor:  HI-Link HLK-LD2450 24GHz mmWave Radar
 *
 * Goal:
 * Establish UART communication with LD2450 sensor.
 * Confirm the sensor is alive and transmitting data
 * by printing raw hex bytes to Serial Monitor.
 *
 * Wiring:
 * LD2450 TX  →  ESP32-S3 GPIO 18 (RX)
 * LD2450 RX  →  ESP32-S3 GPIO 17 (TX)
 * LD2450 5V  →  ESP32-S3 5V
 * LD2450 GND →  ESP32-S3 GND
 *
 * Expected Serial Monitor Output:
 * AA FF 03 00 00 00 00 00 00 00 00 00 00 00 00 00
 * 00 00 00 00 00 00 00 00 00 00 00 00 55 CC
 *
 * How to read this output:
 * AA FF 03 00  =  Frame header — always fixed, confirms sync
 * 00 00 ... 00 =  All zeros = NO targets in range
 *                 This is NOT an error — room is simply empty
 * 55 CC        =  Frame footer — end of 30-byte frame
 *
 * Key Discovery:
 * The LD2450 runs at 256,000 baud — much higher than typical
 * sensors (9600 or 115200). Hardware UART (Serial1) on
 * ESP32-S3 handles this reliably without dropping bytes.
 *
 * Key Learning:
 * Seeing AA FF 03 00 repeating = UART handshake successful.
 * All zeros in data section = sensor working, room is empty.
 * This was the first proof the sensor was alive and talking.
 */

void setup() {
  Serial.begin(115200);                          // PC monitor
  Serial1.begin(256000, SERIAL_8N1, 18, 17);    // LD2450 UART

  Serial.println("=========================================");
  Serial.println("  Phase 2 — UART Handshake");
  Serial.println("  LD2450 TX → GPIO 18 | RX → GPIO 17");
  Serial.println("  Baud Rate: 256,000");
  Serial.println("  Waiting for sensor frames...");
  Serial.println("=========================================");
}

void loop() {
  if (Serial1.available()) {
    byte b = Serial1.read();

    // Print each byte in HEX with leading zero for alignment
    if (b < 0x10) Serial.print("0");
    Serial.print(b, HEX);
    Serial.print(" ");

    // New line after every complete 30-byte frame
    // Frame always ends with 0x55 0xCC per datasheet
    if (b == 0xCC) {
      Serial.println();
    }
  }
}
