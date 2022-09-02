/* Example sketch using TLC591x library  to interface
   with the Texas Instruments TLC5916 and TLC5917
   8-Channel Constant-Current LED Sink Drivers.

   This example uses the built-in Hardware SPI

   https://github.com/Andy4495/TLC591x

*/
/* Version History
   1.0.0    08/31/2022  Andy4495   Original
*/
#include <TLC591x.h>
#if defined(ENERGIA_ARCH_MSP432R)
#include <stdio.h>
#endif

#if !defined(ENERGIA_ARCH_TIVAC) && !defined(ENERGIA_ARCH_MSP432R)
// First parameter in constructor is # of TLC591x chips -- this example assumes 2 chips
// Constructor parameters for HW SPI: # of chips, LE pin, OE pin (optional parameter)
// For Hardware SPI on UNO, connect SDI to UNO pin 11 and CLK to UNO pin 13
//TLC591x myLED(2, 7);    // Uncomment if not using OE pin
TLC591x myLED(2, 5, 9); // Uncomment if using OE pin
#else
// This is here so that automated compilation tests don't fail.
#warning This library does not support hardware SPI on tivac and msp432r platforms
#warning Using software SPI instead
TLC591x myLED(2, 5, 6, 7, 8);
#endif

void setup() {
  myLED.displayEnable();     // This command has no effect if you aren't using OE pin
#if !defined(ENERGIA_ARCH_TIVAC) && !defined(ENERGIA_ARCH_MSP432R)
  SPI.begin();
#endif
}

void loop() {
  int i;
  char s[9];
  uint8_t n[2];

  // Test displayBrightness()    -- This only does anything if /OE pin is connected and is PWM-capable and defined in contructor
  // Display gets dimmer as number increases. 0 is brightest, 255 is dimmest.
  for (i = 0; i <= 255; i = i + 16) {
    sprintf(s, "%2x", i);
    myLED.print(s);
    myLED.displayBrightness(i);
    delay(1000);
  }
  delay(1000);
  myLED.displayBrightness(0); // back to max brightness

  // Test specialMode()  -- /OE pin must be connected and defined in constructor above
  // See datasheet for details. Note that msb is always 1 when setting brightness level in special mode.
  myLED.print("A0");
  myLED.specialMode();
  n[0] = 0xA0;
  n[1] = 0xA0;
  myLED.printDirect(n);
  myLED.normalMode();
  delay(2000);

  myLED.print("C0");
  myLED.specialMode();
  n[0] = 0xc0;
  n[1] = 0xc0;
  myLED.printDirect(n);
  myLED.normalMode();
  delay(2000);

  myLED.print("E0");
  myLED.specialMode();
  n[0] = 0xe0;
  n[1] = 0xe0;
  myLED.printDirect(n);
  myLED.normalMode();
  delay(2000);

  myLED.print("FF");
  myLED.specialMode();
  n[0] = 0xff;          // max brightness
  n[1] = 0xff;
  myLED.printDirect(n);
  myLED.normalMode();
  delay(2000);


  // Example using 2x7 segment display
  // First, scroll "Arduino" across the display
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
