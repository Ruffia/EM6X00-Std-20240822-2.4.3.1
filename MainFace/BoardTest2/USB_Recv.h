#if !defined(AFX_USB_RECV_H__A6929C6A_E307_4B56_888C_31BF08B623AB__INCLUDED_)
#define AFX_USB_RECV_H__A6929C6A_E307_4B56_888C_31BF08B623AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// USB_Recv.h : header file
//


#include <afxtempl.h>		// MFC support for Template
#include <mmsystem.h>		// system timer support

#include <cfifo.h>
#include <mcio.h>
#include <TimeStatistic.h>
#include "FT232HDevice.h"

#include "USB_Functions.h"

/////////////////////////////////////////////////////////////////////////////
// CUSB_Recv thread

class CUSB_Recv : public CWinThread
{
	DECLARE_DYNCREATE(CUSB_Recv)
protected:
	BOOL	m_blPause;

	HWND	m_hMain;
	CFT232HDevice*	m_p232Device;

	SharedParams_Cfg_ADCs*		m_pSPcfgADCs;
	SharedParams_Back_ADCs*		m_pSPbackADCs;
	SharedParams_Back_Others*	m_pSPbackOthers;
	SharedParams_Record_List*	m_pSPlist;
	int							m_nListIndex;

// Operations
public:
	CUSB_Recv();           // protected constructor used by dynamic creation
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
	CLoopRate	m_ReceiveRate;

protected:
	CFIFO<USB_SEM_SIO_Package, USB_SEM_SIO_Package&>	m_fifoRecv;
	CFIFO<CString, CString&>					m_fifoError;

	HANDLE					m_hEventKill;
	HANDLE					m_hEventDead;
	volatile BOOL			m_blClose;

protected:
	// Operations
	BOOL	PostErrorInformation( CString& csError );

	BOOL	NotifyMSGInfo( USB_SEM_SIO_Package& sspTemp, UINT nFlag);

	void	MainLoop();

public:
	void	KillThread();

	BOOL	PostMSGtoHost( USB_SEM_ControlMessage& mcMSG );
	BOOL	GetMSGfromMicrocomputer( USB_SEM_ControlMessage& mcMSG );
	BOOL	GetLastErrorInformation( CString& csError );

	void	AnalyzeReadbackData( BYTE* bBuf, DWORD dwLen );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUSB_Recv)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
public:
	virtual ~CUSB_Recv();
	virtual void Delete();

	// Generated message map functions
	//{{AFX_MSG(CUSB_Recv)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USB_RECV_H__A6929C6A_E307_4B56_888C_31BF08B623AB__INCLUDED_)
