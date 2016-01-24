/*
 * Initialize.c
 *
 *  Created on: Jan 23, 2016
 *      Author: Sean Harrington
 */




//-----------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------

#include "Initialize.h"
#include "GPIO.h"

//-----------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
extern Uint16 RamfuncsLoadSize;

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------

// Value used to set GPxDIR register as GPIO output
#define GPIO_OUT 	1

// Value used to set GPxDIR register as GPIO input
#define GPIO_IN		0

// Value used to set GPxMUXy. Using this value with set the pin as a GPIO
#define GPIO_MUX	0

//-----------------------------------------------------------------------
// Private variables
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Private (Internal) functions
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------
void HardwareInit()
{
	InitSysCtrl();
	memcpy(&RamfuncsRunStart,&RamfuncsLoadStart,(unsigned long)&RamfuncsLoadSize);
	InitFlash();

	Gpio_Init();
	// @todo: I2C setup
	// @todo: CAN setup
	return;
}


