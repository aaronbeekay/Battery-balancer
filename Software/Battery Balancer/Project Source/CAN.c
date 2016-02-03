/*
 * CAN.c
 *
 *  Created on: Jan 26, 2016
 *      Author: Sean Harrington
 */

#include "CAN.h"
#include "Cell_Status.h"
#include "DSP2803x_Device.h"

//-----------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------

extern cell_voltage Cell_Voltages[CELLS_IN_SERIES];

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------

#define MAILBOX_0_DATA_ADDRESS = 0x6104;

#define MAILBOX_0_MSGID_ADDRESS = 0x6100;

#define BIM_MAILBOXES 17

#define CAN_BIM_MAILBOX_INIT(ID1, ID2, CELL1, CELL2, CELL3, CELL4) \
							{ID1, ID2, {CELL1, CELL2, CELL3, CELL4}, TRUE}

//-----------------------------------------------------------------------
// Private variables
//-----------------------------------------------------------------------

static struct ECAN_REGS ECanaShadow;

static can_bim_mailbox_t mailboxes[BIM_MAILBOXES] =
{
	CAN_BIM_MAILBOX_INIT(CELLS_1_TO_4_ID, CELLS_5_TO_8_ID, 1, 2, 3, 4),
	CAN_BIM_MAILBOX_INIT(CELLS_9_TO_12_ID, CELLS_13_TO_16_ID, 9, 10, 11, 12),
	CAN_BIM_MAILBOX_INIT(CELLS_17_TO_20_ID, CELLS_21_TO_24_ID, 17, 18, 19, 20),
	CAN_BIM_MAILBOX_INIT(CELLS_25_TO_28_ID, CELLS_29_TO_32_ID, 25, 26, 27, 28),
	CAN_BIM_MAILBOX_INIT(CELLS_33_TO_36_ID, CELLS_37_TO_40_ID, 33, 34, 35, 36),
	CAN_BIM_MAILBOX_INIT(CELLS_41_TO_44_ID, CELLS_45_TO_48_ID, 41, 42, 43, 44),
	CAN_BIM_MAILBOX_INIT(CELLS_49_TO_52_ID, CELLS_53_TO_56_ID, 49, 50, 51, 52),
	CAN_BIM_MAILBOX_INIT(CELLS_57_TO_60_ID, CELLS_61_TO_64_ID, 57, 58, 59, 60),
	CAN_BIM_MAILBOX_INIT(CELLS_65_TO_68_ID, CELLS_69_TO_72_ID, 65, 66, 67, 68),
	CAN_BIM_MAILBOX_INIT(CELLS_73_TO_76_ID, CELLS_77_TO_80_ID, 73, 74, 75, 76),
	CAN_BIM_MAILBOX_INIT(CELLS_81_TO_84_ID, CELLS_85_TO_88_ID, 81, 82, 83, 84),
	CAN_BIM_MAILBOX_INIT(CELLS_89_TO_92_ID, CELLS_93_TO_96_ID, 89, 90, 91, 92),
	CAN_BIM_MAILBOX_INIT(CELLS_97_TO_100_ID, CELLS_101_TO_104_ID, 97, 98, 99, 100),
	CAN_BIM_MAILBOX_INIT(CELLS_105_TO_108_ID, CELLS_109_TO_112_ID, 105, 106, 107, 108),
	CAN_BIM_MAILBOX_INIT(CELLS_113_TO_116_ID, CELLS_117_TO_120_ID, 113, 114, 115, 116),
	CAN_BIM_MAILBOX_INIT(CELLS_121_TO_124_ID, CELLS_125_TO_128_ID, 121, 122, 123, 124),
	CAN_BIM_MAILBOX_INIT(CELLS_129_TO_132_ID, CELLS_133_TO_134_ID, 129, 130, 131, 132)
};

//-----------------------------------------------------------------------
// Private (Internal) Function Definitions

static void ClearMailboxes(void);

void InitECana(void);

static void SetupMailboxes(void);

//-----------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------

