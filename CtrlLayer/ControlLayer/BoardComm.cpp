// DiagLogging.cpp : implementation file
//
// Last Modify : 2013.03.29

#include "stdafx.h"
#include "BoardComm.h"
#include "Delayer.h"
#include "Calc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXXX2App
/*
long	BoardRead0816( int nRsize, int nModID, BYTE bCktAddr, WORD wWdata, CString& strMsgCounts, CString& strMsgData, BOOL bAnalyze );
long	BoardRead2432( int nRsize, int nModID, BYTE bCktAddr, DWORD dwWdata, CString& strMsgCounts, CString& strMsgData, BOOL bAnalyze );
void	BoardWrite0816( int nWsize, int nModID, BYTE bCktAddr, WORD wWdata );
void	BoardWrite24( int nWsize, int nModID, BYTE bCktAddr, DWORD dwWdata );
*/

CBoardComm& CBoardComm::Instance()
{
	static CBoardComm Inst;
	return Inst;
}

CBoardComm::CBoardComm()
{
	m_pnReadComprData = new unsigned short[ ReadBack_DataSize +256 ];
	ZeroMemory( m_pnReadComprData, sizeof(m_pnReadComprData) );

	m_pnReadCommonData = new unsigned short[ ReadBack_DataSize +256 ];
	ZeroMemory( m_pnReadCommonData, sizeof(m_pnReadCommonData) );

	m_pnReadADCData = new unsigned short[ ReadBack_DataSize +256 ];
	ZeroMemory( m_pnReadADCData, sizeof(m_pnReadADCData) );

	m_bBoardsReady = TRUE;

}

void CBoardComm::Release()
{
	delete [] m_pnReadComprData;
	delete [] m_pnReadCommonData;
	delete [] m_pnReadADCData;
}


void CBoardComm::BoardWrite0816( int nWsize, int nModID, BYTE bCktAddr, WORD wWdata )
{
	// Write 8 or 16 bits to board
	// This mutha accepts arguments to simplify writing to boards with reduced repetition of code
	BYTE bOP1, bOP2, bOP3, bOP4;
	// For all writes, OP1 = circuit address (CktAddr), OP2 upper nibble = transfer type, OP2 lower nibble=Module ID
	// For 8 bit writes, OP3 is always set to 0, OP4 contains data to write to board
	// For 16 bit writes, OP3 contains low byte, OP4 contains high byte
	bOP1	= bCktAddr;
	if( nWsize == 8 )
	{
		bOP2	= USB_TTTT_Write_08 << 4;		// Upper nibble of OP2 = Transfer Type (1 for 8 bit write) 0x10
		bOP3	= 0x0;
		bOP4	= (BYTE)wWdata;
	}
	else
	{
		bOP2	= USB_TTTT_Write_16 << 4;		// Upper nibble of OP2 = Transfer Type (2 for 16 bit write) 0x20
		bOP3	= (BYTE)(wWdata & 0xFF);
		bOP4	= (BYTE)((wWdata /256) & 0xFF);	// Shift data down 8 bits before assigning to OP4
	}
	bOP2		|= (BYTE)nModID;				// OR the target Module ID into lower nibble of OP2
	CSemCtrl::Instance().USB_CommToBoards2(	bOP1,	// Operand 1 = ckt address
					bOP2,						// Operand 2 = Transfer type and Module ID
					bOP3,						// Operand 3: Always set to 0!
					bOP4 );						// Operand 4: ONLY valid data byte for 8 bit write
}

void CBoardComm::BoardWrite24( int nWsize, int nModID, BYTE bCktAddr, DWORD dwWdata )
{
	// Write 8, 16, or 24 bits to a board (SCN,LNS,VAC,SPR,etc.)
	// This mutha accepts arguments to simplify writing to boards with reduced repetition of code
	BYTE bOP1, bOP2, bOP3, bOP4, bOP5, bOP6;			// OP5 and OP6 only used for 24 bit writes. OP1-4 used for all writes.
	BOOL bCluster3;
	// For all writes, OP1 = circuit address (CktAddr), OP2 upper nibble = transfer type, OP2 lower nibble=Module ID
	// For 8 bit writes, OP3 is always set to 0, OP4 contains data to write to board
	// For 16 bit writes, OP3 contains low byte, OP4 contains high byte
	// For 24 bit writes, OP3=Byte #2 of 3, OP4=Byte#1 of 3, OP5=0, OP6=Byte #3 of 3
	switch( nWsize )	// Replicated so 1=8, 2=16, 3=24 in case byte count is passed instead of bit count
	{
	case	1:
		bOP2		= USB_TTTT_Write_08 << 4;			// Upper nibble of OP2 = Transfer Type (1 for 8 bit write) 0x10
		bOP3		= 0x0;
		bOP4		= (BYTE)dwWdata;
		bCluster3	= 0;
		break;
	case	2:
		bOP2		= USB_TTTT_Write_16 << 4;			// Upper nibble of OP2 = Transfer Type (2 for 16 bit write) 0x20
		bOP3		= (BYTE)(dwWdata & 0xFF);
		bOP4		= (BYTE)((dwWdata /256) & 0xFF);	// Shift data down 8 bits before assigning to OP4
		bCluster3	= 0;
		break;
	case	3:
		bOP2		= USB_TTTT_Write_24 << 1;			// 0x30
		bOP3		= (BYTE)((dwWdata /256) & 0xFF);	// (Mid)Byte #2 of 3; shift Wdata down 8 bits
		bOP4		= (BYTE)((dwWdata /65536) & 0xFF);	// (MS) Byte #1 of 3; shift Wdata down 16 bits
		bOP5		= 0x0;
		bOP6		= (BYTE)(dwWdata & 0xFF);			// (LS)Byte #3 of 3 'Just low 8 bits
		bCluster3	= 1;
		break;
	case	8:
		bOP2		= USB_TTTT_Write_08 << 4;			// Upper nibble of OP2 = Transfer Type (1 for 8 bit write) 0x10
		bOP3		= 0x0;
		bOP4		= (BYTE)dwWdata;
		bCluster3	= 0;
		break;
	case	16:
		bOP2		= USB_TTTT_Write_16 << 4;			// Upper nibble of OP2 = Transfer Type (2 for 16 bit write) 0x20
		bOP3		= (BYTE)(dwWdata & 0xFF);
		bOP4		= (BYTE)((dwWdata /256) & 0xFF);	// Shift data down 8 bits before assigning to OP4
		bCluster3	= 0;
		break;
	case	24:
		bOP2		= USB_TTTT_Write_24 << 4;			// 0x30
		bOP3		= (BYTE)(((dwWdata & 0xFF00)/256) & 0xFF);		// (Mid)Byte #2 of 3; shift Wdata down 8 bits
		bOP4		= (BYTE)(((dwWdata & 0xFF0000)/65536) & 0xFF);	// (MS) Byte #1 of 3; shift Wdata down 16 bits
		bOP5		= 0x0;
		bOP6		= (BYTE)(dwWdata & 0xFF);			// (LS)Byte #3 of 3 'Just low 8 bits
		bCluster3	= 1;
		break;
	default:
		return;
		break;
	}
	bOP1	= bCktAddr;			// Regardless of transaction type
	bOP2	|= (BYTE)nModID;	// OR the target Module ID into lower nibble of OP2

	// Done setting OP's, now send data to board:
	if( bCluster3 )
	{
		// Do NOT send this cluster for 8 or 16 bit writes!
		CSemCtrl::Instance().USB_CommToBoards3(	bOP1,		// Operand 1 = ckt address
											bOP2,		// Operand 2 = Transfer type and Module ID
											bOP3,		// Operand 3
											bOP4,		// Operand 4
											bOP5,	// Operand 5: Always set to 0 for 24 bit writes!
											bOP6 );	// Operand 6: LS byte for 24 bit write
	}
	else
	{
		CSemCtrl::Instance().USB_CommToBoards2(	bOP1,		// Operand 1 = ckt address
											bOP2,		// Operand 2 = Transfer type and Module ID
											bOP3,		// Operand 3
											bOP4 );		// Operand 4
	}
}

