#include <GhostLab42Reboot.h>
#include <Wire.h>

GhostLab42Reboot reboot;

void setup() 
{
  reboot.begin();
}

void loop()
{
  for (int i = 0; i <= 100; i++) {
    // Set up the display brightness
    reboot.setDisplayBrightness(4, i);
    reboot.setDisplayBrightness(6, i);
    // Write values to the displays
    reboot.write(4, "8888");
    reboot.write(6, "888888");
    // Wait for a bit then increment the counter
    delay(20);
  }
}

