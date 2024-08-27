// AdapterUSB.cpp : implementation file
//

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <memory.h>

#include "AdapterUSB.h"
#include <USB_Functions.h>

#include <AIOUSB.h>
#include <WinUSB.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCriticalSection CAdapterUSB::m_mutexPorts;

/////////////////////////////////////////////////////////////////////////////
// CAdapterUSB

CAdapterUSB::CAdapterUSB()
{
	bVideoON				= TRUE;
	nLoopbackWords_Times	= 0;
	USB_dwErrorCount		= 0;

	dREAD_CLOCK				= 0.0;
	dWRITE_CLOCK			= 0.0;

	m_bUSB_Ready			= FALSE;
}

CAdapterUSB::~CAdapterUSB()
{
	USB_CloseCard();
}


/////////////////////////////////////////////////////////////////////////////
// CAdapterUSB message handlers

/////////////////////////////////////////////////////////////////////////////
// 

BOOL CAdapterUSB::WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData )
{
	if( !m_bUSB_Ready )
		return FALSE;
	
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

void CAdapterUSB::CommToBoards( BYTE byteLo, BYTE byteHi )
{
	if( !m_bUSB_Ready )
		return;
	
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

DWORD CAdapterUSB::BoardRead0816( int nRsize, int nModID, BYTE bCktAddr, WORD wWdata )
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

	CommToBoards(	bOP1,						// OP1 = ckt address
					bOP2 );						// OP2 = Transfer type and Module ID
	CommToBoards(	bOP3,						// OP3: Set to 0 for 8 bit transaction, Low byte for 16 bit transaction
					bOP4 );						// OP4: ONLY valid data byte for 8 bit write, upper byte for 16 bit transaction

	// **************** READ PORTION ******************** //
	unsigned long lBytesTransferred = 0;

	unsigned long lRet = AIOUSB_ClearFIFO(deviceIndex, 0);//TIME_METHOD_WAIT_INPUT_ENABLE);
	DIO_StreamOpen( deviceIndex, TRUE );	// TRUE for read; FALSE for write.
	DIO_StreamFrame( deviceIndex, ReadBack_DataSize, &(m_nFrameData[0]), &lBytesTransferred );
	DIO_StreamClose( deviceIndex );
	if( lBytesTransferred < 3 )
	{
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
		str.Format( "%04X, ", m_nFrameData[i] );
		strMsg += str;
		strList.Format( "@ %ld (hex %04X)", m_nFrameData[i], m_nFrameData[i] );
//		pParent->m_listReadback.AddString( strList );

		// 直到找到0xD22D才开始计数，前边的数据忽略
		if( m_nFrameData[i] == 0xD22D )
			bFound = TRUE;
		if( !bFound )
			continue;

		m_nRBwdCnt++;
		m_nBoardReadDataeBuf[ m_nRBwdCnt*2 +1 ] = m_nFrameData[i];	// Numeric value (0-FFFF range)
		switch( m_nFrameData[i] )
		{
		case	0xFFFF:
			m_nBoardReadDataeBuf[ m_nRBwdCnt*2 +2 ] = 9;
			break;
		case	0xD22D:
			m_nBoardReadDataeBuf[ m_nRBwdCnt*2 +2 ] = 4;
			break;
		default:
			m_nBoardReadDataeBuf[ m_nRBwdCnt*2 +2 ] = 1;
			break;
		}
//		ADCreadingTB.Text = ADCreadingTB.Text + " " + V2hex$
//      ReadbackLB.AddItem "@" + Str(I) + ":" + S2 + " " + Str(V22) + " " + Str(V21) + " (hex" + V2hex$ + ")"
	}
	m_nBoardReadDataeBuf[0] = m_nRBwdCnt;	// # of valid readback words contained in the array
//	GetDlgItem( IDC_EDIT_ADCreadingTB )->SetWindowText( strMsg );

	return dwRet;
}

DWORD CAdapterUSB::BoardRead2432( int nRsize, int nModID, BYTE bCktAddr, DWORD dwWdata )
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

	CommToBoards(	bOP1,						// OP1 = ckt address
					bOP2 );						// OP2 = Transfer type and Module ID
	CommToBoards(	bOP3,						// OP3: Set to 0 for 8 bit transaction, Low byte for 16 bit transaction
					bOP4 );						// OP4: ONLY valid data byte for 8 bit write, upper byte for 16 bit transaction

	// **************** READ PORTION ******************** //
	unsigned long lBytesTransferred = 0;

	unsigned long lRet = AIOUSB_ClearFIFO(deviceIndex, 0);//TIME_METHOD_WAIT_INPUT_ENABLE);
	DIO_StreamOpen( deviceIndex, TRUE );	// TRUE for read; FALSE for write.
	DIO_StreamFrame( deviceIndex, ReadBack_DataSize, &(m_nFrameData[0]), &lBytesTransferred );
	DIO_StreamClose( deviceIndex );
	if( lBytesTransferred < 3 )
	{
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
		str.Format( "%04X, ", m_nFrameData[i] );
		strMsg += str;
		strList.Format( "@ %ld (hex %04X)", m_nFrameData[i], m_nFrameData[i] );
//		pParent->m_listReadback.AddString( strList );

		// 直到找到0xD22D才开始计数，前边的数据忽略
		if( m_nFrameData[i] == 0xD22D )
			bFound = TRUE;
		if( !bFound )
			continue;

		m_nRBwdCnt++;
		m_nBoardReadDataeBuf[ m_nRBwdCnt*2 +1 ] = m_nFrameData[i];	// Numeric value (0-FFFF range)
		switch( m_nFrameData[i] )
		{
		case	0xFFFF:
			m_nBoardReadDataeBuf[ m_nRBwdCnt*2 +2 ] = 9;
			break;
		case	0xD22D:
			m_nBoardReadDataeBuf[ m_nRBwdCnt*2 +2 ] = 4;
			break;
		default:
			m_nBoardReadDataeBuf[ m_nRBwdCnt*2 +2 ] = 1;
			break;
		}
//		ADCreadingTB.Text = ADCreadingTB.Text + " " + V2hex$
//      ReadbackLB.AddItem "@" + Str(I) + ":" + S2 + " " + Str(V22) + " " + Str(V21) + " (hex" + V2hex$ + ")"
	}
	m_nBoardReadDataeBuf[0] = m_nRBwdCnt;	// # of valid readback words contained in the array
//	GetDlgItem( IDC_EDIT_ADCreadingTB )->SetWindowText( strMsg );

	return dwRet;
}

