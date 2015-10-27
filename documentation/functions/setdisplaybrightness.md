# setDisplayBrightness(int digits, int brightness)
### Description
Changes the brightness level of the display via a percentage.

### Parameters
digits: The display to write to. Input 4 for the four-segment display or 6 for the six-segment display.

brightness: The brightness level of the display as a percentage (ex. 100 = 100%, 25 = 25%, etc.). Currently non-linear.

### Example
```
GhostLab42Reboot reboot;
reboot.begin();
reboot.setDisplayBrightness(4, 10);
reboot.write(4, "01234");
```
