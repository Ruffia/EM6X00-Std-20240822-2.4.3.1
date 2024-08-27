
#ifndef __USBFunctions_H
#define __USBFunctions_H

// 常数π
#define PI 3.1415926535


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 共享数据

// 6*16
typedef struct tagSharedParams_Cfg_ADCs
{
	// 配置值
	int		nNum;	// 0:SCN; 1:LNS; 2:VAC#0; 3:VAC#1; 4:SPR#0; 5:SPR#1
	int		nMID;	// 1:SCN; 2:LNS; 3:VAC; 4:SPR
	int		nCID;
	int		nChn;
	BOOL	bType;	// 1:2448; 0:1859
	BOOL	bDiff;	// 1:diff; 0:S.E.
	double	dFactor;
	double	dVref;
	double	dCOMoffset;
	double	dCampsPerVolt;
	double	dMin;
	double	dMax;
	CString	strLabel;
}SharedParams_Cfg_ADCs;

// 6*16
typedef struct tagSharedParams_Back_ADCs
{
	// 回读及记录值
	int		nNum;		// 0:SCN; 1:LNS; 2:VAC#0; 3:VAC#1; 4:SPR#0; 5:SPR#1
	int		nMID;		// 1:SCN; 2:LNS; 3:VAC; 4:SPR
	int		nChn;
	WORD	wData1;
	WORD	wData2;
	DWORD	dwData;		// ADCread24: Bit 28 --- Bit0 of readRaw, and shift down 5 bits
	double	dVrefPct;	// Percentage of full range of Vref(+) - Vref(-)
	double	dVrefFrac1;
	double	dVrefFrac2;
	double	dEquivCurrent;
}SharedParams_Back_ADCs;

// 7
typedef struct tagSharedParams_Back_Others
{
	int		nType;	// 1:gauge compr; 2:VAC switch; 3:SPR switch; 4:SPR power; 5:Board ID; 6:ISR; 7:MCI Pwr
	int		nChn;	// channel of BoardID, ISR & MCI Pwr
	WORD	wData;	// at most 16 bit, at least 4 bit
}SharedParams_Back_Others;

// 128 (temp)
typedef struct tagSharedParams_ReadbackList
{
	BYTE	nType;	// 0:ADC; 1:gauge compr; 2:VAC switch; 3:SPR switch; 4:SPR power; 5:Board ID; 6:ISR; 7:MCI Pwr
	BYTE	nNum;	// 0:SCN; 1:LNS; 2:VAC#0; 3:VAC#1; 4:SPR#0; 5:SPR#1
	BYTE	nMID;	// 1:SCN; 2:LNS; 3:VAC; 4:SPR
	BYTE	nChn;
	BOOL	bAnalyze;
}SharedParams_Record_List;

// 共享数据
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//====== PC -> MCI BOARD MessageID ==============================

#define		USB_Flag_Bit0		(BYTE)0x01
#define		USB_Flag_Bit1		(BYTE)0x02
#define		USB_Flag_Bit2		(BYTE)0x04
#define		USB_Flag_Bit3		(BYTE)0x08

#define		USB_Flag_Bit4		(BYTE)0x10
#define		USB_Flag_Bit5		(BYTE)0x20
#define		USB_Flag_Bit6		(BYTE)0x40
#define		USB_Flag_Bit7		(BYTE)0x80

#define	WM_USB_SERIAL_RECV		(WM_USER	+51)	// Status byte from the controller is passed to user via the wParam argument
#define	WM_USB_SERIAL_SEND		(WM_USER	+52)

