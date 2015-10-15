/*
 * Arduino library that serves as the driver for GhostLab42's Reboot dual-display board set
 */

#include "Wire.h"
#define IS31FL3730_DIGIT_4_I2C_ADDRESS 0x63 // each I2C has a unique bus address, the 4 digit IS31FL3730 display is 0x63
#define IS31FL3730_DIGIT_6_I2C_ADDRESS 0x60 // each I2C has a unique bus address, the 6 digit IS31FL3730 display is 0x60

const byte IS31FL3730_Data_Registers = 0x01; // "Matrix 1 Data Register" index in the IS31FL3730
// 8 bit value to define which segments are lit. 0 == off, 1 == on
// This is the starting index.  Sequential bytes will go to the next register index.
// Works great for data, but stop after the matrix data is updated and then end the command.

const byte IS31FL3730_Update_Column_Register = 0x0C; // "Update Column Register" index in the IS31FL3730
// The data sent to the Data Registers will be stored in temporary registers. 
// A write operation of any 8-bit value to the Update Column Register is required to update 
// the Data Registers (01h~0Bh, 0Eh~18h)

const byte IS31FL3730_Lighting_Effect_Register = 0x0D; // "Lighting Effect Register" index in the IS31FL3730
// Bottom four bits control the Full Current Setting for Each Row Output 
//   0000: 40mA 
//   0001: 45mA 
//   0010: 50mA 
//   0011: 55mA 
//   0100: 60mA 
//   0101: 65mA 
//   0110: 70mA 
//   0111: 75mA 
//   1000: 5mA 
//   1001: 10mA 
//   1010: 15mA 
//   1011: 20mA 
//   1100: 25mA 
//   1100: 30mA 
//   1110: 35mA
//   1111: ???? - not documented!

const byte IS31FL3730_PWM_Register = 0x19; // "PWM Register" index in the IS31FL3730
// The PWM Register can modulate LED light with 128 different items.
// When the D7 set to “1”, the PWM is the 128 item. 
// When the D7 set to “0”, D6:D0 set the PWM from the 0 item to the 127 item.

const byte IS31FL3730_Reset_Register = 0xFF; // "Reset Register" index in the IS31FL3730
// Once user writes any 8-bit data to the Reset Register, IS31FL3730 will reset all registers to default value.
// On  initial power-up, the IS31FL3730 registers are reset to their default values for a blank display.

/*
 * Android Setup Function
 */
void setup() {
  Wire.begin();
  
  // Set the maximum display power for both displays
  setDisplayPowerMax(4);
  setDisplayPowerMax(6);
}

/*
 * Resets the display and sets the current to the maximum (5mA per segment)
 * 
 * Parameters:
 * Digits Selects the display to dim, 4 = 4 digit display and all others = 6 digit display
 */
void resetDisplay(int Digits) 
{
  // Set up the wire transmission depending on the display being reset
  if (Digits == 4)
  { 
    Wire.beginTransmission(IS31FL3730_DIGIT_4_I2C_ADDRESS); 
  }
  else
  { 
    Wire.beginTransmission(IS31FL3730_DIGIT_6_I2C_ADDRESS); 
  }

  // Reset the lighting effects (dim, bright, etc.)
  // Send any value to reset the lighting (value ignored)
  Wire.write(IS31FL3730_Reset_Register);
  Wire.write(0x00);
  Wire.endTransmission();

  // Reset the current
  setDisplayPowerMax(Digits);
}

/*
 * Sets the current to the minimum (5mA per segment)
 * 
 * Parameters:
 * Digits Selects the display to dim, 4 = 4 digit display and all others = 6 digit display
 */
void setDisplayPowerMin(int Digits) 
{
  // Set up the transmission depending on the display being reset
  if (Digits == 4)
  { 
    Wire.beginTransmission(IS31FL3730_DIGIT_4_I2C_ADDRESS); 
  }
  else
  { 
    Wire.beginTransmission(IS31FL3730_DIGIT_6_I2C_ADDRESS); 
  }
  
  Wire.write(IS31FL3730_Lighting_Effect_Register);
  Wire.write(0x08); // Lowest level, 10mA
  Wire.endTransmission();
}  

/*
 * Sets the current to the maximum alllowed for these displays (0mA per segment)
 * 
 * Parameters:
 * Digits Selects the display to dim, 4 = 4 digit display and all others = 6 digit display
 */
void setDisplayPowerMax(int Digits) {
  /* The display driver does allow currents greater than the displays should take - do not allow tanything over 20mA!
   * The display driver resets to 40mA per segment which is too much.  Somehow we need to make sure that is never
   * violated but the part is write-only, so we need to keep track of the setting somewhere or just always force to the max current.
   */

  // Set up the transmission depending on the display being reset
  if (Digits == 4)
  { 
    Wire.beginTransmission(IS31FL3730_DIGIT_4_I2C_ADDRESS); 
  }
  else
  { 
    Wire.beginTransmission(IS31FL3730_DIGIT_6_I2C_ADDRESS); 
  }
  
  Wire.write(IS31FL3730_Lighting_Effect_Register);
  Wire.write(0x0B); // Highest level, 20mA
  Wire.endTransmission();
}  

/**
 * Dims the display
 * 
 * Parameters:
 * Digits Selects the display to dim, 4 = 4 digit display and all others = 6 digit display
 * PWM    Sets the dimming level, not very linear. 0x80 = full intensity, 0x00 is off
 */
