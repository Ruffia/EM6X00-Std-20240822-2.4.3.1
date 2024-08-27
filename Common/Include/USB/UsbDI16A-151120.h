// UsbDI16A.h: interface for the CUsbDI16A class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USBDI16A_H__EA4F3AAF_3E82_4022_8BC4_D2FE8FA3A0E3__INCLUDED_)
#define AFX_USBDI16A_H__EA4F3AAF_3E82_4022_8BC4_D2FE8FA3A0E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "COMMAPI_USB.h"
#include "COMMAPI_Serial.h"

class AFX_EXT_CLASS CUsbDI16A
{
	BOOL	m_bDI16A_Ready;
	BOOL	m_bFT232_Ready;
	int		m_nCommMode;

public:
	CUsbDI16A();
	virtual ~CUsbDI16A();
	void	SetCommMode( int nMode );

	int		USB_InitCard( void );
	BOOL	USB_CloseCard( void );
	BOOL	USB_isReady();

	int		USB_InitCommPort( void );
	BOOL	USB_CloseCommPort( void );
	BOOL	USB_isCommPortReady();
	int		USB_InitImagePort( void );
	BOOL	USB_CloseImagePort( void );
	BOOL	USB_isImagePortReady();

	BOOL	USB_IoPortRead1( unsigned long BitIndex, unsigned char *Buffer );
	BOOL	USB_IoPortWrite1( unsigned long BitIndex, unsigned char bData );
	BOOL	USB_IoPortRead8( unsigned long ByteIndex, unsigned char *pBuffer );
	BOOL	USB_IoPortWrite8( unsigned long ByteIndex, unsigned char Data );
	BOOL	USB_IoPortReadAll( void *Buffer );
	BOOL	USB_IoPortWriteAll( void *pData );
	BOOL	USB_IoPortRead( void *Buffer, DWORD &dwBytesReturned );
	BOOL	USB_IoPortWrite( void *pData, DWORD &dwBytesWritten );

	BOOL	USB_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData );
	void	USB_CommToBoards( BYTE byteLo, BYTE byteHi );
	void	USB_CommToBoards2( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 );
	void	USB_CommToBoards3( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, BYTE bOp5, BYTE bOp6 );

	void	USB_CommToBoardsDirect( BYTE byteLen, BYTE* pbData );
	BOOL	USB_WriteToBoardDirect( BYTE bOpcode, BOOL blOperand, BYTE byteLo, BYTE byteHi );

	DWORD	USB_GetDMABuffSize();

	DWORD	USB_ImageInputStream1( unsigned long FramePoints, unsigned short *pFrameData, unsigned long *BytesTransferred, short nNeedLog );
	DWORD	USB_ImageInputStream2( unsigned long FramePoints, void * pBuffer, unsigned long *BytesTransferred );
	DWORD	USB_ImageInputStreamTest( unsigned long FramePoints, unsigned short *pFrameData, unsigned long *BytesTransferred );
	BOOL	USB_PhotoOutputStream( void * pBuffer, IN LONG dwBlockSize );

	DWORD	USB_LoopbackWords( unsigned long FramePoints, unsigned short* pFrameData, unsigned long *BytesTransferred, short nNeedLog );
	DWORD	USB_LoopbackWords2( unsigned short nModID, unsigned long FramePoints, unsigned short* pFrameData, unsigned long *BytesTransferred, short nNeedLog );
	DWORD	USB_ReadMonitorData( int nModID, int nADCtype, int nADCnum, int nADCchan, short nNeedLog );
	BOOL	USB_BoardRead2432( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred );

	DWORD	USB_ClearFIFO();
	DWORD	USB_StreamOpen( unsigned long bIsRead );
	DWORD	USB_StreamClose();
	DWORD	USB_ConfigureEx(void *pOutMask, void *pData, void *pTristateMask);

	BOOL	USB_IsVideo();
	DWORD	USB_VideoON();
	DWORD	USB_VideoOFF();

	BOOL	USB_GetPortInfo( BYTE* pPort );
	BOOL	USB_GetMCIPwr( BOOL bAll, int nChn, unsigned short* pPwr );
	BOOL	USB_GetISR( int nMID, unsigned short* pISR );
	BOOL	USB_GetBoardID( int nMID, unsigned short* pBoardID );
	BOOL	USB_GetGaugeCompr( unsigned short* pGauge );
	BOOL	USB_GetLoopbackTest( unsigned short* pData );
};

#endif // !defined(AFX_USBDI16A_H__EA4F3AAF_3E82_4022_8BC4_D2FE8FA3A0E3__INCLUDED_)