/*  MCI Machine Instructions
	(Peter S. Chao, Long River Tech, LLC, 
	May 7, 2010: Initial starter set
	Jan. 22, 2011: Significant update)

This is a working document that will continue expand and evolve.
The machine instructions are crafted to fit the operations of
the Master Control and Interface (MCI) board using Acces IO Products' USB DI16A card
as an USB bridge to communicate with the host PC.
The 16 bit high speed "data" channel is used primarily for
streaming video pixels from MCI to the host.
The 18 bits of slow IO pins are used for
sending instructions from host to MCI, and receiving responses from MCI to host.

0.	ACCES IO Basics:
On power up, all ACCES IO ports are tri-stated.
The host configure the IO ports according to
"Hard Coded (Embedded) Configuration" - ACCES IO User Manual, Page 21,
using function "DIO_ConfigureEx".
Configure Ports A and C as output, and Ports B and D as input,
and the high speed data channel as input. Usage of the slow ports are as:

	Port C[2..0] - host command code to MCI
	Port A[7..0] - opcode or operand from host to MCI
	Port D[2..0] - response code from MCI to host
	Port B[3..0] - response data from MCI to host

Note: Ports C and D are always in opposite directions.

After configuration, un-tristate all ports.
(Suggestion: Gain full understanding of DIO_ConfigureEx from ACCES IO sample code.)

From here on, us the function "DIO_WriteAll"
for sending instructions to MCI via 32bit DIOData.  Format for DIOData follows:

Byte_D, Byte_C, Byte_B, Byte_A

Byte_D:  LSb 3 bits for receiving MCI response code
Byte_C:  LSb 3 bits for sending command code to MCI 
Byte_B:  LSb 4 bits for receiving MCI response data
Byte_A:  all 8 bits for sending opcode or operand to MCI

Example: DIO_WriteAll sends 3bit command code 0b101,
		 and 8bit opcode 0b00010001 to MCI by setting 

 	DioData = 0b11111111,11110101,11111111,00010001, or
			= 0xFFF5FF11

I.	Command protocol from host to MCI:
A host command consists of a command code (cc), an operation code (opcode),
and zero or one operand, which can be one or two bytes long.

 	Command code: C[2..0] =	0b000 - Clear
 						0b001 - Operand low byte in Channel A
 						0b010 - Operand high byte in Channel A
 						0b011 - TBD
 						0b100 - Opcode in Channel A, no operand
 						0b101 - Opcode in Channel A, operand to follow
 						0b111 - Simultaneous update of opcode and operand to MCI
 	Sending a command:
 	A command is sent in three to five steps, depending on the related operand.
 	 	For instruction with opcode only, no operand
 		Step 1:	Set cc = 0b100 from Channel C
 				Set opcode in Channel A
		Step 2:	Set cc = 0b111 in Channel C
		Step 3:	Set cc = 0b000 in Channel C
 
 		For instructions with a 2-byte long operand
  		Step 1:	Set cc = 0b101 in Channel C
 				Set opcode in Channel A
 		Step 2:	Set cc = 0b001 in Channel C
 				Set operand low byte in Channel A
 		Step 3:	Set cc = 0b010 in Channel C
 				Set operand high byte in Channel A
		Step 4:	Set cc = 0b111 in Channel C
		Step 5:	Set cc = 0b000 in Channel C

II.	Instruction for setting pixel clock rate:	
	MCI uses a Phase Lock Loop (PLL) circuit to generate,
	from an 40MHz external oscillator, to generate
	a 32MHz systems clock, and a 8MHz base pixel clock as well.
	In addition, a custom clock generator is used to produce variable pixel clocks on demand.
	This is a "divide by N" clock generator, where N = 1, 2, 3, …, 2K,
	with 50% duty cycle for even N, off by 1 tick for odd N.
	For a 8MHz base, it means the pixel clock can be set from 8MHz to 4KHz.  

	The pixel rate in units of 1/8 MHz, namely 125nS.
	Pixel rate = N means N * 125nS for pixel time.

 	Instruction Set_Pixel_Rate:	Opcode = 0x11
 								Operand =0b0000,RRRR, high byte,0bRRRR,RRRR low byte

IV.	Instructions for setting scan ramps:
	A 4-segment scan ramp consists of the following segments:

 	Vo - Offset in DAC unit for the scan DAC.  MCI uses 16 bit DACs for scanning,
		so the units range from 0 to 64K.  MSB 14 bits are used by the scan DAC.

 	valley	-	number of pixels for staying at DAC value Vo
 
	rise	-	number of pixels for climbing up the ramp

 	dRise	-	for each pixel, how many DAC units to climb

	peak	-	number of pixels for staying at the top of the ramp

 	fall	- 	number of pixels for coming down the ramp

	dFall	-	for each pixel, how many DAC units to descent
*/
#define	USB_SetPixelRate							0x11
#define	USB_StartScan								0x12
#define	USB_StopScan								0x13
#define	USB_UpdateXramp								0x14
#define	USB_UpdateYramp								0x15
#define	USB_SetDefaultRamps							0x16
#define	USB_UpdateRotation							0x17
#define	USB_RotationOFF								0x18
#define	USB_RotationON								0x19
#define	USB_RotationSetCos							0x1A
#define	USB_RotationSetSin							0x1B

#define	USB_SetXoffset								0x21
#define	USB_SetXvalley								0x22
#define	USB_SetXrise								0x23
#define	USB_SetdXrise								0x24
#define	USB_SetXpeak								0x25
#define	USB_SetXfall								0x26
#define	USB_SetdXfall								0x27
#define	USB_SetYoffset								0x28
#define	USB_SetYvalley								0x29
#define	USB_SetYrise								0x2A
#define	USB_SetdYrise								0x2B
#define	USB_SetYpeak								0x2C
#define	USB_SetYfall								0x2D
#define	USB_SetdYfall								0x2E

#define	USB_Det03StoreDAC							0x31
#define	USB_Det03LoadDACs							0x32
#define	USB_Det45StoreDAC							0x33
#define	USB_Det45LoadDACs							0x34
#define	USB_SetDetFilter							0x35
#define	USB_SetDetGain								0x36

#define	USB_VideoXYZ								0x41
#define	USB_VideoZZZ								0x42
#define	USB_ScanModeNormal							0x43
#define	USB_ScanModeLineHorizontal					0x44
#define	USB_ScanModeLineVertical					0x45
#define	USB_ScanModeSpot							0x46

#define	USB_ReadVersion								0x48	// no operand, MCI returns: port D 0x01, port B MCI version number

#define	USB_FreezeX									0x49
#define	USB_FreezeY									0x4A

/*
Opcodes:	0x49 - freezeX, operand = pixel number
				MCI computes Xramp DAC value as Xoffset + pixel number * dXrise
			0x4A - freezeY, operand = line number
				MCI computes Yramp DAC value as Yoffset + line number * dYrise

			0x43 - normal scan mode, no operand
			0x44 - horizotal line mode, line number given by "freezeY"
			0x45 - vertical line mode, line number given by "freezeX"
			0x46 - spot mode, with spot at (freezeX, freezeY)
*/

#define	USB_DuelDet									0x4B	// open duel detector
#define	USB_SingleDet								0x4C	// close duel detector

#define	USB_Video_ON								0x4D
#define	USB_Video_OFF								0x4E

#define	USB_DetPairSelection						0x55

