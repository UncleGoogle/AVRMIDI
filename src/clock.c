// clock.c - libmidi by boourns (c) 2011
// released under MIT license
//
// user can register a clock function, then generate
// clock internally or externally

unsigned char clockInternal = 0;
uint32_t num_instr;
uint16_t top_num_instr;

// clock interrupt
SIGNAL(SIG_OVERFLOW3) {
  TCNT3 = timer3_init;  // FIXME, use CTC mode
  MIDIClockFunc();
}

void setInterrupt(unsigned short bpm)
{
  uint32_t num_instr;
  uint16_t top_num_instr;

  if (set_tempo > MAX_TEMPO) {
    set_tempo = MAX_TEMPO;
  }
  if (set_tempo < MIN_TEMPO) {
    set_tempo = MIN_TEMPO;
  }
 
  // figure out what the interrupt should be!
  // (use counter 3 for finest resolution!)
  num_instr = F_CPU * 60;
  num_instr /= bpm;
  num_instr /= 4;         // sixteenth notes!
  num_instr /= 2;         // call twice per quarter
  num_instr /= DINSYNC_PPQ/4;  // do dinsync on same interrupt
}

void registerClockHandler(clockfunc_t *handler)
{
   MidiClockFunc = handler;
}

void setClockGeneration(unsigned char internal, unsigned short bpm)
{
   clockInternal = internal;

   if (internal)
   {
      
   }
}

