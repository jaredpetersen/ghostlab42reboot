# resetDisplay(int displayID)
### Description
Clears the display.

### Parameters
displayID: Unique identifier for the display that is to be reset. Input 0 for the six-digit display, 1 for the smaller four-digit display, or 2 for the four-digit display.

### Example
```
GhostLab42Reboot reboot;
reboot.begin();
reboot.write(1, "0123");
reboot.resetDisplay(1);
```