long CBoardComm::BoardRead0816( int nRsize, int nModID, BYTE bCktAddr, WORD wWdata, CString& strMsgCounts, CString& strMsgData, BOOL bAnalyze)
{
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

	unsigned long lBytesTransferred = 0;
	if( !CSemCtrl::Instance().USB_BoardRead2432(bOP1, bOP2, bOP3, bOP4, ReadBack_DataSize, m_pnReadADCData, &lBytesTransferred) )
		return 1;
	///////////////////////////////////////////////////////

	CString str, strList;
	long lRet = -1;
	strMsgCounts.Format( "Data Received: %ld words", m_pnReadADCData[0] );
	if( lBytesTransferred > 0 )
		lRet = 0;
	else
		lRet = 1;
	if( !bAnalyze )
		return lRet;

	strMsgData = "";
	m_nRBwdCnt = 0;
	BOOL bFound = FALSE;
	ZeroMemory(&m_nBoardReadDataBuf, sizeof(m_nBoardReadDataBuf));

	for( unsigned long i=1; i<=m_pnReadADCData[0]; i++ )
	{
		str.Format( "%04X, ", m_pnReadADCData[i] );
		strMsgData += str;
		strList.Format( "@ %ld (hex %04X)", m_pnReadADCData[i], m_pnReadADCData[i] );
//		((CInsample0Dlg*)m_pMainWnd)->UpdateListReadbackString( strList );

		// 直到找到0xD22D才开始计数，前边的数据忽略
		if( m_pnReadADCData[i] == 0xD22D )
			bFound = TRUE;
		if( !bFound )
			continue;

		m_nRBwdCnt++;
		m_nBoardReadDataBuf[ m_nRBwdCnt*2 ] = m_pnReadADCData[i];	// Numeric value (0-FFFF range)
		switch( m_pnReadADCData[i] )
		{
		case	0xFFFF:
			m_nBoardReadDataBuf[ m_nRBwdCnt*2 +1 ] = 9;
			break;
		case	0xD22D:
			m_nBoardReadDataBuf[ m_nRBwdCnt*2 +1 ] = 4;
			break;
		default:
			m_nBoardReadDataBuf[ m_nRBwdCnt*2 +1 ] = 1;
			break;
		}
	}
	m_nBoardReadDataBuf[0] = m_nRBwdCnt;	// # of valid readback words contained in the array

	return lRet;
}

long CBoardComm::BoardRead2432( int nRsize, int nModID, BYTE bCktAddr, DWORD dwWdata, CString& strMsgCounts, CString& strMsgData, BOOL bAnalyze )
{
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

	unsigned long lBytesTransferred = 0;
	if( !CSemCtrl::Instance().USB_BoardRead2432(bOP1, bOP2, bOP3, bOP4, ReadBack_DataSize, m_pnReadADCData, &lBytesTransferred) )
		return 1;
	///////////////////////////////////////////////////////

	CString str, strList;
	long lRet = -1;
	strMsgCounts.Format( "Data Received: %ld words", m_pnReadADCData[0] );
	if( lBytesTransferred > 0 )
		lRet = 0;
	else
		lRet = 1;
	if( !bAnalyze )
		return lRet;

	strMsgData = "";
	m_nRBwdCnt = 0;
	BOOL bFound = FALSE;
	ZeroMemory(&m_nBoardReadDataBuf, sizeof(m_nBoardReadDataBuf));

	for( unsigned long i=1; i<=m_pnReadADCData[0]; i++ )
	{
		str.Format( "%04X, ", m_pnReadADCData[i] );
		strMsgData += str;
		strList.Format( "@ %ld (hex %04X)", m_pnReadADCData[i], m_pnReadADCData[i] );
//		((CInsample0Dlg*)m_pMainWnd)->UpdateListReadbackString( strList );

		// 直到找到0xD22D才开始计数，前边的数据忽略
		if( m_pnReadADCData[i] == 0xD22D )
			bFound = TRUE;
		if( !bFound )
			continue;

		m_nRBwdCnt++;
		m_nBoardReadDataBuf[ m_nRBwdCnt*2 ] = m_pnReadADCData[i];	// Numeric value (0-FFFF range)
		switch( m_pnReadADCData[i] )
		{
		case	0xFFFF:
			m_nBoardReadDataBuf[ m_nRBwdCnt*2 +1 ] = 9;
			break;
		case	0xD22D:
			m_nBoardReadDataBuf[ m_nRBwdCnt*2 +1 ] = 4;
			break;
		default:
			m_nBoardReadDataBuf[ m_nRBwdCnt*2 +1 ] = 1;
			break;
		}
	}
	m_nBoardReadDataBuf[0] = m_nRBwdCnt;	// # of valid readback words contained in the array

	return  lRet;
}


void  CBoardComm::ResetADC()
{
	m_nADCnum = m_nADCchn = 0;
	m_dADCcurrentMeas = m_dADCvoltsPerAmp = m_dADCampsPerVolt = m_dADCvref = m_dADCCOMoffset = 0;
	m_bADCtype = m_bADCdiff = FALSE;

	m_nADCmid	= 0x0;		// Module Selection ID
	m_nADCcid	= 0x0;		// Circuit Selection ID
}



