/*
 * GPIO.c
 *
 *  Created on: Jan 23, 2016
 *      Author: Sean Harrington
 */

//-----------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------
#include "GPIO.h"

//-----------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------

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

// @todo: Documentation
Void Gpio_Init()
{
	// Setup GPIO Registers
	EALLOW;

	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = GPIO_MUX;
	GpioCtrlRegs.GPBDIR.bit.GPIO34 = GPIO_OUT;

	GpioCtrlRegs.GPBMUX1.bit.GPIO41 = GPIO_MUX;
	GpioCtrlRegs.GPBDIR.bit.GPIO41 = GPIO_OUT;

	GpioCtrlRegs.GPAMUX1.bit.GPIO9 = GPIO_MUX;
	GpioCtrlRegs.GPADIR.bit.GPIO9 = GPIO_OUT;

	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = GPIO_MUX;
	GpioCtrlRegs.GPADIR.bit.GPIO11 = GPIO_OUT;

	EDIS;
	return;
}

