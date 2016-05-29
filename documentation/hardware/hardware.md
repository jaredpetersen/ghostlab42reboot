# Hardware Guide
## General
The reboot displays are designed to recreate the displays used in the *Ghostbusters* (2016) movie's proton packs. The board set includes three seven-segment displays with a surface-mounted display controller on the back of each board. Each display has a different configuration. They can all be controlled via an external Arduino (not included) so that the display operation can be easily changed to match whatever they end up doing with the displays in the movie.

Each display is given a unique ID so that it can be identified in the code:

| ID  | Digits | Color       | Location on Pack                   |
| --- | ------ | ----------- | ---------------------------------- |
| 0   | 6      | Green       | Top-left side                      |
| 1   | 4      | Yelow/Green | Upper-center left side             |
| 2   | 4      | Green       | Lower-center left side at an angle |

![Proton Pack](/images/protonpack.png?raw=true)

## Board Prep
The kit comes with three boards and five four-pin cables so you can daisy-chain them together and control them from an Arduino using just two signal pins and +5V and GND.

![Kit](/images/kit.png?raw=true)

![Board Dimensions](/images/kit.png?raw=true)

The boards need to be cut apart. They have "rat bites" to make them break apart, but apparently the rat that was used was missing a few teeth so it is best to just cut the boards apart and not count on being able to snap them apart. A Dremel is recommended but a hacksaw will also work fine. The boards are made of glass epoxy, so it is probably best to not breathe in the debris generated while cutting them.

Four short cuts will be required to make three separate rectangular-shaped boards:

![Cut Instructions](/images/cutinstructions.png?raw=true)

## Connections
The boards do not need to be electrically connected to the Arduino and to each other. The kit includes five cables to help you make these connections.

**WARNING:** The cables are not consistently color-coded. Take great care to make sure the proper signals are connected and do not rely on a particular color of the wire.

Both of the connectors on each of board have the same signal names, are connected electrically on the board and are identical -- it does not matter which one you connect to. The reason for two connectors is so you can daisy-chain the boards together: Connect the Arduino to one connector and then a cable from the other connector to a connector on another board, etc.

Here are the signal names for each connector:

![Signal Names](/images/signalnames.png?raw=true)

Here is a diagram showing the same information, but labeled near each of the connectors:

![Signal Names Board](/images/signalnamesboard.png?raw=true)

Different Arduinos have different connections; some are socketed and some provide solder holes. There are also breadboard/dev boards for making your own connections to a circuit board that can plug into the Arduino. In this example, an Arduino UNO which has sockets will just directly attach to it.

Jumper wires can be cut in half and attached to the end of one of the five cables:

![Jumper Wires](/images/jumperwires.png?raw=true)

![Jumper Wires Conversion](/images/jumperwiresconversion.png?raw=true)

Pinout diagram for the Arduino UNO (R3):

![Arduino Uno R3 Pinout](/images/pinout.png?raw=true)

Connecting the four wires to the Arduino Uno (see signal names for reference):
1. SLK from ANALOG IN section on upper-right side (or lower-left side)
2. SDA from ANALOG IN section on upper-right side (or lower-left side)
3. GND (Ground) from POWER section on middle-left side
4. 5V from POWER section on middle-left side

This cable can connect to any one display board and work fine:

![Connection Instruction 1](/images/connectioninstruction1.png?raw=true)

It is ideal to have all three displays controlled without any additional Arduino connections. To accomplish this, use the remaining four cables and solder them together in pairs to make two longer cables with the four-pin connector on both ends:

![Connection Instruction 2](/images/connectioninstruction2.png?raw=true)

Connect the Arduino to all three boards by daisy-chaining them together:

![Connection Instruction 3](/images/connectioninstruction3.png?raw=true)

Here are the same connections but you can see where the cables attach to the back of the boards:

![Connection Instruction 4](/images/connectioninstruction4.png?raw=true)

The two connectors on the board are identical, so it does not matter which one you plug into. There are two on each board, so you can decide how to best route your cables and determine which one will only use one of the connectors (the last one in the chain)