void CAdapterUSB::ADCvconv( int nADCtype )
{
	// Converts raw read data to voltage (and also to current if applicable)
	CString str;
	DWORD dwADCreadRaw, dwADCread24, dwADCreadSignBit;
	BOOL bADCvalSignBit;
	double dADCreadVrefFrac, dADCreadVrefPct;
	if( nADCtype == 1 )
	{
		// 2432 bits
//		str.Format( "%ld %ld", m_nBoardReadDataeBuf[4*2+1], m_nBoardReadDataeBuf[5*2+1] );
		dwADCreadRaw = ((DWORD)m_nBoardReadDataeBuf[4*2+1]<<16) | (DWORD)m_nBoardReadDataeBuf[5*2+1];
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
		str.Format( "%ld %ld", m_nBoardReadDataeBuf[4*2+1], m_nBoardReadDataeBuf[5*2+1] );
		dwADCreadRaw = (DWORD)(m_nBoardReadDataeBuf[4*2+1]<<16);
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


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//Read data in one line in *.ini.
//In one line the data like ***,*****,*****, they are seperated by comma.
const UINT INTSTYLE		= 1;
const UINT DOUBLESTYLE	= 2;
const UINT LONGSTYLE	= 3;
const UINT STRSTYLE		= 4;

int CAdapterUSB::ReadInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator)
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

int CAdapterUSB::SaveInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator)
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

BOOL CAdapterUSB::GetModuleFilePath(LPTSTR exeFullPath)
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

void CAdapterUSB::ReadDataFromINI()
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


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BOOL CAdapterUSB::USB_OpenDevice( void )
{
	///////////////////////////////////////////////////////////////////////////
	unsigned long pID;
	deviceIndex = 0;
	unsigned long status;
	double read_clock, write_clock;
	
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
	///////////////////////////////////////////////////////////////////////////
	
	unsigned char OutMask = 0x05; //A, C output; B, D input.
	unsigned long DIOData = 0x0;//0xFFFFFFFF;
	unsigned char TristateMask = 0x00;
	status = DIO_ConfigureEx(deviceIndex, &OutMask, &DIOData, &TristateMask);
	
	read_clock = dREAD_CLOCK;
	write_clock = dWRITE_CLOCK;
	
	status = DIO_StreamSetClocks(deviceIndex, &read_clock, &write_clock);
	
	if (status != ERROR_SUCCESS)
	{
#ifdef _DEBUG
		afxDump << "\nInitCard-- ";
		afxDump << "Error in setting clocks: Could not set clocks." << "\n";
		afxDump << "\n		ErrorCount = " << USB_dwErrorCount++ << "\n";
#endif
		return FALSE;
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
		return FALSE;
	}
	status = DIO_StreamClose(deviceIndex);
	
	unsigned long lHandle;
	unsigned long lRet = GetDeviceHandle(deviceIndex, &lHandle);
	if( lRet == 0 )
	{
		unsigned long lNewTimeout = 50;	// in milliseconds.
		BOOL bRet = WinUsb_SetPipePolicy( (WINUSB_INTERFACE_HANDLE)lHandle, 0x86, PIPE_TRANSFER_TIMEOUT, sizeof(lNewTimeout), &lNewTimeout );
	}
	
	return TRUE;
}

