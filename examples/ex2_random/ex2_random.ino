#include <GhostLab42Reboot.h>
#include <Wire.h>

GhostLab42Reboot reboot;

void setup()
{
  reboot.begin();
}

void loop()
{
  // Set the six segment display brightness to 10%
  reboot.setDisplayBrightness(6, 10);
  // Write random numbers to both displays
  reboot.writeRandom(4);
  reboot.writeRandom(6);
  delay(200);
}
