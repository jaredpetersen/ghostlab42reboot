/*
 * Arduino library that serves as the driver for GhostLab42's Reboot
 * dual-display board set
 *
 * See README.md and LICENSE for more information
 */

#include <Arduino.h>
#include <Wire.h>
#include "GhostLab42Reboot.h"

// Each I2C has a unique bus address
#define IS31FL3730_DIGIT_4_I2C_ADDRESS 0x63 // 4 digit IS31FL3730 display
#define IS31FL3730_DIGIT_6_I2C_ADDRESS 0x60 // 6 digit IS31FL3730 display

// "Matrix 1 Data Register" index in the IS31FL3730
// 8-bit value to define which segments are lit.
// This is the starting index. Sequential bytes will go to the next
// register index.
const byte IS31FL3730_Data_Registers = 0x01;

// "Update Column Register" index in the IS31FL3730
// The data sent to the Data Registers will be stored in temporary registers
// A write operation of any 8-bit value to the Update Column Register is
// required to update the Data Registers
const byte IS31FL3730_Update_Column_Register = 0x0C;

// "Lighting Effect Register" index in the IS31FL3730
const byte IS31FL3730_Lighting_Effect_Register = 0x0D;

// "PWM Register" index in the IS31FL3730
// The PWM Register can modulate LED light at 128 different points
const byte IS31FL3730_PWM_Register = 0x19;

// "Reset Register" index in the IS31FL3730
// Once user writes any 8-bit data to the Reset Register, IS31FL3730 will reset
// all registers to default value
// On  initial power-up, the IS31FL3730 registers are reset to their default
// values for a blank display.
const byte IS31FL3730_Reset_Register = 0xFF;

// Light correction lookup table for the led displays
// Human eyes do not view light linearly, so this corrects for that using
// the CIE 1931 formula (see developer documentation)
const byte lightCorrectionTable[] =
{
    0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x05,
    0x05, 0x06, 0x06, 0x07, 0x07, 0x07, 0x08, 0x09, 0x09, 0x0A, 0x0A, 0x0B,
    0x0C, 0x0C, 0x0D, 0x0E, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
    0x15, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1F, 0x20, 0x21, 0x23,
    0x24, 0x25, 0x27, 0x28, 0x2A, 0x2C, 0x2D, 0x2F, 0x31, 0x32, 0x34, 0x36,
    0x38, 0x3A, 0x3C, 0x3E, 0x40, 0x42, 0x44, 0x46, 0x49, 0x4B, 0x4D, 0x50,
    0x52, 0x54, 0x57, 0x5A, 0x5C, 0x5F, 0x62, 0x64, 0x67, 0x6A, 0x6D, 0x70,
    0x73, 0x76, 0x79, 0x7D, 0x80
};

/******************************************************************************
 *                                Constructor                                 *
 ******************************************************************************/

GhostLab42Reboot::GhostLab42Reboot(){}

/*
 * Acts as the Constructor
 *
 * Would have liked to just use the constructor, but you can't call
 * Wire.begin there :-/
 */
void GhostLab42Reboot::begin()
{
    Wire.begin();

    // Set the maximum display power for both displays
    setDisplayPowerMax(4);
    setDisplayPowerMax(6);
}

/******************************************************************************
 *                              Public Functions                              *
 ******************************************************************************/

/*
 * Writes the characters to the selected display. The only characters allowed
 * are numbers 0-9 and letters A, b, C, d, E, and F
 */
void GhostLab42Reboot::write(int digits, String value)
{
  // User can technically give us any digit, so we have to do a nice
  // conversion with that data so that we can use it for proper iteration
  if (digits != 4) digits = 6;

  // Reset the display before writing
  resetDisplay(digits);

  // Write the display data in the temporary registers
  setupWireTransmission(digits);
  Wire.write(IS31FL3730_Data_Registers);

  // Character array that stores the substring that is to be written
  char substringValue[2];

  // Iterate over the print value and print out the individual characters
  // Any string that goes over the number of digits gets cut off
  // Any string that goes under the number of digits has blank spaces in
  // in the remaining spots
  for (int i = 0; i < value.length(); i++)
  {
    // Determine how the character should be written
    if (value.charAt(i + 1) == '.')
    {
      // There is a decimal, write it as part of the character
      // Prepare the substring
      substringValue[0] = value.charAt(i);
      substringValue[1] = value.charAt(i + 1);

      // Skip the decimal
      i++;
    }
    else
    {
      // There isn't a decimal, write the character normally
      // Prepare the substring
      substringValue[0] = value.charAt(i);
    }

    // Write the substring
    writeCharacter(substringValue);

    // Clear out the substring array
    memset(&substringValue[0], 0, sizeof(substringValue));
  }

  // End the temporary register transmission
  Wire.endTransmission();

  // Transfer the display data from the temporary registers to the display
  setupWireTransmission(digits);

  // Write to the Update Column Register to let the board know we want to
  // update the display
  Wire.write(IS31FL3730_Update_Column_Register);

  // Send any value to initate the display (value ignored)
  Wire.write(0x00);

  // End the Updte Column Register Transmission
  Wire.endTransmission();
}

