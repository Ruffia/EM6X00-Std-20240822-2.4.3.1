// COMMAPI_USB.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <memory.h>
#include <math.h>

#include <COMMAPI_USB.h>
#include <USB_Functions.h>

#include <AIOUSB.h>
#include <WinUSB.h>

//#include "CriticalResource.h"
#include <afxmt.h>

#ifdef _DEBUG

#define MJY2010_DEBUG
//#define DEBUG_MCI_ACQDATA_TEST

#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef enum USB_SCANNING_FUCTION
{
	SF_1024_768,
    AnalogyPhoto, //with Image scanning
    DigitalPhoto  //without Image scanning
} USB_SCANNING_FUCTION;

typedef struct tagUSB_U32BITS{
	DWORD	Bit0	: 1;	// 00000000 00000000 00000000 0000000?
	DWORD	Bit1	: 1;	// 00000000 00000000 00000000 000000?0
	DWORD	Bit2	: 1;	// 00000000 00000000 00000000 00000?00
	DWORD	Bit3	: 1;	// 00000000 00000000 00000000 0000?000
	DWORD	Bit4	: 1;	// 00000000 00000000 00000000 000?0000
	DWORD	Bit5	: 1;	// 00000000 00000000 00000000 00?00000
	DWORD	Bit6	: 1;	// 00000000 00000000 00000000 0?000000
	DWORD	Bit7	: 1;	// 00000000 00000000 00000000 ?0000000
	DWORD	Bit8	: 1;	// 00000000 00000000 0000000? 00000000
	DWORD	Bit9	: 1;	// 00000000 00000000 000000?0 00000000
	DWORD	Bit10	: 1;	// 00000000 00000000 00000?00 00000000
	DWORD	Bit11	: 1;	// 00000000 00000000 0000?000 00000000
	DWORD	Bit12	: 1;	// 00000000 00000000 000?0000 00000000
	DWORD	Bit13	: 1;	// 00000000 00000000 00?00000 00000000
	DWORD	Bit14	: 1;	// 00000000 00000000 0?000000 00000000
	DWORD	Bit15	: 1;	// 00000000 00000000 ?0000000 00000000
	DWORD	Bit16	: 1;	// 00000000 0000000? 00000000 00000000
	DWORD	Bit17	: 1;	// 00000000 000000?0 00000000 00000000
	DWORD	Bit18	: 1;	// 00000000 00000?00 00000000 00000000
	DWORD	Bit19	: 1;	// 00000000 0000?000 00000000 00000000
	DWORD	Bit20	: 1;	// 00000000 000?0000 00000000 00000000
	DWORD	Bit21	: 1;	// 00000000 00?00000 00000000 00000000
	DWORD	Bit22	: 1;	// 00000000 0?000000 00000000 00000000
	DWORD	Bit23	: 1;	// 00000000 ?0000000 00000000 00000000
	DWORD	Bit24	: 1;	// 0000000? 00000000 00000000 00000000
	DWORD	Bit25	: 1;	// 000000?0 00000000 00000000 00000000
	DWORD	Bit26	: 1;	// 00000?00 00000000 00000000 00000000
	DWORD	Bit27	: 1;	// 0000?000 00000000 00000000 00000000
	DWORD	Bit28	: 1;	// 000?0000 00000000 00000000 00000000
	DWORD	Bit29	: 1;	// 00?00000 00000000 00000000 00000000
	DWORD	Bit30	: 1;	// 0?000000 00000000 00000000 00000000
	DWORD	Bit31	: 1;	// ?0000000 00000000 00000000 00000000
}USB_U32BITS;

typedef union tagUSB_U32DATA
{
	DWORD			dwData;		// DWORD dwData;
	USB_U32BITS		BITS;		// U32BITS BITS;
}USB_U32DATA;
/*
typedef enum _PLX_CHIP
{
    Invalid,
    Pci9080,
    Pci9054,
    Iop480
} PLX_CHIP;
*/
// Define -1 values for the different types
#define USB_MINUS_ONE_LONG           0xFFFFFFFFL
#define USB_MINUS_ONE_SHORT          0xFFFF
#define USB_MINUS_ONE_CHAR           0xFF

#ifdef _DEBUG
DWORD	USB_dwErrorCount = 0;
#endif


#ifdef DEBUG_MCI_ACQDATA_TEST

BOOL USB_DMATEST_OpenDMAChannel0();
BOOL USB_DMATEST_ImageInputStream( void * pBuffer, IN LONG dwBlockSize );

#endif //DEBUG_MCI_ACQDATA_TEST

//=============================================================================
// USB
#define	READ_CLOCK 0.0
#define WRITE_CLOCK 0.0

static	unsigned long	deviceIndex = 0;
static	DWORD			outMask = 0;
static	DWORD			inMask = 0;

BOOL	bVideoON = TRUE;
int		nLoopbackWords_Times = 0;

//CriticalResource		m_mutexPortsDI16A;
static	CCriticalSection		m_mutexPortsDI16A;
//=============================================================================
/*
static RETURN_CODE	DisableAllPlxIntr( HANDLE plxDevice );
static PLX_CHIP		ChipTypeGet( DEVICE_LOCATION device );

static PCI_MEMORY			g_MemoryInfo;
static DMA_TRANSFER_ELEMENT	dmaDataChannel0;
static DMA_TRANSFER_ELEMENT	dmaDataChannel1;
static HANDLE g_hPlxDevice = NULL;

static DWORD					dwBaseIOAddress;

static DWORD					dwChannel0UserAddr = NULL;
static DWORD					dwChannel1UserAddr = NULL;

volatile BOOL					blImageStreamInputStop = TRUE;
volatile BOOL					blPhotoStreamOutputStop = TRUE;
*/
/******************************************************************************
 *  FUNCTION    : ChipTypeGet
 *
 *  Description : This function returns the type of PLX chip that is opened
 *
 *****************************************************************************/
/*
PLX_CHIP
ChipTypeGet(DEVICE_LOCATION device)
{
    S8 serial[16];

    // create a temporary copy
    strcpy((char *)serial, (const char *)device.SerialNumber);
    
    // remove the enumerator suffix
    strtok( (char *)serial, "-" );
	strlwr( (char *)serial );		// 2002-7-9 Add by LMH for Plx 3.0 -> 3.2

    if (!strcmp((char *)serial, "pci9080"))
        return(Pci9080);
    else if (!strcmp((char *)serial, "pci9054"))
        return(Pci9054);
    else if (!strcmp((char *)serial, "iop480"))
        return(Iop480);
    else
        return (Invalid);
}
*/
/******************************************************************************
 *
 * Function Name: DisableAllPlxIntr()
 *
 * Description:
 *     This function will disable all interruptions of the given plxdevice.
 *     It will also reset the PLX_INTR structure.
 *
 ******************************************************************************/
/*
RETURN_CODE 
DisableAllPlxIntr(HANDLE plxDevice)
{
    RETURN_CODE		rc;
    PLX_INTR		intfield;

    // Fill the interrupt bitfield with all 1's
    memset( (void *) &intfield, 0xFF, sizeof(PLX_INTR));

    rc = PlxIntrDisable(plxDevice, &intfield);
    if (rc != ApiSuccess)
    {
		#ifdef _DEBUG
			afxDump << "\nDisableAllPlxIntr-- ";
			afxDump << "Error: DisableAll failed." << "\n";
			afxDump << "Returned code " << rc << "\n";
			afxDump << "\n		ErrorCount = " << dwErrorCount++ << "\n";
		#endif
    }
    return (rc);
}
*/

/////////////////////////////////////////////////////////////////////////////
// 

BOOL WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData )
{
//	TRACE( "Write: bOpcode = 0x%02X, blOperand = 0x%02X, Data = 0x%04X\n", bOpcode, blOperand, wData );
	unsigned long DIOData = 0;
	unsigned char output;

	if( blOperand )
	{
		DIOData = 0xFFF5FF00 | bOpcode;			// opcode in A, operand to follow
		USB_IoPortWriteAll(&DIOData);

		output	= (BYTE)( wData >> 8 );
		DIOData = 0xFFF2FF00 | output;			// High byte operand
		USB_IoPortWriteAll(&DIOData);

		output	= (BYTE)( wData & 0x00FF );
		DIOData = 0xFFF1FF00 | output;			// Low byte operand
		USB_IoPortWriteAll(&DIOData);

		DIOData = 0xFFF7FF00;					// Write it to the board
		USB_IoPortWriteAll(&DIOData);

		DIOData = 0xFFF0FF00;					// Clear it to the board
		USB_IoPortWriteAll(&DIOData);
	}
	else
	{
		DIOData = 0xFFF4FF00 | bOpcode;			// opcode in A, operand to follow
		USB_IoPortWriteAll(&DIOData);

		DIOData = 0xFFF7FF00 | bOpcode;			// Write it to the board
		USB_IoPortWriteAll(&DIOData);

		DIOData = 0xFFF0FF00 | bOpcode;			// Clear it to the board
		USB_IoPortWriteAll(&DIOData);
	}

	Sleep( 1 );

	return TRUE;
}

void CommToBoards( BYTE byteLo, BYTE byteHi )
{
//	TRACE( "Comm: byteLo = 0x%02X, byteHi = 0x%02X\n", byteLo, byteHi );
	// Write/read via MCI to/from SCN board.
	unsigned long DIOData = 0;

	DIOData = 0xFFF5FFF1;							// Set Port C to 5 and Port A to 0xF1  for opcode
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF1FF00 | byteLo;					// Serial Comm operand low byte
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF2FF00 | byteHi;					// Serial Comm operand high byte
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF7FF00;							// Write it to the board step 4
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF0FF00;							// Write it to the board step 5
	USB_IoPortWriteAll(&DIOData);

	Sleep( 1 );
}

int		m_nRBwdCnt;
int		m_nBoardReadDataBuf[256*2];
double	m_dADCchanModes[6*16*4];

DWORD BoardRead816( int nRsize, int nModID, BYTE bCktAddr, WORD wWdata )
{
	DWORD dwRet = 0;
	// READ up to 16 bits from a subboard, Wdata is normally 0.
	// Globals g_RBwdCnt and array: BoardReadData(n,n) is loaded here (and also in BoardRead2432 Sub)
	// Globals: g_TTID , g_MID, g_CID, g_ADCnum, g_ADCchan (these gloabal variables are only used for ADC reads)
	// g_ADCnum is set when ADCselectOB option is selected
	// g_ADCchan is set when ADCsChSelect option is selected

	BYTE bOP1, bOP2, bOP3, bOP4;
	// For all transfers, OP1 = circuit address (CktAddr), OP2 upper nibble = transfer type, OP2 lower nibble=Module ID
	// For 8 bit writes, OP3 is always set to 0, OP4 contains data to write to board
	// For 16 bit writes, OP3 contains low byte, OP4 contains high byte
	// 8 and 16 bit reads don't typically include writing, and setting OP3 and OP4 to 0 (for future possible usage)
	bOP1 = bCktAddr;
	if( nRsize == 8 )
	{
		bOP2	= 0x90;							// Upper nibble of OP2 = Transfer Type (9 for 8 bit Read)
		bOP3	= 0;
		bOP4	= (BYTE)wWdata;
	}
	else
	{
		bOP2	= 0xA0;							// Upper nibble of OP2 = Transfer Type (A for 16 bit Read)
		bOP3	= (BYTE)(wWdata & 0xFF);
		bOP4	= (BYTE)((wWdata /256) & 0xFF);	// Shift data down 8 bits before assigning to OP4
	}
	bOP2		|= (BYTE)nModID;				// OR the target Module ID into lower nibble of OP2

	USB_CommToBoards2(	bOP1,						// OP1 = ckt address
						bOP2,						// OP2 = Transfer type and Module ID
						bOP3,						// OP3: Set to 0 for 8 bit transaction, Low byte for 16 bit transaction
						bOP4 );						// OP4: ONLY valid data byte for 8 bit write, upper byte for 16 bit transaction

	// **************** READ PORTION ******************** //
	long lReadSize = 256;
	unsigned long lBytesTransferred = 0;
	// Allocate on the heap
	unsigned short* pnFrameData = new unsigned short[ lReadSize ];
	ZeroMemory(pnFrameData, sizeof(pnFrameData));

	unsigned long lRet = AIOUSB_ClearFIFO(deviceIndex, 0);//TIME_METHOD_WAIT_INPUT_ENABLE);
	DIO_StreamOpen( deviceIndex, TRUE );	// TRUE for read; FALSE for write.
	DIO_StreamFrame( deviceIndex, lReadSize, pnFrameData, &lBytesTransferred );
	DIO_StreamClose( deviceIndex );
	if( lBytesTransferred < 3 )
	{
		delete [] pnFrameData;
		return 1;
	}

	CString str, strMsg, strList;
	str.Format( "Data Received: %ld words", lBytesTransferred /2 );
//	GetDlgItem( IDC_EDIT_ReadbackDebugTB )->SetWindowText( str );

	strMsg = "";
	m_nRBwdCnt = 0;
//	CTabDialog* pTabDlg = (CTabDialog*)GetParent();
//	CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());
//	pParent->m_listReadback.ResetContent();
	BOOL bFound = FALSE;
	for( unsigned long i=0; i<lBytesTransferred/2; i++ )
	{
		str.Format( "%04X, ", pnFrameData[i] );
		strMsg += str;
		strList.Format( "@ %ld (hex %04X)", pnFrameData[i], pnFrameData[i] );
//		pParent->m_listReadback.AddString( strList );

		// 直到找到0xD22D才开始计数，前边的数据忽略
		if( pnFrameData[i] == 0xD22D )
			bFound = TRUE;
		if( !bFound )
			continue;

		m_nRBwdCnt++;
		m_nBoardReadDataBuf[ m_nRBwdCnt*2 +1 ] = pnFrameData[i];	// Numeric value (0-FFFF range)
		switch( pnFrameData[i] )
		{
		case	0xFFFF:
			m_nBoardReadDataBuf[ m_nRBwdCnt*2 +2 ] = 9;
			break;
		case	0xD22D:
			m_nBoardReadDataBuf[ m_nRBwdCnt*2 +2 ] = 4;
			break;
		default:
			m_nBoardReadDataBuf[ m_nRBwdCnt*2 +2 ] = 1;
			break;
		}
//		ADCreadingTB.Text = ADCreadingTB.Text + " " + V2hex$
//      ReadbackLB.AddItem "@" + Str(I) + ":" + S2 + " " + Str(V22) + " " + Str(V21) + " (hex" + V2hex$ + ")"
	}
	m_nBoardReadDataBuf[0] = m_nRBwdCnt;	// # of valid readback words contained in the array
//	GetDlgItem( IDC_EDIT_ADCreadingTB )->SetWindowText( strMsg );

	delete [] pnFrameData;
	return dwRet;
}