void setDisplayBrightness (int Digits, byte PWM) {
  // Probably should rewrite to allow a user setting in percentage, so an integer and 0 to 100 map to 0 to 128.  
  // Anything above 100 would be clamped to the max useful register value of 128 (0x80).

  // Set up the transmission depending on the display being reset
  if (Digits == 4)
  { 
    Wire.beginTransmission(IS31FL3730_DIGIT_4_I2C_ADDRESS); 
  }
  else
  { 
    Wire.beginTransmission(IS31FL3730_DIGIT_6_I2C_ADDRESS); 
  }

  // Begin dimming the display
  Wire.write(IS31FL3730_PWM_Register); // Lighting Effect Register

  // Do not allow the user to set values higher than the max allowed (always on)
  if (PWM < 0x80)
  { 
    // Value is less than the max, set the requested value
    Wire.write(PWM); // write the value that was requested
  }
  else
  { 
    // Value is higher than the max, set the max
    Wire.write(0x80); // write the max value allowed (always on)
  }
  
  Wire.endTransmission();
}


/*
 * Test Routine to force something interesting on the 4-digit display
 */
void display_update_4a() {
  // Write the display data in the temporary registers
  Wire.beginTransmission(IS31FL3730_DIGIT_4_I2C_ADDRESS);
  Wire.write(IS31FL3730_Data_Registers); // Matrix 1 Data registers
  Wire.write(0x3F); // Digit 1 value (far left) -- Display "0"
  Wire.write(0x5B); // Digit 2 value -- Display "2"
  Wire.write(0x4F); // Digit 3 Value -- Display "3"
  Wire.write(0x66); // Digit 4 value (far right) -- Display "4"
  Wire.endTransmission();
  
  // Transfer the display data from the temporary registers to the display registers (display the values)
  Wire.beginTransmission(IS31FL3730_DIGIT_4_I2C_ADDRESS);
  Wire.write(IS31FL3730_Update_Column_Register); // Update Column Register
  Wire.write(0x00); // Send any value to initate the display (value ignored)
  Wire.endTransmission();
}

/*
 * Test Routine to force something interesting on the 4-digit display
 */
void display_update_4b() {
  // Write the display data in the temporary registers
  Wire.beginTransmission(IS31FL3730_DIGIT_4_I2C_ADDRESS);
  Wire.write(IS31FL3730_Data_Registers); // Matrix 1 Data Registers 
  Wire.write(0xFF); // Digit 1 value (far left)
  Wire.write(0x02); // Digit 2 value
  Wire.write(0x04); // Digit 3 value
  Wire.write(0x08); // Digit 4 value (far right)
  Wire.endTransmission();
  
  // Transfer the display data from the temporary registers to the display registers (display the values)
  Wire.beginTransmission(IS31FL3730_DIGIT_4_I2C_ADDRESS);
  Wire.write(IS31FL3730_Update_Column_Register); // Update Column Register
  Wire.write(0x00); // Send any value to initate the display (value ignored)
  Wire.endTransmission();
}

/*
 * Test Routines to force something interesting on the 6-digit display
 */
void display_update_6a() {
  Wire.beginTransmission(IS31FL3730_DIGIT_6_I2C_ADDRESS);
  Wire.write(IS31FL3730_Data_Registers); // Matrix 1 Data Registers 
  Wire.write(0x06); // Digit 1 value (far left)
  Wire.write(0x5B); // Digit 2 value
  Wire.write(0x4F); // Digit 3 value
  Wire.write(0x66); // Digit 4 value
  Wire.write(0x6D); // Digit 5 value
  Wire.write(0x7D); // Digit 6 value (far right)
  Wire.endTransmission();
  
  // Now transfer the data from the temporary registers to the display registers
  Wire.beginTransmission(IS31FL3730_DIGIT_6_I2C_ADDRESS);
  Wire.write(IS31FL3730_Update_Column_Register); // Update Column Register
  Wire.write(0x00); // Any value should work (value ignored)
  Wire.endTransmission();
}

/*
 * Test Routines to force something interesting on the 6-digit display
 */
void display_update_6b() {
  Wire.beginTransmission(IS31FL3730_DIGIT_6_I2C_ADDRESS);
  Wire.write(IS31FL3730_Data_Registers); // Matrix 1 Data Registers 
  Wire.write(0xFF); // Digit 1 value (far left)
  Wire.write(0x02); // Digit 2 value
  Wire.write(0x04); // Digit 3 value
  Wire.write(0x08); // Digit 4 value
  Wire.write(0x10); // Digit 5 value
  Wire.write(0x20); // Digit 6 value (far right)
  Wire.endTransmission();
  
  // Now transfer the data from the temporary registers to the display registers
  Wire.beginTransmission(IS31FL3730_DIGIT_6_I2C_ADDRESS);
  Wire.write(IS31FL3730_Update_Column_Register); // Update Column Register
  Wire.write(0x00); // Any value should work (value ignored)
  Wire.endTransmission();
}

/*
 * Arduino Overall Looping Function
 */
void loop() {
  // Print "0234" and hold
  display_update_4a();
  
  // Wait and print "12345678"
  delay(500);
  setDisplayBrightness(6,0x80);
  display_update_6a();
  
  delay(500);
  
  setDisplayBrightness(4,0x10);
  display_update_4b();
  
  delay(500);

  setDisplayBrightness(6,0x02);
  display_update_6b();
  
  delay(500);

  setDisplayBrightness(4,0x80);
  resetDisplay(4); // reset the 4 digit display
  
  delay(500);
  
  resetDisplay(6); // reset the 6 digit display
  
  delay(500);
}
