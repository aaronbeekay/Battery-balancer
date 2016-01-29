/*
 * CAN.h
 *
 *  Created on: Jan 26, 2016
 *      Author: Sean Harrington
 */

#ifndef PROJECT_INCLUDES_CAN_H_
#define PROJECT_INCLUDES_CAN_H_

//---------------------------------------------------------------------------------
// Common Includes

#include "Common_Includes.h"


//---------------------------------------------------------------------------------
// Public variables

// Define table of valid CAN IDs battery balancer is looking to receive

#define CELLS_1_TO_4_ID			784
#define CELLS_5_TO_8_ID			785
#define CELLS_9_TO_12_ID		786
#define CELLS_13_TO_16_ID		787
#define CELLS_17_TO_20_ID		788
#define CELLS_21_TO_24_ID		789
#define CELLS_25_TO_28_ID		790
#define CELLS_29_TO_32_ID		791
#define CELLS_33_TO_36_ID		792
#define CELLS_37_TO_40_ID		793
#define CELLS_41_TO_44_ID		794
#define CELLS_45_TO_48_ID		795
#define CELLS_49_TO_52_ID		796
#define CELLS_53_TO_56_ID		797
#define CELLS_57_TO_60_ID		798
#define CELLS_61_TO_64_ID		799
#define CELLS_65_TO_68_ID		800
#define CELLS_69_TO_72_ID		801
#define CELLS_73_TO_76_ID		802
#define CELLS_77_TO_80_ID		803
#define CELLS_81_TO_84_ID		804
#define CELLS_85_TO_88_ID		805
#define CELLS_89_TO_92_ID		806
#define CELLS_93_TO_96_ID		807
#define CELLS_97_TO_100_ID		808
#define CELLS_101_TO_104_ID		809
#define CELLS_105_TO_108_ID		810
#define CELLS_109_TO_112_ID		811
#define CELLS_113_TO_116_ID		812
#define CELLS_117_TO_120_ID		813
#define CELLS_121_TO_124_ID		814
#define CELLS_125_TO_128_ID		815
#define CELLS_129_TO_132_ID		816
#define CELLS_133_TO_134_ID		817

typedef struct
{
	Uint8 CellSel1;
	Uint8 CellSel2;
	Uint8 CellSel3;
	Uint8 CellSel4;
} cells_t;

typedef struct
{
	Uint8 ID1;
	Uint8 ID2;
	cells_t Active_Cells;
	Bool ID1_Active;
} can_bim_mailbox_t;




//---------------------------------------------------------------------------------

Void CAN_Init();

// Setup current as ECANA_INT1 like MCN_Software
Void CAN_Receive_Interrupt();


#endif /* PROJECT_INCLUDES_CAN_H_ */
