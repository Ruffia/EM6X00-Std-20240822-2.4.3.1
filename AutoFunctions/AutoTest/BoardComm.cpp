// DiagLogging.cpp : implementation file
//
// Last Modify : 2013.03.29

#include "stdafx.h"
#include "AutoTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAutoTestApp
/*
long	BoardRead0816( int nRsize, int nModID, BYTE bCktAddr, WORD wWdata, CString& strMsgCounts, CString& strMsgData, BOOL bAnalyze );
long	BoardRead2432( int nRsize, int nModID, BYTE bCktAddr, DWORD dwWdata, CString& strMsgCounts, CString& strMsgData, BOOL bAnalyze );
void	BoardWrite0816( int nWsize, int nModID, BYTE bCktAddr, WORD wWdata );
void	BoardWrite24( int nWsize, int nModID, BYTE bCktAddr, DWORD dwWdata );
*/

void CAutoTestApp::BoardWrite0816( int nWsize, int nModID, BYTE bCktAddr, WORD wWdata )
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

void CAutoTestApp::BoardWrite24( int nWsize, int nModID, BYTE bCktAddr, DWORD dwWdata )
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

long CAutoTestApp::BoardRead0816( int nRsize, int nModID, BYTE bCktAddr, WORD wWdata, CString& strMsgCounts, CString& strMsgData, BOOL bAnalyze )
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

long CAutoTestApp::BoardRead2432( int nRsize, int nModID, BYTE bCktAddr, DWORD dwWdata, CString& strMsgCounts, CString& strMsgData, BOOL bAnalyze )
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

	return lRet;
}
