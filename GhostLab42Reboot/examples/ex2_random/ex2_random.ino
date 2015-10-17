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
  reboot.writeRandom(4);
  reboot.writeRandom(6);
  delay(200);
}

