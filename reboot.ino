/*
 
 Dual display driver board support for Ghostlab42's Reboot display board set.

 The boards each have a IS31FL3730 display driver.
 http://www.issi.com/WW/pdf/31FL3730.pdf
 The Common cathode 7-segment + decimal point LED displays are arranged with each of the 8 segments using the 8 rows and 
 the digit selection uses the columns, 1-4 for the 4 digit display and columns 1-6 for the 6 digit display.
 The leftmost digit is controlled by column 1.
  
 The two displays can be daisy chained together on the same I2C connection since they have different addresses:
 The 4 digit display board is addressed at 0x63
 The 6 digit display board is addressed at 0x60
 
 The 31FL3730 is a write only device, so no way to verify that the data was received correctly.
 Each digit and associated decimal point are in one register.  Writing to the register will only update a temporary register
 and you must write (anything) to the "Update Column Register", 0x0C, to get the display to show what you wrote to these 
 temporary registers.
 
 Single segment values:
 .     --0x01--
 .    |        |
 .   0x20     0x02
 .    |        |
 .     --0x40--
 .    |        |
 .   0x10     0x04
 .    |        |
 .     --0x08--      
 .                0x80
 
 The display was designed for 20mA per segment max, and the display driver defaults to 40mA, so this needs to be corrected 
 immediately.  It may be useful to update the power after every command since the display may become unplugged and we don't 
 ever want to use the default current setting.
 
 The I2C command stream consists of the device address, followed by the register index, followed by the data 
 to be written to that register.  Subsequent bytes will be written to the next register index.

 ==============================================================================
 Electrical Connections
 ==============================================================================
 Included wires for this demo board are badly color coded, but here they are:
 Power
   Yellow -> 5V
   White  -> GND
 I2C (Arduino UNO)
   Red    -> SDA (or ANALOG IN A4)
   Black  -> SCL (or ANALOG IN A5)
 */

#include "Wire.h"
#define IS31FL3730_DIGIT_4_I2C_ADDRESS 0x63 // each I2C has a unique bus address, the 4 digit IS31FL3730 display is 0x63
#define IS31FL3730_DIGIT_6_I2C_ADDRESS 0x60 // each I2C has a unique bus address, the 4 digit IS31FL3730 display is 0x60

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

void setup() {
  Wire.begin();
  // Is the baud rate call even needed?
  // Serial.begin(9600);  

  // The drive current defaults to 40mA per segment - that is too much!  Only allow up to 20mA.
  IS31FL3730_display_power_max(4); // start with max allowed current setting to the display (1/2 of the default current setting)
  IS31FL3730_display_power_max(6); // start with max allowed current setting to the display (1/2 of the default current setting)
}

void IS31FL3730_display_reset(int Digits) {
  // Reset the display and then set the current to the maximum which is 5mA per segment
  if (Digits == 4)
  { 
    Wire.beginTransmission(IS31FL3730_DIGIT_4_I2C_ADDRESS); 
  }
  else
  { 
    Wire.beginTransmission(IS31FL3730_DIGIT_6_I2C_ADDRESS); 
  }
  Wire.write(IS31FL3730_Reset_Register); // Lighting affect Register
  Wire.write(0x00); // does not matter...
  Wire.endTransmission();
  delay(1); // Is a delay needed between commands?
  IS31FL3730_display_power_max(Digits);
}


void IS31FL3730_display_power_min(int Digits) {
  // Set the current to the minimum which is 5mA per segment
  if (Digits == 4)
  { 
    Wire.beginTransmission(IS31FL3730_DIGIT_4_I2C_ADDRESS); 
  }
  else
  { 
    Wire.beginTransmission(IS31FL3730_DIGIT_6_I2C_ADDRESS); 
  }
  Wire.write(IS31FL3730_Lighting_Effect_Register); // Lighting affect Register
  Wire.write(0x08); // lowest level, 10mA
  Wire.endTransmission();
}  

void IS31FL3730_display_power_max(int Digits) {
  // Set the current to the maximum allowed for these displays which is 0mA per segment.
  // The display driver does allow currents greater than the displays should take - do not allow tanything over 20mA!
  // The display driver resets to 40mA per segment which is too much.  SOmehow we need to make sure that is never
  // violated but the part is write-only, so we need to keep track of the setting somewhere or just always force to the max current.
  if (Digits == 4)
  { 
    Wire.beginTransmission(IS31FL3730_DIGIT_4_I2C_ADDRESS); 
  }
  else
  { 
    Wire.beginTransmission(IS31FL3730_DIGIT_6_I2C_ADDRESS); 
  }
  Wire.write(IS31FL3730_Lighting_Effect_Register); // Lighting affect Register
  Wire.write(0x0B); // highest level, 20mA
  Wire.endTransmission();
}  

