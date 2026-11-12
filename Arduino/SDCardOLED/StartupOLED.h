#ifndef STARTUP_OLED_H
#define STARTUP_OLED_H

#include <Wire.h>
#include <avr/pgmspace.h>

// SSD1306 128x64, I2C 0x3C, rotated 180 degrees like OLED.ino.
// No framebuffer, timer, or reset GPIO (pin 10 belongs to the SD).
const byte STATUS_OLED_ADDRESS = 0x3C;
static bool statusOLEDPresent = false;

static void statusOLEDCommand(byte command) {
  Wire.beginTransmission(STATUS_OLED_ADDRESS);
  Wire.write(byte(0x00));
  Wire.write(command);
  Wire.endTransmission();
}

static void statusOLEDPosition(byte page) {
  statusOLEDCommand(0xB0 | page);
  statusOLEDCommand(0x00);
  statusOLEDCommand(0x10);
}

static void startupOLEDInit() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);
#endif
  Wire.beginTransmission(STATUS_OLED_ADDRESS);
  statusOLEDPresent = (Wire.endTransmission() == 0);
  if (!statusOLEDPresent) return;
  static const byte commands[] PROGMEM = {
    0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40,
    0x8D, 0x14, 0x20, 0x02, 0xA0, 0xC0, 0xDA, 0x12,
    0x81, 0x7F, 0xD9, 0xF1, 0xDB, 0x40, 0xA4, 0xA6, 0x2E
  };
  for (byte i = 0; i < sizeof(commands); ++i)
    statusOLEDCommand(pgm_read_byte(commands + i));
  for (byte page = 0; page < 8; ++page) {
    statusOLEDPosition(page);
    for (byte block = 0; block < 8; ++block) {
      Wire.beginTransmission(STATUS_OLED_ADDRESS);
      Wire.write(byte(0x40));
      for (byte col = 0; col < 16; ++col) Wire.write(byte(0));
      Wire.endTransmission();
    }
  }
  statusOLEDCommand(0xAF);
}

// Only the glyphs used by the two messages and File_M001.txt/File_S001.txt.
static const char statusOLEDCharacters[] PROGMEM = " SDok!failedFM_.txt0123456789";
static const byte statusOLEDFont[][5] PROGMEM = {
  {0,0,0,0,0}, {0x46,0x49,0x49,0x49,0x31},
  {0x7F,0x41,0x41,0x22,0x1C}, {0x38,0x44,0x44,0x44,0x38},
  {0x7F,0x10,0x28,0x44,0}, {0,0,0x5F,0,0},
  {0x08,0x7E,0x09,0x01,0x02}, {0x20,0x54,0x54,0x54,0x78},
  {0,0x44,0x7D,0x40,0}, {0,0x41,0x7F,0x40,0},
  {0x38,0x54,0x54,0x54,0x18}, {0x38,0x44,0x44,0x48,0x7F},
  {0x7F,0x09,0x09,0x09,0x01}, {0x7F,0x02,0x0C,0x02,0x7F},
  {0x40,0x40,0x40,0x40,0x40}, {0,0x60,0x60,0,0},
  {0x04,0x3F,0x44,0x40,0x20}, {0x44,0x28,0x10,0x28,0x44},
  {0x04,0x3F,0x44,0x40,0x20},
  {0x3E,0x51,0x49,0x45,0x3E}, {0,0x42,0x7F,0x40,0},
  {0x42,0x61,0x51,0x49,0x46}, {0x21,0x41,0x45,0x4B,0x31},
  {0x18,0x14,0x12,0x7F,0x10}, {0x27,0x45,0x45,0x45,0x39},
  {0x3C,0x4A,0x49,0x49,0x30}, {0x01,0x71,0x09,0x05,0x03},
  {0x36,0x49,0x49,0x49,0x36}, {0x06,0x49,0x49,0x29,0x1E}
};

static void statusOLEDText(byte page, const char *text) {
  statusOLEDPosition(page);
  for (byte pos = 0; text[pos] && pos < 21; ++pos) {
    byte glyph = 0;
    for (byte i = 0; i < sizeof(statusOLEDCharacters) - 1; ++i) {
      if (text[pos] == char(pgm_read_byte(statusOLEDCharacters + i))) {
        glyph = i;
        break;
      }
    }
    Wire.beginTransmission(STATUS_OLED_ADDRESS);
    Wire.write(byte(0x40));
    for (byte col = 0; col < 5; ++col)
      Wire.write(pgm_read_byte(&statusOLEDFont[glyph][col]));
    Wire.write(byte(0));
    Wire.endTransmission();
  }
}

static void startupOLEDStatus(bool ok, const char *filename) {
  if (!statusOLEDPresent) return;
  statusOLEDText(0, ok ? "SD ok!" : "SD failed");
  if (ok) statusOLEDText(1, filename);
}
#endif
