//  Midi.c
//
//  Midi output routines for the atmel atmega163 (and others)
//  

//#include <avr/io.h>
//#include <avr/pgmspace.h>

#include "midi.h"
#include "sysex.h"
#include "uart.h"

// midi state machine
unsigned char midiState;
// buffer for currently read midi event
unsigned char midiEvent[3];
// value 0-2 of where to write the next input byte 
unsigned char midiReadIndex;
unsigned char midiBytesToIgnore;

#define NUM_MIDI_EVENTS 18
unsigned char midiEventLength[NUM_MIDI_EVENTS][2] = {
{MIDI_NOTE_OFF,3},
{MIDI_NOTE_ON,3},
{MIDI_POLY_TOUCH,3},
{MIDI_CONTROL_CHANGE,3},
{MIDI_PROGRAM_CHANGE,2},
{MIDI_CHANNEL_TOUCH,2},
{MIDI_PITCH_BEND,3},
{MIDI_MTC,2},
{MIDI_SPP,3},
{MIDI_SONG_SEL,2},
{MIDI_TUNE_REQ,1},
{MIDI_CLOCK,1},
{MIDI_SYNC,1},
{MIDI_START,1},
{MIDI_STOP,1},
{MIDI_CONT,1},
{MIDI_SENSE,1},
{MIDI_RESET,1}};

/* simple map of midi messages
      first byte
          |
----------------------
|                    |
high nibble  low nibble bytes following (& description)
   /            /       /
  / ------------       /
 / /   ----------------
| /   /
8 ch  2 (note number, velocity=64) NOTE OFF
9 *                                NOTE ON
A *                                AFTERTOUCH
B ch  2 (cc number, value)         CC
C ch  1 (prog number)              PROG CHANGE
D ch  1 (pressure amt)             PRESSURE
E ch  2 (pitch wheel as 14-bit #)  PITCHBEND
F 0   n (bytes until F7)           SYSEX (first byte following F0 = manufacture id)
F 1   1 (timecode)                 MTC
F 2   2 (SPP)                      SONG POSITION POINTER
F 3   1 (song #)                   SONG SELECT
F 6   0                            TUNE REQUEST
F 8   0                            MIDI CLOCK (24ppqn)
F 9   0                            MIDI SYNC (10ms)
F A   0                            MIDI START
F C   0                            MIDI STOP
F B   0                            MIDI CONTINUE
F E   0                            MIDI SENSE (300ms if exists)
F F   0                            Reset
*/

void midiInit() 
{
	uartInit();
	uartSetBaudRate(MIDI_BAUD_RATE);
        midiState = MIDI_WAIT;
}

unsigned char commandLen(unsigned char cmd)
{
   unsigned char i;

   if ((cmd & 0xF0) != 0xF0)
      cmd = cmd & 0xF0;

   for (i = 0; i < NUM_MIDI_EVENTS; i++)
   {
      if (cmd == midiEventLength[i][0])
         return midiEventLength[i][1];
   }
   return 0;
}

// externally defined function to handle incoming midi events
void processMidiEvent(unsigned char, unsigned char, unsigned char);

void midiPoll(void) 
{
   unsigned char byte, tmp;

   while (uartReceiveByte(&byte))
   {
      // state machine for parsing the byte
      switch(midiState)
      {
         // we are currently stateless, waiting to start reading an event we care about.
         case MIDI_WAIT:
            if (byte == 0xF0)
            {
               // start of sysex
               // call sysex handler, which will return the state we should be in.
               midiState = handleSysex();
               break;
            }
            // store length of midi command
            tmp = commandLen(byte);
            // is the message one byte long?
            if (tmp == 1)
            {
               // send event out!
/*               if (byte == MIDI_CLOCK && midiClockFunc)
               {
                  // it's a clock event and we have a registered clock handler
                  midiClockFunc();
               } else {*/
                  processMidiEvent(byte, 0,0);
//               }
            } else if (tmp == 0) {
//                  PORTC = byte;
            } else {
               // save first byte of event, position pointer.. 
               midiEvent[0] = byte;
               midiReadIndex = 1;
               // go to state READING to read rest of event
               midiState = MIDI_READING;
            }
            break;
         case MIDI_READING:
            midiEvent[midiReadIndex++] = byte;
            if (midiReadIndex == commandLen(midiEvent[0]&0xF0))
            {
               processMidiEvent(midiEvent[0], midiEvent[1], midiEvent[2]);
               midiState = MIDI_WAIT;
            }
            break;
         case MIDI_IGNORING:
            if (byte == 0xF7)
            {
               midiState = MIDI_WAIT;
            }
            break;
      }
   }
}


