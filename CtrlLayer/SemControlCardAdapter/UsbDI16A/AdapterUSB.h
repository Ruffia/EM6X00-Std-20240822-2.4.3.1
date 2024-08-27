#if !defined(AFX_ADAPTERUSB_H__38ED7FEA_7391_4646_9F7F_40FB05B8445B__INCLUDED_)
#define AFX_ADAPTERUSB_H__38ED7FEA_7391_4646_9F7F_40FB05B8445B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdapterUSB.h : header file
//

//#include "CriticalResource.h"
#include <afxmt.h>

#define ReadBack_DataSize	256

/////////////////////////////////////////////////////////////////////////////
// CAdapterUSB window

class AFX_EXT_CLASS CAdapterUSB
{
	BOOL	m_bUSB_Ready;

// Construction
public:
	CAdapterUSB();
	virtual ~CAdapterUSB();

// Attributes
public:
	unsigned long		deviceIndex;
	DWORD				outMask;
	DWORD				inMask;
	
	BOOL				bVideoON;
	int					nLoopbackWords_Times;
	DWORD				USB_dwErrorCount;
	
//	CriticalResource	m_mutexPorts;
	static	CCriticalSection	m_mutexPorts;
	double				dREAD_CLOCK;
	double				dWRITE_CLOCK;

	int					m_nRBwdCnt;
	unsigned short		m_nFrameData[ReadBack_DataSize*2];
	int					m_nBoardReadDataeBuf[ReadBack_DataSize*2];
	double				m_dADCchanModes[6*16*4];

	TCHAR				exeFullPath[255];
	
// Operations
public:
	BOOL	WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData );
	void	CommToBoards( BYTE byteLo, BYTE byteHi );
	DWORD	BoardRead0816( int nRsize, int nModID, BYTE bCktAddr, WORD wWdata );
	DWORD	BoardRead2432( int nRsize, int nModID, BYTE bCktAddr, DWORD dwWdata );
	void	ADCvconv( int nADCtype );

	int		ReadInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator=',');
	int		SaveInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator=',');

	BOOL	GetModuleFilePath(LPTSTR exeFullPath);
	void	ReadDataFromINI();

	BOOL	USB_OpenDevice( void );
	void	USB_ResetCard();
	BOOL	USB_InitCard_once( void );
	BOOL	USB_InitCard( void );
	BOOL	USB_CloseCard( void );
	BOOL	USB_isReady(){ return m_bUSB_Ready; }

	DWORD	USB_ClearFIFO( void );
	DWORD	USB_StreamOpen( unsigned long bIsRead );
	DWORD	USB_StreamClose( void );
	DWORD	USB_ConfigureEx(void *pOutMask, void *pData, void *pTristateMask);

	DWORD	USB_ImageInputStream1( unsigned long FramePoints, unsigned short *pFrameData, unsigned long *BytesTransferred, short nNeedLog );

	BOOL	USB_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData );
	void	USB_CommToBoards( BYTE byteLo, BYTE byteHi );
	void	USB_CommToBoards2( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 );
	void	USB_CommToBoardsDirect( BYTE byteLen, BYTE* pbData );
	BOOL	USB_WriteToBoardDirect( BYTE bOpcode, BOOL blOperand, BYTE byteLo, BYTE byteHi );

	BOOL	USB_IsVideo();
	DWORD	USB_VideoON();
	DWORD	USB_VideoOFF();

	DWORD	USB_LoopbackWords( unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred, short nNeedLog );
	DWORD	USB_LoopbackWords2( unsigned short nModID, unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred, short nNeedLog );
	DWORD	USB_ReadMonitorData( int nModID, int nADCtype, int nADCnum, int nADCchan, short nNeedLog );
	BOOL	USB_BoardRead2432( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAPTERUSB_H__38ED7FEA_7391_4646_9F7F_40FB05B8445B__INCLUDED_)