DWORD BoardRead2432( int nRsize, int nModID, BYTE bCktAddr, DWORD dwWdata )
{
	DWORD dwRet = 0;
	// READ up to 32 bits from a subboard, Wdata controls ADC setup.
	// Globals g_RBwdCnt and array: BoardReadData(n,n) is loaded here (and also in BoardRead2432 Sub)
	// Globals: g_TTID , g_MID, g_CID, g_ADCnum, g_ADCchan (these gloabal variables are only used for ADC reads)
	// g_ADCnum is set when ADCselectOB option is selected
	// g_ADCchan is set when ADCsChSelect option is selected
	BYTE bOP1, bOP2, bOP3, bOP4;
	// For all transfers, OP1 = circuit address (CktAddr), OP2 upper nibble = transfer type, OP2 lower nibble=Module ID
	// For 32 bit reads from LTC2448 ADC's, OP3 contains low byte, OP4 contains high byte to SEND TO ADC during read
	bOP1 = bCktAddr;
	if( nRsize == 24 )
	{
		bOP2	= 0xB0;							// Upper nibble of OP2 = Transfer Type (A for 24 bit Read) '24 bit read NOT USED as of 2-21-12
		bOP3	= 0;
		bOP4	= (BYTE)dwWdata;
	}
	else
	{
		bOP2	= 0xC0;							// Upper nibble of OP2 = Transfer Type (C for 32 bit Read)
		bOP3	= (BYTE)(dwWdata & 0xFF);
		bOP4	= (BYTE)((dwWdata /256) & 0xFF);// Shift data down 8 bits before assigning to OP4
	}
	bOP2		|= (BYTE)nModID;				// OR the target Module ID into lower nibble of OP2

	USB_CommToBoards2(	bOP1,						// OP1 = ckt address
						bOP2,						// OP2 = Transfer type and Module ID
						bOP3,						// OP3: Set to 0 for 8 bit transaction, Low byte for 16 bit transaction
						bOP4 );						// OP4: ONLY valid data byte for 8 bit write, upper byte for 16 bit transaction

	// **************** READ PORTION ******************** //
	long lReadSize = 256;
	unsigned long lBytesTransferred = 0;
	// Allocate on the heap
	unsigned short* pnFrameData = new unsigned short[ lReadSize ];
	ZeroMemory(pnFrameData, sizeof(pnFrameData));

	unsigned long lRet = AIOUSB_ClearFIFO(deviceIndex, 0);//TIME_METHOD_WAIT_INPUT_ENABLE);
	DIO_StreamOpen( deviceIndex, TRUE );	// TRUE for read; FALSE for write.
	DIO_StreamFrame( deviceIndex, lReadSize, pnFrameData, &lBytesTransferred );
	DIO_StreamClose( deviceIndex );
	if( lBytesTransferred < 3 )
	{
		delete [] pnFrameData;
		return 1;
	}

	CString str, strMsg, strList;
	str.Format( "Data Received: %ld words", lBytesTransferred /2 );
//	GetDlgItem( IDC_EDIT_ReadbackDebugTB )->SetWindowText( str );

	strMsg = "";
	m_nRBwdCnt = 0;
//	CTabDialog* pTabDlg = (CTabDialog*)GetParent();
//	CInsample0Dlg* pParent = (CInsample0Dlg*)(pTabDlg->GetParent());
//	pParent->m_listReadback.ResetContent();
	BOOL bFound = FALSE;
	for( unsigned long i=0; i<lBytesTransferred/2; i++ )
	{
		str.Format( "%04X, ", pnFrameData[i] );
		strMsg += str;
		strList.Format( "@ %ld (hex %04X)", pnFrameData[i], pnFrameData[i] );
//		pParent->m_listReadback.AddString( strList );

		// 直到找到0xD22D才开始计数，前边的数据忽略
		if( pnFrameData[i] == 0xD22D )
			bFound = TRUE;
		if( !bFound )
			continue;

		m_nRBwdCnt++;
		m_nBoardReadDataBuf[ m_nRBwdCnt*2 +1 ] = pnFrameData[i];	// Numeric value (0-FFFF range)
		switch( pnFrameData[i] )
		{
		case	0xFFFF:
			m_nBoardReadDataBuf[ m_nRBwdCnt*2 +2 ] = 9;
			break;
		case	0xD22D:
			m_nBoardReadDataBuf[ m_nRBwdCnt*2 +2 ] = 4;
			break;
		default:
			m_nBoardReadDataBuf[ m_nRBwdCnt*2 +2 ] = 1;
			break;
		}
//		ADCreadingTB.Text = ADCreadingTB.Text + " " + V2hex$
//      ReadbackLB.AddItem "@" + Str(I) + ":" + S2 + " " + Str(V22) + " " + Str(V21) + " (hex" + V2hex$ + ")"
	}
	m_nBoardReadDataBuf[0] = m_nRBwdCnt;	// # of valid readback words contained in the array
//	GetDlgItem( IDC_EDIT_ADCreadingTB )->SetWindowText( strMsg );

	delete [] pnFrameData;
	return dwRet;
}

void ADCvconv( int nADCtype )
{
	// Converts raw read data to voltage (and also to current if applicable)
	CString str;
	DWORD dwADCreadRaw, dwADCread24, dwADCreadSignBit;
	BOOL bADCvalSignBit;
	double dADCreadVrefFrac, dADCreadVrefPct;
	if( nADCtype == 1 )
	{
		// 2432 bits
//		str.Format( "%ld %ld", m_nBoardReadDataBuf[4*2+1], m_nBoardReadDataBuf[5*2+1] );
		dwADCreadRaw = ((DWORD)m_nBoardReadDataBuf[4*2+1]<<16) | (DWORD)m_nBoardReadDataBuf[5*2+1];
		dwADCreadRaw /= 4;	// Shift down to allow VB integer & long functions to work
		dwADCread24 = dwADCreadRaw & 0x7FFFFFF;
		dwADCread24 /= 8;	// toss out sub-LSB bits 4:0 and re-align
		str.Format( "%06X", dwADCread24 );	// Convert to 6 character hex string
//		GetDlgItem( IDC_EDIT_ADCreadVal24TB )->SetWindowText( str );

		dwADCreadSignBit = dwADCreadRaw & 0x8000000;	// ADC "SIG" bit is now down 2 bits from original position
		if( dwADCreadSignBit > 5 )
			bADCvalSignBit	= TRUE;
		else
			bADCvalSignBit	= FALSE;
		// If ADCvalSignBit = 1, then reading is a positive voltage in the range of 0V to Vref/2 minus 1LSB
		// If ADCvalSignBit = 0, then reading is a negative voltage in the range of 0V to -Vref/2
		if( bADCvalSignBit )
			dADCreadVrefFrac = 1.0 * dwADCread24 / 0x7FFFFF;	// Fractional value of full range of 0 to +Vref/2
		else
			dADCreadVrefFrac = 1.0 - 1.0*(dwADCread24 - 0x800000) / 0x7FFFFF;	// Fractional value of range of 0 to -Vref/2

		dADCreadVrefPct = dADCreadVrefFrac * 100;	// Percentage of full range of Vref(+) - Vref(-)
		if( bADCvalSignBit )
		{
/*			str.Format( "%.4f", dADCreadVrefPct );
			GetDlgItem( IDC_EDIT_PctVrefTB )->SetWindowText( str );
			// g_ADCvref varies per board design
			str.Format( "%.5f", dADCreadVrefFrac * m_dADCvref /2 );
			GetDlgItem( IDC_EDIT_ADCvoltsTB )->SetWindowText( str );
			str.Format( "%.5f", dADCreadVrefFrac * m_dADCvref /2 +m_dADCCOMoffset );
			GetDlgItem( IDC_EDIT_ADCvoltsPlusCOMTB )->SetWindowText( str );
*/		}
		else
		{
/*			str.Format( "-%.4f", dADCreadVrefPct );
			GetDlgItem( IDC_EDIT_PctVrefTB )->SetWindowText( str );
			str.Format( "-%.5f", dADCreadVrefFrac * m_dADCvref /2 );
			GetDlgItem( IDC_EDIT_ADCvoltsTB )->SetWindowText( str );
			str.Format( "%.5f", m_dADCCOMoffset - dADCreadVrefFrac * m_dADCvref /2 );
			GetDlgItem( IDC_EDIT_ADCvoltsPlusCOMTB )->SetWindowText( str );
*/		}
	}	// If LTC2448 ADC read
	else if( nADCtype == 2 )
	{
/*		// 1608 bits
		str.Format( "%ld %ld", m_nBoardReadDataBuf[4*2+1], m_nBoardReadDataBuf[5*2+1] );
		dwADCreadRaw = (DWORD)(m_nBoardReadDataBuf[4*2+1]<<16);
		dwADCreadRaw &= 0xFFFF;
		str.Format( "%04X", dwADCreadRaw );	// Identical value for 16 bit LTC1859
		GetDlgItem( IDC_EDIT_ADCreadVal24TB )->SetWindowText( str );

		dwADCreadSignBit = dwADCreadRaw & 0x8000;	// if MSB = 1 then negative value (if -5V to +5V range selected)
		if( dwADCreadSignBit > 5 )
			bADCvalSignBit	= TRUE;
		else
			bADCvalSignBit	= FALSE;
		// If ADCvalSignBit = 1, then reading is a positive voltage in the range of 0V to Vref/2 minus 1LSB
		// If ADCvalSignBit = 0, then reading is a negative voltage in the range of 0V to -Vref/2
		if( bADCvalSignBit )
		{
			dADCreadVrefFrac = (-1) * (1.0 - (dwADCreadRaw - 0x7FFF) / 0x7FFF);	// Fractional value of full range of 0 to -Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			str.Format( "%.4f", dADCreadVrefPct );
			GetDlgItem( IDC_EDIT_PctVrefTB )->SetWindowText( str );
			str.Format( "%.5f", dADCreadVrefFrac * 5 );
			GetDlgItem( IDC_EDIT_ADCvoltsTB )->SetWindowText( str );
			GetDlgItem( IDC_EDIT_ADCvoltsPlusCOMTB )->SetWindowText( str );	// Same - no adj. needed
		}
		else
		{
			dADCreadVrefFrac = dwADCreadRaw / 0x7FFF;	// Fractional value of full range of 0 to +Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			str.Format( "-%.4f", dADCreadVrefPct );
			GetDlgItem( IDC_EDIT_PctVrefTB )->SetWindowText( str );
			str.Format( "-%.5f", dADCreadVrefFrac * 5 );
			GetDlgItem( IDC_EDIT_ADCvoltsTB )->SetWindowText( str );
			GetDlgItem( IDC_EDIT_ADCvoltsPlusCOMTB )->SetWindowText( str );	// Same - no adj. needed
		}
*/	}	// If LTC1859 ADC read
/*
	double dEquivCurrent = dADCreadVrefFrac * m_dADCvref / 2 * m_dADCampsPerVolt;
	// g_ADCvoltsPerAmp, g_ADCampsPerVolt
	if( m_dADCcurrentMeas > 0 )
	{
		GetDlgItem( IDC_STATIC_CurrentMeasLabel )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_EDIT_ImonEstimateTB )->ShowWindow( SW_SHOW );
		str.Format( "%.3f", dEquivCurrent );
		GetDlgItem( IDC_EDIT_ImonEstimateTB )->SetWindowText( str );
	}
	else
	{
		GetDlgItem( IDC_EDIT_ImonEstimateTB )->SetWindowText( "n/a" );
		GetDlgItem( IDC_STATIC_CurrentMeasLabel )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_EDIT_ImonEstimateTB )->ShowWindow( SW_HIDE );
	}*/
}