void CAdapterUSB::USB_ResetCard() 
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

BOOL CAdapterUSB::USB_InitCard_once( void )
{
	// send version and check returns of port B,D
	WriteToBoard( USB_ReadVersion, FALSE, 0 );

	unsigned char input[4];
	DIO_ReadAll(deviceIndex, &input);
	if( input[3] != 0x07 )	// port D: 0x01, means MCI power on
	{
		ReadDataFromINI();

		// set interrupt mask register, mask all off
		WriteToBoard( USB_SetIMR, TRUE, 0 );

		// switch analog power supplies ON
		BYTE bOP =  USB_TTTT_Write_08 << 4;
		bOP |= (BYTE)USB_MID_SPR;
		CommToBoards(	USB_CID_SPR_AnalogPower_Enable,
						bOP );
		CommToBoards(	0,
						0x01 );

		// set interrupt mask register, enable INT0,1,2,3
		WriteToBoard( USB_SetIMR, TRUE, 0x00 );
		// set mainslock off
		WriteToBoard( USB_MainsLock_OFF, FALSE, 0 );
//		WriteToBoard( USB_INT_test, FALSE, 0 );
//		WriteToBoard( USB_Video_ON, FALSE, 0 );		// bVideo_ON = TRUE;
		USB_VideoON();

		WriteToBoard( USB_DetPairSelection, TRUE, 0x00 );	// DetectorPair 0
/*
		///////////////////////////////////////////////////////////////////////
		long ReadBack_DataSize = 256;
		unsigned long lBytesTransferred = 0;
		// Allocate on the heap
		unsigned short* m_nFrameData = new unsigned short[ ReadBack_DataSize +256 ];
		ZeroMemory(m_nFrameData, sizeof(m_nFrameData));

		USB_CommToBoards2( 0x5A, 0xAD, 0x0, 0x0 );
		USB_LoopbackWords( ReadBack_DataSize, m_nFrameData, &lBytesTransferred, -1 );
		USB_CommToBoards2( 0x5A, 0xAD, 0x0, 0x0 );
		USB_LoopbackWords( ReadBack_DataSize, m_nFrameData, &lBytesTransferred, -1 );

		delete [] m_nFrameData;
*/		///////////////////////////////////////////////////////////////////////
		return TRUE;
	}

	return FALSE;
}

BOOL CAdapterUSB::USB_InitCard( void )
{
	static int nInitTimes = 0;

	if ( !USB_OpenDevice() )
        return FALSE;

	while ( !USB_InitCard_once() )
	{
		if (nInitTimes++ < 10)
		{
			USB_ResetCard();
		}
		else 
		{
//			PlxPciDeviceClose(g_hPlxDevice);
//			g_hPlxDevice = NULL;
		    return FALSE;
		}
	};

	m_bUSB_Ready = TRUE;
	return TRUE;
}

BOOL CAdapterUSB::USB_CloseCard( void )
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
	// set interrupt mask register, mask all off
	WriteToBoard( USB_SetIMR, TRUE, 0 );
	// Clear port B & D
	WriteToBoard( USB_ClearDB, FALSE, 0 );
	// stop scan
	WriteToBoard( USB_StopScan, FALSE, 0 );
	// video off
	WriteToBoard( USB_Video_OFF, FALSE, 0 );
	ClearDevices();

	return TRUE;
}

DWORD CAdapterUSB::USB_ClearFIFO( void )
{
	return (DWORD)AIOUSB_ClearFIFO(deviceIndex, 0);//TIME_METHOD_WAIT_INPUT_ENABLE);
}

