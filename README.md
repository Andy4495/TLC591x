TLC591x Library
====================

This library is designed to interface with the Texas Instruments [TLC5916 and TLC5917 8-Channel Constant-Current LED Sink Drivers][1].

The library currently only supports Normal Mode. A future iteration of the library may also support Special Mode.

The library supports up to two TLC591x chips cascaded together. Future iterations may support additional chips.

The TLC591x chips use a serial interface to transfer data. A minimum of three I/O pins are required (SDI, CLK, LE). An optional fourth pin can be defined for the output enable signal (/OE). If not used, the library assumes that the pin is tied low (and the LED output is always enabled).

The library can control various LED configurations: LED bar graph, common anode 7-segment displays, or individual LEDs.

Usage
-----
*See the sketch included in the `examples` folder.*

Use one of the following forms of the constructor to set up the pins used to interface with the TLC591x chip:

    TLC591x myLED(1, 4, 5, 6, 7);    // OE pin controlled by library
    TLC591x myLED(1, 4, 5, 6);       // OE pin hard-wired low (always enabled)

The parameters to the constructor, in order, are: # of chips, SDI pin, CLK pin, LE pin, and /OE pin (if used). If using the /OE pin, the default state is for the display to be turned off, so the enableDisplay() method needs to be called to turn on the display.

The "# of chips" parameter defines how many chips are cascaded together. The chips include a serial data output (SDO) pin so that several chips can be connected in a daisy-chain configuration. The library currently only supports 1 or 2 chip configurations.

Once you have created a TLC591x object, the following methods can be
used to control the LEDs:

    void print(char* s)
Use this method to control 1- or 2-digit 7-segment displays.

`s` is a char array with a size of at least "# of chips" containing ASCII characters. It is similar to a c-string but does not need to be null-terminated since its size is assumed to be the "# of chips" as defined in the constructor. Any characters beyond the number of chips defined in the constructor are ignored.

The most significant digit is in element 0 of the array.

The library includes a pre-defined mapping for displaying ASCII characters (letters, numbers, symbols) from 32 - 127 and assumes that the TLC591x chip is connected to the 7-segment display as follows:

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

Note that a 7-segment display does not allow an accurate rendering of all ASCII characters and symbols.

    void print(unsigned int n)
Use this method to control individual LEDs. The value `n` is shifted out to the TLC591x chip(s) one bit at a time. If a single-chip configuration was defined in the constructor, then only the least-significant 8 bits are shifted.

    void displayEnable();
Sets the output enable (/OE) signal high, which turns off the display. This method only has an effect if the /OE pin was defined in the constructor. The default state of the /OE pin when defined in the constructor is disabled.

    void displayDisable();
Sets the output enable (/OE) signal low, which turns on the display. This method only has an effect if the /OE pin was defined in the constructor.

Hardware Design
---------------
A reference design incorporating two TLC5916/7 chips and various LEDs configurations is available in the [extras/hardware][2] folder.

References
----------
+ [TLC591x datasheet][1]

[1]: http://www.ti.com/lit/ds/symlink/tlc5916.pdf
[2]: ./extras/hardware
