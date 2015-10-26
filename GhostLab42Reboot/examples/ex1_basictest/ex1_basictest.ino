#include <GhostLab42Reboot.h>
#include <Wire.h>

GhostLab42Reboot reboot;

void setup()
{
  reboot.begin();
}

void loop()
{
  // Set the six segment display brightness to 2%
  reboot.setDisplayBrightness(6, 2);
  // Write specific numbers to both displays in a flashing pattern
  reboot.write(4, "5678");
  reboot.write(6, "9146431");
  delay(200);
  reboot.write(4, "4251");
  reboot.write(6, "1709752");
  delay(200);
}
