TLC591x Library
====================

This library is designed to interface with the Texas Instruments [TLC5916 and TLC5917 8-Channel Constant-Current LED Sink Drivers][1].

The library supports up to 254 TLC591x chips cascaded together.

The TLC591x chips use a serial interface to transfer data. A minimum of three I/O pins are required (SDI, CLK, LE). An optional fourth pin can be defined for the output enable signal (/OE). If not used, the library assumes that the pin is tied low (and the LED output is always enabled). The /OE pin is required if you want to switch the chip to Special Mode. 

The library can control various LED configurations: LED bar graph, common anode 7-segment displays, or individual LEDs.

Usage
-----
*See the sketch included in the `examples` folder.*

Use one of the following forms of the constructor to set up the pins used to interface with the TLC591x chip:

    TLC591x myLED(1, 4, 5, 6, 7);    // OE pin controlled by library
    TLC591x myLED(1, 4, 5, 6);       // OE pin hard-wired low (always enabled)

The parameters to the constructor, in order, are: # of chips, SDI pin, CLK pin, LE pin, and /OE pin (if used). If using the /OE pin, the default state is for the display to be turned off, so the enableDisplay() method needs to be called to turn on the display.

The "# of chips" parameter defines how many chips are cascaded together. The chips include a serial data output (SDO) pin so that several chips can be connected in a daisy-chain configuration.

Once you have created a TLC591x object, the following methods can be
used to control the LEDs:

    void print(char* s)
Use this method to control 7-segment displays.

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
*This method only supports one or two TLC591x chips, and is included for backward compatibility with older versions of the library. If you are using more than two chips, then use `printDirect()` instead.*

    void printDirect(const uint8_t* b)
Use this method to control individual LEDs or LED segments. `b` is an array of size of at least "# of chips". The values are shifted out right to left (i.e., `b[0]` is shifed out last).

    void displayEnable();
Sets the output enable (/OE) signal high, which turns off the display. This method only has an effect if the /OE pin was defined in the constructor. The default state of the /OE pin when defined in the constructor is disabled.

    void displayDisable();  
Sets the output enable (/OE) signal low, which turns on the display. This method only has an effect if the /OE pin was defined in the constructor.  

    void normalMode();  
Switches the chip to Normal Mode (see [datasheet][1] for more info). The chip is in normal mode by default when powered up.

    void specialMode();  
Switches the chip to Special Mode (see [datasheet][1] for more info). The chip must have the /OE signal connected to the microcontroller (and defined in the constructor) for this method to have an effect. Note that switching to Special Mode disables the display. Once the chip is in special mode, the Configuration Latch can be written using the printDirect() method.

    void displayBrightness(byte b);   
Use PWM on /OE pin to control display brightness. This method requires that the /OE pin is a PWM-capable pin and is defined in the constructor. Note that since /OE is active low, a value of zero is brightest and 255 is dimmest. 

Hardware Design
---------------
A reference design incorporating two TLC5916/7 chips and various LEDs configurations is available in the [extras/hardware][2] folder.

References
----------
+ [TLC591x datasheet][1]

License
-------
The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE.txt`][101] in this repository.


[1]: http://www.ti.com/lit/ds/symlink/tlc5916.pdf
[2]: ./extras/hardware
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE.txt
