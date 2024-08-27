// UsbDI16A.cpp: implementation of the CUsbDI16A class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UsbDI16A.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUsbDI16A& CUsbDI16A::Instance()
{
	static CUsbDI16A Inst;
	return Inst;
}

CUsbDI16A::CUsbDI16A()
{
	m_bDI16A_Ready	= FALSE;
	m_bFT232_Ready	= FALSE;
	m_nCommMode		= 1;
	m_nVACBoardType	= 1;
}

CUsbDI16A::~CUsbDI16A()
{
//	USB_CloseCard();
}

void CUsbDI16A::SetCommMode( int nMode )
{
	m_nCommMode = nMode;
}

void CUsbDI16A::SetVACBoardType( int nType )
{
	m_nVACBoardType = nType;
}

int CUsbDI16A::USB_InitCard( void )
{
	if( m_nCommMode < 1 )
	{
		m_bDI16A_Ready	= ::USB_InitCard();
		return m_bDI16A_Ready;
	}
	else
	{
		int nRetFT232 = ::Serial_InitCard( m_nVACBoardType );
		if( nRetFT232 > -1 )
			m_bFT232_Ready = TRUE;
		int nRetDI16A = ::USB_InitCard();
		if( nRetDI16A == 0 )
			m_bDI16A_Ready = TRUE;
// 		if( !m_bDI16A_Ready || !m_bFT232_Ready )
// 			return -1;
// 		else
// 			return nRetFT232;
		if( m_bDI16A_Ready && m_bFT232_Ready )
		{
			if( nRetFT232 > 0 )
				return nRetFT232;
			return 0;
		}
		else
			return (nRetDI16A | nRetFT232);
	}
}

BOOL CUsbDI16A::USB_CloseCard( void )
{
	if( m_nCommMode < 1 )
	{
		m_bDI16A_Ready	= FALSE;
		return ::USB_CloseCard();
	}
	else
	{
		m_bFT232_Ready	= FALSE;
		m_bDI16A_Ready	= FALSE;
		BOOL bRet1	= ::USB_CloseCard();
		BOOL bRet2	= ::Serial_CloseCard();
		return (bRet1 && bRet2);
	}
}

BOOL CUsbDI16A::USB_isReady( void )
{
	if( m_nCommMode < 1 )
		return m_bDI16A_Ready;
	else
		return (m_bDI16A_Ready && m_bFT232_Ready);
}

int CUsbDI16A::USB_InitCommPort( void )
{
	int nRetFT232 = ::Serial_InitCard( m_nVACBoardType );
	if( nRetFT232 > -1 )
		m_bFT232_Ready	= TRUE;
	else
		m_bFT232_Ready	= FALSE;
	if( m_bFT232_Ready )
	{
		if( nRetFT232 > 0 )
			return nRetFT232;
		return 0;
	}
	else
		return nRetFT232;
}

BOOL CUsbDI16A::USB_CloseCommPort( void )
{
	m_bFT232_Ready	= FALSE;
	return (::Serial_CloseCard());
}

BOOL CUsbDI16A::USB_isCommPortReady( void )
{
	return m_bFT232_Ready;
}

int CUsbDI16A::USB_InitImagePort( void )
{
	int nRetDI16A = ::USB_InitCard();
	if( nRetDI16A == 0 )
		m_bDI16A_Ready	= TRUE;
	else
		m_bDI16A_Ready	= FALSE;
	return nRetDI16A;
}

BOOL CUsbDI16A::USB_CloseImagePort( void )
{
	m_bDI16A_Ready	= FALSE;
	return (::USB_CloseCard());
}

BOOL CUsbDI16A::USB_isImagePortReady( void )
{
	return m_bDI16A_Ready;
}

BOOL CUsbDI16A::USB_IoPortRead1( unsigned long BitIndex, unsigned char *Buffer )
{
	if( !m_bDI16A_Ready )
		return FALSE;
	return ::USB_IoPortRead1( BitIndex, Buffer );
}

