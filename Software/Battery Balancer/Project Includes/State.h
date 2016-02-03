/*
 * State.h
 *
 *  Created on: Jan 24, 2016
 *      Author: Sean Harrington
 */

#ifndef PROJECT_INCLUDES_STATE_H_
#define PROJECT_INCLUDES_STATE_H_

//------------------------------------------------
// Common Includes

#include "Common_Includes.h"

//------------------------------------------------
// Public variables

// Bitmask of states the system can be in
typedef enum STATES
{
	WAIT 			= 0,
	CHARGE  		= 2,
	BALANCE 		= 4,
	CHARGE_BALANCE 	= 8,
	ERROR 			= 16
} state;

//------------------------------------------------
// Public functions

Void InitializeState();

state GetState();

Bool SetState(state nextState);

//------------------------------------------------


#endif /* PROJECT_INCLUDES_STATE_H_ */
