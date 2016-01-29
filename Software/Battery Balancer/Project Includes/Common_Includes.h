/*
 * Common_Includes.h
 *
 *  Created on: Jan 23, 2016
 *      Author: Sean Harrington
 */

#ifndef PROJECT_INCLUDES_COMMON_INCLUDES_H_
#define PROJECT_INCLUDES_COMMON_INCLUDES_H_


#include <xdc/std.h>			//mandatory - have to include first, for BIOS types
#include <xdc/cfg/global.h> 				//header file for statically defined objects/handles
#include "DSP28x_Project.h"

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h> 				//header file for statically defined objects/handles

#include <ti/sysbios/BIOS.h>				//mandatory - if you call APIs like BIOS_start()
#include <ti/sysbios/knl/Task.h>

#endif /* PROJECT_INCLUDES_COMMON_INCLUDES_H_ */
