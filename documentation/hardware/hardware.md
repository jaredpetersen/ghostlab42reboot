# Hardware Guide
## General
The reboot displays are designed to recreate the displays used in the *Ghostbusters* (2016) movie's proton packs. The board set includes three seven-segment displays with a surface-mounted display controller on the back of each board. Each display has a different configuration. They can all be controlled via an external Arduino (not included) so that the display operation can be easily changed to match whatever they end up doing with the displays in the movie.

Each display is given a unique ID so that it can be identified in the code:

| ID  | Digits | Color       | Location on Pack                   |
| --- | ------ | ----------- | ---------------------------------- |
| 0   | 6      | Green       | Top-left side                      |
| 1   | 4      | Yelow/Green | Upper-center left side             |
| 2   | 4      | Green       | Lower-center left side at an angle |

<img src="/documentation/hardware/images/protonpack.png?raw=true" width="250">

## Board Prep
The kit comes with three boards and five four-pin cables so you can daisy-chain them together and control them from an Arduino using just two signal pins and +5V and GND.

<img src="/documentation/hardware/images/kit.png?raw=true" width="600">

<img src="/documentation/hardware/images/boarddimensions.png?raw=true" width="600">

The boards need to be cut apart. They have "rat bites" to make them break apart, but apparently the rat that was used was missing a few teeth so it is best to just cut the boards apart and not count on being able to snap them apart. A Dremel is recommended but a hacksaw will also work fine. The boards are made of glass epoxy, so it is probably best to not breathe in the debris generated while cutting them.

Four short cuts will be required to make three separate rectangular-shaped boards:

<img src="/documentation/hardware/images/cutinstructions.png?raw=true" width="600">

## Connections
The boards do not need to be electrically connected to the Arduino and to each other. The kit includes five cables to help you make these connections.

**WARNING:** The cables are not consistently color-coded. Take great care to make sure the proper signals are connected and do not rely on a particular color of the wire.

Both of the connectors on each of board have the same signal names, are connected electrically on the board and are identical -- it does not matter which one you connect to. The reason for two connectors is so you can daisy-chain the boards together: Connect the Arduino to one connector and then a cable from the other connector to a connector on another board, etc.

Here are the signal names for each connector:

<img src="/documentation/hardware/images/signalnames.png?raw" width="250">

Here is a diagram showing the same information, but labeled near each of the connectors:

<img src="/documentation/hardware/images/signalnamesboard.png?raw=true" width="600">

Different Arduinos have different connections; some are socketed and some provide solder holes. There are also breadboard/dev boards for making your own connections to a circuit board that can plug into the Arduino. In this example, an Arduino UNO which has sockets will just directly attach to it.

Jumper wires can be cut in half and attached to the end of one of the five cables:

<img src="/documentation/hardware/images/jumperwires.png?raw=true" width="600">

<img src="/documentation/hardware/images/jumperwiresconversion.png?raw=true" width="600">

Pinout diagram for the Arduino UNO (R3):

<img src="/documentation/hardware/images/unopinout.png?raw=true" width="600">

Connecting the four wires to the Arduino Uno (see signal names for reference):
1. SLK from ANALOG IN section on upper-right side (or lower-left side)
2. SDA from ANALOG IN section on upper-right side (or lower-left side)
3. GND (Ground) from POWER section on middle-left side
4. 5V from POWER section on middle-left side

This cable can connect to any one display board and work fine:

<img src="/documentation/hardware/images/connectioninstruction1.png?raw=true" width="600">

It is ideal to have all three displays controlled without any additional Arduino connections. To accomplish this, use the remaining four cables and solder them together in pairs to make two longer cables with the four-pin connector on both ends:

<img src="/documentation/hardware/images/connectioninstruction2.png?raw=true" width="600">

Connect the Arduino to all three boards by daisy-chaining them together:

<img src="/documentation/hardware/images/connectioninstruction3.png?raw=true" width="600">

Here are the same connections but you can see where the cables attach to the back of the boards:

<img src="/documentation/hardware/images/connectioninstruction4.png?raw=true" width="600">

The two connectors on the board are identical, so it does not matter which one you plug into. There are two on each board, so you can decide how to best route your cables and determine which one will only use one of the connectors (the last one in the chain)
