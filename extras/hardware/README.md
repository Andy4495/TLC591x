TLC591x Interface Board
=======================

This is a reference design (schematic and PCB) that can be used with the TLC591x library.

It uses two [TLC5916/7 driver chips][2] and can support a 10-segment bar graph, 16 individual LEDs, or a 2-digit 7-segment common anode LED display. Any 7-segment display that has a pinout compatible with the HP/Avago/Broadcom [HDSP-K511][1]should work with this design.

The TLC591x chip has an internal pull-down for the LE signal and an internal pull-up for the /OE signal, so neither signal is left floating at powerup.

The circuit has a jumper for the /OE signal so that it can be hard-wired to Vcc or the signal can be under software control from the microcontroller.

The SDO signal is available on the signal header to allow daisy-chaining additional modules.

The design allows for separate VLED and Vcc supplies, or they can be tied together with jumpers for single supply operation.

There are multiple positions to place resistors for R-EXT to control the display current -- this is to allow more easily changing external resistor values by placing a new resistor either in parallel with an existing resistor or clipping the old resistor without having to unsolder parts.

References
----------
+ TLC5916/7 [datasheet][2].
+ HDSP-K511 7-segment display [datasheet][1].

[1]: https://docs.broadcom.com/docs/AV02-2555EN
[2]: http://www.ti.com/lit/ds/symlink/tlc5916.pdf
[100]: https://choosealicense.com/licenses/mit/
[101]: ../../LICENSE.txt
[200]: https://github.com/Andy4495/TLC591x