/*
	DIY Orb main control code
 */


/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include <usart.h>
#include <stdio.h>
#include "system\typedefs.h"
#include "system\usb\usb.h"

#include "io_cfg.h"             // I/O pin mapping
#include "user\user.h"

/** V A R I A B L E S ********************************************************/


#define kPR2_RELOAD				250		// For 1ms TMR2 tick

#define bitset(var,bitno) ((var) |= (1 << (bitno)))
#define bitclr(var,bitno) ((var) &= ~(1 << (bitno)))
#define bittst(var,bitno) (var& (1 << (bitno)))

#pragma udata
// Our error byte
char g_error;


/*
bit n
   |<-------------------------------------->|  totalTime
   |<--->|  onTime[n]
   
   +-----+                                  +-----+
   |     |                                  |     |
---+     +----------------------------------+     +----

bit m
   |<-------->|  onTime[m]

   +----------+                             +----------
   |          |                             |     
---+          +-----------------------------+     
*/


/** P R I V A T E  P R O T O T Y P E S ***************************************/
BOOL ReadFromUSB(void);
void BlinkUSBStatus(void);
BOOL SwitchIsPressed(void);
void ParseProgram(void);


/** D E C L A R A T I O N S **************************************************/
#pragma code

#pragma interruptlow LowISR
void LowISR(void)
{
	// Do we have a Timer2 interrupt? (1ms rate)
	if (PIR1bits.TMR2IF) {
		// Clear the interrupt 
		PIR1bits.TMR2IF = 0;

		DoTimer();
	}
}


#pragma interrupt HighISR
void HighISR(void)
{
}

void UserInit(void)
{
	char i, j;

	current.r = current.g = current.b = 0;

	// Make all of PORTA digital inputs
	LATA = 0x00;
	TRISA = 0xFF;
	// Turn all analog inputs into digital inputs
	ADCON1 = 0x0F;
	// Turn off the ADC
	ADCON0bits.ADON = 0;
	CMCON = 0x07;	// Comparators as digital inputs


	// Make all of PORTB outputs
	LATB = 0xFF;
	TRISB = 0x00;

/*	// Make all of PORTC inputs
	LATC = 0x00;
	TRISC = 0xFF;
*/
	// Initalize LED I/Os to outputs
    mInitAllLEDs();
	// Initalize switch as an input
    mInitSwitch();

    
    // Now init our registers
	// The prescaler will be at 16
    T2CONbits.T2CKPS1 = 1;
    T2CONbits.T2CKPS0 = 1;
    // We want the TMR2 post scaler to be a 3
    T2CONbits.T2OUTPS3 = 0;
    T2CONbits.T2OUTPS2 = 0;
    T2CONbits.T2OUTPS1 = 1;
    T2CONbits.T2OUTPS0 = 0;
	// Set our reload value
	PR2 = kPR2_RELOAD;

    // Enable interrupt priorities
    RCONbits.IPEN = 1;
    
    PIE1bits.TMR2IE = 1;
    IPR1bits.TMR2IP = 0;
    
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;

	// Turn on the timer.
	T2CONbits.TMR2ON = 1;
}//end UserInit

/******************************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        In this function, we check for a new packet that just
 * 					arrived via USB. We do a few checks on the packet to see
 *					if it is worthy of us trying to interpret it. If it is,
 *					we go and call the proper function based upon the first
 *					character of the packet.
 *					NOTE: We need to see everything in one packet (i.e. we
 *					won't treat the USB data as a stream and try to find our
 *					start and end of packets within the stream. We just look 
 *					at the first character of each packet for a command and
 * 					check that there's a CR as the last character of the
 *					packet.
 *
 * Note:            None
 *****************************************************************************/
void ProcessIO(void) {   
	unsigned char length;
	static BOOL sentenceReady = FALSE;

	BlinkUSBStatus();

#ifndef	DEBUG	// simulator cannot simulte USB, so skip off

    // User Application USB tasks
    if( usb_device_state < CONFIGURED_STATE || UCONbits.SUSPND==1)
		return;

	// Pull in some new data if there is new data to pull in
	ReadFromUSB();
#endif
}








void Send(const rom char* msg) {
	if (mUSBUSARTIsTxTrfReady())
		putrsUSBUSART(msg);
}





/******************************************************************************
 * Function:        void BlinkUSBStatus(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        BlinkUSBStatus turns on and off LEDs corresponding to
 *                  the USB device state.
 *
 * Note:            mLED macros can be found in io_cfg.h
 *                  usb_device_state is declared in usbmmap.c and is modified
 *                  in usbdrv.c, usbctrltrf.c, and usb9.c
 *****************************************************************************/
void BlinkUSBStatus(void)
{
    static byte pwmCycle = 0;
	int i; byte bits;
	
	bits = LATB;
    
    if (COLOR_MAX <= pwmCycle) {
		pwmCycle = 0;
		bits = 0; // back to all on
	}

	// make sure current.rgb[i]==0 would completely keep the bit 1.
	for( i=0; i<3; i++ ) {
		if( current.rgb[i] <= pwmCycle ) {
			bitset(bits,i*2);
		}
	}

	LATB = bits;

    pwmCycle++;
}

BOOL SwitchIsPressed(void)
{
	return UserSW == 0;
}




///////////////////////////




rom Executor instructionTable[]= { ExecuteC, ExecuteB, ExecuteJ, ExecuteH };
