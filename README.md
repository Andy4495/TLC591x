# TLC591x Library

[![Arduino Compile Sketches](https://github.com/Andy4495/TLC591x/actions/workflows/arduino-compile-sketches.yml/badge.svg)](https://github.com/Andy4495/TLC591x/actions/workflows/arduino-compile-sketches.yml)
[![Check Markdown Links](https://github.com/Andy4495/TLC591x/actions/workflows/CheckMarkdownLinks.yml/badge.svg)](https://github.com/Andy4495/TLC591x/actions/workflows/CheckMarkdownLinks.yml)
[![Arduino Lint](https://github.com/Andy4495/TLC591x/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/Andy4495/TLC591x/actions/workflows/arduino-lint.yml)

This library is designed to interface with the Texas Instruments [TLC5916 and TLC5917 8-Channel Constant-Current LED Sink Drivers][1].

A minimum of three I/O pins are required (SDI, CLK, LE). An optional fourth pin can be defined for the output enable signal (/OE). If not used, the library assumes that the pin is tied low (and the LED output is always enabled). The /OE pin is required if you want to switch the chip to Special Mode.

This library can use either a processor's built-in hardware SPI interface (by way of the Arduino SPI library), or a software serial interface which allows just about any I/O pin to be used to control the TLC591x chip.

Up to 254 TLC591x chips can be cascaded together to control various LED configurations: LED bar graph, common anode 7-segment displays, or individual LEDs.

## Usage

*See the sketches included in the `examples` folder.*

1. Include the header file:

    ```cpp
    #include <TLC591x.h>
    ```

2. Use a constructor to create an LED object:

    - For software SPI, use one of the following:

    ```cpp
    TLC591x myLED(num_chips, SDI_pin, CLK_pin, LE_pin, OE_pin);  // OE pin controlled by library
    TLC591x myLED(num_chips, SDI_pin, CLK_pin, LE_pin);          // OE pin hard-wired low (always enabled)
    ```

   - For hardware SPI, use one of the following:

    ```cpp
    TLC591x myLED(num_chips, LE_pin, OE_pin);  // OE pin controlled by library
    TLC591x myLED(num_chips, LE_pin);          // OE pin hard-wired low (always enabled)
    ```

    The `num_chips` parameter defines how many chips are cascaded together (maximum of 254). The chips include a serial data output (SDO) pin so that several chips can be connected in a daisy-chain configuration.

    If using the /OE pin, the default state is for the display to be turned off, so the `enableDisplay()` method needs to be called to turn on the display.

3. Once you have created a TLC591x object, the following methods can be used to control the LEDs.

- Print ASCII characters to 7-segment displays:

   ```cpp
   void print(char* s)
   ```

   `s` is a char array with a size of at least `num_chips` containing ASCII characters. It is similar to a c-string but does not need to be null-terminated since its size is assumed to be the "# of chips" as defined in the constructor. Any characters beyond the number of chips defined in the constructor are ignored.

   The most significant digit is in element 0 of the array.

   The library includes a pre-defined mapping for displaying ASCII characters (letters, numbers, symbols) from 32 - 127 and assumes that the TLC591x chip is connected to the 7-segment display as follows:

   ```text
   TLC591x Pin  Display Segment
   -----------  ---------------
      OUT0            DP
      OUT1             A
      OUT2             B
      OUT3             C
      OUT4             E
      OUT5             G
      OUT6             F
      OUT7             D
   ```

   Note that a 7-segment display does not allow an accurate rendering of all ASCII characters and symbols.

- To control individual LEDs:

   ```cpp
    void print(unsigned int n)
   ```

   The value `n` is shifted out to the TLC591x chip(s) one bit at a time. If a single-chip configuration was defined in the constructor, then only the least-significant 8 bits are shifted.

   *This method only supports one or two TLC591x chips, and is included for backward compatibility with older versions of the library. If you are using more than two chips, then use `printDirect()` instead.*

- To control individual LEDs, or directly control LED segments in a 7-segment display, use:

   ```cpp
    void printDirect(const uint8_t* b)
   ```

   `b` is an array of size of at least `num_chips`. The values are shifted out right to left (i.e., `b[0]` is shifed out last).

- Control the output enable (/OE) signal. These meethods only have an effect if the OE pin was defined in the constructor.

   ```cpp
   void displayEnable();
   ```

   Sets the output enable (/OE) signal low, which turns on the display. The default state of the /OE pin when defined in the constructor is disabled.

   ```cpp
    void displayDisable();  
   ```

   Sets the output enable (/OE) signal high, which turns off the display.  

- To put the TLC591x chip into Normal Mode (see [datasheet][1] for more info):

   ```cpp
   void normalMode();
   ```

   The chip is in normal mode by default when powered on.

- To put the TLC591x chip into Special Mode (see [datasheet][1] for more info):

   ```cpp
   void specialMode();
   ```

   The chip must have the /OE signal connected to the microcontroller (and defined in the constructor) for this method to have an effect. While in Special Mode, the display is disabled. Once the chip is in special mode, the Configuration Latch can be written using the printDirect() method.

- To conntrol the display brightness:

   ```cpp
   void displayBrightness(byte b);
   ```

   Uses PWM on /OE pin to control display brightness. This method requires that the /OE pin is a PWM-capable pin and is defined in the constructor. Note that since /OE is active low, a value of zero is brightest and 255 is dimmest.

## Hardware Design

A reference design incorporating two TLC5916/7 chips and various LEDs configurations is available in the [extras/hardware][2] folder.

## Hardware and Software SPI Compatibility

The library has been tested with AVR and MSP430 processors using both hardware and software SPI.

Because it uses platform-agnostic `digitalWrite()` functions to control the hardware when using software SPI,  this library should work with any processor that is supported by the Arduino IDE when using software SPI.

There are known issues when using hardware SPI with Texas Instruments MSP432 and Tiva processors, due to incompatibilities with the SPI library included with those hardware cores. This library and the example sketches include compiler directives to disable hardware SPI support for those platforms. Future versions of this library may fix this. See [Issue #4][3].

## References

- [TLC591x datasheet][1]

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE.txt`][101] in this repository.

[1]: http://www.ti.com/lit/ds/symlink/tlc5916.pdf
[2]: ./extras/hardware
[3]: https://github.com/Andy4495/TLC591x/issues/4
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE.txt
[//]: # ([200]: https://github.com/Andy4495/TLC591x)