/*
For detector pair selection:      
Opcode:		x55,
operand:	x00	-	Detector pair 0-1, default (Note, old default was pair 4-5)
			x01	-	Detector pair 2-3
			x02	-	Detector pair 4-5
			x03	-	spare
input connector:	J5A - pair0-1, J5B - pair2-3, J6A - pair4-5
*/
#define USB_Read_Pwr								0x56

///////////////////////////////////////////////////////////////////////////////
/*
I.  Set interrupt mask register (IMR)
Opcode - 0xF4, operand - LSB[5..0], one bit of each interrupt source.
Bit set to 1 to enable, 0 to mask off.
	Bit 0 -	INT 4 ( IMR = 0x01 ), for SPR board, highest priority, cB will be 0x04
	Bit 1 -	INT 3 ( IMR = 0x02 ), for VAC board, next higher priority, cB will be 0x03
	Bit 2 -	INT 2 ( IMR = 0x04 ), for LNS board, no interrupt from it at present; MCI skips it,
									SW can also set it to 0, cB will be 0x02
	Bit 3 -	INT 1 ( IMR = 0x08 ), for SCN board, cB will be 0x01
	Bit 4 -	INT 5, spare
	Bit 5 -	INT 6, spare

II.  Protocol
1.	On system starting up, host software masks off appropriate bits in the IMR for wanted time
	duration, then sets right bits to enable wanted interrupts.
2.	MCI monitors interrupts from down-stream boards and notifies the host .  If interrupts come 
	from more than one board, the higher priority one will be handled first.

	Case A - while scanning, MCI sets the LSB bit, Z[0], of the video data to 1, USB bridge Channel
				cD to 2, Channel cB to INT number.
				Upon detecting Z[0]=1, host software services required interrupt of the 
				corresponding board indicated by the INT number.
				Once service is done, software sends acknowledgement to MCI by sending "clear_DB"
				command, opcode=ox47, no operand.
				MCI then stops setting Z[0] to 1, and clears cD and cB .
	Case B - while not scanning, host software monitors cD and cB via periodic polling, services
				the interrupt and send MCI "clear_DB" when done.
3.	Send host cD = 3, cB = 1 when BoardCommReady is asserted.
4.	MCI sends active INT# corresponding to the "1" bits in the "INT_mask (IMR)", with
	bit0 having the highest priority, bit1 next highest, etc. It then waits for the host ACK
	(clear_DB).
	Example: IMR=000111, INT4 and INT3 are active, cB=0x4 will be sent to the host
	first. Upon receiving host ACK, cB=0x4 will be sent again if INT4 remains active.
	Otherwise cB=0x3 will be sent.

For temporary testing with MCI only, use command INT_test, opcode 0xF5.
MCI will send 2 interrupts to the host: INT4 and INT1.
*/
#define	USB_ClearDB									0x47	// clear port B & D, no operand
#define	USB_SetIMR									0xF4	// set interrupt mask regiter
//#define	USB_INT_test								0xF5
//#define	USB_Fake_Mains								0xF6	// MCI generates a fake Mainslock pulse to start the next Xramp for one line

#define	USB_MainsLock_ON							0x1C
#define	USB_MainsLock_OFF							0x1D

#define	USB_MCI_COMM_RESET							0xF2	// MCI communicate reset