void CBoardComm::ChangedADCselectOB( int nNum )
{
	// SCN(1x16),LNS(1x8),VAC(2x16),SPR(2x16) (all LTC2448 type ADC's except for LNS board)
	m_nADCnum = nNum;

	// show assignments for each of the channels (16 for all except for LNS board, which as 8)
	CString str;
	switch( m_nADCnum )
	{
	case	0:									// SCN board LTC2448 (one per board)
		m_nADCmid		= USB_MID_SCN;			// SCN board
		m_nADCcid		= USB_CID_SCN_ADC_Read;	// Circuit ID 0xC8
		m_bADCtype		= TRUE;					// Arbitrary assignment for this software to properly format ADC write data for channel select, etc.
		m_dADCvref		= 4;
		m_dADCCOMoffset	= 0;
		str = "% of Vref/2";
		break;
	case	1:									// LNS board LTC1859 (one per board)
		m_nADCmid		= USB_MID_LNS;			// LNS board
		m_nADCcid		= USB_CID_LNS_ADC_Read;	// Circuit ID 0xC8
		m_bADCtype		= FALSE;				// Type = LTC1859
		m_dADCvref		= 10;//2.5;
		m_dADCCOMoffset	= 0;
		str = "% of Vref*2";
		break;
	case	2:									// VAC board LTC2448 #0
		m_nADCmid		= USB_MID_VAC;			// VAC board
		m_nADCcid		= USB_CID_VAC_ADC0_Read;// Circuit ID 0xC6
		m_bADCtype		= TRUE;					// Type = LTC2448
		m_dADCvref		= 4;
		m_dADCCOMoffset	= 1.607;
		str = "% of Vref/2";
		break;
	case	3:									// VAC board LTC2448 #1
		m_nADCmid		= USB_MID_VAC;			// VAC board
		m_nADCcid		= USB_CID_VAC_ADC1_Read;// Circuit ID 0xCE
		m_bADCtype		= TRUE;					// Type = LTC2448
		m_dADCvref		= 4;
		m_dADCCOMoffset	= 1.607;
		str = "% of Vref/2";
		break;
	case	4:									// SPR board LTC2448 #0
		m_nADCmid		= USB_MID_SPR;			// SPR board
		m_nADCcid		= USB_CID_SPR_ADC0_Read;// Circuit ID 0xC6
		m_bADCtype		= TRUE;					// Type = LTC2448
		m_dADCvref		= 4.5;
		m_dADCCOMoffset	= 0;
		str = "% of Vref/2";
		break;
	case	5:									// SPR board LTC2448 #1
		m_nADCmid		= USB_MID_SPR;			// SPR board
		m_nADCcid		= USB_CID_SPR_ADC1_Read;// Circuit ID 0xCE
		m_bADCtype		= TRUE;					// Type = LTC2448
		m_dADCvref		= 4.5;
		m_dADCCOMoffset	= 0;
		str = "% of Vref/2";
		break;
	}
//	GetDlgItem( IDC_STATIC_PctVrefLabel )->SetWindowText( str );
//	str.Format( "%.1f V", m_dADCvref );
//	GetDlgItem( IDC_EDIT_ADCvrefTB )->SetWindowText( str );
}

void CBoardComm::ChangedADCsChSelect( int nChan,ADCchnParam	chn[] )
{
	// SCN(1x16),LNS(1x8),VAC(2x16),SPR(2x16) (all LTC2448 type ADC's except for LNS board)
	m_nADCchn = nChan;
	
//	m_dADCdiffSE		= m_dADCchanModes[m_nADCnum*16*2 +m_nADCchn *2 +0];	// If value = 1 then S.E. / If value = 2 then diff mode
//	m_dADCcurrentMeas	= m_dADCchanModes[m_nADCnum*16*2 +m_nADCchn *2 +1];	// if non-zero value, then convert voltage to current measurement
	m_bADCdiff			= chn[m_nADCnum*16 +m_nADCchn].bDiff;	// If FALSE then S.E. / If TRUE then diff mode
	m_dADCcurrentMeas	= chn[m_nADCnum*16 +m_nADCchn].dFactor;// if non-zero value, then convert voltage to current measurement
	
	if( m_dADCcurrentMeas > 0 )		// Don't calculate unless non-zero, otherwise generate divide-by-0 error:
	{
//		m_dADCvoltsPerAmp = m_dADCchanModes[m_nADCnum*16*2 +m_nADCchn*2 +1];
		m_dADCvoltsPerAmp = chn[m_nADCnum*16 +m_nADCchn].dFactor;
		m_dADCampsPerVolt = 1 / m_dADCvoltsPerAmp;
	}
}

CString CBoardComm::ReadSelectedADC_Procedure( DWORD dwMilliseconds, DL_ADCs_Param& adc, DL_Logging_Param& log, BOOL bLogging )
{
	CString strRes = "";
	double dValue = 0.0;
	if( bLogging )
		strRes = ReadSelectedADC_Logging2( dwMilliseconds, dValue,adc,log );
	else
	{
		if( m_bADCtype )				// 2448
		{
			ReadSelectedADC( FALSE );	// 残留值
			ReadSelectedADC( FALSE );	// 刚刚建立，转换还未完成
			CDelayer::Instance().Delay( dwMilliseconds );		// 等待转换完成
			ReadSelectedADC( TRUE );		// 真正的回读值
			CDelayer::Instance().Delay( dwMilliseconds );		// 等待转换完成
			// Two back-to-back 16 bit transfers to receive RB data
			strRes.Format( "%04X %04X", m_nBoardReadDataBuf[2*2], m_nBoardReadDataBuf[3*2] );
		}
		else								// 1859
		{
			ReadSelectedADC( FALSE );	// 残留值
			ReadSelectedADC( FALSE );	// 刚刚建立，转换还未完成
			CDelayer::Instance().Delay( dwMilliseconds );		// 等待转换完成
			ReadSelectedADC( TRUE );		// 真正的回读值
			// Only 16 bits max of readback data
			strRes.Format( "%04X", m_nBoardReadDataBuf[2*2] );
		}
	}

	return strRes;
}

