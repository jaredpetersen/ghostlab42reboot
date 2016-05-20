#include <GhostLab42Reboot.h>
#include <Wire.h>

GhostLab42Reboot reboot;

void setup()
{
  reboot.begin();

  // Set the six segment display brightness to 2%
  reboot.setDisplayBrightness(0, 2);
}

void loop()
{
  // Write specific numbers to all the displays in a flashing pattern
  reboot.write(0, "9146431");
  reboot.write(1, "1923");
  reboot.write(2, "5678");
  delay(200);

  reboot.write(0, "1709752");
  reboot.write(1, "8210");
  reboot.write(2, "4251");
  delay(200);
}
