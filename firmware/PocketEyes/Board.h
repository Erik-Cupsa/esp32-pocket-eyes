#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Arduino_GFX_Library.h>
#include <esp_heap_caps.h>

// Waveshare ESP32-S3-Touch-AMOLED-1.75 (not 1.75C).
// Initialization/pins follow Waveshare's Apache-2.0 HelloWorld example.
// See THIRD_PARTY_NOTICES.md for the exact upstream revision and notices.
namespace board {
constexpr int kWidth = 466;
constexpr int kStripHeight = 240;
constexpr int kStripY = (466 - kStripHeight) / 2;
constexpr uint16_t kAmber = 0xFD68;
constexpr uint16_t kCyan = 0x673D;
constexpr uint8_t kBrightness = 128;

static Arduino_ESP32QSPI bus(12, 38, 4, 5, 6, 7);
// Keep the vendor's 6-column controller offset and native orientation.
static Arduino_CO5300 panel(&bus, 39, 0, 466, 466, 6, 0, 0, 0);

class Canvas : public Arduino_Canvas {
 public:
  Canvas() : Arduino_Canvas(kWidth, kStripHeight, &panel, 0, kStripY) {}
  bool begin(int32_t = GFX_NOT_DEFINED) override {
    // Explicit PSRAM allocation, leaving internal RAM for USB and QSPI DMA.
    if (!_framebuffer) _framebuffer = static_cast<uint16_t*>(heap_caps_malloc(
      kWidth * kStripHeight * sizeof(uint16_t), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    return _framebuffer != nullptr;
  }
};
static Canvas canvas;
static uint16_t foreground = kAmber;
struct Surface {
  void fillRoundRect(int x, int y, int w, int h, int r, bool on) {
    canvas.fillRoundRect(x, y, w, h, r, on ? foreground : RGB565_BLACK);
  }
};
static Surface drawing;

inline bool begin() {
  // The vendor USB display example does not reconfigure the AXP2101 rails.
  // This project keeps that setup and does not change battery charging settings.
  Wire.begin(15, 14);
  pinMode(0, INPUT_PULLUP);
  if (!psramFound()) {
    Serial.println("PSRAM missing: build with PSRAM=opi.");
    return false;
  }
  if (!canvas.begin() || !panel.begin(40000000)) {
    Serial.println("Display/buffer initialization failed.");
    return false;
  }
  panel.setBrightness(kBrightness);
  panel.fillScreen(RGB565_BLACK); // Once at startup; never clear the panel per frame.
  return true;
}
inline int width() { return kWidth; }
inline int height() { return kStripHeight; }
inline Surface& surface() { return drawing; }
inline void clear() { canvas.fillScreen(RGB565_BLACK); }
inline void present() { canvas.flush(); }
inline void toggleColor() { foreground = foreground == kAmber ? kCyan : kAmber; }

// GPIO0 is BOOT. A tap restarts eyes; a >=800 ms hold toggles stages on release.
// Sample only after boot: holding this button during reset enters the bootloader.
inline char buttonCommand(uint32_t now, bool eyesSelected) {
  static bool raw = true, stable = true, armed = false;
  static uint32_t changed = 0, pressed = 0;
  const bool next = digitalRead(0) != LOW;
  if (next != raw) { raw = next; changed = now; }
  if (uint32_t(now - changed) < 30 || raw == stable) return 0;
  stable = raw;
  if (!stable) { pressed = now; armed = true; return 0; }
  if (!armed) return 0;
  armed = false;
  return uint32_t(now - pressed) >= 800 ? (eyesSelected ? 'd' : 'e') : 'e';
}
} // namespace board