long CBoardComm::ReadSelectedADC( BOOL bAnalyze )
{
	int nRsize, nModID;
	BYTE bCktAddr, bOSR, bChanNib, bEnSE ;
	DWORD dwWdata;
	// Uses globals: g_TTID, g_MID, g_CID, g_ADCtype, g_ADCnum, g_ADCchan
	bOSR	= 0x90;			// 144, which takes ~73mS to complete conversion (not including transfer latencies)
	if( m_bADCdiff )
		bEnSE = 0xA0;		// 0xA0 for diff measurements
	else
		bEnSE = 0xB0;		// 0xB0 for S.E. measurement

	if( m_bADCtype )	// LTC2448 type
	{
		nRsize	= 32;
		switch( m_nADCchn )
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
		if( m_bADCdiff )
			bChanNib &= 7;		// Force bit 3 of ChanNib to 0 if diff measurement

		dwWdata = (bEnSE *256) | (bChanNib *256) | bOSR;
	}	// if LTC2448 type
	else	// LTC1859 type, format write data for channel select, etc.
	{
		nRsize	= 16;
		switch( m_nADCchn )
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
	
	nModID		= m_nADCmid;
	bCktAddr	= (BYTE)m_nADCcid;
	
	long lRet = 0;
	CString strMsgCounts, strMsgData;
	if( m_bADCtype )
		lRet = BoardRead2432( nRsize, nModID, bCktAddr, dwWdata, strMsgCounts, strMsgData, bAnalyze );
	else
		lRet = BoardRead0816( nRsize, nModID, bCktAddr, (WORD)dwWdata, strMsgCounts, strMsgData, bAnalyze );
	
	return lRet;
}

CString CBoardComm::ReadSelectedADC_Logging( DWORD dwMilliseconds, double &dValue,ADCchnParam  chn[] )
{
	// Converts raw read data to voltage (and also to current if applicable)
	CString str, strRes;
	strRes = "";

	long lRet1 = 1;
	long lRet2 = 1;
	long lTimes = 0;
	while( (lRet1 >0 || lRet2 > 0) && (lTimes < 5) )
	{
		lRet1 = ReadSelectedADC( FALSE );
		CDelayer::Instance().Delay( dwMilliseconds );//Sleep( dwMilliseconds );
		lRet2 = ReadSelectedADC( TRUE );
		// 12.11.09 for comm test
		if( lRet1 > 0 || lRet2 > 0 )
		{
			if( lRet1 > 0 && lRet2 == 0 )
				strRes += "1F2S,";
			else if( lRet1 == 0 && lRet2 > 0 )
				strRes += "1S2F,";
			else
				strRes += "12F,";
			lTimes++;
		}
		// 12.11.09 for comm test
	}

	DWORD dwADCreadRaw, dwADCread24, dwADCreadSignBit, dwSIGNmask;
	BOOL bADCvalSignBit;
	double dADCreadVrefFrac, dADCreadVrefPct;
	double dRangeMin = chn[m_nADCnum*16 +m_nADCchn *2].dMin;
	double dRangeMax = chn[m_nADCnum*16 +m_nADCchn *2].dMax;
	double dEquivCurrent = 0.0;

	if( m_bADCtype )
	{
		// 2432 bits, LTC2448
		dwSIGNmask = 0x20000000;		// from page 10 on datasheet of LTC2448, SIG is the bit 29
		dwADCreadRaw = ((DWORD)m_nBoardReadDataBuf[2*2]<<16) | (DWORD)m_nBoardReadDataBuf[3*2];
		if( dwADCreadRaw == 0 )
		{
			strRes.Format( "\t\t\t\t" );
			CDelayer::Instance().Delay( dwMilliseconds +40 );//Sleep( dwMilliseconds +40 );
			return strRes;
		}
		dwADCread24 = (dwADCreadRaw & 0x1FFFFFFF) /32;	// Bit 28 --- Bit0, and shift down 5 bits
		str.Format( "%06X\t", dwADCread24 );				// Convert to 6 character hex string
		strRes += str;

		dwADCreadSignBit = dwADCreadRaw & dwSIGNmask;
		if( dwADCreadSignBit > 0 )
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
			str.Format( "%.4f\t", dADCreadVrefPct );
			strRes += str;
			// g_ADCvref varies per board design
			str.Format( "%.6f\t", dADCreadVrefFrac * m_dADCvref /2 );
			strRes += str;
			dValue = dADCreadVrefFrac * m_dADCvref /2 +m_dADCCOMoffset;
			str.Format( "%.6f\t", dValue );
			strRes += str;
			dEquivCurrent = dValue * m_dADCampsPerVolt;
		}
		else
		{
			str.Format( "-%.4f\t", dADCreadVrefPct );
			strRes += str;
			str.Format( "-%.6f\t", dADCreadVrefFrac * m_dADCvref /2 );
			strRes += str;
			dValue = m_dADCCOMoffset - dADCreadVrefFrac * m_dADCvref /2;
			str.Format( "%.6f\t", dValue );
			strRes += str;
			dEquivCurrent = dValue * m_dADCampsPerVolt;
		}
	}	// If LTC2448 ADC read
	else
	{
		// 1608 bits, LTC1859
		dwADCreadRaw = (DWORD)(m_nBoardReadDataBuf[2*2]);
		if( dwADCreadRaw == 0 )
		{
			strRes.Format( "\t\t\t\t" );
			CDelayer::Instance().Delay( dwMilliseconds +40 );//Sleep( dwMilliseconds +40 );
			return strRes;
		}
		dwADCreadRaw &= 0xFFFF;
		str.Format( "%04X\t", dwADCreadRaw );	// Identical value for 16 bit LTC1859
		strRes += str;

		dwADCreadSignBit = dwADCreadRaw & 0x8000;	// if MSB = 1 then negative value (if -5V to +5V range selected)
		if( dwADCreadSignBit > 5 )
			bADCvalSignBit	= TRUE;
		else
			bADCvalSignBit	= FALSE;
		// If ADCvalSignBit = 1, then reading is a positive voltage in the range of 0V to Vref/2 minus 1LSB
		// If ADCvalSignBit = 0, then reading is a negative voltage in the range of 0V to -Vref/2
		if( bADCvalSignBit )
		{
			dADCreadVrefFrac = (-1) * (1.0 - (1.0 *dwADCreadRaw - 0x7FFF) / 0x7FFF);	// Fractional value of full range of 0 to -Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			str.Format( "%.4f\t", dADCreadVrefPct );
			strRes += str;
			dValue = dADCreadVrefFrac * m_dADCvref / 2;
			str.Format( "%.6f\t", dValue );
			strRes += str;
			strRes += str;
		}
		else
		{
			dADCreadVrefFrac = 1.0 *dwADCreadRaw / 0x7FFF;	// Fractional value of full range of 0 to +Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			str.Format( "%.4f\t", dADCreadVrefPct );
			strRes += str;
			dValue = dADCreadVrefFrac * m_dADCvref / 2;
			str.Format( "%.6f\t", dValue );
			strRes += str;
			strRes += str;
		}
		dEquivCurrent = dValue * m_dADCampsPerVolt;
	}	// If LTC1859 ADC read

//	double dEquivCurrent = dADCreadVrefFrac * m_dADCvref / 2 * m_dADCampsPerVolt;
	// g_ADCvoltsPerAmp, g_ADCampsPerVolt
	if( m_dADCcurrentMeas > 0 )
	{
		str.Format( "%.6f\t", dEquivCurrent );
		strRes += str;
	}
	else
	{
		str.Format( "n/a\t" );
		strRes += str;
	}

//	Delay( dwMilliseconds +40);//Sleep( dwMilliseconds +40 );
	CDelayer::Instance().Delay( dwMilliseconds );//Sleep( dwMilliseconds );
	if( strRes.IsEmpty() )
		strRes.Format( "\t\t\t\t" );
	return strRes;
}