///////////////////////////////////////////////////////////////////////////////
/*
Individual interrupt testing:

INT_test_ON - Opcode 0xF5
INT_test_OFF- Opcode 0xF6

INT_test_SPR - Opcode 0xF7, MCI sets (D,B) to (4,2)
INT_test_VAC - Opcode 0xF8, MCI sets (D,B) to (3,2)
INT_test_LNS - Opcode 0xF9, MCI sets (D,B) to (2,2)
INT_test_SCN - Opcode 0xFA, MCI sets (D,B) to (1,2)

Process:
	INT_test_ON,
		INT_test_XXX,  Clear_DB
		INT_test_YYY,  Clear_DB
		. . . . 
	INT_test_OFF
*/
#define	USB_INT_test_ON								0xF5
#define	USB_INT_test_OFF							0xF6
#define	USB_INT_test_SPR							0xF7
#define	USB_INT_test_VAC							0xF8
#define	USB_INT_test_LNS							0xF9
#define	USB_INT_test_SCN							0xFA
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/*
Line Sync and Frame Sync:

In case a clean mainslock signal is not available, a fake mainslock signal internally generated
in MCI can be used for testing.  This signal, fakemains,  is a square wave currently set at
about 500Hz for easier scoping.
MCI selects either normal mainslock input signal or generate fakemains as mains_pulse, depending ON whether the command fakemains_ON/OFF has been invoked.
Default is fakemains OFF.
A short pulse (1 pixel clock wide), mains_pulse_X is extracted from the rise edge of,
mains_pulse as for line sync, and a wider one, mains_pulse_Y, for frame sync.

Commands:	mainslock_ON - 0x1C
			mianslock_OFF - 0x1D
			line_sync option -0x1E
			frame_sync option - 0x1F
			the above are mutually exclusive, line_sync is the default;
			upon receiving one of the two commands, MCI automatically turns the
			other OFF.
			fakemains_ON - 0xFB
			fakemains_OFF - 0xFC
*/
#define	USB_MainsLock_LineSync						0x1E
#define	USB_MainsLock_FrameSync						0x1F
#define	USB_FakeMains_ON							0xFB
#define	USB_FakeMains_OFF							0xFC
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//====== PC -> SUB BOARD MessageID ==============================
/*  3200 SEM SubBoard Communications (Dan Downer, Long River Tech, LLC, Version Dec. 30, 2011)

Communication to all boards "downstream" from the MCI requires issuance of
at least two contiguously sequential instruction clusters with operands.
Some operations require three or more instruction clusters, depending on the purpose.
For example, 24 bit writes require 3 instruction clusters to be written,
and reads require additional interaction with the MCI's USB interface.  

Each instruction cluster is comprised of five back-to-back 32 bit writes
to the MCI board over the USB interface.
The opcode and operands are sent to the MCI using DIO bytes C and A as follows:


0xFFF5FFF1  		(Instruction for subboard comm.)
0xFFF1FF00 Logically OR'ed with 8 bits of Operand 1 data 
0xFFF2FF00 Logically OR'ed with 8 bits of Operand 2 data
0xFFF7FF00 			(initiate the transfer)
0xFFF0FF00 			(finalize the transfer)


The first instruction cluster defines the transfer type and circuit selection.
Subsequent instruction clusters define the data sent from the MCI to the destination circuit.
One additional instruction cluster is sufficient for 8 bit and 16 bit writes.
A third instruction cluster is required for 24 bit and 32 bit writes.
(At the time this document was written, there have been no 32 bit writes required,
 but several 24 bit writes are implemented.)

Transfer type (TT), module selection (MMMM) and circuit selection (CCCCCCCC):
TTTTMMMMCCCCCCCC

TT[3..0]	= binary 0001 for 8 bit write, 0010 for 16 bit write, 0011 for 24 bit write, 0100 for 32 bit write
			= binary 1001 for 8 bit read, 1010 for 16 bit read (1 word back to MCI)
			= binary 1011 for 24 bit read, 1100 for 32 bit read* (2 words back to MCI)
				* 32 bit reads not necessarily supported by all boards as of 12-3-11
			= binary 0101 for clearing of INT's from subboards (data selects which INT to clear)

M[3..0] = module selection:
MCI (local) address: 1101 (for diagnostic tests only - not for streaming video)
SCN board address:	0001
LNS board address:	0010
VAC board address:	0011
SPR module address:	0100

C[7..0] = Circuit selection (see individual module sections for details)
*/
//===========================================================
// Transfer type
//===========================================================
#define	USB_TTTT_Write_08							0x01	// 8 bit write
#define	USB_TTTT_Write_16							0x02	// 16 bit write
#define	USB_TTTT_Write_24							0x03	// 24 bit write
#define	USB_TTTT_Write_32							0x04	// 32 bit write

#define	USB_TTTT_Read_08							0x09	// 8 bit read
#define	USB_TTTT_Read_16							0x0A	// 16 bit read
#define	USB_TTTT_Read_24							0x0B	// 24 bit read
#define	USB_TTTT_Read_32							0x0C	// 32 bit read

#define	USB_TTTT_INT								0x02	// for clearing of INT's from subboards (data selects which INT to clear)
#define	USB_TTTT_Mon_Err							0x03	// have error when read monitor data
#define	USB_TTTT_Switches							0x04	// read selected external switch state


//===========================================================
// MID --- Module Selection
//===========================================================
#define	USB_MID_MCI1								0x0D	// MCI (local), for diagnostic tests only - not for streaming video
#define	USB_MID_MCI2								0x0E	// MCI (local), for enable 262ms inactivity timer self-reset of communication

#define	USB_MID_SCN									0x01	// SCN board
#define	USB_MID_LNS									0x02	// LNS board
#define	USB_MID_VAC									0x03	// VAC board
#define	USB_MID_SPR									0x04	// SPR module


//===========================================================
// TTTTMMMM --- Type & Module Selection
//===========================================================
#define	USB_TM_SCN_Write_08							0x11
#define	USB_TM_SCN_Write_16							0x21
#define	USB_TM_SCN_Write_24							0x31
#define	USB_TM_SCN_Write_32							0x41
#define	USB_TM_SCN_Read_08							0x91
#define	USB_TM_SCN_Read_16							0xA1
#define	USB_TM_SCN_Read_24							0xB1
#define	USB_TM_SCN_Read_32							0xC1

#define	USB_TM_LNS_Write_08							0x12
#define	USB_TM_LNS_Write_16							0x22
#define	USB_TM_LNS_Write_24							0x32
#define	USB_TM_LNS_Write_32							0x42
#define	USB_TM_LNS_Read_08							0x92
#define	USB_TM_LNS_Read_16							0xA2
#define	USB_TM_LNS_Read_24							0xB2
#define	USB_TM_LNS_Read_32							0xC2

#define	USB_TM_VAC_Write_08							0x13
#define	USB_TM_VAC_Write_16							0x23
#define	USB_TM_VAC_Write_24							0x33
#define	USB_TM_VAC_Write_32							0x43
#define	USB_TM_VAC_Read_08							0x93
#define	USB_TM_VAC_Read_16							0xA3
#define	USB_TM_VAC_Read_24							0xB3
#define	USB_TM_VAC_Read_32							0xC3

#define	USB_TM_SPR_Write_08							0x14
#define	USB_TM_SPR_Write_16							0x24
#define	USB_TM_SPR_Write_24							0x34
#define	USB_TM_SPR_Write_32							0x44
#define	USB_TM_SPR_Read_08							0x94
#define	USB_TM_SPR_Read_16							0xA4
#define	USB_TM_SPR_Read_24							0xB4
#define	USB_TM_SPR_Read_32							0xC4

