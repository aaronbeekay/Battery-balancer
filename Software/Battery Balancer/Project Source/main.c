/*
 *  ======== main.c ========
 */

//----------------------------------------
// BIOS header files
//----------------------------------------

#include "Common_Includes.h"

//-----------------------------------------
// ControlSuite Header Files
//-----------------------------------------

#include "Initialize.h"
#include "State.h"

// @todo: Document events


/*
 *  ======== taskFxn ========
 */
Void taskFxn(UArg a0, UArg a1)
{
    System_printf("enter taskFxn()\n");

    Task_sleep(10);

    System_printf("exit taskFxn()\n");

    System_flush(); /* force SysMin output to console */
}

/*
 *  ======== main ========
 */
Int main()
{ 
	HardwareInit();
	SoftwareInit();
	// @todo: Determine if I should keep default project code
    Task_Handle task;
    Error_Block eb;

    System_printf("enter main()\n");

    Error_init(&eb);
    task = Task_create(taskFxn, NULL, &eb);
    if (task == NULL) {
        System_printf("Task_create() failed!\n");
        BIOS_exit(0);
    }

    BIOS_start();    /* does not return */
    return(0);
}

Void UpdateState()
{
	state currentState = GetState();
	switch (currentState)
	{
		case WAIT:
			// Check for problems.
			// Perform wait routine here
			break;
		case CHARGE:
			// Perform safety checks
			// Update structures
			break;
	}
}

