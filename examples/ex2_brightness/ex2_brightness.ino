#include <GhostLab42Reboot.h>
#include <Wire.h>

GhostLab42Reboot reboot;

void setup() 
{
  reboot.begin();
}

void loop()
{
  // Loop over the 100 different display brightness settings
  for (int i = 0; i <= 100; i++)
  {
    // Set the display brightness
    reboot.setDisplayBrightness(0, i);
    reboot.setDisplayBrightness(1, i);
    reboot.setDisplayBrightness(2, i);
    
    
    // Write values to the displays
    reboot.write(0, "8.8.8.8.8.8.");
    reboot.write(1, "8.8.8.8.");
    reboot.write(2, "8.8.8.8.");
    
    // Wait for 20ms then use the next brightness setting
    delay(20);
  }
}