DWORD CAdapterUSB::USB_StreamOpen( unsigned long bIsRead )
{
	unsigned long status = DIO_StreamOpen (deviceIndex, bIsRead); //the 1 means it is a read. Send 0 for a write
	if (status != ERROR_SUCCESS) //this really should never happen.
	{
		DIO_StreamClose(deviceIndex);
	}
	return (DWORD)status;
}

DWORD CAdapterUSB::USB_StreamClose( void )
{
	unsigned long status = DIO_StreamClose(deviceIndex);
	if( status != ERROR_SUCCESS )
	{
		status = DIO_StreamOpen(deviceIndex, 1);
		status = DIO_StreamClose(deviceIndex);
	}
	return (DWORD)status;
}

DWORD CAdapterUSB::USB_ConfigureEx(void *pOutMask, void *pData, void *pTristateMask)
{
	return (DWORD)DIO_ConfigureEx(deviceIndex, pOutMask, pData, pTristateMask);
}

#define NUM_SCANS_Test		1
#define NUM_READINGS_Test	2048
#define	LOOP_COUNTS			4

DWORD CAdapterUSB::USB_ImageInputStream1( unsigned long FramePoints, unsigned short *pFrameData, unsigned long *BytesTransferred, short nNeedLog )
{
	if( !bVideoON )	// if Video_OFF, return directly
	{
//		Sleep( 10 );
		return 5;
	}

	unsigned long numreadings = LOOP_COUNTS * (FramePoints +512);
//	unsigned short* pReadings = new unsigned short[numreadings];
	unsigned long bytes_transferred, total_readings, readingsCounts[LOOP_COUNTS], ret[LOOP_COUNTS];
	unsigned long status, Calls;
	DWORD dwRet = 0;

//	status = DIO_StreamOpen (deviceIndex, 1); //the 1 means it is a read. Send 0 for a write
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
//			ret[iScan] = 3;
			dwRet = 3;
		}
		else if( !bVideoON )
		{
//			ret[iScan] = 4;
			dwRet = 4;
		#ifdef _DEBUG
			afxDump << "\nUSB_ImageInputStream3--Error.\n";
		#endif
			delete [] pFrameData;
			return 4;
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
			for ( int iReading = 0; iReading < readingsCounts[iScan]; iReading ++ )
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
			afxDump << "\nUSB_ImageInputStream3--Error.\n";
			afxDump << "dwRet = " << dwRet << "\n";
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

BOOL CAdapterUSB::USB_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData )
{
	if( !m_bUSB_Ready )
		return FALSE;

//	TRACE( "USB_Write: bOpcode = 0x%02X, blOperand = 0x%02X, wData = 0x%04X\n", bOpcode, blOperand, wData );
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

void CAdapterUSB::USB_CommToBoards( BYTE byteLo, BYTE byteHi )
{
	if( !m_bUSB_Ready )
		return;
	
//	TRACE( "USB_Comm: byteLo = 0x%02X, byteHi = 0x%02X\n", byteLo, byteHi );
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

	Sleep(1);
}

void CAdapterUSB::USB_CommToBoards2( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 )
{
	if( !m_bUSB_Ready )
		return;

//	TRACE( "USB_Comm2: op1 - 4 = 0x%02X, 0x%02X, 0x%02X, 0x%02X\n", bOp1, bOp2, bOp3, bOp4 );
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

	Sleep(1);

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

	Sleep(1);
}

void CAdapterUSB::USB_CommToBoardsDirect( BYTE byteLen, BYTE* pbData )
{
	// Write via MCI to subboards.
//	TRACE( "\nLen = %d, ID = 0x%02X\n", byteLen, pbData[0] );
//	if( byteLen == 4 )
//		USB_CommToBoards2( pbData[0], pbData[1], pbData[2], pbData[3] );
//	else
		for ( int i=0; i<byteLen; i+=2 )
			CommToBoards( pbData[i], pbData[i+1] );
}

BOOL CAdapterUSB::USB_WriteToBoardDirect( BYTE bOpcode, BOOL blOperand, BYTE byteLo, BYTE byteHi )
{
	if( !m_bUSB_Ready )
		return FALSE;
	
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

	Sleep(1);

	return TRUE;
}

BOOL CAdapterUSB::USB_IsVideo()
{
	return bVideoON;
}

DWORD CAdapterUSB::USB_VideoON()
{
	WriteToBoard( USB_Video_ON, FALSE, 0 );
	bVideoON = TRUE;
	return 0;
}

DWORD CAdapterUSB::USB_VideoOFF()
{
	WriteToBoard( USB_Video_OFF, FALSE, 0 );
	bVideoON = FALSE;
	return 0;
}

DWORD CAdapterUSB::USB_LoopbackWords( unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred, short nNeedLog )
{
	DWORD dwRet = 0;

	ZeroMemory(pnFrameData, sizeof(pnFrameData));

	Sleep(70);

	unsigned long status = DIO_StreamOpen( deviceIndex, TRUE );	// TRUE for read; FALSE for write.
	status = DIO_StreamFrame( deviceIndex, FramePoints, &(pnFrameData[1]), BytesTransferred );
	status = DIO_StreamClose( deviceIndex );
	status = AIOUSB_ClearFIFO(deviceIndex, 0);//TIME_METHOD_WAIT_INPUT_ENABLE);
	pnFrameData[0] = (*BytesTransferred) /2UL;

	nLoopbackWords_Times++;
	if( nLoopbackWords_Times > 50 || pnFrameData[0] < 1 )
	{
		WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
		nLoopbackWords_Times = 0;
	}

	return dwRet;
}

DWORD CAdapterUSB::USB_LoopbackWords2( unsigned short nModID, unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred, short nNeedLog )
{
	DWORD dwRet = 0;
	ZeroMemory(pnFrameData, sizeof(pnFrameData));

	// set pix_clock to 2
	WriteToBoard( USB_SetPixelRate, TRUE, 2 );
	// 清空FIFO
	USB_ClearFIFO();
	CommToBoards(	USB_CID_VAC_ReadThermoCCGComp,
					USB_TM_VAC_Read_16 );
	CommToBoards(	0x0,
					0x0 );
	Sleep(45);

	unsigned long status = DIO_StreamOpen( deviceIndex, TRUE );	// TRUE for read; FALSE for write.
	unsigned long status2 = DIO_StreamFrame( deviceIndex, FramePoints, &(pnFrameData[1]), BytesTransferred );
	unsigned long status3 = DIO_StreamClose( deviceIndex );
	unsigned long status4 = AIOUSB_ClearFIFO(deviceIndex, 0);//TIME_METHOD_WAIT_INPUT_ENABLE);
	pnFrameData[0] = (*BytesTransferred) /2UL;

	nLoopbackWords_Times++;
	if( nLoopbackWords_Times > 50 || pnFrameData[0] < 1 )
	{
//		TRACE( "Status = %ld, %ld, %ld, %ld.\r\n", status, status2, status3, status4 );
		WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
		nLoopbackWords_Times = 0;
	}

	return dwRet;
}

BOOL CAdapterUSB::USB_BoardRead2432( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred )
{
	USB_CommToBoards2( bOp1, bOp2, bOp3, bOp4 );

	unsigned long status = DIO_StreamOpen( deviceIndex, TRUE );	// TRUE for read; FALSE for write.
	unsigned long status2 = DIO_StreamFrame( deviceIndex, FramePoints, &(pnFrameData[1]), BytesTransferred );
	unsigned long status3 = DIO_StreamClose( deviceIndex );
	unsigned long status4 = AIOUSB_ClearFIFO(deviceIndex, 0);//TIME_METHOD_WAIT_INPUT_ENABLE);
	pnFrameData[0] = (*BytesTransferred) /2UL;

	if( status > 0 || status2 > 0 || status3 > 0 || status4 > 0 || pnFrameData[0] < 1 )
		return FALSE;
	return TRUE;
}

DWORD CAdapterUSB::USB_ReadMonitorData( int nModID, int nADCtype, int nADCnum, int nADCchan, short nNeedLog )
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
		nTTID			= 16;					// Transfer type = 16 bit read, argument "Rsize" passed to BoardRead0816 with value of "16"
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
//		str.Format( "%ld %ld", m_nBoardReadDataeBuf[4*2+1], m_nBoardReadDataeBuf[5*2+1] );
//		GetDlgItem( IDC_EDIT_ADCreadValInTB )->SetWindowText( str );
	}
	else if( nADCtype == 2 )
	{
		dwRet = BoardRead0816( nTTID, nModID, (BYTE)nCID, (WORD)dwWdata );
//		CString str;
		// Only 16 bits max of readback data
//		str.Format( "%ld", m_nBoardReadDataeBuf[4*2+1] );
//		GetDlgItem( IDC_EDIT_ADCreadValInTB )->SetWindowText( str );
	}
//	else
//		TRACE( "USER ERROR: NO ADC and channel has been selected" );

	if( dwRet == 0 )
		ADCvconv( nADCtype );
	else if( dwRet > 0 )
	{
		WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
		nLoopbackWords_Times = 0;
	}

	return dwRet;
}