BOOL CUsbDI16A::USB_IoPortWrite1( unsigned long BitIndex, unsigned char bData )
{
	if( !m_bDI16A_Ready )
		return FALSE;
	return ::USB_IoPortWrite1( BitIndex, bData );
}

BOOL CUsbDI16A::USB_IoPortRead8( unsigned long ByteIndex, unsigned char *pBuffer )
{
	if( !m_bDI16A_Ready )
		return FALSE;
	return ::USB_IoPortRead8( ByteIndex, pBuffer );
}

BOOL CUsbDI16A::USB_IoPortWrite8( unsigned long ByteIndex, unsigned char Data )
{
	if( !m_bDI16A_Ready )
		return FALSE;
	return ::USB_IoPortWrite8( ByteIndex, Data );
}

BOOL CUsbDI16A::USB_IoPortReadAll( void *Buffer )
{
	if( !m_bDI16A_Ready )
		return FALSE;
	return ::USB_IoPortReadAll( Buffer );
}

BOOL CUsbDI16A::USB_IoPortWriteAll( void *pData )
{
	if( !m_bDI16A_Ready )
		return FALSE;
	return ::USB_IoPortWriteAll( pData );
}

BOOL CUsbDI16A::USB_IoPortRead( void *Buffer, DWORD &dwBytesReturned )
{
	if( !m_bFT232_Ready )
		return FALSE;
	return ::Serial_Read( Buffer, dwBytesReturned );
}

BOOL CUsbDI16A::USB_IoPortWrite( void *pData, DWORD &dwBytesWritten )
{
	if( m_nCommMode < 1 )
	{
		if( !m_bDI16A_Ready )
			return FALSE;
		return USB_IoPortWrite( pData, dwBytesWritten );
	}
	return TRUE;
}

DWORD CUsbDI16A::USB_GetDMABuffSize()
{
	if( !m_bDI16A_Ready )
		return FALSE;
	return ::USB_GetDMABuffSize();
}

DWORD CUsbDI16A::USB_ImageInputStream1( unsigned long FramePoints, unsigned short *pFrameData, unsigned long *BytesTransferred, short nNeedLog )
{
	if( !m_bDI16A_Ready )
		return 0;
	return ::USB_ImageInputStream1( FramePoints, pFrameData, BytesTransferred, nNeedLog );
}

DWORD CUsbDI16A::USB_ImageInputStream2( unsigned long FramePoints, void * pBuffer, unsigned long *BytesTransferred )
{
	if( !m_bDI16A_Ready )
		return 0;
	return ::USB_ImageInputStream2( FramePoints, pBuffer, BytesTransferred );
}

DWORD CUsbDI16A::USB_ImageInputStreamTest( unsigned long FramePoints, unsigned short *pFrameData, unsigned long *BytesTransferred )
{
	if( !m_bDI16A_Ready )
		return 0;
	return ::USB_ImageInputStream5( FramePoints, pFrameData, BytesTransferred );
}

BOOL CUsbDI16A::USB_WriteToBoardDirect( BYTE bOpcode, BOOL blOperand, BYTE byteLo, BYTE byteHi )
{
	if( m_nCommMode < 1 )
	{
		if( !m_bDI16A_Ready )
			return FALSE;
		return ::USB_WriteToBoardDirect( bOpcode, blOperand, byteLo, byteHi );
	}
	else
	{
		if( !m_bFT232_Ready )
			return FALSE;
		return ::Serial_WriteToBoardDirect( bOpcode, blOperand, byteLo, byteHi );
	}
}

void CUsbDI16A::USB_CommToBoardsDirect( BYTE byteLen, BYTE* pbData )
{
	if( m_nCommMode < 1 )
	{
		if( m_bDI16A_Ready )
			::USB_CommToBoardsDirect( byteLen, pbData );
	}
	else
	{
		if( m_bFT232_Ready )
			::Serial_CommToBoardsDirect( byteLen, pbData );
	}
}

