// COMMAPI_SERIAL.h : header file for the COMMAPI.cpp
//

#if !defined(AFX_MCICARD_COMMAPI_SERIAL_)
#define AFX_MCICARD_COMMAPI_SERIAL_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COMMAPI_SERIAL
// See COMMAPI_SERIAL.cpp for the implementation of these functions.
//

BOOL	WINAPI	Serial_InitCard( int nVACBoardType );
BOOL	WINAPI	Serial_CloseCard( void );

BOOL	WINAPI	Serial_Write( void *pData, DWORD dwBufferSize, DWORD &dwBytesWritten );
BOOL	WINAPI	Serial_Read(void *Buffer, DWORD &dwBytesReturned );
/*
BOOL	WINAPI	Serial_IoPortRead1( unsigned long BitIndex, unsigned char *Buffer );
BOOL	WINAPI	Serial_IoPortWrite1( unsigned long BitIndex, unsigned char bData );
BOOL	WINAPI	Serial_IoPortRead8( unsigned long ByteIndex, unsigned char *pBuffer );
BOOL	WINAPI	Serial_IoPortWrite8( unsigned long ByteIndex, unsigned char Data );
BOOL	WINAPI	Serial_IoPortReadAll( void *Buffer );
BOOL	WINAPI	Serial_IoPortWriteAll( void *pData );
*/
BOOL	WINAPI	Serial_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData );
void	WINAPI	Serial_CommToBoards2( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 );
void	WINAPI	Serial_CommToBoards3( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, BYTE bOp5, BYTE bOp6 );

void	WINAPI	Serial_CommToBoardsDirect( BYTE byteLen, BYTE* pbData );
BOOL	WINAPI	Serial_WriteToBoardDirect( BYTE bOpcode, BOOL blOperand, BYTE byteLo, BYTE byteHi );

BOOL	WINAPI	Serial_ReadPortInfo( BYTE* pPort );
BOOL	WINAPI	Serial_ReadMCIPwr( BOOL bAll, int nChn, unsigned short* pPwr );
BOOL	WINAPI	Serial_ReadISR( int nMID, unsigned short* pISR );
BOOL	WINAPI	Serial_ReadBoardID( int nMID, unsigned short* pBoardID );
BOOL	WINAPI	Serial_ReadGaugeCompr( unsigned short* pGauge );
BOOL	WINAPI	Serial_ReadExternalSwitch( unsigned short* pSwitch );
BOOL	WINAPI	Serial_ReadLoopbackTest( unsigned short* pData );
/*
DWORD	WINAPI	Serial_LoopbackWords( unsigned long FramePoints, unsigned short* pFrameData, unsigned long *BytesTransferred, short nNeedLog );
DWORD	WINAPI	Serial_LoopbackWords2( unsigned short nModID, unsigned long FramePoints, unsigned short* pFrameData, unsigned long *BytesTransferred, short nNeedLog );
DWORD	WINAPI	Serial_ReadMonitorData( int nModID, int nADCtype, int nADCnum, int nADCchan, short nNeedLog );
*/
BOOL	WINAPI	Serial_BoardRead2432( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred );
DWORD	WINAPI	Serial_ReadForVAC( unsigned short nModID, unsigned short* pFrameData, unsigned long *BytesTransferred );

#endif // !defined(AFX_MCICARD_COMMAPI_SERIAL_)