CString CBoardComm::ReadSelectedADC_Logging2( DWORD dwMilliseconds, double &dValue,DL_ADCs_Param& adc, DL_Logging_Param& log )
{
	CString str, strRes = "";
	if( m_bADCtype )				// 2448
	{
		ReadSelectedADC( FALSE );	// 残留值
		ReadSelectedADC( FALSE );	// 刚刚建立，转换还未完成
		CDelayer::Instance().Delay( dwMilliseconds );		// 等待建立及转换
		if( adc.nAveCounts > 0 )
		{
			// 读多次，求平均
			double dTotal = 0.0;
			int nCounts = adc.nAveCounts +1;
			for( int i=0; i<nCounts; i++ )
			{
				ReadSelectedADC( TRUE );
				CDelayer::Instance().Delay( dwMilliseconds );
				str = ADCvconv_Logging( log.dAve[i],adc.chn );
				dTotal += log.dAve[i];
//				strRes += str;
			}
			dValue = dTotal /nCounts;
			strRes = str;
		}
		else
		{
			ReadSelectedADC( TRUE );	// 真正的回读值，只读一次
			CDelayer::Instance().Delay( dwMilliseconds );
			strRes = ADCvconv_Logging( dValue,adc.chn );
		}
	}
	else								// 1859
	{
		ReadSelectedADC( FALSE );	// 残留值
		ReadSelectedADC( FALSE );	// 刚刚建立，转换还未完成
 		CDelayer::Instance().Delay( dwMilliseconds );		// 等待转换完成
		if( adc.nAveCounts > 0 )
		{
			// 读多次，求平均
			double dTotal = 0.0;
			int nCounts = adc.nAveCounts +1;
			for( int i=0; i<nCounts; i++ )
			{
				ReadSelectedADC( TRUE );
				CDelayer::Instance().Delay( dwMilliseconds );
				str = ADCvconv_Logging( log.dAve[i],adc.chn );
				dTotal += log.dAve[i];
//				strRes += str;
			}
			dValue = dTotal /nCounts;
			strRes = str;
		}
		else
		{
			ReadSelectedADC( TRUE );	// 真正的回读值，只读一次
			CDelayer::Instance().Delay( dwMilliseconds );
			strRes = ADCvconv_Logging( dValue,adc.chn );
		}
	}
	return strRes;
}

CString CBoardComm::ADCvconv_Logging( double &dValue, ADCchnParam chn[], BOOL bLog )
{
	// Converts raw read data to voltage (and also to current if applicable)
	CString str, strRes = "";
	DWORD dwADCreadRaw, dwADCread24, dwADCreadSignBit, dwSIGNmask;
	BOOL bADCvalSignBit;
	double dADCreadVrefFrac, dADCreadVrefPct;
	double dRangeMin = chn[m_nADCnum*16 + m_nADCchn *2].dMin;
	double dRangeMax = chn[m_nADCnum*16 + m_nADCchn *2].dMax;
	double dEquivCurrent = 0.0;

	if( m_bADCtype )
	{
		// 2432 bits, LTC2448
		dwSIGNmask = 0x20000000;		// from page 10 on datasheet of LTC2448, SIG is the bit 29
		dwADCreadRaw = ((DWORD)m_nBoardReadDataBuf[2*2]<<16) | (DWORD)m_nBoardReadDataBuf[3*2];
		if( dwADCreadRaw == 0 )
		{
			if( bLog )
				strRes.Format( "\t\t\t\t" );
			return strRes;
		}
		dwADCread24 = (dwADCreadRaw & 0x1FFFFFFF) /32;	// Bit 28 --- Bit0, and shift down 5 bits
		if( bLog )
		{
			str.Format( "%06X\t", dwADCread24 );				// Convert to 6 character hex string
			strRes += str;
		}

		dwADCreadSignBit = dwADCreadRaw & dwSIGNmask;
		if( dwADCreadSignBit > 0 )
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
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
				// g_ADCvref varies per board design
				str.Format( "%.6f\t", dADCreadVrefFrac * m_dADCvref /2 );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * m_dADCvref /2 + m_dADCCOMoffset;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
			}
			dEquivCurrent = dValue * m_dADCampsPerVolt;
		}
		else
		{
			if( bLog )
			{
				str.Format( "-%.4f\t", dADCreadVrefPct );
				strRes += str;
				str.Format( "-%.6f\t", dADCreadVrefFrac * m_dADCvref /2 );
				strRes += str;
			}
			dValue = m_dADCCOMoffset - dADCreadVrefFrac * m_dADCvref /2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
			}
			dEquivCurrent = dValue * m_dADCampsPerVolt;
		}
	}	// If LTC2448 ADC read
	else
	{
		// 1608 bits, LTC1859
		dwADCreadRaw = (DWORD)(m_nBoardReadDataBuf[2*2]);
		if( dwADCreadRaw == 0 )
		{
			if( bLog )
				strRes.Format( "\t\t\t\t" );
			return strRes;
		}
		dwADCreadRaw &= 0xFFFF;
		if( bLog )
		{
			str.Format( "%04X\t", dwADCreadRaw );	// Identical value for 16 bit LTC1859
			strRes += str;
		}

		dwADCreadSignBit = dwADCreadRaw & 0x8000;	// if MSB = 1 then negative value (if -5V to +5V range selected)
		if( dwADCreadSignBit > 5 )
			bADCvalSignBit	= TRUE;
		else
			bADCvalSignBit	= FALSE;
		// If ADCvalSignBit = 1, then reading is a positive voltage in the range of 0V to Vref/2 minus 1LSB
		// If ADCvalSignBit = 0, then reading is a negative voltage in the range of 0V to -Vref/2
		if( bADCvalSignBit )
		{
			dADCreadVrefFrac = (-1) * (1.0 - (1.0 *dwADCreadRaw - 0x7FFF) / 0x7FFF);	// Fractional value of full range of 0 to -Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * m_dADCvref / 2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
				strRes += str;
			}
		}
		else
		{
			dADCreadVrefFrac = 1.0 *dwADCreadRaw / 0x7FFF;	// Fractional value of full range of 0 to +Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * m_dADCvref / 2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
				strRes += str;
			}
		}
		dEquivCurrent = dValue * m_dADCampsPerVolt;
	}	// If LTC1859 ADC read

