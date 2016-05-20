#include <GhostLab42Reboot.h>
#include <Wire.h>

GhostLab42Reboot reboot;

void setup() 
{
  reboot.begin();

  // Set the initial display brightness
  reboot.setDisplayBrightness(0, 0);
  reboot.setDisplayBrightness(1, 0);
  reboot.setDisplayBrightness(2, 0);

  // Write values to the displays
  reboot.write(0, "8.8.8.8.8.8.");
  reboot.write(1, "8.8.8.8.");
  reboot.write(2, "8.8.8.8.");
}

void loop()
{
  // Loop over the 100 different display brightness settings
  for (int i = 0; i <= 100; i++)
  {
    // Set the new display brightness
    reboot.setDisplayBrightness(0, i);
    reboot.setDisplayBrightness(1, i);
    reboot.setDisplayBrightness(2, i);
    
    // Wait for 20ms and then use the next brightness setting
    delay(20);
  }
}

