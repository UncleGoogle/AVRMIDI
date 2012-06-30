//yeah
#include <stdio.h>
#include "midi.h"

void processMidiEvent(unsigned char evt0, unsigned char evt1, unsigned char evt2)
{
  if ((evt0 & 0xF0) == MIDI_NOTE_ON)
  {
     // if velocity is 0 then fake it as a note off
     if (evt2 != 0)
     {
        synth_start_note(evt1);
     } else {
        synth_stop_note();
     }
  }

  if ((evt0 & 0xF0) == MIDI_NOTE_OFF)
  {
     synth_stop_note();
  } 
  else if ((evt0 & 0xF0) == MIDI_CONTROL_CHANGE)
  {
    printf("CC RECEIVED");
  }
}

// also implement all the SYSEX-related functions
const char manufacturerID = 0x55;
const char deviceID = 0x66;


