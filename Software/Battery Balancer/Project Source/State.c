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
