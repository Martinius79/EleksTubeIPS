#ifndef TFTS_H
#define TFTS_H

// Call up the LittleFS FLASH filing system this is part of the ESP Core
#define FS_NO_GLOBALS
#include <FS.h>
#include <TFT_eSPI.h>
#include "ChipSelect.h"

class TFTs : public TFT_eSPI {
public:
  TFTs() : TFT_eSPI(), chip_select(), enabled(false)
    { for (uint8_t digit=0; digit < NUM_DIGITS; digit++) digits[digit] = 0; }

  // no == Do not send to TFT. yes == Send to TFT if changed. force == Send to TFT.
  enum show_t { no, yes, force };
  // A digit of 0xFF means blank the screen.
  const static uint8_t blanked = 255;
  const static uint8_t invalid = 254;

  uint8_t dimming = 255; // amount of dimming graphics
  
  void claim();
  void release();

  void begin(fs::FS& fs, const char *imageRoot);
  void setClockFace(uint8_t index);
  void clear();
  void setDigit(uint8_t digit, uint8_t value, show_t show=yes);
  uint8_t getDigit(uint8_t digit)                 { return digits[digit]; }

  void invalidateAllDigits() { for (uint8_t digit=0; digit < NUM_DIGITS; digit++) digits[digit] = invalid; }

  void showAllDigits()               { for (uint8_t digit=0; digit < NUM_DIGITS; digit++) showDigit(digit); }
  void showDigit(uint8_t digit);

  // Controls the power to all displays
  void enableAllDisplays()           { digitalWrite(TFT_ENABLE_PIN, HIGH); enabled = true; }
  void disableAllDisplays()          { digitalWrite(TFT_ENABLE_PIN, LOW); enabled = false; }
  void toggleAllDisplays()           { if (enabled) disableAllDisplays(); else enableAllDisplays(); }
  bool isEnabled()                   { return enabled; }

  // Making chip_select public so we don't have to proxy all methods, and the caller can just use it directly.
  ChipSelect chip_select;

  void LoadNextImage();
  void InvalidateImageInBuffer(); // force reload from Flash with new dimming settings

  size_t printlnon(uint8_t display, const char[]);
  size_t printlnall(const char[]);
  size_t printlnall(const String&);
  void setStatus(const char*);
  void setStatus(const String& s) { setStatus(s.c_str()); }
  void checkStatus();

  void drawMeter(int val, bool first, const char *legend);

private:
  static SemaphoreHandle_t tftMutex;

  TFT_eSprite& getStatusSprite();
  void drawStatus();

  unsigned long statusTime = 0;
  bool statusSet = false;

  uint16_t defaultTextColor = TFT_WHITE;
  uint16_t defaultTextBackground = TFT_BLACK;
  uint8_t current_graphic = 1;
  uint8_t digits[NUM_DIGITS];
  bool enabled;
  fs::FS* fs;
  const char *imageRoot;

  bool LoadImageIntoBuffer(uint8_t file_index);
  void DrawImage(uint8_t file_index);
  uint16_t read16(fs::File &f);
  uint32_t read32(fs::File &f);

  static uint16_t output_buffer[TFT_HEIGHT][TFT_WIDTH];
  uint8_t FileInBuffer=255; // invalid, always load first image
  uint8_t NextFileRequired = 0; 
};

extern TFTs tfts;


#endif // TFTS_H