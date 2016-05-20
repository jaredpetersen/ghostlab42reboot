# write(int displayID, String value)
### Description
Writes characters to the display. Supports integers, decimals, letters, and some punctuation (periods, question marks, exclamation points, and hyphens). Please note that decimals/periods will be wrapped into the previous character's digit display unless extra "spaces" are inserted or if the decimal/period is the first character in the input string (in which case there is technically a "space" added in front of it).

If the number of characters being written exceed the display length, the display will cut off the overflowing characters. For example, writing a string with six digits like "012345" to the four-segment display will result in the display only showing "0123".

If the number of characters being written are fewer than the display length, the remaining display spaces will not be lit up. For example, writing a string with two digits like "22" to the four-segment display will result in the display showing "22  ".

If the library does not recognize a character that you are trying to write, the space that the character would normally occupy will not be lit up. For example, writing a string like "8$3#57" to the six-segment display will result in "8 3 57" being displayed.

Make sure to call the `resetDisplay(int displayID)` function between write calls if the length of the input differs. Not doing so will leave the previous character in the display. For example, writing "0123" and then "98" to one of the four-digit displays without calling the `resetDisplay(int displayID)` function between the two write calls will leave the display showing "9823"

### Parameters
displayID: Unique identifier for the display that is to be written to. Input 0 for the six-digit display, 1 for the smaller four-digit display, or 2 for the four-digit display.

value: String with the value that you would like to display.

### Example
```
GhostLab42Reboot reboot;
reboot.begin();
reboot.write(1, "0123");
```
