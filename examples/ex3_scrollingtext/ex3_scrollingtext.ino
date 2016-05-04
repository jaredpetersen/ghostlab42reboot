#include <GhostLab42Reboot.h>
#include <Wire.h>

GhostLab42Reboot reboot;

void setup()
{
  delay(500);
  reboot.begin();
  reboot.write(4, "-42.9");
}

void loop()
{
  // String to be scrolled across the display
  // Needs the extra spaces to make the scrolling smooth
  String displayText = "      Who ya gonna call?     Ghostbusters!";

  // Commence scrolling
  for (int i = 0; i < displayText.length(); i++)
  {
    reboot.write(6, displayText.substring(i, i+6));
    delay(250);
  }

  // Clear and start over
  reboot.resetDisplay(6);
  delay(400);
}