/*
 * Resets the display and sets the current to the maximum (5mA per segment)
 *
 * Parameters:
 * digits Selects the display to set the current, 4 = 4 digit display and all
 *        others = 6 digit display
 */
void GhostLab42Reboot::resetDisplay(int digits)
{
  setupWireTransmission(digits);

  // Reset the lighting effects (dim, bright, etc.)
  // Send any value to reset the lighting (value ignored)
  Wire.write(IS31FL3730_Reset_Register);
  Wire.write(0x00);
  Wire.endTransmission();

  // Reset the current
  setDisplayPowerMax(digits);
}

/**
 * Dims the display
 *
 * Parameters:
 * digits     Selects the display to set the current, 4 = 4 digit display and all
 *            others = 6 digit display
 * brightness The dimming level percentage as an int 0 - 100.
 */
void GhostLab42Reboot::setDisplayBrightness (int digits, int brightness)
{
  // Begin dimming the display
  setupWireTransmission(digits);
  // Tell the lighting effect register to display at the desired
  // brightness level with values from the light correction lookup table
  Wire.write(IS31FL3730_PWM_Register);
  Wire.write(lightCorrectionTable[brightness]);
  Wire.endTransmission();
}

/******************************************************************************
 *                             Private Functions                              *
 ******************************************************************************/

/*
 * Sets the current to the minimum (5mA per segment)
 *
 * Parameters:
 * digits Selects the display to set the current, 4 = 4 digit display and all
 *        others = 6 digit display
 */
void GhostLab42Reboot::setDisplayPowerMin(int digits)
{
  setupWireTransmission(digits);

  Wire.write(IS31FL3730_Lighting_Effect_Register);
  Wire.write(0x08); // Lowest level, 10mA
  Wire.endTransmission();
}

/*
 * Sets the current to the maximum allowed for these displays (0mA per segment)
 *
 * Parameters:
 * digits Selects the display to set the current, 4 = 4 digit display and all
 *        others = 6 digit display
 */
void GhostLab42Reboot::setDisplayPowerMax(int digits)
{
  // The display driver does allow currents greater than the displays should
  // take - do not allow anything over 20mA!
  // The display driver resets to 40mA per segment which is too much.  Somehow
  // we need to make sure that is never violated but the part is write-only, so
  // we need to keep track of the setting somewhere or just always force to the
  // max current.

  setupWireTransmission(digits);

  Wire.write(IS31FL3730_Lighting_Effect_Register);
  Wire.write(0x0B); // Highest level, 20mA
  Wire.endTransmission();
}

/*
 * Set up the Wire transmission depending on the display being used
 *
 * Parameters:
 * digits Selects the display to set the current, 4 = 4 digit display and all
 *        others = 6 digit display
 */
void GhostLab42Reboot::setupWireTransmission(int digits)
{
  if (digits == 4)
  {
    // Use the four digit display
    Wire.beginTransmission(IS31FL3730_DIGIT_4_I2C_ADDRESS);
  }
  else
  {
    // Use the six digit display
    Wire.beginTransmission(IS31FL3730_DIGIT_6_I2C_ADDRESS);
  }
}

/*
 * Converts characters into the appropriate bytes for display (gfedcba format)
 *
 * Parameters:
 * displayCharacters The character(s) to be converted into a byte for the
 *                   display. Some characters like "W" need multiple digits.
 */
