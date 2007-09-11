/////////////////////////////////
/////////////////////////////////
//
//		Executor
//
/////////////////////////////////
/////////////////////////////////
#include <p18cxxx.h>
#include <usart.h>
#include <stdio.h>
#include "system\typedefs.h"
#include "system\usb\usb.h"
#include "user\user.h"



// instruction that we are executing right now.
// NULL means we are at halt, and nothing is executed.
//#ifdef DEBUG
byte program[] = {0x00,0x0F,0x00,0x05, 0x00,0xF0,0x00,0x05, 0x0F,0x00,0x00,0x05, 0x10,0x0C,0x00,0x60, 0x30}; // 0x0F,0xFF,0x00,0x01, 0x10,0x08,0x00,0x60, 0x30};
volatile Op* ip = (Op*)program;
//#else
//volatile Op* ip = NULL;
//#endif

Color current; // current color of the orb. 128 scale

word timeElapsed=0;// how much time has elapsed since we started the current C command?


/*
	Invoked periodically by a timer interrupt handler to do the work
*/
void DoTimer() {
	if(timeElapsed==0) {
		// we'll switch to a new program now
		if (programReady!=NULL) {
			if(mUSBUSARTIsTxTrfReady()) {
				putrsUSBUSART("Switching\r\n");
			}
			ip = (Op*)programReady;
			programReady = NULL;	// this will tell the parser to start accepting another program
			return;
		}
	}

	if(ip!=NULL) {
		if(ip->opcode<0 || ip->opcode>4) {
			Send("illegal instruction\r\n");
			ip = NULL;
		} else {
			instructionTable[ip->opcode]();
		}
	}
}


void moveIP(int offset) {
	ip = (Op*)(((byte*)ip)+offset);
}

void ExecuteC() {
	static Color start;	// the color of the orb when the current 'C' command started
	static Color target;	// what's the target color when this 'C' command is over?
	char i;

	if(timeElapsed > ip->c.time) {
		// transition complete. move on.
		timeElapsed = 0;
		moveIP(sizeof(COp));
		return;
	}

	if(timeElapsed==0) {
		// start of the transition. compute various values.
		start = current;
		target.r = ip->c.r*8;
		target.g = ip->c.g*8;
		target.b = ip->c.b*8;
	} else {
		// in transition. update color
		for(i=0;i<3;i++) {
			current.rgb[i] = (char)start.rgb[i] +
				(char)((short long)(char)(target.rgb[i]-start.rgb[i])*timeElapsed/ip->c.time);
		}
	}
	timeElapsed++;
}

void ExecuteH() {
	Send("halted\r\n");
	ip = NULL;
}

int offset(int h,byte low) {
	return (h<<8)|low;
}

// unconditional jump
void ExecuteJ() {
	moveIP(-offset(ip->j.offsetH, ip->j.offsetL));
}

void ExecuteB() {
	char l;

	if( ++(ip->b.counter) >= ip->b.total) {
		// hit the repeat count. move on to the next instruction
		ip->b.counter=0;
		moveIP(sizeof(BOp));
		return;
	} else {
		// loop
		moveIP(-offset(ip->b.offsetH, ip->b.offsetL));
	}
}