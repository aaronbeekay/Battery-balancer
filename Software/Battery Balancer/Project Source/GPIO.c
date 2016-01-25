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
#include "State.h"
//-----------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------

// Value used to set GPxDIR register as GPIO output
#define GPIO_OUT 		1

// Value used to set GPxDIR register as GPIO input
#define GPIO_IN			0

// Value used to set GPxMUXy. Using this value with set the pin as a GPIO
#define GPIO_MUX		0

#define DISABLE_PULLUP	1

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

	// LED Setup (temporary)
	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = GPIO_MUX;
	GpioCtrlRegs.GPBDIR.bit.GPIO34 = GPIO_OUT;

	GpioCtrlRegs.GPBMUX1.bit.GPIO41 = GPIO_MUX;
	GpioCtrlRegs.GPBDIR.bit.GPIO41 = GPIO_OUT;

	GpioCtrlRegs.GPAMUX1.bit.GPIO9 = GPIO_MUX;
	GpioCtrlRegs.GPADIR.bit.GPIO9 = GPIO_OUT;

	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = GPIO_MUX;
	GpioCtrlRegs.GPADIR.bit.GPIO11 = GPIO_OUT;

	EDIS;

	EALLOW;

	// Hex encoder as 3 position switch (temporary)
	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = GPIO_MUX;
	GpioCtrlRegs.GPADIR.bit.GPIO12 = GPIO_IN;
	GpioCtrlRegs.GPAPUD.bit.GPIO12 = DISABLE_PULLUP;

	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = GPIO_MUX;
	GpioCtrlRegs.GPADIR.bit.GPIO13 = GPIO_IN;
	GpioCtrlRegs.GPAPUD.bit.GPIO13 = DISABLE_PULLUP;

	GpioCtrlRegs.GPAMUX1.bit.GPIO14 = GPIO_MUX;
	GpioCtrlRegs.GPADIR.bit.GPIO14 = GPIO_IN;
	GpioCtrlRegs.GPAPUD.bit.GPIO14 = DISABLE_PULLUP;

	GpioCtrlRegs.GPAMUX1.bit.GPIO15 = GPIO_MUX;
	GpioCtrlRegs.GPADIR.bit.GPIO15 = GPIO_IN;
	GpioCtrlRegs.GPAPUD.bit.GPIO15 = DISABLE_PULLUP;

	// Toggle XINT1 on GPIO 12
	GpioIntRegs.GPIOXINT1SEL.all = 12;
	XIntruptRegs.XINT1CR.bit.POLARITY = 1;
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;

	EDIS;
}

Void HWI_Switch_Service()
{
	if (GpioDataRegs.GPADAT.bit.GPIO12 == 1)
	{
		GpioDataRegs.GPASET.bit.GPIO9 = 1;
		GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
		GpioDataRegs.GPBCLEAR.bit.GPIO41 = 1;
		GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
		SetState(CHARGE);
	}
	if (GpioDataRegs.GPADAT.bit.GPIO12 == 1 &&
			GpioDataRegs.GPADAT.bit.GPIO13 == 1)
	{
		GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
		GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
		GpioDataRegs.GPBCLEAR.bit.GPIO41 = 1;
		GpioDataRegs.GPASET.bit.GPIO11 = 1;
		SetState(BALANCE);
	}
	if (GpioDataRegs.GPADAT.bit.GPIO12 == 1 &&
			GpioDataRegs.GPADAT.bit.GPIO14 == 1)
	{
		GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
		GpioDataRegs.GPBSET.bit.GPIO34 = 1;
		GpioDataRegs.GPBCLEAR.bit.GPIO41 = 1;
		GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
		SetState(CHARGE_BALANCE);
	}
	if (GpioDataRegs.GPADAT.bit.GPIO12 == 1 &&
			GpioDataRegs.GPADAT.bit.GPIO15 == 1)
	{
		GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
		GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
		GpioDataRegs.GPBSET.bit.GPIO41 = 1;
		GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
		SetState(WAIT);
	}
}