/*
Operand data alignment:

Each instruction cluster of 5 USB write transfers to the MCI board allow up to two operands of data.
The first cluster's operands 1 and 2 specify the transfer type,
module selection and circuit selection as described previously.
The second (and third if necessary) instruction(s)' operands 3 and 4,
and 5 and 6 if necessary, contain data to be written to the selected module and circuit.  

All reads of subboards include Operands 3 and 4 for write data
for preconditioning the circuitry for the read in process.
This is necessary for operations such as reading a multi-channel ADC (analog to digital converter)
that must first be instructed to select a channel or conversion mode.
In other cases no write data is necessary, in which case it is recommended to set Operands 3 and 4 to 0.

Instruction cluster #1 (always required):
0xFFF5FFF1	(Instruction for subboard comm.)
0xFFF1FF00 	Logically OR'ed with 8 bits of Operand 1 data 
0xFFF2FF00 	Logically OR'ed with 8 bits of Operand 2 data
0xFFF7FF00	(initiate the transfer)
0xFFF0FF00	(finalize the transfer)

Instruction cluster #2 (always required):
0xFFF5FFF1	(Instruction for subboard comm.)
0xFFF1FF00 	Logically OR'ed with 8 bits of Operand 3 data 
0xFFF2FF00 	Logically OR'ed with 8 bits of Operand 4 data
0xFFF7FF00	(initiate the transfer)
0xFFF0FF00	(finalize the transfer)

Third instruction cluster - send ONLY  for 24 bit or 32 bit write transfer types 
- DO NOT SEND a third instruction cluster for other transfer types!
0xFFF5FFF1	(Instruction for subboard comm.)
0xFFF1FF00 	Logically OR'ed with 8 bits of Operand 5 data 
0xFFF2FF00 	Logically OR'ed with 8 bits of Operand 6 data
0xFFF7FF00	(initiate the transfer)
0xFFF0FF00	(finalize the transfer)

Required contents of each operand per transfer type are shown in the table below.
Where "Set to 0" is indicated, operand value should be set to a value of 0.
Note that data operands (Operands 3 through 6) contain different byte fields
depending on the type of transfer.  Where "xx" appears, operands 5 and 6 do not exist.

Transfer Type	Operand 1	Operand 2	Operand 3		Operand 4		Operand 5	Operand 6
8 bit write		CCCCCCCC	0001MMMM	Set to 0		data[7..0]		xx			xx
16 bit write	CCCCCCCC	0010MMMM	data[7..0]		data[15..8]		xx			xx
24 bit write	CCCCCCCC	0011MMMM	data[15..8]		data[23..16]	Set to 0	data[7..0]
32 bit write	CCCCCCCC	0100MMMM	data[23..16]	data[31..24]	data[7..0]	data[15..8]
All reads		CCCCCCCC	TTTTMMMM	data[7..0]		data[15..8]		xx			xx
Clear INT		CCCCCCCC	0101MMMM	Set to 0		data[7..0]		xx			xx
*/
//===========================================================
// CID --- Circuit Selection
//===========================================================
//																Write		Read	Data Format / Usage Comments
// SCN board
#define	USB_CID_SCN_Stigmation						0x12	// 16 bit	-			MSB: Set to 0 for X Stig, set to 1 for Y Stig.  12 bit value in bits 11...0

#define	USB_CID_SCN_Xshift_Crossover				0x22	// 16 bit	-			MSB: Set to 0 for X shift, set to 1 for X Crossover Scaling value.  12 bit value in bits 11..0
#define	USB_CID_SCN_Yshift_Crossover				0x32	// 16 bit	-			MSB: Set to 0 for Y shift, set to 1 for Y Crossover Scaling value.  12 bit value in bits 11..0

#define	USB_CID_SCN_XMainGain_FeedbackSetting		0x24	// 8 bit	-			Bits 3..2 select X main path feedback, Bits 1..0 select gain.
#define	USB_CID_SCN_XLowRangeFeedbackSelect			0x26	// 8 bit	-			Bits 3..0 select feedback for X low range path
#define	USB_CID_SCN_XLowRangeAnalogMultiplexerSel	0x28	// 8 bit	-			Bits 3..0 select X low range mux

#define	USB_CID_SCN_YMainGain_FeedbackSetting		0x34	// 8 bit	-			Bits 3..2 select Y main path feedback, Bits 1..0 select gain.
#define	USB_CID_SCN_YLowRangeFeedbackSelect			0x36	// 8 bit	-			Bits 3..0 select feedback for Y low range path
#define	USB_CID_SCN_YLowRangeAnalogMultiplexerSel	0x38	// 8 bit	-			Bits 3..0 select Y low range mux

#define	USB_CID_SCN_ExpanderChip					0x72	// 8 bit	-			Set bit 7 to strobe to High, Set bit 6 to strobe to Low.  Bits 4..0 select output pin to be changed.

#define	USB_CID_SCN_ADC_Write						0xC2
#define	USB_CID_SCN_ADC_Busy						0xC5	// 16 bit	-	8 bit	Bit 0 = 1 if ADC is busy (conversion not done), Bit 0 = 0 if ADC data is ready to send data to host
#define	USB_CID_SCN_ADC_Read						0xC8	// 16 bit	-	32 bit	Upper 13 bits written at the beginning of a read sequence.  Read clocks shared with write clocks, one cycle delayed.
#define	USB_CID_SCN_Test_LED						0xD0	// 8 bit	-			Bit 0	

#define	USB_CID_SCN_ReadBoardID						0xE1	//			-	8 bit	Initial LRT released SCN board value is fixed at 0xA1
#define	USB_CID_SCN_Soft_Reset						0xEE	// 8 bit	-			Data-less write causes "soft" reset/initialization of SCN board.  DAC's are set to midrange, outputs off.

