#include <GhostLab42Reboot.h>
#include <Wire.h>

GhostLab42Reboot reboot;

void setup()
{
  reboot.begin();
  reboot.write(1, "126.2");
  reboot.write(2, "-42.9");
}

void loop()
{
  // String to be scrolled across the six digit display
  // Need the extra spaces to make the scrolling smooth
  String displayText = "      Who ya gonna call?     Ghostbusters!      ";

  // A NOTE ABOUT SCROLLING:
  // This works fine if there are not any periods/decimals, since a period/decimal
  // is either considered part of the previous character or its own character if
  // it is in the first position of the display (in which case there is technically
  // a "space" in front of it)
  // Check out ex4_scrollingtextadvanced instead if you are planning on scrolling a
  // string with a period/decimal

  // Commence scrolling
  for (int i = 0; i < displayText.length(); i++)
  {
    reboot.write(0, displayText.substring(i, i + 6));
    delay(250);
  }

  // Clear and start over
  reboot.resetDisplay(0);
}
