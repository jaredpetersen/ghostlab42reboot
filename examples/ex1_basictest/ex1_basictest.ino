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
  reboot.setDisplayBrightness(3, 2);
  
  // Write specific numbers to all the displays in a flashing pattern
  reboot.write(1, "5678");
  reboot.write(2, "1923");
  reboot.write(3, "9146431");
  delay(200);

  reboot.write(1, "4251");
  reboot.write(2, "8210");
  reboot.write(3, "1709752");
  delay(200);
}
