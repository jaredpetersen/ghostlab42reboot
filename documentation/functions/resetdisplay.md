# resetDisplay(int displayID)
### Description
Clears the display and resets the brightness level to 100%.

### Parameters
displayID: Unique identifier for the display that is to be reset. Input 1 for the four-digit display, 2 for the smaller four-digit display, or 3 for the six-digit display.

### Example
```
GhostLab42Reboot reboot;
reboot.begin();
reboot.write(1, "0123");
reboot.resetDisplay(1);
```
