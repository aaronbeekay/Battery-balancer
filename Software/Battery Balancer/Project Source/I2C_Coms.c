/*
 * I2C_Coms.c
 *
 *  Created on: Jan 23, 2016
 *      Author: Sean Harrington
 */

//-----------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------

#include "I2C_Coms.h"

//-----------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------

// IC2 Address of TCA9555 defined by hardware switches
#define SLAVE_ADDRESS 		0x20

// TCA9555 Register Addresses
#define INPUT_PORT_0			0x0
#define INPUT_PORT_1			0x1
#define OUTPUT_PORT_0			0x2
#define OUTPUT_PORT_1			0x3
#define POLARITY_INV_PORT_0		0x4
#define POLARITY_INV_PORT_1		0x5
#define CONFIG_PORT_0			0x6
#define CONFIG_PORT_1			0x7

typedef enum {
	NONE = 0,
	ARB_LOST = 1,
	NOACK = 2,
	REGS_READY = 3,
	RX_DATA_READY = 4,
	TX_DATA_READY = 5,
	STOP = 6,
	ADDR_AS_SLAVE = 7
} i2c_intcodes;

//-----------------------------------------------------------------------
// Private variables
//-----------------------------------------------------------------------

static Bool mGpioExpanderIntVal;
static Uint8 mCurrentState;

static Uint8 mCurrentRegisterAddr;

typedef enum {
	I2C_NOT_IN_PROGRESS = 0,
	I2C_SENDING_READ = 1,
	I2C_DATA_READY = 3,
	I2C_SENDING_WRITE = 4,
} i2c_states;

//-----------------------------------------------------------------------
// Private (Internal) functions
//-----------------------------------------------------------------------

void ExpanderInterrupt(void);

//-----------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------

Void I2C_Init()
{
	mGpioExpanderIntVal = TRUE;		// Active Low
	mCurrentState = I2C_NOT_IN_PROGRESS;
	mCurrentRegisterAddr = INPUT_PORT_0;
	EALLOW;

	// Enable I2C-A on GPIO32 - GPIO33
	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;   // Enable pullup on GPIO32
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;  // GPIO32 = SDAA
	GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3; // Asynch input
	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;   // Enable pullup on GPIO33
	GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3; // Asynch input
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;  // GPIO33 = SCLA

	// Setup GP0 as interrupt source XINT0
	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;		// use GPIO17 as GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO26 = 0;		// GPIO17 is input
	GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0;		//Enable pullup

	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 26;		// XINT1 is GPIO17
	XIntruptRegs.XINT1CR.bit.POLARITY = 0;		// falling edge
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;		// enable XINT1

	// todo Confirm. This address means all pins are tied low.
	I2caRegs.I2CSAR = SLAVE_ADDRESS;

	//---------------------------------------------------------
	// Clock setup 400 KHz
	//	(1/400 KHz) = (I2CPSC + 1)[(5 + CLKL) + (5 + (CLKH)]
	//					------------------------------------
	//						SYSCLKOUT Freq (60,000,000)
	//---------------------------------------------------------
	I2caRegs.I2CPSC.all = 4;
	I2caRegs.I2CCLKL = 5;
	I2caRegs.I2CCLKH = 5;

	// Configure interrupt setup
	I2caRegs.I2CIER.all = 0xF;
	// Enable I2C module
	I2caRegs.I2CMDR.all = 0x20;
	// Check FIFO configuration
	I2caRegs.I2CFFTX.all = 0x6000;

	EDIS;
	return;
}

// todo: Set up HWI for this in cfg
void ExpanderInterrupt(void)
{
	if (I2caRegs.I2CMDR.bit.STP || I2caRegs.I2CSTR.bit.BB)
	{
		// Message still in flight
		mGpioExpanderIntVal = FALSE;
		return;
	}
	// todo: Change interrupt to fire when all TX bytes sent.

	I2caRegs.I2CCNT = 2;

	I2caRegs.I2CDXR = SLAVE_ADDRESS;
	I2caRegs.I2CDXR = mCurrentRegisterAddr;

	// Send with bits: Start, Mst, Trx, IRS
	I2caRegs.I2CEMDR.all = 0x2620;
}

void I2C_Interrupt(void)
{
	do
	{
		// Each read of INTCODE clears the flag the caused the interrupt
		// and loads in the next lower priority interrupt code if pending
		Uint16 interruptSource = I2caRegs.I2CISRC.bit.INTCODE;

		if ((interruptSource == ARB_LOST) || (interruptSource == NOACK))
		{
			// todo: Find out if something needs to be reset
			return;
		}
		// todo: Add check as well for what caused the interrupt
		else if (mCurrentState & I2C_SENDING_READ &&
				(interruptSource == REGS_READY))
		{
			I2caRegs.I2CCNT = 2;
			I2caRegs.I2CDXR = SLAVE_ADDRESS;

			// Master Receiver, STOP, START, NACK
			I2caRegs.I2CMDR.all = 0xAC20;
		}
		else if ((mCurrentState & I2C_DATA_READY) &&
				(interruptSource == RX_DATA_READY))
		{
			I2caRegs.I2CDRR;

			if (mGpioExpanderIntVal = GpioDataRegs.GPADAT.bit.GPIO26)
			{
				mCurrentRegisterAddr = INPUT_PORT_1;
				// Another read is required for the other register
				// todo: Figure out if to call in this HWI or call outside
			}
		}
	} while (interruptSoruce != NONE);
}