void CAN_Init(void)
{
	EALLOW;

	/* Enable internal pull-up for the selected CAN pins */
	GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;     // Enable pull-up for GPIO30 (CANRXA)
	GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;     // Enable pull-up for GPIO31 (CANTXA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;   // Asynch qual for GPIO30 (CANRXA)

	/* Configure eCAN-A pins using GPIO regs*/
	GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;    // Configure GPIO30 for CANRXA operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;    // Configure GPIO31 for CANTXA operation

	InitECana();

	ClearMailboxes();

	ECanaShadow.CANGIM.all = 0;
	ECanaShadow.CANGAM.bit.AMI = 0; //must be standard
	ECanaShadow.CANGIM.bit.I1EN = 1;  // enable I1EN
	ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
	ECanaShadow.CANME.all = ECanaRegs.CANME.all;

	EDIS;
	SetupMailboxes();

	EALLOW;
	ECanaShadow.CANMIM.all = 0x1FFFF;	// Which mailboxes to toggle interrupt on
	ECanaShadow.CANMIL.all = 0x1FFFF;
	ECanaRegs.CANGAM.all = ECanaShadow.CANGAM.all;
	ECanaRegs.CANGIM.all = ECanaShadow.CANGIM.all;
	ECanaRegs.CANMIM.all = ECanaShadow.CANMIM.all;
	ECanaRegs.CANMIL.all = ECanaShadow.CANMIL.all;
	ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
	ECanaRegs.CANME.all = ECanaShadow.CANME.all;
	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.STM = 0;    // No self-test mode
	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
	EDIS;

	//ENABLE PIE INTERRUPTS
	IER |= M_INT9;
	PieCtrlRegs.PIEIER9.bit.INTx6= 1;
}


//-----------------------------------------------------------------------
// Private (Internal) functions
//-----------------------------------------------------------------------

static void ClearMailboxes(void)
{
	ECanaMboxes.MBOX0.MDH.all = 0;
	ECanaMboxes.MBOX0.MDL.all = 0;
	ECanaMboxes.MBOX1.MDH.all = 0;
	ECanaMboxes.MBOX1.MDL.all = 0;
	ECanaMboxes.MBOX2.MDH.all = 0;
	ECanaMboxes.MBOX2.MDL.all = 0;
	ECanaMboxes.MBOX3.MDH.all = 0;
	ECanaMboxes.MBOX3.MDL.all = 0;
	ECanaMboxes.MBOX4.MDH.all = 0;
	ECanaMboxes.MBOX4.MDL.all = 0;
	ECanaMboxes.MBOX5.MDH.all = 0;
	ECanaMboxes.MBOX5.MDL.all = 0;
	ECanaMboxes.MBOX6.MDH.all = 0;
	ECanaMboxes.MBOX6.MDL.all = 0;
	ECanaMboxes.MBOX7.MDH.all = 0;
	ECanaMboxes.MBOX7.MDL.all = 0;
	ECanaMboxes.MBOX8.MDH.all = 0;
	ECanaMboxes.MBOX8.MDL.all = 0;
	ECanaMboxes.MBOX9.MDH.all = 0;
	ECanaMboxes.MBOX9.MDL.all = 0;
	ECanaMboxes.MBOX10.MDH.all = 0;
	ECanaMboxes.MBOX10.MDL.all = 0;
	ECanaMboxes.MBOX11.MDH.all = 0;
	ECanaMboxes.MBOX11.MDL.all = 0;
	ECanaMboxes.MBOX12.MDH.all = 0;
	ECanaMboxes.MBOX12.MDL.all = 0;
	ECanaMboxes.MBOX13.MDH.all = 0;
	ECanaMboxes.MBOX13.MDL.all = 0;
	ECanaMboxes.MBOX14.MDH.all = 0;
	ECanaMboxes.MBOX14.MDL.all = 0;
	ECanaMboxes.MBOX15.MDH.all = 0;
	ECanaMboxes.MBOX15.MDL.all = 0;
	ECanaMboxes.MBOX16.MDH.all = 0;
	ECanaMboxes.MBOX16.MDL.all = 0;
	ECanaMboxes.MBOX17.MDH.all = 0;
	ECanaMboxes.MBOX17.MDL.all = 0;
	ECanaMboxes.MBOX18.MDH.all = 0;
	ECanaMboxes.MBOX18.MDL.all = 0;
	ECanaMboxes.MBOX19.MDH.all = 0;
	ECanaMboxes.MBOX19.MDL.all = 0;
	ECanaMboxes.MBOX20.MDH.all = 0;
	ECanaMboxes.MBOX20.MDL.all = 0;
	ECanaMboxes.MBOX21.MDH.all = 0;
	ECanaMboxes.MBOX21.MDL.all = 0;
	ECanaMboxes.MBOX22.MDH.all = 0;
	ECanaMboxes.MBOX22.MDL.all = 0;
	ECanaMboxes.MBOX23.MDH.all = 0;
	ECanaMboxes.MBOX23.MDL.all = 0;
	ECanaMboxes.MBOX24.MDH.all = 0;
	ECanaMboxes.MBOX24.MDL.all = 0;
	ECanaMboxes.MBOX25.MDH.all = 0;
	ECanaMboxes.MBOX25.MDL.all = 0;
	ECanaMboxes.MBOX26.MDH.all = 0;
	ECanaMboxes.MBOX26.MDL.all = 0;
	ECanaMboxes.MBOX27.MDH.all = 0;
	ECanaMboxes.MBOX27.MDL.all = 0;
	ECanaMboxes.MBOX28.MDH.all = 0;
	ECanaMboxes.MBOX28.MDL.all = 0;
	ECanaMboxes.MBOX29.MDH.all = 0;
	ECanaMboxes.MBOX30.MDL.all = 0;
	ECanaMboxes.MBOX30.MDH.all = 0;
	ECanaMboxes.MBOX31.MDL.all = 0;
	ECanaMboxes.MBOX31.MDH.all = 0;
}