BOOL USB_OpenDevice( void )
{
	///////////////////////////////////////////////////////////////////////////
	unsigned long pID;
	deviceIndex = 0;

	if (QueryDeviceInfo(diOnly, &pID, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
	{
		if ( pID == 0x800D )
			deviceIndex = diOnly;
		else
		{
			#ifdef _DEBUG
				afxDump << "\nInitCard-- ";
				afxDump << "Invalid Chip Type found, exiting...." << "\n";
			#endif
		}
	}

	if (deviceIndex == 0)
	{
		#ifdef _DEBUG
			afxDump << "\nInitCard-- ";
			afxDump << "Error in scanning for devices: No devices found." << "\n";
			afxDump << "\n		ErrorCount = " << USB_dwErrorCount++ << "\n";
		#endif
		return FALSE;
	}
	return TRUE;
	///////////////////////////////////////////////////////////////////////////
}

TCHAR exeFullPath[255];

/////////////////////////////////////////////////////////////////////////////
// COMMAPI
void USB_ResetCard() 
{
/*	// TODO: Add your control notification handler code here
	RETURN_CODE rc;
	
	const UINT	uRegisterCNTRLAddr = 0x6C;
	U32DATA		RegisterCNTRL;

	// Local reset 过程：
	// 1）首先读出local config register ( address 0x6C )中的值CONTROL；
	RegisterCNTRL.dwData = PlxRegisterRead( g_hPlxDevice, uRegisterCNTRLAddr, &rc );
	if( rc != ApiSuccess )
	{
		//DescribPlxApiReturnCode( rc );
		return;
	}

	// 2）给CONTROL的第30位写“1”；
	RegisterCNTRL.BITS.Bit30 = 1;
	rc = PlxRegisterWrite( g_hPlxDevice, uRegisterCNTRLAddr, RegisterCNTRL.dwData );
	//DescribPlxApiReturnCode( rc );

	// 3）给CONTROL的第30位写“0”；
	RegisterCNTRL.BITS.Bit30 = 0;
	rc = PlxRegisterWrite( g_hPlxDevice, uRegisterCNTRLAddr, RegisterCNTRL.dwData );
	//DescribPlxApiReturnCode( rc );

	// 4）给CONTROL的第29位写“1”；
	RegisterCNTRL.BITS.Bit29 = 1;
	rc = PlxRegisterWrite( g_hPlxDevice, uRegisterCNTRLAddr, RegisterCNTRL.dwData );
	//DescribPlxApiReturnCode( rc );

	// 5）给CONTROL的第29位写“0”；
	RegisterCNTRL.BITS.Bit29 = 0;
	rc = PlxRegisterWrite( g_hPlxDevice, uRegisterCNTRLAddr, RegisterCNTRL.dwData );
	//DescribPlxApiReturnCode( rc );

*/	// OVER.
}

BOOL GetModuleFilePath(LPTSTR exeFullPath)
{
	DWORD i, dwLength;

	dwLength = GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	for ( i=dwLength-1; i > 0; i-- )
	{
		if ( exeFullPath[i] == '\\' )
		{
			exeFullPath[i+1] = '\0';
			break;
		}
	}
	
	if ( dwLength == 0 )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//Read data in one line in *.ini.
//In one line the data like ***,*****,*****, they are seperated by comma.
int ReadInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator=',');
int SaveInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator=',');

int ReadInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator)
{
	char szBuf1[255];
	char szBuf2[255];
	int* pint = (int*) pData;
	double* pdouble = (double*)pData;
	long* plong = (long*)pData;
	CString* pstr = (CString*)pData;
	int i = 0;
	if(GetPrivateProfileString(Section, Entry, "", szBuf1, 255, path) != 0)
	{
		for(i=0; i<nTotal; i++)
		{
			strcpy(szBuf2, szBuf1);
			if(strchr(szBuf2, Separator) != NULL)
			{
				strcpy(szBuf1, strchr(szBuf2, Separator)+1);
				*strchr(szBuf2, Separator) = '\0';
				switch( uDataStyle )
				{
				case	INTSTYLE:
					pint[i] = atoi(szBuf2);
					break;
				case	DOUBLESTYLE:
					pdouble[i] = atof(szBuf2);
					break;
				case	LONGSTYLE:
					plong[i] = atol(szBuf2);
					break;
				case	STRSTYLE:
					pstr[i] = szBuf2;
					break;
				default:
					return 0;
				}
			}
			else
			{
				if(i == nTotal-1)
				{
					// This is the last data.
					switch( uDataStyle )
					{
					case	INTSTYLE:
						pint[i] = atoi(szBuf2);
						break;
					case	DOUBLESTYLE:
						pdouble[i] = atof(szBuf2);
						break;
					case	LONGSTYLE:
						plong[i] = atol(szBuf2);
						break;
					case	STRSTYLE:
						pstr[i] = szBuf2;
						break;
					default:
						return 0;
					}
					nTotal = i+1;
				}
				else
					nTotal = i;
				break;
			}
		}
	}
	return i+1;
}

int SaveInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator)
{
	char szBuf1[255];
	char szBuf2[255];
	*szBuf1 = '\0';
	int* pint = (int*) pData;
	double* pdouble = (double*)pData;
	long* plong = (long*)pData;
	int i = 0;
	for(i=0; i<nTotal; i++)
	{
		if(i == nTotal-1)
		{
			// This is the last data.
			switch( uDataStyle )
			{
			case	INTSTYLE:
				sprintf(szBuf2, "%d", pint[i]);
				break;
			case	DOUBLESTYLE:
				sprintf(szBuf2, "%f", pdouble[i]);
				break;
			case	LONGSTYLE:
				sprintf(szBuf2, "%ld", plong[i]);
				break;
			default:
				return 0;
			}
		}
		else
		{
			switch( uDataStyle )
			{
			case	INTSTYLE:
				sprintf(szBuf2, "%d%c", pint[i], Separator);
				break;
			case	DOUBLESTYLE:
				sprintf(szBuf2, "%f%c", pdouble[i], Separator);
				break;
			case	LONGSTYLE:
				sprintf(szBuf2, "%ld%c", plong[i], Separator);
				break;
			default:
				return 0;
			}
		}
		strcat(szBuf1, szBuf2);
	}
	WritePrivateProfileString(Section, Entry, szBuf1, path);
	return nTotal;
}
/////////////////////////////////////////////////////////////////////////////

