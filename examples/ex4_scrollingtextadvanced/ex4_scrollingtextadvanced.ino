#include <GhostLab42Reboot.h>
#include <Wire.h>

GhostLab42Reboot reboot;

void setup()
{
  reboot.begin();
  reboot.write(1, "126.2");
  reboot.write(2, "-46.9");
}

void loop()
{
  // String to be scrolled across the six digit display
  // Need the extra spaces to make the scrolling smooth
  String displayText = "       . Test 1.2.3.4.  ...      ";

  // A NOTE ABOUT SCROLLING:
  // This more complicated method of scrolling handles periods/decimals, which are
  // more complicated than other characters. A period/decimal is either considered 
  // part of the previous character or its own character if it is in the first
  // position of the display (in which case there is technically a "space" in
  // front of it).
  // Check out ex3_scrollingtext instead if you are not planning on scrolling a
  // string with a period/decimal

  // Commence scrolling
  for (int i = 0; i < displayText.length(); i++)
  {
    // Offset helps manage the number of characters we're sending to the display
    // offset = number of digits on the display + 1
    int offset = 7;

    // Loop over the first six characters of this iteration
    for (int j = 0; j < offset; j++)
    {
      // If the string has a period, don't count it as a separate character
      if (displayText.charAt(i+j) == '.')
      {
        offset += 1;
      }
    }

    // If the first character in the string is a period/decimal and the
    // previous character was not a period/decimal, the previous character
    // was cut off during the scrolling
    // Just let the period/decimal disappear since we do not want it to
    // scroll off the display like a regular character.
    if (displayText.charAt(i) == '.' && displayText.charAt(i - 1) != '.')
    {
      // Write the string to the display with the extra offset
      reboot.write(0, displayText.substring(i + 1, i + offset + 1));
    }
    else
    {
      // Write the string to the display with the normal offset
      reboot.write(0, displayText.substring(i, i + offset));

      // Essentially the refresh rate of the display
      // No need to use the delay in the previous case
      delay(250);
    }

  }

  // Clear the display and start over
  reboot.resetDisplay(0);
}
