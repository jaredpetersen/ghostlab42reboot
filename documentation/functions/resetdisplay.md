# resetDisplay(int digits)
### Description
Clears the display and resets the brightness level to 100%.

### Parameters
digits: The display to write to. Input 4 for the four-segment display or 6 for the six-segment display.

### Example
```
GhostLab42Reboot reboot;
reboot.begin();
reboot.write(4, "01234");
reboot.resetDisplay(4);
```
