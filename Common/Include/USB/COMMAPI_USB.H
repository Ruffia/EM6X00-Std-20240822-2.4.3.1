// COMMAPI_USB.h : header file for the COMMAPI.cpp
//

#if !defined(AFX_MCICARD_COMMAPI_USB_)
#define AFX_MCICARD_COMMAPI_USB_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COMMAPI_USB
// See COMMAPI_USB.cpp for the implementation of these functions.
//

const DWORD	USB_ImageFIFOStartOffset	= 0x78;
const DWORD	USB_PhotoFIFOStartOffset	= 0x64;
const DWORD	USB_SCANFIFOSIZE			= 1024 * 64;	// 64K
const DWORD	USB_PHOTOFIFOSIZE			= 1024 * 8;		// 8K


BOOL	WINAPI USB_SetCommMode( int nMode );
int		WINAPI USB_InitCard( void );
BOOL	WINAPI USB_CloseCard( void );

BOOL	WINAPI USB_IoPortRead1( unsigned long BitIndex, unsigned char *Buffer );
BOOL	WINAPI USB_IoPortWrite1( unsigned long BitIndex, unsigned char bData );
BOOL	WINAPI USB_IoPortRead8( unsigned long ByteIndex, unsigned char *pBuffer );
BOOL	WINAPI USB_IoPortWrite8( unsigned long ByteIndex, unsigned char Data );
BOOL	WINAPI USB_IoPortReadAll( void *Buffer );
BOOL	WINAPI USB_IoPortWriteAll( void *pData );

BOOL	WINAPI USB_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData );
void	WINAPI USB_CommToBoards( BYTE byteLo, BYTE byteHi );
void	WINAPI USB_CommToBoards2( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 );
void	WINAPI USB_CommToBoards3( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, BYTE bOp5, BYTE bOp6 );

void	WINAPI USB_CommToBoardsDirect( BYTE byteLen, BYTE* pbData );
BOOL	WINAPI USB_WriteToBoardDirect( BYTE bOpcode, BOOL blOperand, BYTE byteLo, BYTE byteHi );

DWORD	WINAPI USB_ImageInputStream1( unsigned long FramePoints, unsigned short *pFrameData, unsigned long *BytesTransferred, short nNeedLog );
DWORD	WINAPI USB_ImageInputStream2( unsigned long FramePoints, void * pBuffer, unsigned long *BytesTransferred );
DWORD	WINAPI USB_ImageInputStream5( unsigned long FramePoints, unsigned short *pFrameData, unsigned long *BytesTransferred );
BOOL	WINAPI USB_PhotoOutputStream( void * pBuffer, IN LONG dwBlockSize );

DWORD	WINAPI USB_LoopbackWords( unsigned long FramePoints, unsigned short* pFrameData, unsigned long *BytesTransferred, short nNeedLog );
DWORD	WINAPI USB_LoopbackWords2( unsigned short nModID, unsigned long FramePoints, unsigned short* pFrameData, unsigned long *BytesTransferred, short nNeedLog );
DWORD	WINAPI USB_ReadMonitorData( int nModID, int nADCtype, int nADCnum, int nADCchan, short nNeedLog );

BOOL	WINAPI USB_BoardRead2432( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred );

DWORD	USB_GetDMABuffSize();
DWORD	WINAPI USB_ClearFIFO();
DWORD	WINAPI USB_StreamOpen( unsigned long bIsRead );
DWORD	WINAPI USB_StreamClose( void );
DWORD	WINAPI USB_ConfigureEx(void *pOutMask, void *pData, void *pTristateMask);

BOOL	WINAPI USB_IsVideo();
DWORD	WINAPI USB_VideoON();
DWORD	WINAPI USB_VideoOFF();
#endif // !defined(AFX_MCICARD_COMMAPI_USB_)