// ExpanderChip definition
#define	SCN_ExpanderChip_YGainRlyCtrl				0x01	// SCN_ExpanderChip_Xcros1
#define	SCN_ExpanderChip_Xcros2						0x02
#define	SCN_ExpanderChip_Xcros3						0x03
#define	SCN_ExpanderChip_Xcros4						0x04
#define	SCN_ExpanderChip_XcrossFBsel1				0x05
#define	SCN_ExpanderChip_XcrossFBsel2				0x06
#define	SCN_ExpanderChip_XcrossFBsel3				0x07
#define	SCN_ExpanderChip_XcrossFBsel4				0x08
#define	SCN_ExpanderChip_Ycros1						0x09
#define	SCN_ExpanderChip_Ycros2						0x0A	// 10
#define	SCN_ExpanderChip_Ycros3						0x0B	// 11
#define	SCN_ExpanderChip_Ycros4						0x0C	// 12
#define	SCN_ExpanderChip_YcrossFBsel1				0x0D	// 13
#define	SCN_ExpanderChip_YcrossFBsel2				0x0E	// 14
#define	SCN_ExpanderChip_YcrossFBsel3				0x0F	// 15
#define	SCN_ExpanderChip_YcrossFBsel4				0x10	// 16
#define	SCN_ExpanderChip_XGainRlyCtrl				0x11	// 17
#define	SCN_ExpanderChip_XShortRlyCtrl				0x12	// 18
#define	SCN_ExpanderChip_XAmpStigESCtrl				0x13	// 19
#define	SCN_ExpanderChip_XMainRlyCtrl				0x14	// 20
#define	SCN_ExpanderChip_XMainCathCtrl				0x15	// 21
#define	SCN_ExpanderChip_XMainAmpESCtrl				0x16	// 22
#define	SCN_ExpanderChip_XShiftESCtrl				0x17	// 23 SCN_ExpanderChip_XCrosESCtrl
#define	SCN_ExpanderChip_XLowAmpESCtrl				0x18	// 24
#define	SCN_ExpanderChip_YShortRlyCtrl				0x19	// 25
#define	SCN_ExpanderChip_YAmpStigESCtrl				0x1A	// 26
#define	SCN_ExpanderChip_YMainRlyCtrl				0x1B	// 27
#define	SCN_ExpanderChip_YMainCathCtrl				0x1C	// 28
#define	SCN_ExpanderChip_YMainAmpESCtrl				0x1D	// 29
#define	SCN_ExpanderChip_YShiftESCtrl				0x1E	// 30 SCN_ExpanderChip_YCrosESCtrl
#define	SCN_ExpanderChip_YLowAmpESCtrl				0x1F	// 31
// ExpanderChip definition

//===========================================================
// LNS board
#define	USB_CID_LNS_CondenserLens					0x12	// 16 bit	-			Bits 1 and 0 are ignored if LTC1591-1 type DAC is used instead of LTC1597-1.
#define	USB_CID_LNS_CondenserLens_LoopTest			0x1A	// 8 bit	-			
#define	USB_CID_LNS_CondenserLens_Wobbler			0x1C	// 8 bit	-			Set bit 0 to 1 to enable wobbler to modulate Condenser lens.
#define	USB_CID_LNS_CondenserLens_OutputDrive		0x1E	// 8 bit	-			

#define	USB_CID_LNS_ObjectiveLens_Static			0x22	// 16 bit	-			Bits 1 and 0 are ignored if LTC1591-1 type DAC is used instead of LTC1597-1.
//#define	USB_CID_LNS_ObjectiveLens_DynFocusPolarity	0x24	// 8 bit	-			Set bit 0 to 1 to INCREASE value each scan line, set bit 0 to 0 to DECREASE value each scan line.
#define	USB_CID_LNS_ObjectiveLens_DynFocusPolarityAtten		0x28	// 8 bit	-			Set bit 1 to 1 to activate full range dynamic focus, 0 to disable.  Set bit 0 to 1 to activate 10% range dynamic focus.
#define	USB_CID_LNS_ObjectiveLens_LoopTest			0x2A	// 8 bit	-			
#define	USB_CID_LNS_ObjectiveLens_Wobbler			0x2C	// 8 bit	-			Set bit 0 to 1 to enable wobbler to modulate Objective lens.
#define	USB_CID_LNS_ObjectiveLens_OutputDrive		0x2E	// 8 bit	-			

#define	USB_CID_LNS_ObjectiveLens_DynFocusSync		0x30	// 8 bit	-			Data-less write signals Objective Lens dynamic focus circuit that a new scan line has just been started.
#define	USB_CID_LNS_ObjectiveLens_DynFocusSteps		0x32	// 16 bit	-			Objective Lens dynamic focus change per 125nS, in 14 bit DAC steps
#define	USB_CID_LNS_ObjectiveLens_DnyFocusStartPoint 0x34	// 16 bit	-			Objective Lens dynamic focus level at start of line, scaled the same as DAC settings (bits 1:0 ignored).
#define	USB_CID_LNS_ObjectiveLens_DnyFocusEndBound	0x36	// 16 bit	-			Objective Lens dynamic focus level at end of line, scaled the same as DAC settings (bits 1:0 ignored).

#define	USB_CID_LNS_XAlignment						0x42	// 16 bit	-			
#define	USB_CID_LNS_XAlignment_LoopTest				0x4A	// 8 bit	-			
#define	USB_CID_LNS_XAlignment_Wobbler				0x4C	// 8 bit	-			
#define	USB_CID_LNS_XY_OutputDrive					0x4E	// 8 bit	-			

