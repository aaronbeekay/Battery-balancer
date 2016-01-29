/*
 * State.c
 *
 *  Created on: Jan 24, 2016
 *      Author: Sean Harrington
 */


//-----------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------

#include "State.h"
#include "Events.h"

//-----------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Private variables
//-----------------------------------------------------------------------

typedef struct
{
	enum STATES balancer_state;
	Bool initialized;
} balancer_state_t;

static balancer_state_t system_state;

//-----------------------------------------------------------------------
// Private (Internal) functions
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------

Void InitializeState()
{
	system_state.balancer_state = WAIT;
	system_state.initialized = TRUE;
}

state GetState()
{
	// Check if initialized. Throw error if not.
	return system_state.balancer_state;
}

Bool SetState(state nextState)
{
	// Semaphore for safety?
	// Check for value of nextState to see if within bounds?
	system_state.balancer_state = nextState;
	return TRUE;
}


//---------------------------------------------------------------------
// SYS/BIOS Functions (USER SHOULD NOT CALL)
//---------------------------------------------------------------------

Void StateChangeTask()
{
	UInt events;
	while (TRUE)
	{
		events = Event_pend(StateChangeEvent, Event_Id_NONE, ALL_EVENTS, BIOS_WAIT_FOREVER);

		// Determine event posted
		if (events & ERROR_EVENT)
		{
			SetState(ERROR);
			// Go into error state
			return;
		}
		else if (events & WAIT_EVENT)
		{
			SetState(WAIT);
			return;
		}
		else if (events & CHARGE_EVENT)
		{
			// Go into charge state
			SetState(CHARGE);
			return;
		}
		else if (events & BALANCE_EVENT)
		{
			SetState(BALANCE);
			return;
		}
		else if (events & CHARGE_BALANCE_EVENT)
		{
			SetState(CHARGE_BALANCE);
			return;
		}
		else
		{
			// What event just fired? Error
			return;
		}
	}
}

