#include "uart.h"
#include "midi.h"
#include "sysex.h"

unsigned char sysexBuffer[MAX_SYSEX_LENGTH];

char getSysexIndex(unsigned char id)
{
   char i;
   for (i = 0; i < numSysexFunctions; i++)
   {
      if (sysexFunctions[i].tag == id)
      {
         return i;
      }
   }
   return -1;
}

// reads until we get to the end of a sysex message
void finishSysex(void)
{
   unsigned char tmp = 0;
   while (tmp != MIDI_SYSEX_END)
   {
      while (!uartReceiveByte(&tmp));
   }
}

char handleSysex(void)
{
   unsigned char tmp, i, j;
   // if its not for us, read off the rest of the sysex message
   while (!uartReceiveByte(&tmp));

   if (tmp != manufacturerID)
   {
      finishSysex();
   }

   while (!uartReceiveByte(&tmp));
   
   if (tmp != deviceID)
   {
      finishSysex();
   }

   // we're in a loop here until the sysex ends
   char done = 0;
   while (!done)
   {
      while (!uartReceiveByte(&tmp));

      i = getSysexIndex(tmp);
      if (i == -1)
      {
         finishSysex();
         return MIDI_WAIT;
      }

      for (j = 0; j < sysexFunctions[i].len; j++)
      {
         while (!uartReceiveByte(&sysexBuffer[j]));

         if (sysexBuffer[j] == MIDI_SYSEX_END)
         {
            return MIDI_WAIT;
         }
      }

      sysexFunctions[i].callback(tmp, sysexFunctions[i].len, &sysexBuffer[0]);
   }  

   return MIDI_WAIT;
}