#define	USB_CID_LNS_YAlignment						0x52	// 16 bit	-			
#define	USB_CID_LNS_YAlignment_LoopTest				0x5A	// 8 bit	-			
#define	USB_CID_LNS_YAlignment_Wobbler				0x5C	// 8 bit	-			

#define	USB_CID_LNS_WobblerSetting					0x62	// 16 bit	-			12 bit value sets Wobbler level

#define	USB_CID_LNS_ADC_Write						0xC2	// 16 bit	-			
#define	USB_CID_LNS_ADC_HD_Start_Conversion			0xC4	// 16 bit	-			Can start conversion independently, then wait for required conversion time, then read.
#define	USB_CID_LNS_ADC_Read						0xC8	// 16 bit	-	16 bit	Writing while reading results in data being returned to host from previous conversion.
#define	USB_CID_LNS_Test_LED						0xD0	// 8 bit	-			Set bit 0 to 1 to test front panel LED 4 (near term engineering use only - will be reassigned at a later date)

#define	USB_CID_LNS_ReadBoardID						0xE1	//			-	8 bit	Initial LRT released LNS board value is fixed at 0xA2
#define	USB_CID_LNS_Soft_Reset						0xEE	// 8 bit	-			Data-less write causes "soft" reset/initialization of LNS board.  DAC's are set to midrange, outputs off.

//===========================================================
// VAC board
#define	USB_CID_VAC_CalCur_TC0_3					0x12	// 24 bit	-			ChA: TC0 Cal I, ChB: TC1 Cal I, ChC: TC2 Cal I, ChD: TC3 Cal I  (DAC type: DAC7565)
#define	USB_CID_VAC_CompAdjPoints_TC0_3				0x22	// 24 bit	-			ChA: TC0 comparator adjustment, ChB: TC1 adj, ChC: TC2 adj, ChD: TC3 adj (DAC type: DAC7565)
#define	USB_CID_VAC_CalCur_CompAdjPoints_TC4_5		0x32	// 24 bit	-			ChA: TC5 Cal I, ChB: TC6 Cal I, ChC: TC5 comparator adjustment, ChD: TC6 adj (DAC type: DAC7565)

#define	USB_CID_VAC_Relay00Enable					0x40	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay01Enable					0x41	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay02Enable					0x42	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay03Enable					0x43	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay04Enable					0x44	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay05Enable					0x45	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay06Enable					0x46	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay07Enable					0x47	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay08Enable					0x48	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay09Enable					0x49	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay10Enable					0x4A	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay11Enable					0x4B	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off

//#define	USB_CID_VAC_Auxiliary_IO_Comp				0x52	//			-	8 bit	2 lines, Aux1 and Aux0 (parallel read)

#define	USB_CID_VAC_ExpanderChip					0x72	// 8 bit	-			Set bit 7 to strobe to High, Set bit 6 to strobe to Low.  Bits 5..0 select output pin to be changed.

//#define	USB_CID_VAC_ReadExternalSwitch				0x92	//			-	8 bit	8 lines, SwClrIn[7..0] (parallel read)
#define	USB_CID_VAC_ReadThermoCCGComp				0x94	// 16 bit	-	16 bit	Bits 13 & 12 are CCG comparators, Bits 11..0 are TC comparators 5 through 0 respectively (B/high and A/low per channel)
#define	USB_CID_VAC_ReadSelExtSwitchState			0x98	//			-	16 bit	The switch to be read is selected by the VAC Expander, setting outputs 0x19, 0x1A and 0x1B to select the switch

#define	USB_CID_VAC_INT_Enable_Register				0xA1	// 16 bit	-			Each bit enables a designated interrupt source (see "Interrupts" tab for details)
#define	USB_CID_VAC_INT_Source_Register				0xA4	//			-	16 bit	Identifies which (if any) enabled interrupts are currently being asserted. Reading this register clears all bits unless condition persists.
//#define	USB_CID_VAC_ExtSwitch_Polarity_Selects		0xA8	// 8 bit	-			Bits 0 through 7 choose whether each external switch generates an (enabled) interrupt in CLOSED or OPEN positions

#define	USB_CID_VAC_ADC0_Write						0xC2	//						It is possible that no dedicated writes will be needed since writes are also part of read sequence.
#define	USB_CID_VAC_ADC01_Busy						0xC5	// 16 bit	-	16 bit	Bit 0 = 1 if ADC #0 is busy (conversion not done), Bit 0 = 0 if ADC #0 data is ready to send data to host. Bit 1 same for ADC #1
#define	USB_CID_VAC_ADC0_Read						0xC6	// 16 bit	-	32 bit	Upper 13 bits written at the beginning of a read sequence.  Read clocks shared with write clocks, one cycle delayed.
#define	USB_CID_VAC_ADC1_Write						0xCA	//						It is possible that no dedicated writes will be needed since writes are also part of read sequence.
//#define	USB_CID_VAC_ADC1_Busy						0xCD	// 16 bit	-	8 bit	Bit 0 = 1 if ADC #1 is busy (conversion not done), Bit 0 = 0 if ADC #1 data is ready to send data to host
#define	USB_CID_VAC_ADC1_Read						0xCE	// 16 bit	-	32 bit	Upper 13 bits written at the beginning of a read sequence.  Read clocks shared with write clocks, one cycle delayed.

