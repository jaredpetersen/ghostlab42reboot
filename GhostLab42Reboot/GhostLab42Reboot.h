/*
 * Arduino library that serves as the driver for GhostLab42's Reboot
 * dual-display board set
 *
 * See README.md and LICENSE for more information
 */

#ifndef GhostLab42Reboot_h
#define GhostLab42Reboot_h

#include "Arduino.h"
#include "Wire.h"

class GhostLab42Reboot
{
  public:
    GhostLab42Reboot();
    void setup();
    void print(int digits, char value[]);
    void resetDisplay(int digits);
    void setDisplayBrightness (int digits, byte PWM);
  private:
    void setDisplayPowerMin(int digits);
    void setDisplayPowerMax(int digits);
    void setupWireTransmission(int digits);
    byte charToDisplayByte(char displayCharacter);
};

#endif