BOOL CUsbDI16A::USB_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData )
{
	if( m_nCommMode < 1 )
	{
		if( !m_bDI16A_Ready )
			return FALSE;
		return ::USB_WriteToBoard( bOpcode, blOperand, wData );
	}
	else
	{
		if( !m_bFT232_Ready )
			return FALSE;
		return ::Serial_WriteToBoard( bOpcode, blOperand, wData );
	}
}

void CUsbDI16A::USB_CommToBoards( BYTE byteLo, BYTE byteHi )
{
	if( m_bDI16A_Ready )
		::USB_CommToBoards( byteLo, byteHi );
}

void CUsbDI16A::USB_CommToBoards2( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 )
{
	if( m_nCommMode < 1 )
	{
		if( m_bDI16A_Ready )
			::USB_CommToBoards2( bOp1, bOp2, bOp3, bOp4 );
	}
	else
	{
		if( m_bFT232_Ready )
			::Serial_CommToBoards2( bOp1, bOp2, bOp3, bOp4 );
	}
}

void CUsbDI16A::USB_CommToBoards3( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, BYTE bOp5, BYTE bOp6 )
{
	if( m_nCommMode < 1 )
	{
		if( m_bDI16A_Ready )
			::USB_CommToBoards3( bOp1, bOp2, bOp3, bOp4, bOp5, bOp6 );
	}
	else
	{
		if( m_bFT232_Ready )
			::Serial_CommToBoards3( bOp1, bOp2, bOp3, bOp4, bOp5, bOp6 );
	}
}

DWORD CUsbDI16A::USB_LoopbackWords( unsigned long FramePoints, unsigned short* pFrameData, unsigned long *BytesTransferred, short nNeedLog )
{
	if( m_nCommMode < 1 )
	{
		if( !m_bDI16A_Ready )
			return 0;
		return ::USB_LoopbackWords( FramePoints, pFrameData, BytesTransferred, nNeedLog );
	}
	else
	{
		if( !m_bFT232_Ready )
			return FALSE;
		return ::Serial_Read( pFrameData, *BytesTransferred );
	}
}

DWORD CUsbDI16A::USB_LoopbackWords2( unsigned short nModID, unsigned long FramePoints, unsigned short* pFrameData, unsigned long *BytesTransferred, short nNeedLog )
{
	if( m_nCommMode < 1 )
	{
		if( !m_bDI16A_Ready )
			return 0;
		return ::USB_LoopbackWords2( nModID, FramePoints, pFrameData, BytesTransferred, nNeedLog );
	}
	else
	{
		if( !m_bFT232_Ready )
			return FALSE;
		return ::Serial_ReadForVAC( nModID, pFrameData, BytesTransferred );
	}
}

BOOL CUsbDI16A::USB_BoardRead2432( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred )
{
	if( m_nCommMode < 1 )
	{
		if( !m_bDI16A_Ready )
			return 0;
		return ::USB_BoardRead2432( bOp1, bOp2, bOp3, bOp4, FramePoints, pnFrameData, BytesTransferred );
	}
	else
	{
		if( !m_bFT232_Ready )
			return FALSE;
		return ::Serial_BoardRead2432( bOp1, bOp2, bOp3, bOp4, FramePoints, pnFrameData, BytesTransferred );
	}
}

DWORD CUsbDI16A::USB_ReadMonitorData( int nModID, int nADCtype, int nADCnum, int nADCchan, short nNeedLog )
{
	if( !m_bDI16A_Ready )
		return 0;
	return ::USB_ReadMonitorData( nModID, nADCtype, nADCnum, nADCchan, nNeedLog );
}

BOOL CUsbDI16A::USB_PhotoOutputStream( void * pBuffer, IN LONG dwBlockSize )
{
	if( !m_bDI16A_Ready )
		return FALSE;
	return ::USB_PhotoOutputStream( pBuffer, dwBlockSize );
}

