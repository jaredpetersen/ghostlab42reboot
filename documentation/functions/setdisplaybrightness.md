# setDisplayBrightness(int displayID, int brightness)
### Description
Changes the brightness level of the display via a percentage.

### Parameters
displayID: Unique identifier for the display that is to have its brightness set. Input 0 for the six-digit display, 1 for the smaller four-digit display, or 2 for the four-digit display.

brightness: The brightness level of the display as a percentage (ex. 100 = 100%, 25 = 25%, etc.).

### Example
```
GhostLab42Reboot reboot;
reboot.begin();
reboot.setDisplayBrightness(1, 10);
reboot.write(1, "0123");
```
