#include <GhostLab42Reboot.h>
#include <Wire.h>

GhostLab42Reboot reboot;

void setup() 
{
  reboot.begin();
}

void loop()
{
  reboot.setDisplayBrightness(6, 0x02);
  reboot.write(4, "5678");
  reboot.write(6, "9146431");
  delay(200);
  reboot.write(4, "4251");
  reboot.write(6, "1709752");
  delay(200);
}
