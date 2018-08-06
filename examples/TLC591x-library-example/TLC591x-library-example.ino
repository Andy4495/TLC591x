/* Example sketch using TLC591x library  to interface
   with the Texas Instruments TLC5916 and TLC5917
   8-Channel Constant-Current LED Sink Drivers.
   https://gitlab.com/Andy4495/TLC591x

*/
/* Version History
   1.0.0    08/06/2018  A.T.   Original
*/
#include <TLC591x.h>

//TLC591x myLED(2, 7, 8, 9);    // Uncomment if not using OE pin
TLC591x myLED(2, 7, 8, 9, 6); // Uncomment if using OE pin

void setup() {
  myLED.displayEnable();     // This command has no effect if you aren't using OE pin
}

void loop() {
  int i;

  // Example using 2x7 segment display
  // First, scroll "Arduino" across the display
  unsigned char s[9];       // Note that this is defined as unsigned
  strncpy(s, "Arduino ", 8);
  for (i = 0; i < 7; i++) {
    myLED.print(&s[i]);
    delay(500);
  }
  delay(1000);

  // Now step through the full character map
  s[0] = 32;
  s[1] = 33;
  for (i = 0; i < 96; i++) {
    myLED.print(s);
    s[0]++;
    s[1]++;
    delay(500);
  }
  delay(1000);

  // Example using 10-segment bar graph
  // or 10 individual LEDs.
  for (i = 0; i < 1024; i++) {
    myLED.print(i);
    delay(100);
  }
}

