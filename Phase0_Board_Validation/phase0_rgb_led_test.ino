/*
 * Phase 0 — ESP32-S3 Board Validation
 * Project: 24GHz mmWave Fall Detection System
 * Board:   7Semi ESP32-S3-Dev-BoardC-1-N8R8
 *
 * Goal: Confirm the development environment is correctly
 * communicating with the ESP32-S3 silicon before touching
 * any radar hardware.
 *
 * Key Discovery:
 * Standard ESP32-S3 documentation lists the onboard RGB LED
 * at GPIO 38. This specific 7Semi board routes it to GPIO 48.
 * Always validate your board's actual pin mapping.
 *
 * Library required:
 * Adafruit NeoPixel — install via Arduino IDE Library Manager
 * Sketch → Include Library → Manage Libraries → "Adafruit NeoPixel"
 */

#include <Adafruit_NeoPixel.h>

#define LED_PIN   48   // NOTE: GPIO 48 on this board, NOT 38
#define NUM_LEDS   1

Adafruit_NeoPixel pixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setColor(uint8_t r, uint8_t g, uint8_t b, const char* name) {
  Serial.print("Setting Color: ");
  Serial.print(name);
  Serial.print(" -> R:"); Serial.print(r);
  Serial.print(" G:");    Serial.print(g);
  Serial.print(" B:");    Serial.println(b);

  pixel.setPixelColor(0, pixel.Color(r, g, b));
  pixel.show();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("===== ESP32-S3 Board Validation =====");
  Serial.print("RGB LED GPIO: ");
  Serial.println(LED_PIN);

  pixel.begin();
  pixel.setBrightness(50);

  Serial.println("NeoPixel initialized successfully.");
  Serial.println("Watch for RED → GREEN → BLUE → WHITE → OFF");
}

void loop() {
  setColor(255, 0,   0,   "RED");   delay(1000);
  setColor(0,   255, 0,   "GREEN"); delay(1000);
  setColor(0,   0,   255, "BLUE");  delay(1000);
  setColor(255, 255, 255, "WHITE"); delay(1000);
  setColor(0,   0,   0,   "OFF");   delay(1000);
}
