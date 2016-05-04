# write(int digits, String value)
### Description
Writes characters to the display. Supports integers, decimals, letters, and some punctuation (periods, question marks, exclamation points, and hyphens).

If the number of characters being written exceed the display length, the display will cut off the overflowing characters. For example, writing a string with six digits like "012345" to the four-segment display will result in the display only showing "0123".

If the number of characters being written are fewer than the display length, the remaining display spaces will not be lit up. For example, writing a string with two digits like "22" to the four-segment display will result in the display showing "22  ".

If the library does not recognize a character that you are trying to write, the space that the character would normally occupy will not be lit up. For example, writing a string like "8$3#57" to the six-segment display will result in "8 3 57" being displayed.

### Parameters
digits: The display to write to. Input 4 for the four-segment display or 6 for the six-segment display.

value: Character array with the integer that you would like to display.

### Example
```
GhostLab42Reboot reboot;
reboot.begin();
reboot.write(4, "0123");
```
