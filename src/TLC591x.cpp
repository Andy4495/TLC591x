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
   1.3.0    08/31/2022  Andy4495  Add hardware SPI support
   1.4.0    09/02/2022  Andy4495  Fix hardware SPI support for msp432r and tivac
                                  Fix potential race condition between pwm brightness and special mode
   1.4.2    09/13/2022  Andy4495  Fix compilation issue with ESP32 (#6)
*/

#include "TLC591x.h"

// Software SPI constructors
TLC591x::TLC591x(byte n, byte SDI, byte CLK, byte LE, byte OE) {
  SDI_pin = SDI;
  CLK_pin = CLK;
  LE_pin  = LE;
  OE_pin  = OE;
  numchips = n;

  digitalWrite(OE_pin, HIGH);  // Default is disabled
  enableState = TLC_DISABLED;
  pwmMode = TLC_DISABLED;
  pinMode(OE_pin, OUTPUT);
  spiType = SW_SPI;
  init();
}

TLC591x::TLC591x(byte n, byte SDI, byte CLK, byte LE) {
  SDI_pin = SDI;
  CLK_pin = CLK;
  LE_pin  = LE;
  OE_pin  = NO_PIN;
  numchips = n;

  spiType = SW_SPI;
  init();
}

// Hardware SPI constructors
TLC591x::TLC591x(byte n, byte LE, byte OE) {
  LE_pin  = LE;
  OE_pin  = OE;
  CLK_pin = SCK;
  SDI_pin = NO_PIN;
  numchips = n;
  digitalWrite(OE_pin, HIGH);  // Default is disabled
  enableState = TLC_DISABLED;
  pwmMode = TLC_DISABLED;   // Used to indicate if the the displayBrightness() method was used
  pinMode(OE_pin, OUTPUT);  
  spiType = HW_SPI;
  init();
}

TLC591x::TLC591x(byte n, byte LE) {
  LE_pin  = LE;
  OE_pin  = NO_PIN;
  CLK_pin = SCK;
  SDI_pin = NO_PIN;
  numchips = n;
  spiType = HW_SPI;
  init();
}

void TLC591x::init() {
  if (numchips < MINCHIPS) numchips = MINCHIPS;
  if (numchips > MAXCHIPS) numchips = MAXCHIPS;

  if (spiType == SW_SPI) {
    pinMode(SDI_pin, OUTPUT);
    pinMode(CLK_pin, OUTPUT);
  }
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
    enableState = TLC_ENABLED;
    pwmMode = TLC_DISABLED;
  }
}

void TLC591x::displayDisable() {
  if (OE_pin != NO_PIN) {
    digitalWrite(OE_pin, HIGH);
    enableState = TLC_DISABLED;
    pwmMode = TLC_DISABLED;
  }
}

void TLC591x::normalMode() {
  if (OE_pin != NO_PIN) {
    if (spiType == HW_SPI) {
      SPI.end();
      digitalWrite(CLK_pin, LOW);
      pinMode(CLK_pin, OUTPUT);
    }
    pwmMode = TLC_DISABLED;
    digitalWrite(OE_pin, HIGH);
    toggleCLK();
    digitalWrite(OE_pin, LOW);
    toggleCLK();
    digitalWrite(OE_pin, HIGH); // Disables the display
    toggleCLK();
    toggleCLK();   // Mode switching
    toggleCLK();   // Now in normal mode
    if (enableState == TLC_ENABLED) displayEnable(); // Re-enable display if it was enabled previously
    if (spiType == HW_SPI) {
      SPI.begin();
    }
  }
}

void TLC591x::specialMode() {
  if (OE_pin != NO_PIN) {
    if (spiType == HW_SPI) {
      SPI.end();
      digitalWrite(CLK_pin, LOW);
      pinMode(CLK_pin, OUTPUT);
    }
    pwmMode = TLC_DISABLED;
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
    // Switching to special mode disables the display
    // normalMode() automatically re-enables display if it was previously enabled before specialMode()
    if (spiType == HW_SPI) {
      SPI.begin();
    }
  }
}

void TLC591x::displayBrightness(byte b) {
  if (OE_pin != NO_PIN) {
    analogWrite(OE_pin, b);
    pwmMode = TLC_ENABLED;
    enableState = TLC_ENABLED;
    brightness = b;
  }
}

void TLC591x::write(byte n) {
  if (OE_pin != NO_PIN && pwmMode == TLC_ENABLED) digitalWrite(OE_pin, TLC_ENABLED);  // Need a continuous level on OE when writing
  if (spiType == SW_SPI) {
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
#if !defined(ENERGIA_ARCH_MSP432R) && !defined(ENERGIA_ARCH_MSP432E) && !defined(ENERGIA_ARCH_MSP432)
  else {
    SPI.beginTransaction(SPISettings(10000000, LSBFIRST, SPI_MODE0));
    SPI.transfer(n);
    SPI.endTransaction();
  }
#else  // Special code for MSP432 since it has an older version of SPI library
  else {
    SPI.setBitOrder(LSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV4);
    SPI.setDataMode(SPI_MODE0);
    SPI.transfer(n);
  }
#endif
  if (OE_pin != NO_PIN && pwmMode == TLC_ENABLED ) displayBrightness(brightness);   // Switch back to previous setting
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
