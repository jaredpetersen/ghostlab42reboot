# write(int digits, char value[])
### Description
Writes characters to the display. Only supports integers at this time but decimal numbers and letters are planned to be supported.

If the characters being written exceed the display length, the display will cut off the overflowing characters. For example, writing an string with six digits like "012345" to the four-segment display will result in the display only showing "0123".

If the display does not recognize a character that you are trying to write, the space that character would normally occupy will not be blank (not lit up). For example, writing a string like "8$3#57" to the six-segment display will result in "8 3 57" being displayed.

### Parameters
digits: The display to write to. Input 4 for the four-segment display or 6 for the six-segment display.

value: Character array with the integer that you would like to display.

### Example
```
GhostLab42Reboot reboot;
reboot.begin();
reboot.write(4, "0123");
```