void InitECana(void)
{
	/* Create a shadow register structure for the CAN control registers. This is
	needed, since only 32-bit access is allowed to these registers. 16-bit access
	to these registers could potentially corrupt the register contents or return
	false data. */

    EALLOW;     // EALLOW enables access to protected bits

    /* Configure eCAN RX and TX pins for CAN operation using eCAN regs*/

    ECanaShadow.CANTIOC.all = ECanaRegs.CANTIOC.all;
    ECanaShadow.CANTIOC.bit.TXFUNC = 1;
    ECanaRegs.CANTIOC.all = ECanaShadow.CANTIOC.all;

    ECanaShadow.CANRIOC.all = ECanaRegs.CANRIOC.all;
    ECanaShadow.CANRIOC.bit.RXFUNC = 1;
    ECanaRegs.CANRIOC.all = ECanaShadow.CANRIOC.all;

    /* Configure eCAN for HECC mode - (reqd to access mailboxes 16 thru 31) */
                                    // HECC mode also enables time-stamping feature

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.SCB = 1;
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

	/* Initialize all bits of 'Message Control Register' to zero */
	// Some bits of MSGCTRL register come up in an unknown state. For proper operation,
	// all bits (including reserved bits) of MSGCTRL must be initialized to zero

    ECanaMboxes.MBOX0.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX1.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX2.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX3.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX4.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX5.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX6.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX7.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX8.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX9.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX10.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX11.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX12.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX13.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX14.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX15.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX16.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX17.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX18.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX19.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX20.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX21.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX22.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX23.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX24.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX25.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX26.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX27.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX28.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX29.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX30.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX31.MSGCTRL.all = 0x00000000;

	// TAn, RMPn, GIFn bits are all zero upon reset and are cleared again
	//  as a matter of precaution.

    ECanaRegs.CANTA.all = 0xFFFFFFFF;   /* Clear all TAn bits */

    ECanaRegs.CANRMP.all = 0xFFFFFFFF;  /* Clear all RMPn bits */

    ECanaRegs.CANGIF0.all = 0xFFFFFFFF; /* Clear all interrupt flag bits */
    ECanaRegs.CANGIF1.all = 0xFFFFFFFF;

	/* Configure bit timing parameters for eCANA*/

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.DBO = 1 ;
    ECanaShadow.CANMC.bit.CCR = 1 ;            // Set CCR = 1
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    // Wait until the CPU has been granted permission to change the configuration registers
    do
    {
      ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 1 );       // Wait for CCE bit to be set..

    ECanaShadow.CANBTC.all = 0;
    /* The following block is only for 60 MHz SYSCLKOUT. (30 MHz CAN module clock Bit rate = 1 Mbps
       See Note at end of file. */
	//    ECanaShadow.CANBTC.bit.BRPREG = 2;
	//    ECanaShadow.CANBTC.bit.TSEG2REG = 1;
	//    ECanaShadow.CANBTC.bit.TSEG1REG = 6;

    //Use table 33 in eCAN manual to determine values
    //BT = 15
    //TSEG1 = 10
    //TSEG2 = 2
    //JW  = 2;
    //Sampling point = 80%
    //bus speed 1Mbps
    //BRP_reg = 1
    ECanaShadow.CANBTC.bit.BRPREG = 1;
    ECanaShadow.CANBTC.bit.TSEG1REG = 10;
    ECanaShadow.CANBTC.bit.TSEG2REG = 2;
    ECanaShadow.CANBTC.bit.SJWREG = 1;


    ECanaShadow.CANBTC.bit.SAM = 0;
    ECanaRegs.CANBTC.all = ECanaShadow.CANBTC.all;

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    // Wait until the CPU no longer has permission to change the configuration registers
    do
    {
      ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 0 );       // Wait for CCE bit to be  cleared..

    /* Disable all Mailboxes  */
    ECanaRegs.CANME.all = 0;        // Required before writing the MSGIDs

    EDIS;
}

