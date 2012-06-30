AVRMIDI
=======

C library for quickly creating AVR MIDI devices

Prerequisites:
* avr-gcc
* AVRLIB (available http://gitorious.org/avrlib)

The idea behind AVRMIDI is to encapsulate the configuration and byte processing necessary to be a MIDI device.

Users of AVRMIDI only need to write functions to process complete MIDI events.

The processMidiEvent function is mandatory and will be sent all 2- and 3- byte MIDI messages (NOTEON/OFF, CC, etc)

Note that at this point the library is incomplete.

What works:
- receiving MIDI events in your custom processMidiEvent function (see examples/)

What should work:
- receiving sysex snippets by registering callbacks

What doesn't work:
- clock infrastructure for libmidi to act as a MIDI sequencer master


