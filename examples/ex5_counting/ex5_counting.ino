#include <GhostLab42Reboot.h>
#include <Wire.h>

GhostLab42Reboot reboot;

void setup()
{
  reboot.begin();
  
  // Turn down the brightness of the main displays
  reboot.setDisplayBrightness(0, 60);
  reboot.setDisplayBrightness(1, 100);
  reboot.setDisplayBrightness(2, 60);
}

void loop()
{
  String countStr;
  int count = 2087;
  
  for (int i = 0; i < 1000; i++)
  {
    // Count down
    reboot.write(0, String(120999 - i));

    // Count up with leading 0s really fast
    countStr = "000" + String(16 * i);
    reboot.write(1, countStr.substring(countStr.length() - 4));
    
    // Show a number that is about 2087 but moves around randomly by a few counts
    // Slow the update down so it changes more slowly
    if ((i % 30) == 0)
    {
      count = 2087 + random(-2, 3);
    }
    
    reboot.write(2, String(count));

    delay(30);  
  }
}
