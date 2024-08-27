#if !defined(AFX_USB_SEND_H__7DED8C92_2468_4813_96D0_711FE1EDD38F__INCLUDED_)
#define AFX_USB_SEND_H__7DED8C92_2468_4813_96D0_711FE1EDD38F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// USB_Send.h : header file
//


#include <afxtempl.h>		// MFC support for Template
#include <mmsystem.h>		// system timer support

#include <cfifo.h>
#include <mcio.h>
#include <TimeStatistic.h>
#include "FT232HDevice.h"

#include "USB_Functions.h"

/////////////////////////////////////////////////////////////////////////////
// CUSB_Send thread

class CUSB_Send : public CWinThread
{
	DECLARE_DYNCREATE(CUSB_Send)
protected:
	BOOL	m_blPause;

	HWND	m_hMain;
	CFT232HDevice*	m_p232Device;

	SharedParams_Record_List*	m_pSPlist;
	int		m_nListIndex;

// Operations
public:
	CUSB_Send();           // protected constructor used by dynamic creation
	void	Bind( CFT232HDevice* pUsb )
	{ 
		//CFT232HDevice * pUsbSafety
		//	= DYNAMIC_DOWNCAST(CFT232HDevice, pUsb);
		//m_p232Device = pUsbSafety;
		m_p232Device = pUsb;
	}
	
	BOOL	isPause() const { return m_blPause; };
	
	BOOL	SetPause( BOOL	blPause ) 
	{ 
		BOOL blRet = m_blPause;
		m_blPause = blPause;
		
		return blRet;
	};

	void	SetHandle( HWND h){ m_hMain = h; };
	void	SetSharedParams( int nIndex, void* pParams );

// Attributes
public:
	CLoopRate	m_PostRate;
	CLoopRate	m_SendRate;
	CLoopRate	m_FifoOverflowRate;

protected:
	CFIFO<USB_SEM_SIO_Package, USB_SEM_SIO_Package&>	m_fifoHostMSG;
	CFIFO<CString, CString&>					m_fifoError;

	HANDLE					m_hEventKill;
	HANDLE					m_hEventDead;
	volatile BOOL			m_blClose;

protected:
	// Operations
	BOOL	CopyMSGfromHost( USB_SEM_ControlMessage& hostMSG );
	BOOL	GetMSGfromHost( USB_SEM_ControlMessage& hostMSG );
	BOOL	PostErrorInformation( CString& csError );

	BOOL	NotifyMSGInfo( USB_SEM_SIO_Package& sspTemp, UINT nFlag);

	void	MainLoop();

public:
	void	KillThread();

	BOOL	PostMSGtoFPGA( USB_SEM_ControlMessage& hostMSG );
	BOOL	GetLastErrorInformation( CString& csError );

	BOOL	PostMCIOpcr( int nProcessID, BYTE bOpcode, BOOL blOperand, WORD wData );
	BOOL	PostSubOpcr( int nProcessID, BYTE bOp1, BYTE bOp2 );
	BOOL	PostSubOpcr2( int nProcessID, BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 );
	BOOL	PostSubOpcr3( int nProcessID, BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, BYTE bOp5, BYTE bOp6 );
	BOOL	PostSubReadADC( int nProcessID, BYTE bModID, BYTE bCID, int nADCnum, int nADCchn, BYTE bTM, WORD wData, BOOL bAnalyze );
	void	AddToReadbackSerial( int nProcessID, BOOL bType, BYTE bOp1, BYTE bOp2 );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUSB_Send)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
public:
	virtual ~CUSB_Send();
	void	Delete();

	// Generated message map functions
	//{{AFX_MSG(CUSB_Send)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USB_SEND_H__7DED8C92_2468_4813_96D0_711FE1EDD38F__INCLUDED_)
