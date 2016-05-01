/*
 * Arduino library that serves as the driver for GhostLab42's Reboot
 * dual-display board set
 *
 * See README.md and LICENSE for more information
 */

#ifndef GhostLab42Reboot_h
#define GhostLab42Reboot_h

#include <Arduino.h>
#include <Wire.h>

class GhostLab42Reboot
{
  public:
    GhostLab42Reboot();
    void begin();
    void write(int digits, String value);
    void writeRandom(int digits);
    void resetDisplay(int digits);
    void setDisplayBrightness (int digits, int brightness);
  private:
    void setDisplayPowerMin(int digits);
    void setDisplayPowerMax(int digits);
    void setupWireTransmission(int digits);
    void getRandomString(int digits, char randomString[]);
    byte charToDisplayByte(char displayCharacter);
};

#endif
