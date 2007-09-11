/////////////////////////////////
/////////////////////////////////
//
//		Parser
//
/////////////////////////////////
/////////////////////////////////
#include <p18cxxx.h>
#include <usart.h>
#include <stdio.h>
#include "system\usb\usb.h"
#include "system\typedefs.h"
#include "user\user.h"


// to support seamless transition, we use two banks of program spaces
// and switch them.

#pragma udata big_buf1=0x100
byte program1[256];

#pragma udata big_buf2=0x200
byte program2[256];

#pragma udata

// used to store data to be sent over to PC
byte output_buffer[32];

// indicate that the newly read program is fully ready to execute.
// Set to null when the executor picks it up.
//
// when this is non-NULL, it means executor is executing one bank,
// and the reader filled up the other bank, so we won't read any
// further data from PC.
volatile byte* programReady = NULL;










// Receive data from USB, and place that in the buffer.
void ReadFromUSB() {
	// expected # of bytes to read. 0 if we are looking for preamble
	static byte remainingLength = 0;
	// count up the preamble '0xFF'. We need 4 of them.
	static byte preambleCount = 0;
	// where to store the next byte received from PC?
	static byte* readPtr = program1;
	// which program bank are we reading now?
	static byte currentBank = 0;
	byte ch;
	byte length;

	if(programReady!=NULL)
		return;	// waiting for the executor to pick up the new program. can't read ahead.

	if(remainingLength==0) {
		// in a preamble mode
		if(getsUSBUSART(&ch,1)==0)
			return;	// no data
	
		if(preambleCount==4) {
			// found the length of the program
			remainingLength = ch;
			preambleCount=0;
			return;
		}

		if(ch==0xFF)
			preambleCount++;
		else {
			// junk
			if (mUSBUSARTIsTxTrfReady()) {
				sprintf(output_buffer,"? %d %d\r\n",preambleCount,ch);
				putsUSBUSART(output_buffer);
			}

			preambleCount=0;
		}
		return;
	}

	// reading program
	length = getsUSBUSART(readPtr, remainingLength);
	remainingLength -= length;
	readPtr += length;

	if(remainingLength>0)
		return;	// still expecting more program data

	// completed!
	if(currentBank==0) {
		programReady = program1;
		currentBank = 1;
		readPtr = program2;
	} else {
		programReady = program2;
		currentBank = 0;
		readPtr = program1;
	}

	if(mUSBUSARTIsTxTrfReady())
		putrsUSBUSART("OK\r\n");
}
