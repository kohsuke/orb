/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        user.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the ìCompanyÅE for its PICmicroÆ Microcontroller is intended and
 * supplied to you, the Companyís customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN ìAS ISÅECONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Rawin Rojvanit       11/19/04    Original.
 ********************************************************************/

#ifndef USER_H
#define USER_H

// define this symbol for running inside simulator.
//		- interrupt vectors are remapped to work without bootloader
//		- the lack of USB simulator is compensated by literals
//#define DEBUG

#define LENGTH(x)		sizeof(x)/sizeof(x[0])



// represents RGB color trio.
typedef union _Color {
	byte rgb[3]; // each byte ranges from [0,COLOR_MAX]
	struct {
		byte r;
		byte g;
		byte b;
	};
} Color;

enum { COLOR_MAX = 15*8 };

// current color of the orb.
extern Color current;

// instruction code
typedef enum _Inst {
	CINST=0,	// color instruction
	BINST=1,	// branch instruction
	JINST=2,	// unconditional jump instruction
	HINST=3		// halt instruction
} Inst;

// 'C' command
// note that higher bits come later
typedef struct _COp {
	unsigned	r:4;
	unsigned	opcode:4;	// always 0

	unsigned	b:4;	// target RGB value
	unsigned	g:4;

	word		time;	// time it takes to get there
} COp;

// 'B' command
typedef struct _BOp {
	signed		offsetH:4;	// jump offset
	unsigned	opcode:4;	// always 1
	byte		offsetL;	// jump offset
	byte		counter;	// space for counting up
	byte		total;		// loop count total
} BOp;

// 'J' command
typedef struct _JOp {
	signed		offsetH:4;	// jump offset
	unsigned	opcode:4;	// always 2
	byte		offsetL;	// jump offset
} JOp;

// the common header structure of the instruction
typedef union {
	struct {
		unsigned	undefined:4;// depends on the opcode
		unsigned	opcode:4;	// opcode
	};
	COp c;
	BOp b;
	JOp j;
} Op;




// function to run the command
typedef void (*Executor)(void);

// execution code for each instruction
extern rom Executor instructionTable[];



/** P U B L I C  P R O T O T Y P E S *****************************************/
void UserInit(void);
void ProcessIO(void);
void LowISR(void);
void HighISR(void);

// parse functions
/*****************************************************************************/
extern volatile byte* programReady;// if non-NULL, indicate that another program is ready to run
extern byte output_buffer[];	// stores text to be sent to USB
void Send(const rom char* msg);	// send text


// executor functions
/*****************************************************************************/
void ExecuteC();
void ExecuteB();
void ExecuteJ();
void ExecuteH();

void DoTimer(void);	// called by the timer thread

#endif //USER_H
