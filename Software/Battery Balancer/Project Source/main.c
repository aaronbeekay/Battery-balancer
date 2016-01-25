/*
 *  ======== main.c ========
 */

//----------------------------------------
// BIOS header files
//----------------------------------------

#include "Common_Includes.h"

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h> 				//header file for statically defined objects/handles

#include <ti/sysbios/BIOS.h>				//mandatory - if you call APIs like BIOS_start()
#include <ti/sysbios/knl/Task.h>

//-----------------------------------------
// ControlSuite Header Files
//-----------------------------------------

#include "Initialize.h"

// @todo: Document events
#define WAIT_EVENT				0x1
#define CHARGE_EVENT 			0x2
#define BALANCE_EVENT 			0x4
#define CHARGE_BALANCE_EVENT 	0x8
#define ERROR_EVENT				0x10
#define ALL_EVENTS				0x3E

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

Void StateChangeTask()
{
	UInt events;
	while (TRUE)
	{
		events = Event_pend(StateChangeEvent, Event_Id_NONE, ALL_EVENTS, BIOS_WAIT_FOREVER);

		// Determine event posted
		if (events & ERROR_EVENT)
		{
			// Go into error state
			return;
		}
		else if (events & CHARGE_EVENT)
		{
			// Go into charge state
			return;
		}
		else if (events & BALANCE_EVENT)
		{
			return;
		}
		else if (events & CHARGE_BALANCE_EVENT)
		{
			return;
		}
		else if (events & WAIT_EVENT)
		{
			return;
		}
		else
		{
			// What event just fired? Error
			return;
		}
	}
}