//	double dEquivCurrent = dADCreadVrefFrac * m_dADCvref / 2 * m_dADCampsPerVolt;
	// g_ADCvoltsPerAmp, g_ADCampsPerVolt
	if( bLog )
	{
		if( m_dADCcurrentMeas > 0 )
		{
			str.Format( "%.6f\t", dEquivCurrent );
			strRes += str;
		}
		else
		{
			str.Format( "n/a\t" );
			strRes += str;
		}

		if( strRes.IsEmpty() )
			strRes.Format( "\t\t\t\t" );
	}
	return strRes;
}

CString CBoardComm::ReadSelectedADC_Logging_onetime( DWORD dwMilliseconds, double &dValue, ADCchnParam chn[], BOOL bDelay )
{
	// Converts raw read data to voltage (and also to current if applicable)
	CString str, strRes;
	strRes = "";

	long lRet1 = 1;
	long lTimes = 0;
	while( (lRet1 >0) && (lTimes < 5) )
	{
		lRet1 = ReadSelectedADC( TRUE );
		// 12.11.09 for comm test
		if( lRet1 > 0 )
		{
			strRes += "Fail,";
			lTimes++;
		}
		// 12.11.09 for comm test
	}
	strRes = ADCvconv_Logging( dValue,chn );
	if( bDelay )
		CDelayer::Instance().Delay( dwMilliseconds );
	return strRes;

	DWORD dwADCreadRaw, dwADCread24, dwADCreadSignBit, dwSIGNmask;
	BOOL bADCvalSignBit;
	double dADCreadVrefFrac, dADCreadVrefPct;
	double dRangeMin = chn[m_nADCnum*16 + m_nADCchn *2].dMin;
	double dRangeMax = chn[m_nADCnum*16 + m_nADCchn *2].dMax;
	double dEquivCurrent = 0.0;

	if( m_bADCtype )
	{
		// 2432 bits, LTC2448
		dwSIGNmask = 0x20000000;		// from page 10 on datasheet of LTC2448, SIG is the bit 29
		dwADCreadRaw = ((DWORD)m_nBoardReadDataBuf[2*2]<<16) | (DWORD)m_nBoardReadDataBuf[3*2];
		if( dwADCreadRaw == 0 )
		{
			strRes.Format( "\t\t\t\t" );
			if( bDelay )
				CDelayer::Instance().Delay( dwMilliseconds );//Sleep( dwMilliseconds +40 );
			return strRes;
		}
		dwADCread24 = (dwADCreadRaw & 0x1FFFFFFF) /32;	// Bit 28 --- Bit0, and shift down 5 bits
		str.Format( "%06X\t", dwADCread24 );				// Convert to 6 character hex string
		strRes += str;

		dwADCreadSignBit = dwADCreadRaw & dwSIGNmask;
		if( dwADCreadSignBit > 0 )
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
			str.Format( "%.4f\t", dADCreadVrefPct );
			strRes += str;
			// g_ADCvref varies per board design
			str.Format( "%.6f\t", dADCreadVrefFrac * m_dADCvref /2 );
			strRes += str;
			dValue = dADCreadVrefFrac * m_dADCvref /2 + m_dADCCOMoffset;
			str.Format( "%.6f\t", dValue );
			strRes += str;
			dEquivCurrent = dValue * m_dADCampsPerVolt;
		}
		else
		{
			str.Format( "-%.4f\t", dADCreadVrefPct );
			strRes += str;
			str.Format( "-%.6f\t", dADCreadVrefFrac * m_dADCvref /2 );
			strRes += str;
			dValue = m_dADCCOMoffset - dADCreadVrefFrac * m_dADCvref /2;
			str.Format( "%.6f\t", dValue );
			strRes += str;
			dEquivCurrent = dValue * m_dADCampsPerVolt;
		}
	}	// If LTC2448 ADC read
	else
	{
		// 1608 bits, LTC1859
		dwADCreadRaw = (DWORD)(m_nBoardReadDataBuf[2*2]);
		if( dwADCreadRaw == 0 )
		{
			strRes.Format( "\t\t\t\t" );
			if( bDelay )
				CDelayer::Instance().Delay( dwMilliseconds );//Sleep( dwMilliseconds +40 );
			return strRes;
		}
		dwADCreadRaw &= 0xFFFF;
		str.Format( "%04X\t", dwADCreadRaw );	// Identical value for 16 bit LTC1859
		strRes += str;

		dwADCreadSignBit = dwADCreadRaw & 0x8000;	// if MSB = 1 then negative value (if -5V to +5V range selected)
		if( dwADCreadSignBit > 5 )
			bADCvalSignBit	= TRUE;
		else
			bADCvalSignBit	= FALSE;
		// If ADCvalSignBit = 1, then reading is a positive voltage in the range of 0V to Vref/2 minus 1LSB
		// If ADCvalSignBit = 0, then reading is a negative voltage in the range of 0V to -Vref/2
		if( bADCvalSignBit )
		{
			dADCreadVrefFrac = (-1) * (1.0 - (1.0 *dwADCreadRaw - 0x7FFF) / 0x7FFF);	// Fractional value of full range of 0 to -Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			str.Format( "%.4f\t", dADCreadVrefPct );
			strRes += str;
			dValue = dADCreadVrefFrac * m_dADCvref / 2;
			str.Format( "%.6f\t", dValue );
			strRes += str;
			strRes += str;
		}
		else
		{
			dADCreadVrefFrac = 1.0 *dwADCreadRaw / 0x7FFF;	// Fractional value of full range of 0 to +Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			str.Format( "%.4f\t", dADCreadVrefPct );
			strRes += str;
			dValue = dADCreadVrefFrac * m_dADCvref / 2;
			str.Format( "%.6f\t", dValue );
			strRes += str;
			strRes += str;
		}
		dEquivCurrent = dValue * m_dADCampsPerVolt;
	}	// If LTC1859 ADC read

//	double dEquivCurrent = dADCreadVrefFrac * m_dADCvref / 2 * m_dADCampsPerVolt;
	// g_ADCvoltsPerAmp, g_ADCampsPerVolt
	if( m_dADCcurrentMeas > 0 )
	{
		str.Format( "%.6f\t", dEquivCurrent );
		strRes += str;
	}
	else
	{
		str.Format( "n/a\t" );
		strRes += str;
	}

	if( bDelay )
	{
		CDelayer::Instance().Delay( dwMilliseconds );//Sleep( dwMilliseconds );
	}
	if( strRes.IsEmpty() )
		strRes.Format( "\t\t\t\t" );
	return strRes;
}