void GhostLab42Reboot::writeCharacter(char displayCharacters[])
{
  // Although this isn't very time efficient (linear time) it's apparently a
  // very bad idea to use hashmaps on Arduino because they're resource
  // intensive :-(

  // Sometimes decimals are involved, so we need something to add it
  byte decimalOffset = 0x00; // No decimal to add
  if (displayCharacters[1] == '.')
  {
    // Add a decimal
    decimalOffset = 0x80;
  }

  // Numbers
  if      (displayCharacters[0] == '0') Wire.write(0x3F + decimalOffset);
  else if (displayCharacters[0] == '1') Wire.write(0x06 + decimalOffset);
  else if (displayCharacters[0] == '2') Wire.write(0x5B + decimalOffset);
  else if (displayCharacters[0] == '3') Wire.write(0x4F + decimalOffset);
  else if (displayCharacters[0] == '4') Wire.write(0x66 + decimalOffset);
  else if (displayCharacters[0] == '5') Wire.write(0x6D + decimalOffset);
  else if (displayCharacters[0] == '6') Wire.write(0x7D + decimalOffset);
  else if (displayCharacters[0] == '7') Wire.write(0x07 + decimalOffset);
  else if (displayCharacters[0] == '8') Wire.write(0x7F + decimalOffset);
  else if (displayCharacters[0] == '9') Wire.write(0x6F + decimalOffset);

  // Letters
  else if (displayCharacters[0] == 'A' || displayCharacters[0] == 'a') Wire.write(0x77 + decimalOffset);
  else if (displayCharacters[0] == 'B' || displayCharacters[0] == 'b') Wire.write(0x7C + decimalOffset);
  else if (displayCharacters[0] == 'C' || displayCharacters[0] == 'c') Wire.write(0x39 + decimalOffset);
  else if (displayCharacters[0] == 'D' || displayCharacters[0] == 'd') Wire.write(0x5E + decimalOffset);
  else if (displayCharacters[0] == 'E' || displayCharacters[0] == 'e') Wire.write(0x79 + decimalOffset);
  else if (displayCharacters[0] == 'F' || displayCharacters[0] == 'f') Wire.write(0x71 + decimalOffset);
  else if (displayCharacters[0] == 'G' || displayCharacters[0] == 'g') Wire.write(0x3D + decimalOffset);
  else if (displayCharacters[0] == 'H' || displayCharacters[0] == 'h') Wire.write(0x76 + decimalOffset);
  else if (displayCharacters[0] == 'I' || displayCharacters[0] == 'i') Wire.write(0x06 + decimalOffset);
  else if (displayCharacters[0] == 'J' || displayCharacters[0] == 'j') Wire.write(0x1E + decimalOffset);
  else if (displayCharacters[0] == 'K' || displayCharacters[0] == 'k') Wire.write(0x76 + decimalOffset);
  else if (displayCharacters[0] == 'L' || displayCharacters[0] == 'l') Wire.write(0x38 + decimalOffset);
  else if (displayCharacters[0] == 'M' || displayCharacters[0] == 'm')
  {
      Wire.write(0x33);
      Wire.write(0x27 + decimalOffset);
  }
  else if (displayCharacters[0] == 'N' || displayCharacters[0] == 'n') Wire.write(0x54 + decimalOffset);
  else if (displayCharacters[0] == 'O' || displayCharacters[0] == 'o') Wire.write(0x3F + decimalOffset);
  else if (displayCharacters[0] == 'P' || displayCharacters[0] == 'p') Wire.write(0x73 + decimalOffset);
  else if (displayCharacters[0] == 'Q' || displayCharacters[0] == 'q') Wire.write(0x67 + decimalOffset);
  else if (displayCharacters[0] == 'R' || displayCharacters[0] == 'r') Wire.write(0x50 + decimalOffset);
  else if (displayCharacters[0] == 'S' || displayCharacters[0] == 's') Wire.write(0x6D + decimalOffset);
  else if (displayCharacters[0] == 'T' || displayCharacters[0] == 't') Wire.write(0x78 + decimalOffset);
  else if (displayCharacters[0] == 'U' || displayCharacters[0] == 'u') Wire.write(0x3E + decimalOffset);
  else if (displayCharacters[0] == 'V' || displayCharacters[0] == 'v') Wire.write(0x3E + decimalOffset);
  else if (displayCharacters[0] == 'W' || displayCharacters[0] == 'w')
  {
      Wire.write(0x3C);
      Wire.write(0x1E + decimalOffset);
  }
  else if (displayCharacters[0] == 'X' || displayCharacters[0] == 'x') Wire.write(0x76 + decimalOffset);
  else if (displayCharacters[0] == 'Y' || displayCharacters[0] == 'y') Wire.write(0x6E + decimalOffset);
  else if (displayCharacters[0] == 'Z' || displayCharacters[0] == 'z') Wire.write(0x5B + decimalOffset);

  // Symbols
  else if (displayCharacters[0] == '?') Wire.write(0xA3);
  else if (displayCharacters[0] == '!') Wire.write(0x82);
  else if (displayCharacters[0] == '-') Wire.write(0x40);

  // Anything else turns into a blank for that character space
  else Wire.write(0x00);
}