static void SetupMailboxes(void)
{

	ECanaMboxes.MBOX0.MSGCTRL.all = 0x18; 	// DLC 8, RTR Transmit
	ECanaMboxes.MBOX0.MSGID.bit.STDMSGID = (mailboxes[0].ID1_Active)
			? mailboxes[0].ID1
			: mailboxes[0].ID2;
	ECanaShadow.CANMD.bit.MD0 = 1; 			//receive
	ECanaShadow.CANME.bit.ME0 = 1;			//enable

	ECanaMboxes.MBOX1.MSGCTRL.all = 0x18; 	// DLC 8, RTR Transmit
	ECanaMboxes.MBOX1.MSGID.bit.STDMSGID = (mailboxes[1].ID1_Active)
			? mailboxes[1].ID1
			: mailboxes[1].ID2;
	ECanaShadow.CANMD.bit.MD1 = 1; 			//receive
	ECanaShadow.CANME.bit.ME1 = 1;			//enable

	ECanaMboxes.MBOX2.MSGCTRL.all = 0x18; 	// DLC 8, RTR Transmit
	ECanaMboxes.MBOX2.MSGID.bit.STDMSGID = (mailboxes[2].ID1_Active)
			? mailboxes[2].ID1
			: mailboxes[2].ID2;
	ECanaShadow.CANMD.bit.MD2 = 1; 			//receive
	ECanaShadow.CANME.bit.ME2 = 1;			//enable

	ECanaMboxes.MBOX3.MSGCTRL.all = 0x18; 	// DLC 8, RTR Transmit
	ECanaMboxes.MBOX3.MSGID.bit.STDMSGID = (mailboxes[3].ID1_Active)
			? mailboxes[3].ID1
			: mailboxes[3].ID2;
	ECanaShadow.CANMD.bit.MD3 = 1; 			//receive
	ECanaShadow.CANME.bit.ME3 = 1;			//enable

	ECanaMboxes.MBOX4.MSGCTRL.all = 0x18; 	// DLC 8, RTR Transmit
	ECanaMboxes.MBOX4.MSGID.bit.STDMSGID = (mailboxes[4].ID1_Active)
			? mailboxes[4].ID1
			: mailboxes[4].ID2;
	ECanaShadow.CANMD.bit.MD4 = 1; 			//receive
	ECanaShadow.CANME.bit.ME4 = 1;			//enable

	ECanaMboxes.MBOX5.MSGCTRL.all = 0x18; 	// DLC 8, RTR Transmit
	ECanaMboxes.MBOX5.MSGID.bit.STDMSGID = (mailboxes[5].ID1_Active)
			? mailboxes[5].ID1
			: mailboxes[5].ID2;
	ECanaShadow.CANMD.bit.MD5 = 1; 			//receive
	ECanaShadow.CANME.bit.ME5 = 1;			//enable

	ECanaMboxes.MBOX6.MSGCTRL.all = 0x18; 	// DLC 8, RTR Transmit
	ECanaMboxes.MBOX6.MSGID.bit.STDMSGID = (mailboxes[6].ID1_Active)
			? mailboxes[6].ID1
			: mailboxes[6].ID2;
	ECanaShadow.CANMD.bit.MD6 = 1; 			//receive
	ECanaShadow.CANME.bit.ME6 = 1;			//enable

	ECanaMboxes.MBOX7.MSGCTRL.all = 0x18; 	// DLC 8, RTR Transmit
	ECanaMboxes.MBOX7.MSGID.bit.STDMSGID = (mailboxes[7].ID1_Active)
			? mailboxes[7].ID1
			: mailboxes[7].ID2;
	ECanaShadow.CANMD.bit.MD7 = 1; 			//receive
	ECanaShadow.CANME.bit.ME7 = 1;			//enable

	ECanaMboxes.MBOX8.MSGCTRL.all = 0x18; 	// DLC 8, RTR Transmit
	ECanaMboxes.MBOX8.MSGID.bit.STDMSGID = (mailboxes[8].ID1_Active)
			? mailboxes[8].ID1
			: mailboxes[8].ID2;
	ECanaShadow.CANMD.bit.MD8 = 1; 			//receive
	ECanaShadow.CANME.bit.ME8 = 1;			//enable

	ECanaMboxes.MBOX9.MSGCTRL.all = 0x18; 	// DLC 8, RTR Transmit
	ECanaMboxes.MBOX9.MSGID.bit.STDMSGID = (mailboxes[9].ID1_Active)
			? mailboxes[9].ID1
			: mailboxes[9].ID2;
	ECanaShadow.CANMD.bit.MD9 = 1; 			//receive
	ECanaShadow.CANME.bit.ME9 = 1;			//enable

	ECanaMboxes.MBOX10.MSGCTRL.all = 0x18; 	// DLC 8, RTR Transmit
	ECanaMboxes.MBOX10.MSGID.bit.STDMSGID = (mailboxes[10].ID1_Active)
			? mailboxes[10].ID1
			: mailboxes[10].ID2;
	ECanaShadow.CANMD.bit.MD10 = 1; 		//receive
	ECanaShadow.CANME.bit.ME10 = 1;			//enable

	ECanaMboxes.MBOX11.MSGCTRL.all = 0x18; 	// DLC 8, RTR Transmit
	ECanaMboxes.MBOX11.MSGID.bit.STDMSGID = (mailboxes[11].ID1_Active)
			? mailboxes[11].ID1
			: mailboxes[11].ID2;
	ECanaShadow.CANMD.bit.MD11 = 1; 		//receive
	ECanaShadow.CANME.bit.ME11 = 1;			//enable

	ECanaMboxes.MBOX12.MSGCTRL.all = 0x18; 	// DLC 8, RTR Transmit
	ECanaMboxes.MBOX12.MSGID.bit.STDMSGID = (mailboxes[12].ID1_Active)
			? mailboxes[12].ID1
			: mailboxes[12].ID2;
	ECanaShadow.CANMD.bit.MD12 = 1; 		//receive
	ECanaShadow.CANME.bit.ME12 = 1;			//enable

	ECanaMboxes.MBOX13.MSGCTRL.all = 0x18; 	// DLC 8, RTR Transmit
	ECanaMboxes.MBOX13.MSGID.bit.STDMSGID = (mailboxes[13].ID1_Active)
			? mailboxes[13].ID1
			: mailboxes[13].ID2;
	ECanaShadow.CANMD.bit.MD13 = 1; 			//receive
	ECanaShadow.CANME.bit.ME13 = 1;			//enable

	ECanaMboxes.MBOX14.MSGCTRL.all = 0x18; 	// DLC 8, RTR Transmit
	ECanaMboxes.MBOX14.MSGID.bit.STDMSGID = (mailboxes[14].ID1_Active)
			? mailboxes[14].ID1
			: mailboxes[14].ID2;
	ECanaShadow.CANMD.bit.MD14 = 1; 			//receive
	ECanaShadow.CANME.bit.ME14 = 1;			//enable

	ECanaMboxes.MBOX15.MSGCTRL.all = 0x18; 	// DLC 8, RTR Transmit
	ECanaMboxes.MBOX15.MSGID.bit.STDMSGID = (mailboxes[15].ID1_Active)
			? mailboxes[15].ID1
			: mailboxes[15].ID2;
	ECanaShadow.CANMD.bit.MD15 = 1; 			//receive
	ECanaShadow.CANME.bit.ME15 = 1;			//enable

	ECanaMboxes.MBOX16.MSGCTRL.all = 0x18; 	// DLC 8, RTR Transmit
	ECanaMboxes.MBOX16.MSGID.bit.STDMSGID = (mailboxes[16].ID1_Active)
			? mailboxes[16].ID1
			: mailboxes[16].ID2;
	ECanaShadow.CANMD.bit.MD16 = 1; 			//receive
	ECanaShadow.CANME.bit.ME16 = 1;			//enable
}

