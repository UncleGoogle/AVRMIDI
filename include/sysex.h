#ifndef __SYSEX_H__
#define __SYSEX_H__

/* ---------------------------------------
   IMPLEMENT THE FOLLOWING IN YOUR CODE THAT LINKS TO LIBMIDI!
   ---------------------------------------
*/

typedef void (*sysexCallback)(unsigned char tag, unsigned char len, unsigned char *buf);

typedef struct
{
   unsigned char tag;
   unsigned char len;
   sysexCallback callback;
} sysexCallback_t;

// these all need to be defined in your code
extern const sysexCallback_t sysexFunctions[];
extern const int numSysexFunctions;
extern const unsigned char manufacturerID, deviceID;

// this is the maximum length your sysex handlers can be registered to handle
#define MAX_SYSEX_LENGTH 32


/* -- internal function definitions */
char handleSysex(void);



#endif

