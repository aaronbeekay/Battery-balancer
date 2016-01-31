/*
 * Battery_Status.h
 *
 *  Created on: Jan 28, 2016
 *      Author: Sean Harrington
 */

#ifndef PROJECT_INCLUDES_CELL_STATUS_H_
#define PROJECT_INCLUDES_CELL_STATUS_H_


//---------------------------------------------------------------------------------
// Common Includes

#include "Common_Includes.h"

#define CELLS_IN_SERIES		134

//---------------------------------------------------------------------------------
// Public variables

typedef Int16 cell_voltage;

extern cell_voltage Cell_Voltages[CELLS_IN_SERIES];

//---------------------------------------------------------------------------------


#endif /* PROJECT_INCLUDES_CELL_STATUS_H_ */