//---------------------------------------------------------------------
// SYS/BIOS Functions (USER SHOULD NOT CALL)
//---------------------------------------------------------------------

Void CAN_Receive_Interrupt()
{
	Uint32 rcvd_box =  ECanaRegs.CANGIF1.bit.MIV1;
	Uint32 * CAN_Data_Address = 0x00006104 + (8 * rcvd_box);

	// CAN MDL_Low Word Address
	Cell_Voltages[mailboxes[rcvd_box].Active_Cells.CellSel1-1] = (*CAN_Data_Address);
	// CAN MDL_High Word Address
	Cell_Voltages[mailboxes[rcvd_box].Active_Cells.CellSel2-1] = (*(CAN_Data_Address) >> 16);
	// CAN MDH_Low Word Address
	Cell_Voltages[mailboxes[rcvd_box].Active_Cells.CellSel3-1] = (*(CAN_Data_Address + 1));
	// CAN MDH_High Word Address
	Cell_Voltages[mailboxes[rcvd_box].Active_Cells.CellSel4-1] = (*(CAN_Data_Address + 1) >> 16);

	EALLOW;
	// Disable received mailbox
	ECanaRegs.CANME.all ^= (1UL << rcvd_box);
    while (ECanaRegs.CANES.bit.CCE != 0UL)
    {
    	// Wait..
    }
	// Swap MSGID
	Uint32 * CAN_MSGID_Address = 0x00006100 + (8 * rcvd_box);
	*(CAN_MSGID_Address+1) = 0x18;
	if (mailboxes[rcvd_box].ID1_Active)
	{
		*CAN_MSGID_Address = ((Uint32)mailboxes[rcvd_box].ID2) << 18;
		mailboxes[rcvd_box].Active_Cells.CellSel1 += 4;
		mailboxes[rcvd_box].Active_Cells.CellSel2 += 4;
		mailboxes[rcvd_box].Active_Cells.CellSel3 += 4;
		mailboxes[rcvd_box].Active_Cells.CellSel4 += 4;
	}
	else
	{
		*CAN_MSGID_Address = ((Uint32)mailboxes[rcvd_box].ID1) << 18;
		mailboxes[rcvd_box].Active_Cells.CellSel1 -= 4;
		mailboxes[rcvd_box].Active_Cells.CellSel2 -= 4;
		mailboxes[rcvd_box].Active_Cells.CellSel3 -= 4;
		mailboxes[rcvd_box].Active_Cells.CellSel4 -= 4;
	}
	// Re-enable mailbox with new MSGID
	mailboxes[rcvd_box].ID1_Active = !mailboxes[rcvd_box].ID1_Active;
	ECanaRegs.CANME.all ^= (1UL << rcvd_box);
	ECanaRegs.CANRMP.all |= (1UL < rcvd_box);

	EDIS;
}

Void SendCAN()
{
	Uint32 system_mask = 0x1FFFF;
	EALLOW;
	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	if (ECanaShadow.CANMC.bit.CCR == 1)
	{
		//BUS_OFF();
	}
	ECanaShadow.CANTRS.all = system_mask;
	ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;

	do
	{
		ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
	}
	while((ECanaShadow.CANTA.all & system_mask) != 0); //wait to send or hit stop watch

	ECanaShadow.CANTA.all = system_mask;
	ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;						//clear flag

	EDIS;
}
