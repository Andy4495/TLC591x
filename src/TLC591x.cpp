/* Library to interface with the Texas Instruments TLC5916 and TLC5917
   8-Channel Constant-Current LED Sink Drivers.
   https://github.com/Andy4495/TLC591x

*/
/* Version History
   1.0.0    08/03/2018  A.T.   Original
   1.1.0    09/25/2020  A.T.   Support more daisy-chained displays.
                               Fixed array bound issue in previous
                               un-numbered file version.
   1.2.0    01/17/2021  A.T.   Add support for special mode.
*/

#include "TLC591x.h"

TLC591x::TLC591x(byte n, byte SDI, byte CLK, byte LE, byte OE) {
  SDI_pin = SDI;
  CLK_pin = CLK;
  LE_pin  = LE;
  OE_pin  = OE;
  numchips = n;

  digitalWrite(OE_pin, HIGH);
  enableState = DISABLED;
  pinMode(OE_pin, OUTPUT);
  init();
}

TLC591x::TLC591x(byte n, byte SDI, byte CLK, byte LE) {
  SDI_pin = SDI;
  CLK_pin = CLK;
  LE_pin  = LE;
  OE_pin  = NO_PIN;
  numchips = n;

  init();
}

void TLC591x::init() {
  if (numchips < MINCHIPS) numchips = MINCHIPS;
  if (numchips > MAXCHIPS) numchips = MAXCHIPS;

  pinMode(SDI_pin, OUTPUT);
  pinMode(CLK_pin, OUTPUT);
  digitalWrite(LE_pin, LOW);
  pinMode(LE_pin, OUTPUT);
}

void TLC591x::print(const char* s) {
  int i;
  char c;
  byte pos;

  for (i = numchips - 1; i >= 0; i--) {
    // Need a range check and adjustment, since segment map starts
    // at ASCII 32 (element 0) and ends at ASCII 0x7f (element 96)
    // Out of range default to a blank character
    // if (s[i] > 127) pos = 0; --> This would be a negative value, included in next check
    if (s[i] < 32) pos = 0;
    else pos = s[i] - 32;
    c = ICM7218_segment_map[pos] ;
    write(c);
  }
  toggleLE();
}

void TLC591x::print(unsigned int n){
  byte size;
  if (numchips > 2) size = 2;
  else size = numchips;
  for (byte i = 0; i < size; i++) {
    write(n);
    n = n/256; // Get the next most significant byte
  }
  toggleLE();
}

void TLC591x::printDirect(const uint8_t* s) {
  for (int i = numchips - 1; i >= 0; i--) {
    write(s[i]);
  }
  toggleLE();
}

void TLC591x::displayEnable() {
  if (OE_pin != NO_PIN) {
    digitalWrite(OE_pin, LOW);
    enableState = ENABLED;
  }
}

void TLC591x::displayDisable() {
  if (OE_pin != NO_PIN) {
    digitalWrite(OE_pin, HIGH);
    enableState = DISABLED;
  }
}

void TLC591x::normalMode() {
  if (OE_pin != NO_PIN) {
    digitalWrite(OE_pin, HIGH);
    toggleCLK();
    digitalWrite(OE_pin, LOW);
    toggleCLK();
    digitalWrite(OE_pin, HIGH); // Disables the display
    toggleCLK();
    toggleCLK();   // Mode switching
    toggleCLK();   // Now in normal mode
    if (enableState == ENABLED) displayEnable(); // Re-enable display if it was enabled previously
  }
}

void TLC591x::specialMode() {
  if (OE_pin != NO_PIN) {
    digitalWrite(OE_pin, HIGH);
    toggleCLK();
    digitalWrite(OE_pin, LOW);
    toggleCLK();
    digitalWrite(OE_pin, HIGH); // Disables the display
    toggleCLK();
    digitalWrite(LE_pin, HIGH);
    toggleCLK();   // Mode switching
    digitalWrite(LE_pin, LOW);
    toggleCLK();   // Now in special mode
    // Switching to special mode disables the display by default
    enableState = DISABLED;
  }
}

void TLC591x::write(byte n) {
  digitalWrite(SDI_pin, n & 0x01);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x02);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x04);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x08);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x10);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x20);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x40);
  toggleCLK();
  digitalWrite(SDI_pin, n & 0x80);
  toggleCLK();
}

void TLC591x::toggleLE() {
  // TLC5916 minimum LE pulse time is 20 ns, so don't need to worry about
  // putting in a hard-coded delay.
  digitalWrite(LE_pin, HIGH);
  digitalWrite(LE_pin, LOW);
}

void TLC591x::toggleCLK() {
  // TLC5916 minimum CLK pulse time is 20 ns, so don't need to worry about
  // putting in a hard-coded delay.
  digitalWrite(CLK_pin, HIGH);
  digitalWrite(CLK_pin, LOW);
}