void CBoardComm::OnlySendReadSelectedADCcmd( logParam lP)
{
	if( lP.nType == 2 )
	{
		// read gauge compr
		CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_VAC_ReadThermoCCGComp,	// OP1 = ckt addr. of comparator read register on VAC board
											USB_TM_VAC_Read_16,				// OP2 = 0xA3 (16 bit read from VAC board)
											0x0,							// OP3 = 0 (No data to be written)
											0x0 );							// OP4 = 0 (No data to be written)
		return;
	}
	if( lP.nType == 3 )
	{
		// read Ext. switch
		return;
	}

	BYTE bOSR, bChanNib, bEnSE ;
	DWORD dwWdata;
	// Uses globals: g_TTID, g_MID, g_CID, g_ADCtype, g_ADCnum, g_ADCchan
	bOSR	= 0x90;			// 144, which takes ~73mS to complete conversion (not including transfer latencies)
	if( lP.bDiff )
		bEnSE = 0xA0;		// 0xA0 for diff measurements
	else
		bEnSE = 0xB0;		// 0xB0 for S.E. measurement

	if( lP.bType )	// LTC2448 type
	{
		switch( lP.nChn )
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
		if( lP.bDiff )
			bChanNib &= 7;		// Force bit 3 of ChanNib to 0 if diff measurement

		dwWdata = (bEnSE *256) | (bChanNib *256) | bOSR;
	}	// if LTC2448 type
	else	// LTC1859 type, format write data for channel select, etc.
	{
		switch( lP.nChn )
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

	BYTE bOP2, bOP3, bOP4;
	if( lP.bType )	// 2448, 32 bit read adc
		// Upper nibble of OP2 = Transfer Type (C for 32 bit Read)
		// OR the target Module ID into lower nibble of OP2
		bOP2	= 0xC0 | (BYTE)lP.nMID;
	else			// 1859, 16 bit read adc
		// Upper nibble of OP2 = Transfer Type (A for 16 bit Read)
		// OR the target Module ID into lower nibble of OP2
		bOP2	= 0xA0 | (BYTE)lP.nMID;
	bOP3	= (BYTE)(dwWdata & 0xFF);
	bOP4	= (BYTE)((dwWdata /256) & 0xFF);// Shift data down 8 bits before assigning to OP4
	CSemCtrl::Instance().USB_CommToBoards2(	(BYTE)lP.nCID,		// Operand 1 = ckt address
										bOP2,				// Operand 2 = Transfer type and Module ID
										bOP3,
										bOP4 );
}

CString CBoardComm::ADCvconv_Logging( int nData1, int nData2, logParam lP, double &dValue, ADCchnParam	chn[], BOOL bLog )
{
	// Converts raw read data to voltage (and also to current if applicable)
	CString str, strRes = "";
	DWORD dwADCreadRaw, dwADCread24, dwADCreadSignBit, dwSIGNmask;
	BOOL bADCvalSignBit;
	double dADCreadVrefFrac, dADCreadVrefPct;

	double dRangeMin = chn[lP.nNum*16 +lP.nChn].dMin;
	double dRangeMax = chn[lP.nNum*16 +lP.nChn].dMax;
	double dEquivCurrent = 0.0;

	double dADCvref, dADCCOMoffset;
	switch( lP.nNum )
	{
	case	0:
		dADCvref		= 4;
		dADCCOMoffset	= 0;
		break;
	case	1:
		dADCvref		= 10;
		dADCCOMoffset	= 0;
		break;
	case	2:
		dADCvref		= 4;
		dADCCOMoffset	= 1.607;
		break;
	case	3:
		dADCvref		= 4;
		dADCCOMoffset	= 1.607;
		break;
	case	4:
		dADCvref		= 4.5;
		dADCCOMoffset	= 0;
		break;
	case	5:
		dADCvref		= 4.5;
		dADCCOMoffset	= 0;
		break;
	}
	double dADCcurrentMeas	= chn[lP.nNum*16 +lP.nChn].dFactor;// if non-zero value, then convert voltage to current measurement
	double dADCvoltsPerAmp, dADCampsPerVolt;
	dADCvoltsPerAmp = dADCampsPerVolt = 0;
	if( dADCcurrentMeas > 0 )		// Don't calculate unless non-zero, otherwise generate divide-by-0 error:
	{
		dADCvoltsPerAmp = chn[lP.nNum*16 +lP.nChn].dFactor;
		dADCampsPerVolt = 1 / dADCvoltsPerAmp;
	}

	dValue = 0.0;
	if( lP.bType )
	{
		// 2432 bits, LTC2448
		dwSIGNmask = 0x20000000;		// from page 10 on datasheet of LTC2448, SIG is the bit 29
		dwADCreadRaw = ((DWORD)nData1<<16) | (DWORD)nData2;
		if( dwADCreadRaw == 0 )
		{
			if( bLog )
				strRes.Format( "\t\t\t\t" );
			return strRes;
		}
		dwADCread24 = (dwADCreadRaw & 0x1FFFFFFF) /32;	// Bit 28 --- Bit0, and shift down 5 bits
		if( bLog )
		{
			str.Format( "%06X\t", dwADCread24 );				// Convert to 6 character hex string
			strRes += str;
		}

		dwADCreadSignBit = dwADCreadRaw & dwSIGNmask;
		if( dwADCreadSignBit > 0 )
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
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
				// g_ADCvref varies per board design
				str.Format( "%.6f\t", dADCreadVrefFrac * dADCvref /2 );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * dADCvref /2 +dADCCOMoffset;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
			}
			dEquivCurrent = dValue * dADCampsPerVolt;
		}
		else
		{
			if( bLog )
			{
				str.Format( "-%.4f\t", dADCreadVrefPct );
				strRes += str;
				str.Format( "-%.6f\t", dADCreadVrefFrac * dADCvref /2 );
				strRes += str;
			}
			dValue = dADCCOMoffset - dADCreadVrefFrac * dADCvref /2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
			}
			dEquivCurrent = dValue * dADCampsPerVolt;
		}
	}	// If LTC2448 ADC read
	else
	{
		// 1608 bits, LTC1859
		dwADCreadRaw = (DWORD)nData1;
		if( dwADCreadRaw == 0 )
		{
			if( bLog )
				strRes.Format( "\t\t\t\t" );
			return strRes;
		}
		dwADCreadRaw &= 0xFFFF;
		if( bLog )
		{
			str.Format( "%04X\t", dwADCreadRaw );	// Identical value for 16 bit LTC1859
			strRes += str;
		}

		dwADCreadSignBit = dwADCreadRaw & 0x8000;	// if MSB = 1 then negative value (if -5V to +5V range selected)
		if( dwADCreadSignBit > 5 )
			bADCvalSignBit	= TRUE;
		else
			bADCvalSignBit	= FALSE;
		// If ADCvalSignBit = 1, then reading is a positive voltage in the range of 0V to Vref/2 minus 1LSB
		// If ADCvalSignBit = 0, then reading is a negative voltage in the range of 0V to -Vref/2
		if( bADCvalSignBit )
		{
			dADCreadVrefFrac = (-1) * (1.0 - (1.0 *dwADCreadRaw - 0x7FFF) / 0x7FFF);	// Fractional value of full range of 0 to -Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * dADCvref / 2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
				strRes += str;
			}
		}
		else
		{
			dADCreadVrefFrac = 1.0 *dwADCreadRaw / 0x7FFF;	// Fractional value of full range of 0 to +Vref*2
			dADCreadVrefPct = dADCreadVrefFrac * 100;
			if( bLog )
			{
				str.Format( "%.4f\t", dADCreadVrefPct );
				strRes += str;
			}
			dValue = dADCreadVrefFrac * dADCvref / 2;
			if( bLog )
			{
				str.Format( "%.6f\t", dValue );
				strRes += str;
				strRes += str;
			}
		}
		dEquivCurrent = dValue * dADCampsPerVolt;
	}	// If LTC1859 ADC read