#define	USB_CID_VAC_Test_LED						0xD0	// 8 bit	-			Set bit 0 to 1 to test front panel LED 4 (near term engineering use only - will be reassigned at a later date)

#define	USB_CID_VAC_ReadBoardID						0xE1	//			-	16 bit	Initial LRT released VAC board value is fixed at 0xA3A3
#define	USB_CID_VAC_Soft_Reset						0xEE	// 8 bit	-			Data-less write causes "soft" reset/initialization of VAC board.  DAC's are midranged. Most drives set to off.

//===========================================================
// SPR module
#define	USB_CID_SPR_AnalogPower_Enable				0x10	// 8 bit	-			Set bit 0 to 1 to begin orderly analog power-up sequence.  Set bit 0 to 0 to begin analog power shutdown sequence.
#define	USB_CID_SPR_General_Purpose_DACs			0x14	// 24 bit	-			ChA: PMT, ChB: AnOut2, ChC: AnOut3, ChD: AnOut4  (DAC type: DAC8565)

#define	USB_CID_SPR_Fan_PWM_Control					0x44	// 8 bit	-			Set FF for full speed control of fans, 0 for off, other values for PWM speed control
#define	USB_CID_SPR_ExpanderChip					0x72	// 8 bit	-			Set bit 7 to strobe to High, Set bit 6 to strobe to Low.  Bits 5..0 select output pin to be changed.

#define	USB_CID_SPR_PowerStatus_Readback			0x80	// 16 bit	-	8 bit	Bits 2..0 Report status of Emerson Power Supply ACOK and DCOK signals, Fan OK

#define	USB_CID_SPR_ReadExternalSwitch				0x92	// 16 bit	-	8 bit	2 lines, SwClrIn[1..0] (parallel read) (Note: could be expanded to four in future if necessary)

#define	USB_CID_SPR_INT_Enable_Register				0xA1	// 8 bit	-			Each bit enables a designated interrupt source
#define	USB_CID_SPR_INT_Source_Register				0xA4	//			-	8 bit	Identifies which (if any) enabled interrupts are currently being asserted. Reading this register clears all bits unless condition persists.
#define	USB_CID_SPR_ExtSwitch_Polarity_Selects		0xA8	// 8 bit	-			Bits 0 through 3 choose whether each external switch generates an (enabled) interrupt in CLOSED or OPEN positions

#define	USB_CID_SPR_Legacy_HV_Power					0xB4	// 16 bit	-			This is for optional control of the older CPS Power Supply used in legacy systems.  9 bits controls 0-30kV output level.

#define	USB_CID_SPR_ADC0_Write						0xC2	//						It is possible that no dedicated writes will be needed since writes are also part of read sequence.
#define	USB_CID_SPR_ADC01_Busy						0xC5	//			-	8 bit	Bit 0 = 1 if ADC #0 is busy (conversion not done), Bit 0 = 0 if ADC #0 data is ready to send data to host. Bit 1 same for ADC #1
#define	USB_CID_SPR_ADC0_Read						0xC6	// 16 bit	-	32 bit	Upper 14 bits written at the beginning of a read sequence.  Read clocks shared with write clocks, one cycle delayed.
#define	USB_CID_SPR_ADC1_Write						0xCA	//						It is possible that no dedicated writes will be needed since writes are also part of read sequence.
//#define	USB_CID_SPR_ADC1_Busy						0xCD	//			-	8 bit	Bit 0 = 1 if ADC #1 is busy (conversion not done), Bit 0 = 0 if ADC #1 data is ready to send data to host
#define	USB_CID_SPR_ADC1_Read						0xCE	// 16 bit	-	32 bit	Upper 14 bits written at the beginning of a read sequence.  Read clocks shared with write clocks, one cycle delayed.

#define	USB_CID_SPR_Test_LED						0xD0	// 8 bit	-			Set bit 0 to 1 to test front panel LED 4 (near term engineering use only - will be reassigned at a later date)

#define	USB_CID_SPR_ReadBoardID						0xE1	//			-	8 bit	Initial LRT released SPR board value is fixed at 0xA4
#define	USB_CID_SPR_Soft_Reset						0xEE	// 8 bit	-			Data-less write causes "soft" reset/initialization of VAC board.  DAC's are midranged. Most drives set to off.

//===========================================================
// MCI (Test only - does not use comm link)
#define	USB_CID_MCI_ReadRevisionDate1				0x00	// 16 bit	-	16 bit	Data-less 8 bit write - returns Communications Firmware Revision Code - 4 nibbles: Year (1 digit) + R.RR (2103 as of 1-8-12)
#define	USB_CID_MCI_ReadRevisionDate2				0x00	//			-	16 bit	Returns 4 nibbles: M,DD,Y (last digit only)
#define	USB_CID_MCI_Readback16wordsofABCD			0x00	// 16 bit	-	16 bit	Data-less 8 bit write results in MCI reading back 16 words of 0xABCD
#define	USB_CID_MCI_EnableMarker					0x00	// 8 bit	-			Set bit 0 to 1 to enable host readback marker generation (One 16 bit word: 0xD22D) at start of ALL board readbacks.  As of 2-5-12, Marker preceding subboard readback data is ALWAYS enabled.
#define	USB_CID_MCI_Enable_SelfReset				0x00	// 8 bit	-			Set bit 0 to 1 to enable automatic self-reset of communications circuitry in the event of a software/hardware sync problem
#define	USB_CID_MCI_Reset_Init_Board_Comm			0x00	//						Note that this command uses a different MCI opcode: "F2" that is unique to this reset function only.

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////





#endif
