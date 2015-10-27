# writeRandom(int digits)
### Description
Writes random integer characters to the display. The integer range for the four-segment display is between 1000 and 9999 (inclusive) and the integer range for the six-segment display is between 1000000 and 999999 (inclusive).

### Parameters
digits: The display to write to. Input 4 for the four-segment display or 6 for the six-segment display.

### Example
```
GhostLab42Reboot reboot;
reboot.begin();
reboot.writeRandom(4);
```