//	double dEquivCurrent = dADCreadVrefFrac * dADCvref / 2 * dADCampsPerVolt;
	// g_ADCvoltsPerAmp, g_ADCampsPerVolt
	if( bLog )
	{
		if( dADCcurrentMeas > 0 )
		{
			str.Format( "%.6f\t", dEquivCurrent );
			strRes += str;
		}
		else
		{
			str.Format( "n/a\t" );
			strRes += str;
		}

		if( strRes.IsEmpty() )
			strRes.Format( "\t\t\t\t" );
	}
	return strRes;
}

CString CBoardComm::ReadComprs_Logging( int nData )
{
	// TODO: Add your control notification handler code here
	CString strRes = "";
/*	
	// 4th readback word (7th and 8th ASCII characters) contains Comparator data
	WORD wData, wRBit, wMskBit;
	wData = (WORD)nData;
	for( wRBit=0; wRBit<14; wRBit += 2 )
	{
		wMskBit = (WORD)(pow(2, wRBit) );	// Odd numbered bits are Upper trip point comparator states
		if( wData & wMskBit )
			GetDlgItem( IDC_EDIT_VACgaugeUpperComps0 + wRBit /2 )->SetWindowText( "1" );
		else
			GetDlgItem( IDC_EDIT_VACgaugeUpperComps0 + wRBit /2 )->SetWindowText( "0" );
		
		wMskBit *= 2;					// Shift up one bit to check Lower trip point comparator state
		if( wData & wMskBit )
			GetDlgItem( IDC_EDIT_VACgaugeLowerComps0 + wRBit /2 )->SetWindowText( "1" );
		else
			GetDlgItem( IDC_EDIT_VACgaugeLowerComps0 + wRBit /2 )->SetWindowText( "0" );
	}
*/	
	strRes.Format( "%04X\t", nData );
	return strRes;
}

CString CBoardComm::ReadComprs_Logging()
{
	unsigned long lBytesTransferred = 0;
	// Allocate on the heap
	//	unsigned short* pnFrameData = new unsigned short[ lReadSize +256 ];
	//	ZeroMemory(pnFrameData, sizeof(pnFrameData));
	//	delete [] pnFrameData;

	BOOL bFound = FALSE;
	CString strRes = "";
	if( CSemCtrl::Instance().USB_LoopbackAndReadData( 1, 256, m_pnReadComprData, &lBytesTransferred, -1 ) )
	{
//		UpdateReadbackData( m_pnReadComprData );
		CString str, strMsg, strList;
		unsigned long lCount = m_pnReadComprData[0];
		for( unsigned long i=1; i<=lCount; i++ )
		{
			// 直到找到0xD22D才开始计数，前边的数据忽略
			if( m_pnReadComprData[i] == 0xD22D )
				bFound = TRUE;
			if( !bFound )
				continue;
			
			m_nRBwdCnt++;
			m_nBoardReadDataBuf[ m_nRBwdCnt*2 ] = m_pnReadComprData[i];	// Numeric value (0-FFFF range)
			if( m_nRBwdCnt % 2 == 0 )	// 0xD22D后面的数就是需要的状态数据
			{
				// 4th readback word (7th and 8th ASCII characters) contains Comparator data
				strRes.Format( "%04X\t", m_pnReadComprData[i] );
				return strRes;
			}
		}
	}
	return strRes;
}

// ADC回读后使用ADCvconv将模拟量转换为数字量
// 现在要由ADC回读值数字量得到实际物理量（电压、电流）
double CBoardComm::GetPhysicsFromAdcs( int nIndex, double dVadc, CString strCalc[] )
{
	// 由曲线图索引得到ADC通道号索引
//	int nIndex = m_DL.graph[nIndexGraph].nIndex;
	// 取出该通道的公式字符串
	CString strFunc = strCalc[nIndex];
	if( strFunc == "#" )
		return dVadc;
	
	// 进行公式改写
	CString strVfront, strVback, strNew;	// V前字符串，V后字符串，加入数值的公式字符串
	int nPos = strFunc.Find( "V" );
	if( nPos > 0 )	// V不是公式中的第一个字符
	{
		strVfront	= strFunc.Left( nPos );
		strVback	= strFunc.Right( strFunc.GetLength() - nPos -1 );
		strNew.Format( "%s%.6f%s", strVfront, dVadc, strVback );
	}
	else
	{
		strVback	= strFunc.Right( strFunc.GetLength() -1 );	// 取除去V的所有字符串
		strNew.Format( "%.6f%s", dVadc, strVback );
	}
	
	// 调用公式分析计算
	char cFunc[100];
	sprintf( cFunc, "%s", strNew );
	return counter( cFunc );
}


double  CBoardComm::GetChnRangeMin(ADCchnParam chn[])
{
	double dRangeMin = chn[m_nADCnum*16 + m_nADCchn *2].dMin;
	return dRangeMin;
}


double  CBoardComm::GetChnRangeMax(ADCchnParam chn[])
{
	double dRangeMax = chn[m_nADCnum*16 + m_nADCchn *2].dMax;
	return dRangeMax;
}

DWORD  CBoardComm::GetADCReadRaw()
{
	DWORD dwReadRaw = ((DWORD)m_nBoardReadDataBuf[2*2]<<16) | (DWORD)m_nBoardReadDataBuf[3*2];
	return dwReadRaw;
}