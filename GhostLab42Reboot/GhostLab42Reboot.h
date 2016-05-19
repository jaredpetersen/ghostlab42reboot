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
    void write(int displayID, String value);
    void resetDisplay(int displayID);
    void setDisplayBrightness (int displayID, int brightness);
  private:
    bool verifyDisplayID(int displayID);
    void setDisplayPowerMin(int displayID);
    void setDisplayPowerMax(int displayID);
    void setupWireTransmission(int displayID);
    void writeCharacter(char displayCharacters[]);
};

#endif
