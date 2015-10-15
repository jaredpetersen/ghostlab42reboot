# Developer Information
## Overview
The boards each have a [IS31FL3730 display driver](http://www.issi.com/WW/pdf/31FL3730.pdf). The Common cathode 7-segment + decimal point LED displays are arranged with each of the 8 segments using the 8 rows and the digit selection uses the columns, 1-4 for the 4 digit display and columns 1-6 for the 6 digit display. The leftmost digit is controlled by column 1.

The two displays can be daisy chained together on the same I2C connection since they have different addresses:
* The 4 digit display board is addressed at 0x63
* The 6 digit display board is addressed at 0x60

The 31FL3730 is a write only device, so no way to verify that the data was received correctly.

Each digit and associated decimal point are in one register.  Writing to the register will only update a temporary register and you must write (anything) to the "Update Column Register", 0x0C, to get the display to show what you wrote to these temporary registers.

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

The display was designed for 20mA per segment max, and the display driver defaults to 40mA, so this needs to be corrected immediately.  It may be useful to update the power after every command since the display may become unplugged and we don't ever want to use the default current setting.

The I2C command stream consists of the device address, followed by the register index, followed by the data to be written to that register.  Subsequent bytes will be written to the next register index.

## Electrical Connections
Included wires for this demo board are badly color coded, but here they are:
* Power
  * Yellow -> 5V
  * White  -> GND
* I2C (Arduino UNO)
  * Red    -> SDA (or ANALOG IN A4)
  * Black  -> SCL (or ANALOG IN A5)