void ReadDataFromINI()
{
	char stabuff[20];
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	double*		pdouble	= new double[16];
	int			nTotal	= 16;
	int			i;

	/////////////////////////////////////////////////////////////////
	// SCN ADC (only one ADC on SCN board):
	// Set channels 0/1, 2/3, 4/5, 6/7 to diff (value of x,x,1 = 2), all others to single-ended measurement mode (value of x,x,1 = 1)
	sprintf(stabuff, "ADCchanModes");
	if( ReadInOneLine(path, stabuff, "SCN-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_dADCchanModes[0*6+i*16+1] = pdouble[i];
	// Set current interpretation mode and scale factor as determined by circuitry (non-zero value = scale factor)
	if( ReadInOneLine(path, stabuff, "SCN-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_dADCchanModes[0*6+i*16+2] = pdouble[i];
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// LNS ADC (one LTC1859 8 channel ADC):
	// Set all 8 channels' input modes as Single-ended:
	nTotal = 8;
	sprintf(stabuff, "ADCchanModes");
	if( ReadInOneLine(path, stabuff, "LNS-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_dADCchanModes[1*6+i*16+1] = pdouble[i];
	// ADC channels 0-3 are assigned to current measurements. Set scale factors according to circuit:
	if( ReadInOneLine(path, stabuff, "LNS-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_dADCchanModes[1*6+i*16+2] = pdouble[i];
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// VAC ADC#0:
	nTotal = 16;
	// Set all channels to single-ended measurement mode (value of x,x,1 = 1)
	sprintf(stabuff, "ADCchanModes");
	if( ReadInOneLine(path, stabuff, "VAC0-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_dADCchanModes[2*6+i*16+1] = pdouble[i];
	// Set current interpretation mode and scale factor as determined by circuitry (non-zero value = scale factor)
	if( ReadInOneLine(path, stabuff, "VAC0-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_dADCchanModes[2*6+i*16+2] = pdouble[i];
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// VAC ADC#1:
	// Set all channels to single-ended measurement mode (value of x,x,1 = 1)
	sprintf(stabuff, "ADCchanModes");
	if( ReadInOneLine(path, stabuff, "VAC1-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_dADCchanModes[3*6+i*16+1] = pdouble[i];
	// Set current interpretation mode and scale factor as determined by circuitry (non-zero value = scale factor)
	if( ReadInOneLine(path, stabuff, "VAC1-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_dADCchanModes[3*6+i*16+2] = pdouble[i];
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// SPR ADC#0:
	// Set all channels to single-ended measurement mode (value of x,x,1 = 1)
	sprintf(stabuff, "ADCchanModes");
	if( ReadInOneLine(path, stabuff, "SPR0-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_dADCchanModes[4*6+i*16+1] = pdouble[i];
	// Set current measurement scale factor if applicable (non-zero value = scale factor in Volts per Amp)
	if( ReadInOneLine(path, stabuff, "SPR0-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_dADCchanModes[4*6+i*16+2] = pdouble[i];
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// SPR ADC#1:
	// Set all channels to single-ended measurement mode (value of x,x,1 = 1)
	sprintf(stabuff, "ADCchanModes");
	if( ReadInOneLine(path, stabuff, "SPR1-1", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_dADCchanModes[5*6+i*16+1] = pdouble[i];
	// Set current interpretation mode and scale factor as determined by circuitry (non-zero value = scale factor)
	if( ReadInOneLine(path, stabuff, "SPR1-2", DOUBLESTYLE, nTotal, pdouble) == nTotal )
		for( i=0; i<nTotal; i++ )
			m_dADCchanModes[5*6+i*16+2] = pdouble[i];
	/////////////////////////////////////////////////////////////////
	
	delete [] pdouble;
}

int USB_InitCard_once( void )
{
	int nRet = 0;
	unsigned long status;
	double read_clock, write_clock;

	unsigned char OutMask = 0x05;	//A, C output; B, D input.
	unsigned long DIOData = 0x0;	//0xFFFFFFFF;
	unsigned char TristateMask = 0x00;
	status = DIO_ConfigureEx(deviceIndex, &OutMask, &DIOData, &TristateMask);
	if (status != ERROR_SUCCESS)
	{
		nRet = 0x10;
		return nRet;
	}

	unsigned char input[4];
	USB_IoPortReadAll(&input);
	if( input[3] == 0x07 )		// port D: 0x01, means MCI power on
	{
		nRet = 0x20;
		return nRet;
	}

	read_clock = READ_CLOCK;
	write_clock = WRITE_CLOCK;

	status = DIO_StreamSetClocks(deviceIndex, &read_clock, &write_clock);

	if (status != ERROR_SUCCESS)
	{
#ifdef _DEBUG
		afxDump << "\nInitCard-- ";
		afxDump << "Error in setting clocks: Could not set clocks." << "\n";
		afxDump << "\n		ErrorCount = " << USB_dwErrorCount++ << "\n";
#endif
		nRet = 0x40;
		return nRet;
	}

	status = AIOUSB_SetStreamingBlockSize(deviceIndex,0xFFFF);	// default is 8192 (0x2000)

	// by DIFast
	status = DIO_StreamOpen (deviceIndex, 1); //the 1 means it is a read. Send 0 for a write

	if (status != ERROR_SUCCESS) //this really should never happen.
	{
#ifdef _DEBUG
		afxDump << "\nInitCard-- ";
		afxDump << "Error in stream open: Could not open stream." << "\n";
		afxDump << "\n		ErrorCount = " << USB_dwErrorCount++ << "\n";
#endif
		DIO_StreamClose(deviceIndex);
		nRet = 0x80;
		return nRet;
	}
	DIO_StreamClose(deviceIndex);

	unsigned long lHandle;
	unsigned long lRet = GetDeviceHandle(deviceIndex, &lHandle);
	if( lRet == 0 )
	{
		unsigned long lNewTimeout = 50;	// in milliseconds.
		BOOL bRet = WinUsb_SetPipePolicy( (WINUSB_INTERFACE_HANDLE)lHandle, 0x86, PIPE_TRANSFER_TIMEOUT, sizeof(lNewTimeout), &lNewTimeout );
	}

	return nRet;
}

int WINAPI USB_InitCard( void )
{
	static int nInitTimes = 0;

	if ( !USB_OpenDevice() )
        return -1;

	int nRet = 0;
	while ( (nRet = USB_InitCard_once()) && (nRet > 0) )
	{
		if (nInitTimes++ < 10)
			USB_ResetCard();
		else 
		    return nRet;
	};

	return 0;
}

BOOL WINAPI USB_CloseCard( void )
{
/*	if ( g_hPlxDevice == NULL )
        return FALSE;

	blImageStreamInputStop = TRUE;
	blPhotoStreamOutputStop = TRUE;

    PlxDmaBlockChannelClose( g_hPlxDevice, PrimaryPciChannel0 );
    PlxDmaBlockChannelClose( g_hPlxDevice, PrimaryPciChannel1 );
	PlxPciDeviceClose( g_hPlxDevice );
	g_hPlxDevice = NULL;

	#ifdef _DEBUG
		afxDump << "\n ========= CloseCard ========= \n";
	#endif
*/
	DIO_StreamClose(deviceIndex);
	ClearDevices();

	return TRUE;
}

DWORD WINAPI USB_ClearFIFO( void )
{
	return (DWORD)AIOUSB_ClearFIFO(deviceIndex, 0);//TIME_METHOD_WAIT_INPUT_ENABLE);
}

DWORD WINAPI USB_StreamOpen( unsigned long bIsRead )
{
	unsigned long status = DIO_StreamOpen (deviceIndex, bIsRead); //the 1 means it is a read. Send 0 for a write
	if (status != ERROR_SUCCESS) //this really should never happen.
	{
		DIO_StreamClose(deviceIndex);
	}
	return (DWORD)status;
}

DWORD WINAPI USB_StreamClose( void )
{
	unsigned long status = DIO_StreamClose(deviceIndex);
	return (DWORD)status;
}

DWORD WINAPI USB_ConfigureEx(void *pOutMask, void *pData, void *pTristateMask)
{
	return (DWORD)DIO_ConfigureEx(deviceIndex, pOutMask, pData, pTristateMask);
}

#ifdef _MEM_Emulate_DEBUG

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif /* __cplusplus */

#define HADDRL    0x680      /*  Host Address Low       */
#define HADDRH    0x682      /*  Host Address High      */
#define HDATA     0x684      /*  Host Data              */

unsigned short upload_register(unsigned long gsp_addr)
{
	unsigned short  value;

	//-----------start------------

	__asm CLI	
	_outpw (HADDRH, (unsigned short) (gsp_addr>>16));
	_outpw (HADDRL, (unsigned short) (gsp_addr));
	value = _inpw(HDATA);
	__asm STI

	//------------end-------------

	//Sleep(0);
	return value;
}

unsigned short download_register(unsigned short value, unsigned long gsp_addr)
{
	//-----------start------------

	__asm CLI
	_outpw (HADDRH, (unsigned short) (gsp_addr>>16));
	_outpw (HADDRL, (unsigned short) (gsp_addr));
	_outpw (HDATA, value);
	__asm STI

	//------------end-------------

	//Sleep(0);
	return value;
}

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif       /* __cplusplus */

#endif

BOOL WINAPI USB_IoPortRead1( unsigned long BitIndex, unsigned char *Buffer )
{
/*
#ifdef _MEM_Emulate_DEBUG
	wValue = upload_register( dwLocalAddress * 32 );
	return TRUE;
#endif
*/
	if (deviceIndex == 0)
    {
/*		#ifdef _DEBUG
			afxDump << "\nIoPortReadW-- ";
			afxDump << "Error in g_hPlxDevice == NULL.\n";
			afxDump << "\n		ErrorCount = " << dwErrorCount++ << "\n";
		#endif
*/        return FALSE;
    }

	m_mutexPortsDI16A.Lock();
	BOOL bRet = TRUE;
	unsigned long	rc;
	rc = DIO_Read1(deviceIndex, BitIndex, Buffer);
    if (rc != 0)
    {
		#ifdef _DEBUG
			afxDump << "\nUSB_IoPortRead1-- ";
			afxDump << "Error: Unable to read data.\n" << "Returned code " << rc << "\n";
			afxDump << "\n		ErrorCount = " << USB_dwErrorCount++ << "\n";
		#endif
        bRet = FALSE;
    }
	m_mutexPortsDI16A.Unlock();
	return bRet;
}

BOOL WINAPI USB_IoPortWrite1( unsigned long BitIndex, unsigned char bData )
{
/*
#ifdef _MEM_Emulate_DEBUG
	download_register( wValue, dwLocalAddress  * 32 );
	return TRUE;
#endif
*/
	if (deviceIndex == 0)
    {
/*		#ifdef _DEBUG
			afxDump << "\nIoPortWriteW-- ";
			afxDump << "Error in g_hPlxDevice == NULL.\n";
			afxDump << "\n		ErrorCount = " << dwErrorCount++ << "\n";
		#endif
*/        return FALSE;
    }

	m_mutexPortsDI16A.Lock();
	BOOL bRet = TRUE;
	unsigned long	rc;

	outMask ^= 0x04;
	rc = DIO_Write1(deviceIndex, BitIndex, (outMask & 0x0800) ? 1 : 0);

    if (rc != 0)
    {
		#ifdef _DEBUG
			afxDump << "\nUSB_IoPortWrite1-- ";
			afxDump << "Error: Unable to read data.\n" << "Returned code " << rc << "\n";
			afxDump << "\n		ErrorCount = " << USB_dwErrorCount++ << "\n";
		#endif
			bRet = FALSE;
    }
	m_mutexPortsDI16A.Unlock();
	return bRet;
}

BOOL WINAPI USB_IoPortRead8( unsigned long ByteIndex, unsigned char *pBuffer )
{
/*
#ifdef _MEM_Emulate_DEBUG
	dwValue = (DWORD)upload_register( dwLocalAddress * 32 );
	return TRUE;
#endif
*/
	if (deviceIndex == 0)
    {
/*		#ifdef _DEBUG
			afxDump << "\nIoPortReadL-- ";
			afxDump << "Error in g_hPlxDevice == NULL.\n";
			afxDump << "\n		ErrorCount = " << dwErrorCount++ << "\n";
		#endif
*/        return FALSE;
    }

	m_mutexPortsDI16A.Lock();
	BOOL bRet = TRUE;
	unsigned long	rc;
	rc = DIO_Read8(deviceIndex, ByteIndex, pBuffer);
    if (rc != 0)
    {
		#ifdef _DEBUG
			afxDump << "\nUSB_IoPortRead8-- ";
			afxDump << "Error: Unable to read data.\n" << "Returned code " << rc << "\n";
			afxDump << "\n		ErrorCount = " << USB_dwErrorCount++ << "\n";
		#endif
			bRet = FALSE;
    }
	m_mutexPortsDI16A.Unlock();
	return bRet;
}

BOOL WINAPI USB_IoPortWrite8( unsigned long ByteIndex, unsigned char Data )
{
/*
#ifdef _MEM_Emulate_DEBUG
	download_register((WORD)dwValue, dwLocalAddress * 32 );
	return TRUE;
#endif
*/
	if (deviceIndex == 0)
    {
/*		#ifdef _DEBUG
			afxDump << "\nIoPortWriteL-- ";
			afxDump << "Error in g_hPlxDevice == NULL.\n";
			afxDump << "\n		ErrorCount = " << dwErrorCount++ << "\n";
		#endif
*/        return FALSE;
    }

	m_mutexPortsDI16A.Lock();
	BOOL bRet = TRUE;
	unsigned long	rc;
	rc = DIO_Write8(deviceIndex, ByteIndex, Data);
    if (rc != 0)
    {
		#ifdef _DEBUG
			afxDump << "\nUSB_IoPortWrite8-- ";
			afxDump << "Error: Unable to write data.\n" << "Returned code " << rc << "\n";
			afxDump << "\n		ErrorCount = " << USB_dwErrorCount++ << "\n";
		#endif
			bRet = FALSE;
    }
	m_mutexPortsDI16A.Unlock();
	return bRet;
}

BOOL WINAPI USB_IoPortReadAll( void *Buffer )
{
	if (deviceIndex == 0)
		return FALSE;

	m_mutexPortsDI16A.Lock();
	BOOL bRet = TRUE;
	unsigned long	rc;
	rc = DIO_ReadAll(deviceIndex, Buffer);
    if (rc != 0)
    {
		#ifdef _DEBUG
			afxDump << "\nIoPortReadAll-- ";
			afxDump << "Error: Unable to read data.\n" << "Returned code " << rc << "\n";
			afxDump << "\n		ErrorCount = " << USB_dwErrorCount++ << "\n";
		#endif
        bRet = FALSE;
    }
	m_mutexPortsDI16A.Unlock();
	return bRet;
}

BOOL WINAPI USB_IoPortWriteAll( void *pData )
{
	if (deviceIndex == 0)
		return FALSE;

//	m_mutexPortsDI16A.Lock();
	BOOL bRet = TRUE;
	unsigned long	rc;
	rc = DIO_WriteAll(deviceIndex, pData);
    if (rc != 0)
    {
		#ifdef _DEBUG
			afxDump << "\nIoPortWriteAll-- ";
			afxDump << "Error: Unable to write data.\n" << "Returned code " << rc << "\n";
			afxDump << "deviceindex = " << deviceIndex << ", Data = " << pData << "\n";
			afxDump << "\n		ErrorCount = " << USB_dwErrorCount++ << "\n";
		#endif
        bRet = FALSE;
    }
//	m_mutexPortsDI16A.Unlock();
	return bRet;
}

DWORD WINAPI USB_ImageInputStream5( unsigned long FramePoints, unsigned short *pFrameData, unsigned long *BytesTransferred )
{
	unsigned long numreadings = LOOP_COUNTS * (FramePoints +512);
	unsigned long bytes_transferred, total_readings;
	unsigned long status, Calls;
	DWORD dwRet = 0;

	for ( int iScan = 0; iScan < LOOP_COUNTS; ++iScan )
	{
		total_readings = 0;
		Calls = 0;
		do
		{
			//If making a "remainder" request, round the remainder up to the next 256-sample packet.
			unsigned long MasterCt = FramePoints - total_readings;
			unsigned long RoundCt = MasterCt & 0xFFFFFF00;
			if ( MasterCt & 0xFF )
				RoundCt += 0x100;
//			status = DIO_StreamFrame(deviceIndex, RoundCt, &(readings2D[iScan][total_readings]), &bytes_transferred);
			status = DIO_StreamFrame(deviceIndex, RoundCt, &(pFrameData[iScan*(FramePoints+512) +total_readings]), &bytes_transferred);
//			status = DIO_StreamFrame(deviceIndex, RoundCt, &(m_pnFrameData[total_readings]), &bytes_transferred);
			total_readings += bytes_transferred / 2UL;
			++Calls;
/*			if ( status )
			{
				dwRet = 2;
				DIO_StreamClose(deviceIndex);
				delete [] pReadings;
				return dwRet;
			}
			else if ( bytes_transferred >= 2 )
			{
				TRACE("Got %ld bytes...\r\n", bytes_transferred);
				dwRet = 0;
			}
			else
			{
				dwRet = 3;
			}
*/		} while((status < 1) && (total_readings < FramePoints) && (Calls < 10));
#ifdef _DEBUG
			afxDump << "\nUSB_ImageInputStream5--";
			afxDump << "status = " << status;
			afxDump << ", total_readings = " << total_readings;
			afxDump << ", Calls = " << Calls << "\n";
#endif
		if( status )
		{
			dwRet = status;
		}
		else if( Calls > 10 )
		{
			dwRet = 20000;
		}
	}

	*BytesTransferred = total_readings *2;
	///////////////////////////////////////////////////////////////////////////
    if ( dwRet != 0)
    {
		#ifdef _DEBUG
			afxDump << "\nUSB_ImageInputStream5--Error.\n";
			afxDump << "dwRet = " << dwRet << "\n";
		#endif
    }

	return dwRet;
}

DWORD WINAPI USB_ImageInputStream2( unsigned long FramePoints, void * pBuffer, unsigned long *BytesTransferred )
{
	unsigned short* pReadings = new unsigned short[ FramePoints ];
	unsigned long bytes_transferred, total_readings;
	unsigned long status, Calls;
	DWORD dwRet = 0;

	unsigned long cbRet;
	char Buf[32];
	CString str, strTemp;
	CTime	timeCurrent = CTime::GetCurrentTime();
	strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
	str.Format( "%szscan%s.log", exeFullPath, strTemp );
	HANDLE hFil = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);

	status = DIO_StreamOpen (deviceIndex, 1); //the 1 means it is a read. Send 0 for a write
	if (status != ERROR_SUCCESS) //this really should never happen.
	{
		DIO_StreamClose(deviceIndex);
		delete [] pReadings;
		CloseHandle(hFil);
		return 1;
	}

	total_readings = 0;
	Calls = 0;
//	do
//	{
		//If making a "remainder" request, round the remainder up to the next 256-sample packet.
//		status = DIO_StreamFrame(deviceIndex, FramePoints, (unsigned short*)pBuffer, &bytes_transferred);
		status = DIO_StreamFrame(deviceIndex, FramePoints, pReadings, &bytes_transferred);
//		status = DIO_StreamFrame(deviceIndex, RoundCt, &(m_pnFrameData[total_readings]), &bytes_transferred);
		total_readings += bytes_transferred / 2UL;
		++Calls;
		if ( status )
		{
			dwRet = 2;
			DIO_StreamClose(deviceIndex);
			delete [] pReadings;
			CloseHandle(hFil);
			return dwRet;
		}
		else if ( bytes_transferred >= 2 )
		{
			dwRet = 0;
		}
		else
		{
			dwRet = 3;
		}
//	} while(total_readings < NUM_READINGS_Test);

	*BytesTransferred = total_readings *2;
	DIO_StreamClose(deviceIndex);

	///////////////////////////////////////////////////////////////////////////
//	for ( iScan = 0; iScan < NUM_SCANS_Test; ++iScan )
	{
		sprintf(Buf, "/BEGIN SCAN\\\r\n");
		WriteFile(hFil, Buf, strlen(Buf), &cbRet, NULL);

		for ( unsigned long iReading = 0; (iReading+3) < total_readings; iReading +=3 )
		{
//			sprintf(Buf, "%04X\r\n", pReadings[iReading]);
			sprintf(Buf, "%04X, %04X, %04X, ", pReadings[iReading], pReadings[iReading+1], pReadings[iReading+2] );
			WriteFile(hFil, Buf, strlen(Buf), &cbRet, NULL);
		}

		sprintf(Buf, "\\END SCAN/\r\n\r\n");
		WriteFile(hFil, Buf, strlen(Buf), &cbRet, NULL);
	}

	CloseHandle(hFil);

	///////////////////////////////////////////////////////////////////////////
    if ( dwRet != 0)
    {
		#ifdef _DEBUG
			afxDump << "\nUSB_ImageInputStream4--Error.\n";
			afxDump << "dwRet = " << dwRet << "\n";
		#endif

		delete [] pReadings;
    }
	else
	{
		//process data
		ASSERT(pBuffer != NULL);

		memcpy( pBuffer,
			(const void *)pReadings,
			total_readings );
		delete [] pReadings;
	}

	return dwRet;
}

DWORD WINAPI USB_ImageInputStream1( unsigned long FramePoints, unsigned short* pFrameData, unsigned long *BytesTransferred, short nNeedLog )
{
	if( !bVideoON )	// if Video_OFF, return directly
	{
//		Sleep( 10 );
		return 5;
	}

	unsigned long numreadings = LOOP_COUNTS * (FramePoints +512);
//	unsigned short* pReadings = new unsigned short[numreadings];
	unsigned long bytes_transferred = 0, total_readings = 0;
	unsigned long readingsCounts[LOOP_COUNTS] = {0};//, ret[LOOP_COUNTS];
	unsigned long status = 0, Calls = 0;
	DWORD dwRet = 0;

//	status = DIO_StreamOpen (deviceIndex, 1); //the 1 means it is a read. Send 0 for a write
	int iScan = 0;
	for (iScan = 0; iScan < LOOP_COUNTS; ++iScan )
	{
		total_readings = 0;
		Calls = 0;
		do
		{
			//If making a "remainder" request, round the remainder up to the next 256-sample packet.
			unsigned long MasterCt = FramePoints - total_readings;
			unsigned long RoundCt = MasterCt & 0xFFFFFF00;
			if ( MasterCt & 0xFF )
				RoundCt += 0x100;
//			status = DIO_StreamFrame(deviceIndex, RoundCt, &(readings2D[iScan][total_readings]), &bytes_transferred);
			status = DIO_StreamFrame(deviceIndex, RoundCt, &(pFrameData[iScan*(FramePoints+512) +total_readings +1]), &bytes_transferred);
//			status = DIO_StreamFrame(deviceIndex, RoundCt, &(m_pnFrameData[total_readings]), &bytes_transferred);
			total_readings += bytes_transferred / 2UL;
			++Calls;
/*			if ( status )
			{
				dwRet = 2;
				DIO_StreamClose(deviceIndex);
				delete [] pFrameData;
				return dwRet;
			}
			else if ( bytes_transferred >= 2 )
			{
				TRACE("Got %ld bytes...\r\n", bytes_transferred);
				dwRet = 0;
			}
			else
			{
				dwRet = 3;
			}
*/		} while( (status < 1) && (total_readings < FramePoints) && (Calls <= 10) && bVideoON );
		if( status )
		{
			dwRet = status;
		}
		else if( Calls > 10 )
		{
//			ret[iScan] = 20000;
			dwRet = 20000;
		}
		else if( !bVideoON )
		{
//			ret[iScan] = 20001;
			dwRet = 20001;
		#ifdef _DEBUG
			//afxDump << "\nUSB_ImageInputStream3--Error.\n";
		#endif
			delete [] pFrameData;
			return dwRet;
		}
//		else
//		{
//			ret[iScan] = 0;
//			dwRet = 0;
//		}
		readingsCounts[iScan] = total_readings;
		pFrameData[iScan*(FramePoints+512)] = total_readings;
	}
//	status = DIO_StreamClose (deviceIndex);

//	*BytesTransferred = total_readings *2;
	*BytesTransferred = numreadings *2;
	total_readings = 0;
	for ( iScan = 0; iScan < LOOP_COUNTS; ++iScan )
	{
		total_readings += readingsCounts[iScan];
	}
	*BytesTransferred = total_readings *2;
//	TRACE( "dwRet = %d, %d, %d, %d, %d.\n", ret[0], ret[1], ret[2], ret[3], ret[4] );
//	TRACE( "Counts = %d, %d, %d, %d, %d.\n", readingsCounts[0], readingsCounts[1], readingsCounts[2], readingsCounts[3], readingsCounts[4] );

	if( nNeedLog > 0 )
	{
		unsigned long cbRet;
		char Buf[32];
		CString str, strTemp;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
		str.Format( "%szscan%s.log", exeFullPath, strTemp );
		HANDLE hFil = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);

		///////////////////////////////////////////////////////////////////////////
		for ( iScan = 0; iScan < LOOP_COUNTS; ++iScan )
		{
			sprintf(Buf, "/BEGIN SCAN\\\r\n");
			WriteFile(hFil, Buf, strlen(Buf), &cbRet, NULL);

			unsigned long lOffset = iScan*(FramePoints+512);
			for ( unsigned long iReading = 0; iReading < readingsCounts[iScan]; iReading ++ )
			{
	//			sprintf(Buf, "%04X\r\n", pFrameData[iReading]);
				sprintf(Buf, "%04X, ", pFrameData[lOffset+iReading] );
				WriteFile(hFil, Buf, strlen(Buf), &cbRet, NULL);
			}

			sprintf(Buf, "\\END SCAN/\r\n\r\n");
			WriteFile(hFil, Buf, strlen(Buf), &cbRet, NULL);
		}

		CloseHandle(hFil);
	}

	///////////////////////////////////////////////////////////////////////////
    if ( dwRet != 0)
    {
		#ifdef _DEBUG
			//afxDump << "\nUSB_ImageInputStream3--Error.\n";
			//afxDump << "dwRet = " << dwRet << "\n";
		#endif

//		delete [] pReadings;
    }
/*	else
	{
		//process data
		ASSERT(pFrameData != NULL);

//		memcpy( pFrameData, pReadings,	total_readings * sizeof( unsigned short ) );
		memcpy( pFrameData, pReadings,	numreadings * sizeof( unsigned short ) );
		delete [] pReadings;
	}
*/
	return dwRet;
}


BOOL WINAPI USB_PhotoOutputStream( void * pBuffer, IN LONG dwBlockSize )
{
/*	unsigned long	rc;

	if (deviceIndex != 0)
    if( !blPhotoStreamOutputStop )
    {
        dmaDataChannel1.Pci9054Dma.LowPciAddr       = (U32)g_MemoryInfo.PhysicalAddr;
        dmaDataChannel1.Pci9054Dma.IopAddr          = PhotoFIFOStartOffset;
	    dmaDataChannel1.Pci9054Dma.TransferCount    = dwBlockSize;
        dmaDataChannel1.Pci9054Dma.IopToPciDma      = 0;
        dmaDataChannel1.Pci9054Dma.TerminalCountIntr= 0;

		//process data
		ASSERT(pBuffer != NULL);

		memcpy( (void *)dwChannel1UserAddr,
			(const void *)pBuffer,
			dwBlockSize );
		
        // Perform the transfer and wait for its completion
        rc = PlxDmaBlockTransfer(g_hPlxDevice, 
                                 PrimaryPciChannel1,
                                 &dmaDataChannel1,
                                 FALSE);            //return upon completion
        if ( rc != ApiSuccess)
        {
			#ifdef _DEBUG
				afxDump << "\nPhotoOutputStream-- ";
				afxDump << "Errors in Block Dma channel-1. ";
				afxDump << "Returned code " << rc << "\n";
				afxDump << "\n		ErrorCount = " << dwErrorCount++ << "\n";
			#endif
    
			return FALSE;
        }
	}
*/	
	return TRUE;
}

BOOL WINAPI USB_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData )
{
//	TRACE( "USB_Write: bOpcode = 0x%02X, blOperand = 0x%02X, wData = 0x%04X\n", bOpcode, blOperand, wData );
	m_mutexPortsDI16A.Lock();
	unsigned long DIOData = 0;
	unsigned char output;

	if( blOperand )
	{
		DIOData = 0xFFF5FF00 | bOpcode;			// opcode in A, operand to follow
		USB_IoPortWriteAll(&DIOData);

		output	= (BYTE)( wData >> 8 );
		DIOData = 0xFFF2FF00 | output;			// High byte operand
		USB_IoPortWriteAll(&DIOData);

		output	= (BYTE)( wData & 0x00FF );
		DIOData = 0xFFF1FF00 | output;			// Low byte operand
		USB_IoPortWriteAll(&DIOData);

		DIOData = 0xFFF7FF00;					// Write it to the board
		USB_IoPortWriteAll(&DIOData);

		DIOData = 0xFFF0FF00;					// Clear it to the board
		USB_IoPortWriteAll(&DIOData);
	}
	else
	{
		DIOData = 0xFFF4FF00 | bOpcode;			// opcode in A, operand to follow
		USB_IoPortWriteAll(&DIOData);

		DIOData = 0xFFF7FF00 | bOpcode;			// Write it to the board
		USB_IoPortWriteAll(&DIOData);

		DIOData = 0xFFF0FF00 | bOpcode;			// Clear it to the board
		USB_IoPortWriteAll(&DIOData);
	}

	Sleep( 1 );
	m_mutexPortsDI16A.Unlock();

	return TRUE;
}

void WINAPI USB_CommToBoards( BYTE byteLo, BYTE byteHi )
{
//	TRACE( "USB_Comm: byteLo = 0x%02X, byteHi = 0x%02X\n", byteLo, byteHi );
	// Write/read via MCI to/from SCN board.
	m_mutexPortsDI16A.Lock();

	unsigned long DIOData = 0;

	DIOData = 0xFFF5FFF1;							// Set Port C to 5 and Port A to 0xF1  for opcode
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF1FF00 | byteLo;					// Serial Comm operand low byte
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF2FF00 | byteHi;					// Serial Comm operand high byte
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF7FF00;							// Write it to the board step 4
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF0FF00;							// Write it to the board step 5
	USB_IoPortWriteAll(&DIOData);

// 	USB_IoPortWrite8( USB_PORT_C, 0x5 );
// 	USB_IoPortWrite8( USB_PORT_A, 0xF1 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x1 );
// 	USB_IoPortWrite8( USB_PORT_A, byteLo );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x2 );
// 	USB_IoPortWrite8( USB_PORT_A, byteHi );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x7 );
// 	USB_IoPortWrite8( USB_PORT_A, 0x0 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x0 );
// 	USB_IoPortWrite8( USB_PORT_A, 0x0 );

	Sleep(1);
	m_mutexPortsDI16A.Unlock();
}

void WINAPI USB_CommToBoards2( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 )
{
//	TRACE( "USB_Comm2: op1 - 4 = 0x%02X, 0x%02X, 0x%02X, 0x%02X\n", bOp1, bOp2, bOp3, bOp4 );
	m_mutexPortsDI16A.Lock();

	int i, nTimes = 0;
	unsigned char input[4];
	while( nTimes < 10 )
	{
		DIO_ReadAll(deviceIndex, &input);
		if( input[1] == 0x01 && input[3] == 0x03 )	// cB,cD = 1,3 means board comm ready
			break;
		nTimes++;
	}
//	TRACE( "Comm2 before times = %d, 0x%02X,0x%02X,0x%02X,0x%02X\n", nTimes, bOp1, bOp2, bOp3, bOp4 );

	// Write/read via MCI to/from sub boards.
	unsigned long DIOData = 0;

	DIOData = 0xFFF5FFF1;							// Set Port C to 5 and Port A to 0xF1  for opcode
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF1FF00 | bOp1;					// Serial Comm operand low byte
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF2FF00 | bOp2;					// Serial Comm operand high byte
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF7FF00;							// Write it to the board step 4
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF0FF00;							// Write it to the board step 5
	USB_IoPortWriteAll(&DIOData);

// 	USB_IoPortWrite8( USB_PORT_C, 0x5 );
// 	USB_IoPortWrite8( USB_PORT_A, 0xF1 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x1 );
// 	USB_IoPortWrite8( USB_PORT_A, bOp1 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x2 );
// 	USB_IoPortWrite8( USB_PORT_A, bOp2 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x7 );
// 	USB_IoPortWrite8( USB_PORT_A, 0x0 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x0 );
// 	USB_IoPortWrite8( USB_PORT_A, 0x0 );

	// To assure that software execution is not overrunning the board communication time window,
	// software waits at least 10uS between >> sending sets of instruction clusters.
	for(i=0; i<10000; i++)							// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);

	DIOData = 0xFFF5FFF1;							// Set Port C to 5 and Port A to 0xF1  for opcode
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF1FF00 | bOp3;					// Serial Comm operand low byte
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF2FF00 | bOp4;					// Serial Comm operand high byte
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF7FF00;							// Write it to the board step 4
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF0FF00;							// Write it to the board step 5
	USB_IoPortWriteAll(&DIOData);
	
// 	USB_IoPortWrite8( USB_PORT_C, 0x5 );
// 	USB_IoPortWrite8( USB_PORT_A, 0xF1 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x1 );
// 	USB_IoPortWrite8( USB_PORT_A, bOp3 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x2 );
// 	USB_IoPortWrite8( USB_PORT_A, bOp4 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x7 );
// 	USB_IoPortWrite8( USB_PORT_A, 0x0 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x0 );
// 	USB_IoPortWrite8( USB_PORT_A, 0x0 );

	// To assure that software execution is not overrunning the board communication time window,
	// software waits at least 10uS between >> sending sets of instruction clusters.
	for(i=0; i<10000; i++)						// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);

	nTimes = 0;
	while( nTimes < 10 )
	{
		DIO_ReadAll(deviceIndex, &input);
		if( input[1] == 0x01 && input[3] == 0x03 )	// cB,cD = 1,3 means board comm ready
			break;
		nTimes++;
	}
//	TRACE( "Comm2 AFTER times = %d\n", nTimes );
	if( nTimes == 10 )
	{
		USB_WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
		TRACE( "Comm Reset\n" );
	}

	m_mutexPortsDI16A.Unlock();
}

void WINAPI USB_CommToBoards3( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, BYTE bOp5, BYTE bOp6 )
{
//	TRACE( "USB_Comm3: op1 - 6 = 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X\n", bOp1, bOp2, bOp3, bOp4, bOp5, bOp6 );
	m_mutexPortsDI16A.Lock();

	int i, nTimes = 0;
	unsigned char input[4];
	while( nTimes < 10 )
	{
		DIO_ReadAll(deviceIndex, &input);
		if( input[1] == 0x01 && input[3] == 0x03 )	// cB,cD = 1,3 means board comm ready
			break;
		nTimes++;
	}
//	TRACE( "Comm3 before times = %d, 0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,\n", nTimes, bOp1, bOp2, bOp3, bOp4, bOp5, bOp6 );

	// Write/read via MCI to/from SCN board.
	unsigned long DIOData = 0;

	DIOData = 0xFFF5FFF1;							// Set Port C to 5 and Port A to 0xF1  for opcode
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF1FF00 | bOp1;					// Serial Comm operand low byte
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF2FF00 | bOp2;					// Serial Comm operand high byte
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF7FF00;							// Write it to the board step 4
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF0FF00;							// Write it to the board step 5
	USB_IoPortWriteAll(&DIOData);

// 	USB_IoPortWrite8( USB_PORT_C, 0x5 );
// 	USB_IoPortWrite8( USB_PORT_A, 0xF1 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x1 );
// 	USB_IoPortWrite8( USB_PORT_A, bOp1 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x2 );
// 	USB_IoPortWrite8( USB_PORT_A, bOp2 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x7 );
// 	USB_IoPortWrite8( USB_PORT_A, 0x0 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x0 );
// 	USB_IoPortWrite8( USB_PORT_A, 0x0 );

	// To assure that software execution is not overrunning the board communication time window,
	// software waits at least 10uS between >> sending sets of instruction clusters.
	for(i=0; i<10000; i++)							// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);

	DIOData = 0xFFF5FFF1;							// Set Port C to 5 and Port A to 0xF1  for opcode
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF1FF00 | bOp3;					// Serial Comm operand low byte
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF2FF00 | bOp4;					// Serial Comm operand high byte
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF7FF00;							// Write it to the board step 4
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF0FF00;							// Write it to the board step 5
	USB_IoPortWriteAll(&DIOData);

// 	USB_IoPortWrite8( USB_PORT_C, 0x5 );
// 	USB_IoPortWrite8( USB_PORT_A, 0xF1 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x1 );
// 	USB_IoPortWrite8( USB_PORT_A, bOp3 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x2 );
// 	USB_IoPortWrite8( USB_PORT_A, bOp4 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x7 );
// 	USB_IoPortWrite8( USB_PORT_A, 0x0 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x0 );
// 	USB_IoPortWrite8( USB_PORT_A, 0x0 );

	// To assure that software execution is not overrunning the board communication time window,
	// software waits at least 10uS between >> sending sets of instruction clusters.
	for(i=0; i<10000; i++)						// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);

	DIOData = 0xFFF5FFF1;							// Set Port C to 5 and Port A to 0xF1  for opcode
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF1FF00 | bOp5;					// Serial Comm operand low byte
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF2FF00 | bOp6;					// Serial Comm operand high byte
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF7FF00;							// Write it to the board step 4
	USB_IoPortWriteAll(&DIOData);

	DIOData = 0xFFF0FF00;							// Write it to the board step 5
	USB_IoPortWriteAll(&DIOData);

// 	USB_IoPortWrite8( USB_PORT_C, 0x5 );
// 	USB_IoPortWrite8( USB_PORT_A, 0xF1 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x1 );
// 	USB_IoPortWrite8( USB_PORT_A, bOp5 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x2 );
// 	USB_IoPortWrite8( USB_PORT_A, bOp6 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x7 );
// 	USB_IoPortWrite8( USB_PORT_A, 0x0 );
// 
// 	USB_IoPortWrite8( USB_PORT_C, 0x0 );
// 	USB_IoPortWrite8( USB_PORT_A, 0x0 );

	// To assure that software execution is not overrunning the board communication time window,
	// software waits at least 10uS between >> sending sets of instruction clusters.
	for(i=0; i<10000; i++)						// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);

	nTimes = 0;
	while( nTimes < 10 )
	{
		DIO_ReadAll(deviceIndex, &input);
		if( input[1] == 0x01 && input[3] == 0x03 )	// cB,cD = 1,3 means board comm ready
			break;
		nTimes++;
	}
//	TRACE( "Comm3 AFTER times = %d\n", nTimes );
	if( nTimes == 10 )
	{
		USB_WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
		TRACE( "Comm Reset\n" );
	}

	m_mutexPortsDI16A.Unlock();
}

void WINAPI USB_CommToBoardsDirect( BYTE byteLen, BYTE* pbData )
{
	// Write via MCI to subboards.
//	TRACE( "\nLen = %d, ID = 0x%02X\n", byteLen, pbData[0] );
	if( byteLen == 4 )
		USB_CommToBoards2( pbData[0], pbData[1], pbData[2], pbData[3] );
	else// if( byteLen == 6 )
		USB_CommToBoards3( pbData[0], pbData[1], pbData[2], pbData[3], pbData[4], pbData[5] );
// 	else
// 		for ( int i=0; i<byteLen; i+=2 )
// 			USB_CommToBoards( pbData[i], pbData[i+1] );
}

BOOL WINAPI USB_WriteToBoardDirect( BYTE bOpcode, BOOL blOperand, BYTE byteLo, BYTE byteHi )
{
	m_mutexPortsDI16A.Lock();
//	TRACE( "USB_Write_Direct: bOpcode = 0x%02X, blOperand = 0x%02X, byteLo = 0x%02X, byteHi = 0x%02X\n", bOpcode, blOperand, byteLo, byteHi );
	unsigned long DIOData = 0;

	if( blOperand )
	{
		DIOData = 0xFFF5FF00 | bOpcode;			// opcode in A, operand to follow
		USB_IoPortWriteAll(&DIOData);

		DIOData = 0xFFF2FF00 | byteHi;			// High byte operand
		USB_IoPortWriteAll(&DIOData);

		DIOData = 0xFFF1FF00 | byteLo;			// Low byte operand
		USB_IoPortWriteAll(&DIOData);

		DIOData = 0xFFF7FF00;					// Write it to the board
		USB_IoPortWriteAll(&DIOData);

		DIOData = 0xFFF0FF00;					// Clear it to the board
		USB_IoPortWriteAll(&DIOData);

// 		USB_IoPortWrite8( USB_PORT_C, 0x5 );
// 		USB_IoPortWrite8( USB_PORT_A, bOpcode );
// 
// 		USB_IoPortWrite8( USB_PORT_C, 0x1 );
// 		USB_IoPortWrite8( USB_PORT_A, byteLo );
// 
// 		USB_IoPortWrite8( USB_PORT_C, 0x2 );
// 		USB_IoPortWrite8( USB_PORT_A, byteHi );
// 
// 		USB_IoPortWrite8( USB_PORT_C, 0x7 );
// 		USB_IoPortWrite8( USB_PORT_A, 0x0 );
// 
// 		USB_IoPortWrite8( USB_PORT_C, 0x0 );
//		USB_IoPortWrite8( USB_PORT_A, 0x0 );
	}
	else
	{
		DIOData = 0xFFF4FF00 | bOpcode;			// opcode in A, operand to follow
		USB_IoPortWriteAll(&DIOData);

		DIOData = 0xFFF7FF00 | bOpcode;			// Write it to the board
		USB_IoPortWriteAll(&DIOData);

		DIOData = 0xFFF0FF00 | bOpcode;			// Clear it to the board
		USB_IoPortWriteAll(&DIOData);

// 		USB_IoPortWrite8( USB_PORT_C, 0x4 );
// 		USB_IoPortWrite8( USB_PORT_A, bOpcode );
// 
// 		USB_IoPortWrite8( USB_PORT_C, 0x7 );
// 		USB_IoPortWrite8( USB_PORT_A, bOpcode );
// 
// 		USB_IoPortWrite8( USB_PORT_C, 0x0 );
// 		USB_IoPortWrite8( USB_PORT_A, bOpcode );
	}

	Sleep(1);

	m_mutexPortsDI16A.Unlock();
	return TRUE;
}

BOOL WINAPI USB_IsVideo()
{
	return bVideoON;
}

DWORD WINAPI USB_VideoON()
{
	USB_WriteToBoard( USB_Video_ON, FALSE, 0 );
	bVideoON = TRUE;
	return 0;
}

DWORD WINAPI USB_VideoOFF()
{
	USB_WriteToBoard( USB_Video_OFF, FALSE, 0 );
	bVideoON = FALSE;
	return 0;
}

DWORD WINAPI USB_LoopbackWords( unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred, short nNeedLog )
{
	DWORD dwRet = 0;

	ZeroMemory(pnFrameData, sizeof(pnFrameData));

	Sleep(10);

	unsigned long status = DIO_StreamOpen( deviceIndex, TRUE );	// TRUE for read; FALSE for write.
	status = DIO_StreamFrame( deviceIndex, FramePoints, &(pnFrameData[1]), BytesTransferred );
	status = DIO_StreamClose( deviceIndex );
	status = AIOUSB_ClearFIFO(deviceIndex, 0);//TIME_METHOD_WAIT_INPUT_ENABLE);
	pnFrameData[0] = (*BytesTransferred) /2UL;

	return dwRet;
}

DWORD WINAPI USB_LoopbackWords2( unsigned short nModID, unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred, short nNeedLog )
{
	m_mutexPortsDI16A.Lock();

	DWORD dwRet = 0;
	ZeroMemory(pnFrameData, sizeof(pnFrameData));

	// set pix_clock to 2
	USB_WriteToBoard( USB_SetPixelRate, TRUE, 2 );
	// 清空FIFO
	USB_ClearFIFO();

	unsigned long DIOData = 0;
	BYTE bOp1, bOp2;
	if( nModID > 1 )
	{
		// 从功能规划长远考虑，还是读VAC板的外部开关
		bOp1	= USB_CID_VAC_ReadSelExtSwitchState;
		bOp2	= USB_TM_VAC_Read_16;

		///////////////////////////////////////////////////
		// 先发三个设置外部开关的命令
// 		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip,
// 			USB_TM_VAC_Write_08,
// 			0x0,
// 			0x19 | 0x40 );
		DIOData = 0xFFF5FFF1;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF1FF00 | USB_CID_VAC_ExpanderChip;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF2FF00 | USB_TM_VAC_Write_08;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF7FF00;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF0FF00;
		USB_IoPortWriteAll(&DIOData);
		for(int i=0; i<10000; i++)	// 1000000次开平方运算16ms，10000次开平方运算16us
			double x = sqrt(4.);

		DIOData = 0xFFF5FFF1;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF1FF00;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF2FF00 | 0x19 | 0x40;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF7FF00;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF0FF00;
		USB_IoPortWriteAll(&DIOData);
		for(int i=0; i<10000; i++)	// 1000000次开平方运算16ms，10000次开平方运算16us
			double x = sqrt(4.);

// 		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip,
// 			USB_TM_VAC_Write_08,
// 			0x0,
// 			0x1A | 0x40 );
		DIOData = 0xFFF5FFF1;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF1FF00 | USB_CID_VAC_ExpanderChip;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF2FF00 | USB_TM_VAC_Write_08;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF7FF00;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF0FF00;
		USB_IoPortWriteAll(&DIOData);
		for(int i=0; i<10000; i++)	// 1000000次开平方运算16ms，10000次开平方运算16us
			double x = sqrt(4.);

		DIOData = 0xFFF5FFF1;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF1FF00;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF2FF00 | 0x1A | 0x40;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF7FF00;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF0FF00;
		USB_IoPortWriteAll(&DIOData);
		for(int i=0; i<10000; i++)	// 1000000次开平方运算16ms，10000次开平方运算16us
			double x = sqrt(4.);

// 		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip,
// 			USB_TM_VAC_Write_08,
// 			0x0,
// 			0x1B | 0x40 );
		DIOData = 0xFFF5FFF1;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF1FF00 | USB_CID_VAC_ExpanderChip;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF2FF00 | USB_TM_VAC_Write_08;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF7FF00;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF0FF00;
		USB_IoPortWriteAll(&DIOData);
		for(int i=0; i<10000; i++)	// 1000000次开平方运算16ms，10000次开平方运算16us
			double x = sqrt(4.);

		DIOData = 0xFFF5FFF1;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF1FF00;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF2FF00 | 0x1B | 0x40;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF7FF00;
		USB_IoPortWriteAll(&DIOData);
		DIOData = 0xFFF0FF00;
		USB_IoPortWriteAll(&DIOData);
		for(int i=0; i<10000; i++)	// 1000000次开平方运算16ms，10000次开平方运算16us
			double x = sqrt(4.);
		///////////////////////////////////////////////////
	}
	else
	{
		// VAC板TC等的状态
// 		USB_CommToBoards2(	USB_CID_VAC_ReadThermoCCGComp,
// 						USB_TM_VAC_Read_16,
// 						0x0,
// 						0x0 );
		bOp1	= USB_CID_VAC_ReadThermoCCGComp;
		bOp2	= USB_TM_VAC_Read_16;
	}

	///////////////////////////////////////////////////////
	// Write/read via MCI to/from sub boards.
	DIOData = 0xFFF5FFF1;
	USB_IoPortWriteAll(&DIOData);
	DIOData = 0xFFF1FF00 | bOp1;
	USB_IoPortWriteAll(&DIOData);
	DIOData = 0xFFF2FF00 | bOp2;
	USB_IoPortWriteAll(&DIOData);
	DIOData = 0xFFF7FF00;
	USB_IoPortWriteAll(&DIOData);
	DIOData = 0xFFF0FF00;
	USB_IoPortWriteAll(&DIOData);
	for(int i=0; i<10000; i++)	// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);

	DIOData = 0xFFF5FFF1;
	USB_IoPortWriteAll(&DIOData);
	DIOData = 0xFFF1FF00;
	USB_IoPortWriteAll(&DIOData);
	DIOData = 0xFFF2FF00;
	USB_IoPortWriteAll(&DIOData);
	DIOData = 0xFFF7FF00;
	USB_IoPortWriteAll(&DIOData);
	DIOData = 0xFFF0FF00;
	USB_IoPortWriteAll(&DIOData);
	for(int i=0; i<10000; i++)	// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
	///////////////////////////////////////////////////////

	unsigned long status = DIO_StreamOpen( deviceIndex, TRUE );	// TRUE for read; FALSE for write.
	unsigned long status2 = DIO_StreamFrame( deviceIndex, FramePoints, &(pnFrameData[1]), BytesTransferred );
	unsigned long status3 = DIO_StreamClose( deviceIndex );
	unsigned long status4 = AIOUSB_ClearFIFO(deviceIndex, 0);//TIME_METHOD_WAIT_INPUT_ENABLE);
	pnFrameData[0] = (*BytesTransferred) /2UL;

	if( status > 0 || status2 > 0 || status3 > 0 || status4 > 0 )
		dwRet = 1;

	m_mutexPortsDI16A.Unlock();
	return dwRet;
}

BOOL WINAPI	USB_BoardRead2432( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred )
{
//	USB_CommToBoards2( bOp1, bOp2, bOp3, bOp4 );
	m_mutexPortsDI16A.Lock();
	ZeroMemory(pnFrameData, sizeof(pnFrameData));

	unsigned long DIOData = 0;
	DIOData = 0xFFF5FFF1;
	USB_IoPortWriteAll(&DIOData);
	DIOData = 0xFFF1FF00 | bOp1;
	USB_IoPortWriteAll(&DIOData);
	DIOData = 0xFFF2FF00 | bOp2;
	USB_IoPortWriteAll(&DIOData);
	DIOData = 0xFFF7FF00;
	USB_IoPortWriteAll(&DIOData);
	DIOData = 0xFFF0FF00;
	USB_IoPortWriteAll(&DIOData);
// 	USB_IoPortWrite8( USB_PORT_C, 0x5 );
// 	USB_IoPortWrite8( USB_PORT_A, 0xF1 );
// 	USB_IoPortWrite8( USB_PORT_C, 0x1 );
// 	USB_IoPortWrite8( USB_PORT_A, bOp1 );
// 	USB_IoPortWrite8( USB_PORT_C, 0x2 );
// 	USB_IoPortWrite8( USB_PORT_A, bOp2 );
// 	USB_IoPortWrite8( USB_PORT_C, 0x7 );
// 	USB_IoPortWrite8( USB_PORT_A, 0x0 );
// 	USB_IoPortWrite8( USB_PORT_C, 0x0 );
// 	USB_IoPortWrite8( USB_PORT_A, 0x0 );
	for(int i=0; i<10000; i++)	// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
	DIOData = 0xFFF5FFF1;
	USB_IoPortWriteAll(&DIOData);
	DIOData = 0xFFF1FF00 | bOp3;
	USB_IoPortWriteAll(&DIOData);
	DIOData = 0xFFF2FF00 | bOp4;
	USB_IoPortWriteAll(&DIOData);
	DIOData = 0xFFF7FF00;
	USB_IoPortWriteAll(&DIOData);
	DIOData = 0xFFF0FF00;
	USB_IoPortWriteAll(&DIOData);
// 	USB_IoPortWrite8( USB_PORT_C, 0x5 );
// 	USB_IoPortWrite8( USB_PORT_A, 0xF1 );
// 	USB_IoPortWrite8( USB_PORT_C, 0x1 );
// 	USB_IoPortWrite8( USB_PORT_A, bOp3 );
// 	USB_IoPortWrite8( USB_PORT_C, 0x2 );
// 	USB_IoPortWrite8( USB_PORT_A, bOp4 );
// 	USB_IoPortWrite8( USB_PORT_C, 0x7 );
// 	USB_IoPortWrite8( USB_PORT_A, 0x0 );
// 	USB_IoPortWrite8( USB_PORT_C, 0x0 );
// 	USB_IoPortWrite8( USB_PORT_A, 0x0 );
	for(int i=0; i<10000; i++)	// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);

	unsigned long status = DIO_StreamOpen( deviceIndex, TRUE );	// TRUE for read; FALSE for write.
	unsigned long status2 = DIO_StreamFrame( deviceIndex, FramePoints, &(pnFrameData[1]), BytesTransferred );
	unsigned long status3 = DIO_StreamClose( deviceIndex );
	unsigned long status4 = AIOUSB_ClearFIFO(deviceIndex, 0);//TIME_METHOD_WAIT_INPUT_ENABLE);
	pnFrameData[0] = (*BytesTransferred) /2UL;

	BOOL bRet = TRUE;
	if( status > 0 || status2 > 0 || status3 > 0 || status4 > 0 || pnFrameData[0] < 1 )
		bRet = FALSE;
	m_mutexPortsDI16A.Unlock();
	return bRet;
}

DWORD WINAPI USB_ReadMonitorData( int nModID, int nADCtype, int nADCnum, int nADCchan, short nNeedLog )
{
	DWORD dwRet = -1;

    int nTTID = 32;	// Transfer Type ID
    int nCID;		// Circuit Selection ID
	BYTE bOSR, bChanNib, bEnSE ;
	DWORD dwWdata;
	bOSR = 0x90;			// 144, which takes ~73mS to complete conversion (not including transfer latencies)
	double m_dADCdiffSE = m_dADCchanModes[nADCnum*6 +nADCchan *16 +1];			// If value = 1 then S.E. / If value = 2 then diff mode
	if( m_dADCdiffSE == 2 )
		bEnSE = 0xA0;		// 0xA0 for diff measurements
	else
		bEnSE = 0xB0;		// 0xB0 for S.E. measurement

	if( nADCtype == 1 )	// LTC2448 type
	{
		switch( nADCchan )
		{
		case	0:
			bChanNib	= 0;
//			dwWdata = (DWROD)( 0xB000 | dwOSR & 0xFFFF );	// Select ADC channel 0 (Odd = 0), OSR level 9, TWOX = 0
			break;
		case	1:
			bChanNib	= 8;
//			dwWdata = (DWROD)( 0xB800 | dwOSR & 0xFFFF );	// Select ADC channel 1 (Odd = 1)
			break;
		case	2:
			bChanNib	= 1;
//			dwWdata = (DWROD)( 0xB100 | dwOSR & 0xFFFF );	// Select ADC channel 2 (Odd = 0)
			break;
		case	3:
			bChanNib	= 9;
//			dwWdata = (DWROD)( 0xB900 | dwOSR & 0xFFFF );	// Select ADC channel 3 (Odd = 1)
			break;
		case	4:
			bChanNib	= 2;
//			dwWdata = (DWROD)( 0xB200 | dwOSR & 0xFFFF );	// Select ADC channel 4 (Odd = 0)
			break;
		case	5:
			bChanNib	= 10;
//			dwWdata = (DWROD)( 0xBA00 | dwOSR & 0xFFFF );	// Select ADC channel 5 (Odd = 1)
			break;
		case	6:
			bChanNib	= 3;
//			dwWdata = (DWROD)( 0xB300 | dwOSR & 0xFFFF );	// Select ADC channel 6 (Odd = 0)
			break;
		case	7:
			bChanNib	= 11;
//			dwWdata = (DWROD)( 0xBB00 | dwOSR & 0xFFFF );	// Select ADC channel 7 (Odd = 1)
			break;
		case	8:
			bChanNib	= 4;
//			dwWdata = (DWROD)( 0xB400 | dwOSR & 0xFFFF );	// Select ADC channel 8 (Odd = 0)
			break;
		case	9:
			bChanNib	= 12;
//			dwWdata = (DWROD)( 0xBC00 | dwOSR & 0xFFFF );	// Select ADC channel 9 (Odd = 1)
			break;
		case	10:
			bChanNib	= 5;
//			dwWdata = (DWROD)( 0xB500 | dwOSR & 0xFFFF );	// Select ADC channel 10 (Odd = 0)
			break;
		case	11:
			bChanNib	= 13;
//			dwWdata = (DWROD)( 0xBD00 | dwOSR & 0xFFFF );	// Select ADC channel 11 (Odd = 1)
			break;
		case	12:
			bChanNib	= 6;
//			dwWdata = (DWROD)( 0xB600 | dwOSR & 0xFFFF );	// Select ADC channel 12 (Odd = 0)
			break;
		case	13:
			bChanNib	= 14;
//			dwWdata = (DWROD)( 0xBE00 | dwOSR & 0xFFFF );	// Select ADC channel 13 (Odd = 1)
			break;
		case	14:
			bChanNib	= 7;
//			dwWdata = (DWROD)( 0xB700 | dwOSR & 0xFFFF );	// Select ADC channel 14 (Odd = 0)
			break;
		case	15:
			bChanNib	= 15;
//			dwWdata = (DWROD)( 0xBF00 | dwOSR & 0xFFFF );	// Select ADC channel 15 (Odd = 1)
			break;
		default:
			break;
		}
		if( m_dADCdiffSE == 2 )
			bChanNib &= 7;		// Force bit 3 of ChanNib to 0 if diff measurement

		dwWdata = (bEnSE *256) | (bChanNib *256) | bOSR;
	}	// if LTC2448 type
	else if( nADCtype == 2 )	// LTC1859 type, format write data for channel select, etc.
	{
		switch( nADCchan )
		{
		case	0:				// Condenser Lens current monitor
			dwWdata	= 0x8000;	// ADC Channel 0, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	1:				// Objective Lens current monitor
			dwWdata	= 0xC000;	// ADC Channel 1, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	2:				// X alignment coil current monitor
			dwWdata	= 0x9000;	// ADC Channel 2, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	3:				// Y alignment coil current monitor
			dwWdata	= 0xD000;	// ADC Channel 3, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	4:				// Amplifier positive power supplies voltages monitor
			dwWdata	= 0xA000;	// ADC Channel 4, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	5:				// Amplifier positive power supplies voltages monitor
			dwWdata	= 0xE000;	// ADC Channel 5, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	6:				// Thermistor input 1
			dwWdata	= 0xB000;	// ADC Channel 6, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		case	7:				// Thermistor input 2
			dwWdata	= 0xF000;	// ADC Channel 7, S.E., Uni/Gain = 00, NAP/Sleep = 00
			break;
		}
	}	// LTC1859 type

	switch( nADCnum )
	{
	case	0:									// SCN board LTC2448 (one per board)
		nCID			= USB_CID_SCN_ADC_Read;	// Circuit ID 0xC8
		break;
	case	1:									// LNS board LTC1859 (one per board)
		nTTID			= 16;					// Transfer type = 16 bit read, argument "Rsize" passed to BoardRead816 with value of "16"
		nCID			= USB_CID_LNS_ADC_Read;	// Circuit ID 0xC8
		break;
	case	2:									// VAC board LTC2448 #0
		nCID			= USB_CID_VAC_ADC0_Read;// Circuit ID 0xC6
		break;
	case	3:									// VAC board LTC2448 #1
		nCID			= USB_CID_SPR_ADC1_Read;// Circuit ID 0xCE
		break;
	case	4:									// SPR board LTC2448 #0
		nCID			= USB_CID_SPR_ADC0_Read;// Circuit ID 0xC6
		break;
	case	5:									// SPR board LTC2448 #1
		nCID			= USB_CID_SPR_ADC1_Read;// Circuit ID 0xCE
		break;
	}

	if( nADCtype == 1 )
	{
		dwRet = BoardRead2432( nTTID, nModID, (BYTE)nCID, dwWdata );
//		CString str;
		// Two back-to-back 16 bit transfers to receive RB data
//		str.Format( "%ld %ld", m_nBoardReadDataBuf[4*2+1], m_nBoardReadDataBuf[5*2+1] );
//		GetDlgItem( IDC_EDIT_ADCreadValInTB )->SetWindowText( str );
	}
	else if( nADCtype == 2 )
	{
		dwRet = BoardRead816( nTTID, nModID, (BYTE)nCID, (WORD)dwWdata );
//		CString str;
		// Only 16 bits max of readback data
//		str.Format( "%ld", m_nBoardReadDataBuf[4*2+1] );
//		GetDlgItem( IDC_EDIT_ADCreadValInTB )->SetWindowText( str );
	}
//	else
//		TRACE( "USER ERROR: NO ADC and channel has been selected" );

	if( dwRet == 0 )
		ADCvconv( nADCtype );
	else if( dwRet > 0 )
	{
//		USB_WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
		nLoopbackWords_Times = 0;
	}

	return dwRet;
}
/*
BOOL WaitDoorbell( DWORD& dwValue, DWORD dwTimeOut )
{
	unsigned long	rc;
	U32			dwRet;

    PLX_INTR	intfield;
    HANDLE		mgeventHandle;

    memset ( &intfield, 0, sizeof(PLX_INTR) );
	intfield.PciDoorbell = 1;
	rc = PlxIntrAttach( g_hPlxDevice, intfield, &mgeventHandle );
    if ( rc != ApiSuccess )
    {
        return FALSE;
    }
    __try
    {
		dwRet = WaitForSingleObject( mgeventHandle, dwTimeOut );
    }
    __except( EXCEPTION_EXECUTE_HANDLER )
    {
		;
    }
    if ( dwRet == WAIT_FAILED || dwRet == WAIT_TIMEOUT )
    {
        return FALSE;
    }

	dwValue = PlxRegisterDoorbellRead( g_hPlxDevice, &rc );
	if ( rc != ApiSuccess )
	{
        return FALSE;
	}

	return TRUE;
}
*/
DWORD USB_GetDMABuffSize()
{
	return 0;//g_MemoryInfo.Size;
}



#ifdef DEBUG_MCI_ACQDATA_TEST


BOOL DMATEST_OpenDMAChannel0() 
{
	// TODO: Add your control notification handler code here
/*	unsigned long	rc;
	
	UINT		uRegisterAddr;
	U32DATA		RegisterData;

	if (deviceIndex == 0)
		return FALSE;

	// DMA0 初始化 过程：
	// 1）工作方式	0x80	写	0x2CDC3；
	uRegisterAddr	= 0x80;
	RegisterData.dwData	= 0x2C9C3; //0x2CDC3;
	rc = PlxRegisterWrite( g_hPlxDevice, uRegisterAddr, RegisterData.dwData );
	//DescribPlxApiReturnCode( rc );
	if ( rc != ApiSuccess ) return FALSE;

	// 2）PCI地址	0x84	写	g_MemoryInfo.PhysicalAddr；
	uRegisterAddr	= 0x84;
	RegisterData.dwData	= g_MemoryInfo.PhysicalAddr;
	rc = PlxRegisterWrite( g_hPlxDevice, uRegisterAddr, RegisterData.dwData );
	//DescribPlxApiReturnCode( rc );
	if ( rc != ApiSuccess ) return FALSE;

	// 3）Local地址 0x88	写	0x78；
	uRegisterAddr	= 0x88;
	RegisterData.dwData	= ImageFIFOStartOffset;
	rc = PlxRegisterWrite( g_hPlxDevice, uRegisterAddr, RegisterData.dwData );
	//DescribPlxApiReturnCode( rc );
	if ( rc != ApiSuccess ) return FALSE;

	// 4）Local地址 0x8C	写	m_dwDMA1Size；
	uRegisterAddr	= 0x8C;
	RegisterData.dwData	= 1024;
	rc = PlxRegisterWrite( g_hPlxDevice, uRegisterAddr, RegisterData.dwData );
	//DescribPlxApiReturnCode( rc );
	if ( rc != ApiSuccess ) return FALSE;

	// 5）DMA0传输方向 0x90	写	8；
	uRegisterAddr	= 0x90;
	RegisterData.dwData	= 8;
	rc = PlxRegisterWrite( g_hPlxDevice, uRegisterAddr, RegisterData.dwData );
	//DescribPlxApiReturnCode( rc );
	if ( rc != ApiSuccess ) return FALSE;

	// OVER.
*/	
	return TRUE;
}


BOOL USB_DMATEST_ImageInputStream( void * pBuffer, IN LONG dwBlockSize )
{
/*	UINT			uRegisterAddr;
	U32DATA			RegisterData;
	UINT			uRegisterValue;
	unsigned long	rc;

	if (deviceIndex != 0)
    if( !blImageStreamInputStop )
	{
		// 1）检查 DMA0通道 状态 读 0xA8 的 Bit[0:1]；
		uRegisterAddr	= 0xA8;
		uRegisterValue	= PlxRegisterRead( g_hPlxDevice, uRegisterAddr, &rc );
		uRegisterValue &= 0x00FF;
		if (( uRegisterValue != 0x11 ) && ( uRegisterValue != 0x10 ))
			return FALSE;

		// 2）Local地址 0x8C	写	m_dwDMA1Size；
		uRegisterAddr		= 0x8C;
		RegisterData.dwData	= dwBlockSize;
		rc = PlxRegisterWrite( g_hPlxDevice, uRegisterAddr, RegisterData.dwData );
		//DescribPlxApiReturnCode( rc );

		// 3）发起DMA0传输 0xA8	写	0x03；
		uRegisterAddr	= 0xA8;
		RegisterData.dwData	= 0x03;
		rc = PlxRegisterWrite( g_hPlxDevice, uRegisterAddr, RegisterData.dwData );
		//DescribPlxApiReturnCode( rc );

		do{
			// 1）检查 DMA0通道 状态 读 0xA8 的 Bit[0:1]；
			uRegisterAddr	= 0xA8;
			uRegisterValue	= PlxRegisterRead( g_hPlxDevice, uRegisterAddr, &rc );
			uRegisterValue &= 0x00FF;
			if ( uRegisterValue == 0x01 )
				return FALSE;
			else if ( uRegisterValue != 0x11 )
				Sleep(1);
		}while ( uRegisterValue != 0x11 ); // DMA传输完成信号

        if ( rc != ApiSuccess)
        {
			return FALSE;
        }
		else
		{
			//process data
			ASSERT(pBuffer != NULL);

			memcpy( pBuffer,
				(const void *)dwChannel0UserAddr,
				dwBlockSize );
		}
	}
*/	
	return TRUE;
}


#endif //DEBUG_MCI_ACQDATA_TEST


/*
//
    The following definintions define the starting value for each enumerated
    type.  These constants must ensure that no overlapping happens between
    the error code definitions.

#define API_RETURN_CODE_STARTS              0x200
#define BSP_RETURN_CODE_STARTS              0x800

//++++++++++ Enumerated Type Definitions +++++++++++++++++++++++++++++++++++++

// API Return Code Values
typedef enum _RETURN_CODE
{
    ApiSuccess = API_RETURN_CODE_STARTS,
    ApiFailed,
    ApiAccessDenied,
    ApiDmaChannelUnavailable,
    ApiDmaChannelInvalid,
    ApiDmaChannelTypeError,
    ApiDmaInProgress,
    ApiDmaDone,
    ApiDmaPaused,
    ApiDmaNotPaused,
    ApiDmaCommandInvalid,
    ApiDmaManReady,
    ApiDmaManNotReady,
    ApiDmaInvalidChannelPriority,
    ApiDmaManCorrupted,
    ApiDmaInvalidElementIndex,
    ApiDmaNoMoreElements,
    ApiDmaSglInvalid,
    ApiDmaSglQueueFull,
    ApiNullParam,
    ApiInvalidBusIndex,
    ApiUnsupportedFunction,
    ApiInvalidPciSpace,
    ApiInvalidIopSpace,
    ApiInvalidSize,
    ApiInvalidAddress,
    ApiInvalidAccessType,
    ApiInvalidIndex,                // Invalid IOP Outbound Index
    ApiMuNotReady,
    ApiMuFifoEmpty,
    ApiMuFifoFull,
    ApiInvalidRegister,
    ApiDoorbellClearFailed,
    ApiInvalidUserPin,
    ApiInvalidUserState,
    ApiEepromNotPresent,
    ApiEepromTypeNotSupported,
    ApiEepromBlank,
    ApiConfigAccessFailed,
    ApiInvalidDeviceInfo,
    ApiNoActiveDriver,
    ApiInsufficientResources,
    ApiObjectAlreadyAllocated,
    ApiAlreadyInitialized,
    ApiNotInitialized,
    ApiBadConfigRegEndianMode,
    ApiInvalidPowerState,
    ApiPowerDown,
    ApiFlybyNotSupported,           // added for IOP 480
    ApiNotSupportThisChannel,       // added for IOP 480
    ApiNoAction,                    // added for IOP 480
    ApiHSNotSupported,              // added for IOP 480
    ApiVPDNotSupported,             // added
    ApiVpdNotEnabled,               // added
    ApiNoMoreCap,                   // added for IOP 480
    ApiInvalidOffset,               // added for IOP 480
    ApiBadPinDirection,             // added for IOP 480
    ApiPciTimeout,                  // added for all Plx chips
    ApiDmaChannelClosed,            // added for IOP 480
    ApiDmaChannelError,             // added for IOP 480
    ApiInvalidHandle,  
    ApiBufferNotReady,              // added
    ApiInvalidData,                 // Invalid data values
    ApiDoNothing,                   // added for IOP 480
    ApiDmaSglBuildFailed, 
    ApiLastError            // do not add API errors below this line
} RETURN_CODE, *PRETURN_CODE;


// BSP Error Code Values
typedef enum _BSP_ERROR_CODE
{
    BspSuccess  = BSP_RETURN_CODE_STARTS,

    // Starting of LinkSystemVectors Return Code
    BspGetSystemPointerPtrNull,
    BspPlxPrintfPtrNull,
    BspPlxGetCharsPtrNull,
    BspPlxEchoEnablePtrNull,
    BspBemPtrNull,
    BspPlxIntrHandlerGetPtrNull,
    BspPlxIntrHandlerSetPtrNull,
    BspPlxKernelResetPtrNull,
    // End of LinkSystemVectors Return Code

    BspFailedToInitBem,
    BspFailedToTestMemory,
    
    BspLastError            // do not add BSP errors below this line
} BSP_ERROR_CODE, * PBSP_ERROR_CODE;


// PCI driver error codes

#define STATUS_DMA_CHANNEL_INVALID          0xE000F204L
#define STATUS_IRP_USER_CANCEL              0xE000F2F1L
#define STATUS_DMA_CHANNEL_UNAVAILABLE      0xE000F203L
#define STATUS_NULL_PARAM                   0xE000F213L
#define STATUS_DMA_IN_PROGRESS              0xE000F206L
#define STATUS_DMA_SGL_BUILD_FAILURE        0xE000F2F0L
#define STATUS_POWER_DOWN                   0xE000F22FL

*/