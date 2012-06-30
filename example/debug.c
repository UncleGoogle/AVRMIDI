
void processMidiEvent(unsigned char evt0, unsigned char evt1, unsigned char evt2)
{
  if ((evt0 & 0xF0) == MIDI_NOTE_ON)
  {
     // if velocity is 0 then fake it as a note off
     if (evt2 != 0)
     {
        start_note();
     } else {
        end_note();
     }
  }

  if ((evt0 & 0xF0) == MIDI_NOTE_OFF)
  {
     end_note();
  }
  else if ((evt0 & 0xF0) == MIDI_CONTROL_CHANGE)
  {
  }
}

// also implement all the SYSEX-related functions
const char manufacturerID = 0x55;
const char deviceID = 0x66;

// debug MIDI implementation:
// 4 sysex handlers of different length
// MIDI command parser

int main(int argc, char **argv)
{

   while (!done)
   {


      
   }

}