void IS31FL3730_Display_Dimming (int Digits, byte PWM) {
  // "Digits" selects which display
  //    4 == 4 digit display
  //    all other values select the 6 digit display
  // "PWM" sets the dimming level, not very linear!
  //    0x80 is the max (full intensity)
  //    0x00 is off
  // Probably should rewrite to allow a user setting in percentage, so an integer and 0 to 100 map to 0 to 128.  
  // Anything above 100 would be clamped to the max useful register value of 128 (0x80).
  if (Digits == 4)
  { 
    Wire.beginTransmission(IS31FL3730_DIGIT_4_I2C_ADDRESS); 
  }
  else
  { 
    Wire.beginTransmission(IS31FL3730_DIGIT_6_I2C_ADDRESS); 
  }
  Wire.write(IS31FL3730_PWM_Register); // Lighting Effect Register
  if (PWM < 0x80)
  { 
    Wire.write(PWM); // write the value that was requested
  }
  else
  { 
    Wire.write(0x80); // write the max value allowed (always on)
  }
  Wire.endTransmission();
}


// test routines to force something interesting on the display
void display_update_4a() {
  // write the data to be displayed in the temporary registers
  Wire.beginTransmission(IS31FL3730_DIGIT_4_I2C_ADDRESS);
  Wire.write(IS31FL3730_Data_Registers); // Matrix 1 Data registers
  Wire.write(0x06); // Digit 1 value (far left)
  Wire.write(0x5B); // Digit 2 value
  Wire.write(0x4F); // Digit 3 Value
  Wire.write(0x66); // Digit 4 value (far right)
  Wire.endTransmission();
  // Now transfer the data from the temporary registers to the display registers
  delay(1); //is this needed?
  Wire.beginTransmission(IS31FL3730_DIGIT_4_I2C_ADDRESS);
  Wire.write(IS31FL3730_Update_Column_Register); // Update Column Register
  Wire.write(0x00); // Any value should work (value ignored)
  Wire.endTransmission();
}

void display_update_4b() {
  // write the data to be displayed in the temporary registers
  Wire.beginTransmission(IS31FL3730_DIGIT_4_I2C_ADDRESS);
  Wire.write(IS31FL3730_Data_Registers); // Matrix 1 Data Registers 
  Wire.write(0xFF); // Digit 1 value (far left)
  Wire.write(0x02); // Digit 2 value
  Wire.write(0x04); // Digit 3 value
  Wire.write(0x08); // Digit 4 value (far right)
  Wire.endTransmission();
  // Now transfer the data from the temporary registers to the display registers
  delay(1); // is this needed?
  Wire.beginTransmission(IS31FL3730_DIGIT_4_I2C_ADDRESS);
  Wire.write(IS31FL3730_Update_Column_Register); // Update Column Register
  Wire.write(0x00); // Any value should work (value ignored)
  Wire.endTransmission();
}

void display_update_6a() {
  // write the data to be displayed in the temporary registers
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
  delay(1); // is this needed?
  Wire.beginTransmission(IS31FL3730_DIGIT_6_I2C_ADDRESS);
  Wire.write(IS31FL3730_Update_Column_Register); // Update Column Register
  Wire.write(0x00); // Any value should work (value ignored)
  Wire.endTransmission();
}

void display_update_6b() {
  // write the data to be displayed in the temporary registers
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
  delay(1); // is this needed?
  Wire.beginTransmission(IS31FL3730_DIGIT_6_I2C_ADDRESS);
  Wire.write(IS31FL3730_Update_Column_Register); // Update Column Register
  Wire.write(0x00); // Any value should work (value ignored)
  Wire.endTransmission();
}

void loop() {
  display_update_4a();
  IS31FL3730_Display_Dimming(6,0x80);
  delay(500);
  display_update_6a();
  IS31FL3730_Display_Dimming(4,0x10);
  delay(500);
  display_update_4b();
  IS31FL3730_Display_Dimming(6,0x02);
  delay(500);
  display_update_6b();
  IS31FL3730_Display_Dimming(4,0x80);
  delay(500);
  IS31FL3730_display_reset(4); // reset the 4 digit display
  delay(500);
  IS31FL3730_display_reset(6); // reset the 6 digit display
  delay(500);
}