DWORD CUsbDI16A::USB_ConfigureEx(void *pOutMask, void *pData, void *pTristateMask)
{
	if( !m_bDI16A_Ready )
		return 0;
	return ::USB_ConfigureEx( pOutMask, pData, pTristateMask );
}

DWORD CUsbDI16A::USB_ClearFIFO()
{
	if( !m_bDI16A_Ready )
		return 0;
	return ::USB_ClearFIFO();
}

DWORD CUsbDI16A::USB_StreamOpen( unsigned long bIsRead )
{
	if( !m_bDI16A_Ready )
		return 0;
	return ::USB_StreamOpen( bIsRead );
}

DWORD CUsbDI16A::USB_StreamClose()
{
	if( !m_bDI16A_Ready )
		return 0;
	return ::USB_StreamClose();
}

BOOL CUsbDI16A::USB_IsVideo()
{
	if( !m_bDI16A_Ready )
		return FALSE;
	return ::USB_IsVideo();
}

DWORD CUsbDI16A::USB_VideoON()
{
	if( m_nCommMode < 1 )
	{
		if( !m_bDI16A_Ready )
			return 0;
		return ::USB_VideoON();
	}
	else
	{
		if( !m_bFT232_Ready )
			return 0;
		::Serial_WriteToBoard( 0x4D, FALSE, 0 );
		return ::USB_VideoON();
	}
}

DWORD CUsbDI16A::USB_VideoOFF()
{
	if( m_nCommMode < 1 )
	{
		if( !m_bDI16A_Ready )
			return 0;
		return ::USB_VideoOFF();
	}
	else
	{
		if( !m_bFT232_Ready )
			return 0;
		::Serial_WriteToBoard( 0x4E, FALSE, 0 );
		return ::USB_VideoOFF();
	}
}

BOOL CUsbDI16A::USB_GetPortInfo( BYTE* pPort )
{
//	if( m_nCommMode < 1 )
	{
		if( !m_bDI16A_Ready )
			return FALSE;
//		::USB_IoPortRead8(3, &(pPort[0]) );
//		::USB_IoPortRead8(1, &(pPort[1]) );
		unsigned char input[4];
		::USB_IoPortReadAll(&input);
		pPort[0] = input[3];
		pPort[1] = input[1];
		return TRUE;
	}
// 	else
// 	{
// 		return ::Serial_ReadPortInfo( pPort );
// 	}
}

BOOL CUsbDI16A::USB_GetMCIPwr( BOOL bAll, int nChn, unsigned short* pPwr )
{
	if( !m_bFT232_Ready )
		return FALSE;
	return ::Serial_ReadMCIPwr( bAll, nChn, pPwr );
}

BOOL CUsbDI16A::USB_GetISR( int nMID, unsigned short* pISR )
{
	if( !m_bFT232_Ready )
		return FALSE;
	return ::Serial_ReadISR( nMID, pISR );
}

BOOL CUsbDI16A::USB_GetBoardID( int nMID, unsigned short* pBoardID )
{
	if( !m_bFT232_Ready )
		return FALSE;
	return ::Serial_ReadBoardID( nMID, pBoardID );
}

BOOL CUsbDI16A::USB_GetGaugeCompr( unsigned short* pGauge )
{
	if( !m_bFT232_Ready )
		return FALSE;
	return ::Serial_ReadGaugeCompr( pGauge );
}

BOOL CUsbDI16A::USB_GetSwitchStatus( unsigned short* pSwitch )
{
	if( !m_bFT232_Ready )
		return FALSE;
	return ::Serial_ReadExternalSwitch( pSwitch );
}

BOOL CUsbDI16A::USB_GetLoopbackTest( unsigned short* pData )
{
	if( !m_bFT232_Ready )
		return FALSE;
	return ::Serial_ReadLoopbackTest( pData );
}