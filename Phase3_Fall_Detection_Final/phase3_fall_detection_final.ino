/*
 * Phase 3 — Full Fall Detection System (Final)
 * Project: 24GHz mmWave Fall Detection System
 * Board:   7Semi ESP32-S3-Dev-BoardC-1-N8R8
 * Sensor:  HI-Link HLK-LD2450 24GHz mmWave Radar
 *
 * This is the final combined phase that solves three
 * problems discovered during development:
 *
 * Problem 1 — Sign-Magnitude Encoding (Phase 3)
 * LD2450 does NOT use standard two's complement.
 * Bit 15 = sign flag: 1 means positive, 0 means negative.
 * Formula: ((raw & 0x8000) ? -(raw & 0x7FFF) : raw) / 10.0
 *
 * Problem 2 — Environmental Noise (Phase 4)
 * Sensor detected walls, chairs and desk reflections.
 * Fix: resolution byte threshold — static objects return
 * low resolution values. Only process if resolution > 300.
 *
 * Problem 3 — Fall Detection Logic (Phase 5)
 * Velocity signatures observed during testing:
 * Walking:        50–120 cm/s
 * Sitting down:   150–180 cm/s
 * Simulated fall: spike above 250 cm/s
 * Threshold set at 50 cm/s for sensitivity tuning.
 *
 * RGB LED Indicators:
 * GREEN = Standby / no valid target detected
 * BLUE  = Human present, normal movement
 * RED   = Fall detected — velocity spike triggered
 *
 * Wiring:
 * LD2450 TX  → ESP32-S3 GPIO 18 (RX)
 * LD2450 RX  → ESP32-S3 GPIO 17 (TX)
 * LD2450 5V  → ESP32-S3 5V
 * LD2450 GND → ESP32-S3 GND
 *
 * Serial Plotter Output (3 lines):
 * Orange = Y_Dist  (forward distance in cm)
 * Green  = Speed   (velocity in cm/s)
 * Yellow = Threshold (fall detection limit)
 */

#include <Adafruit_NeoPixel.h>

// Hardware Pins
#define RGB_PIN  48   // Built-in RGB on this 7Semi board (NOT GPIO 38)
#define RXD1     18   // Connected to Sensor TX
#define TXD1     17   // Connected to Sensor RX

// Detection Thresholds — tune these for your environment
#define NUM_PIXELS     1
#define FALL_THRESHOLD 50    // cm/s — above this = fall detected
#define PRESENCE_RES   300   // below this = static object, ignore

Adafruit_NeoPixel pixels(NUM_PIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);                        // PC Serial Plotter
  Serial1.begin(256000, SERIAL_8N1, RXD1, TXD1); // LD2450 UART

  pixels.begin();
  pixels.setBrightness(40);
  pixels.show(); // Start OFF

  // Initial Serial Plotter baseline
  Serial.println("Y_Dist:0,Speed:0,Threshold:50");
}

void loop() {
  // Each LD2450 frame is exactly 30 bytes
  if (Serial1.available() >= 30) {
    uint8_t buffer[30];
    Serial1.readBytes(buffer, 30);

    // Validate frame header: AA FF 03 00
    if (buffer[0] == 0xAA && buffer[1] == 0xFF && buffer[2] == 0x03) {

      // Extract Target 1 raw bytes (little-endian: low byte first)
      int16_t xRaw   = (buffer[5]  << 8) | buffer[4];
      int16_t yRaw   = (buffer[7]  << 8) | buffer[6];
      int16_t sRaw   = (buffer[9]  << 8) | buffer[8];
      int16_t resRaw = (buffer[11] << 8) | buffer[10];

      // SIGN-MAGNITUDE DECODING — NOT standard two's complement
      // Bit 15 = sign flag: 1 = positive, 0 = negative
      float xCm      = ((xRaw & 0x8000) ? -(xRaw & 0x7FFF) : xRaw) / 10.0;
      float yCm      = ((yRaw & 0x8000) ? -(yRaw & 0x7FFF) : yRaw) / 10.0;
      float speedCmS = ((sRaw & 0x8000) ? -(sRaw & 0x7FFF) : sRaw) / 10.0;
      int   resolution = resRaw;

      // NOISE FILTER — skip static objects like walls and furniture
      if (resolution > PRESENCE_RES) {

        // Serial Plotter output — 3 named lines
        Serial.print("Y_Dist:");    Serial.print(abs(yCm));
        Serial.print(",Speed:");    Serial.print(abs(speedCmS));
        Serial.println(",Threshold:50");

        // FALL DETECTION — velocity spike check
        if (abs(speedCmS) > FALL_THRESHOLD) {
          // RED = Fall detected
          pixels.setPixelColor(0, pixels.Color(255, 0, 0));
        } else {
          // BLUE = Human present, moving normally
          pixels.setPixelColor(0, pixels.Color(0, 0, 255));
        }

      } else {
        // GREEN = Standby, no valid human target
        pixels.setPixelColor(0, pixels.Color(0, 255, 0));
        Serial.println("Y_Dist:0,Speed:0,Threshold:50");
      }

      pixels.show();
    }
  }
}
