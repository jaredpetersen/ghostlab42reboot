#include <GhostLab42Reboot.h>
#include <Wire.h>

GhostLab42Reboot reboot;

void setup()
{
  reboot.begin();
}

void loop()
{
  String countStr;
  
  for (int i = 0; i < 1000; i++)
  {
    // Count up with leading 0's
    countStr = "00000" + String(i);
    reboot.write(0, countStr.substring(countStr.length() - 6));

    // Count down slowly
    reboot.write(1, String(5000 - i/23));
    
    // Count up with a negative sign very slowly
    reboot.write(2, String(-300 + i/80));

    delay(40);  
  }
}
