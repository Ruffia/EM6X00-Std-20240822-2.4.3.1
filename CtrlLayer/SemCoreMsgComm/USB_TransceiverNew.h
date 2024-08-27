#if !defined(AFX_USB_TRANSCEIVERNEW_H__9A022AD5_3D3A_4951_AF07_31FD70742E70__INCLUDED_)
#define AFX_USB_TRANSCEIVERNEW_H__9A022AD5_3D3A_4951_AF07_31FD70742E70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// USB_TransceiverNew.h : header file
//

#include <afxtempl.h>		// MFC support for Template
#include <mmsystem.h>		// system timer support

#include <cfifo.h>
#include <mcio.h>
#include <TimeStatistic.h>
#include <UsbDI16A.h>
//#include <AdapterUSB.h>

/////////////////////////////////////////////////////////////////////////////
// CUSB_TransceiverNew thread

enum USB_TransceiverState     // State Machine
{
	USB_Transceiver_Listen,
	USB_Transceiver_Recieve,
	USB_Transceiver_Send,
	USB_Transceiver_Pause,
	USB_Transceiver_Error,
};

class CUSB_TransceiverNew : public CWinThread
{
	DECLARE_DYNCREATE(CUSB_TransceiverNew)
protected:
	// protected constructor used by dynamic creation
	BOOL	m_blPause;
	BYTE	m_bLastIoRead;

public:
	CUSB_TransceiverNew(MicrocomputerID mcID = SEM_MCI);

	BOOL isPause() const { return m_blPause; };

	BOOL SetPause( BOOL	blPause ) 
	{ 
		BOOL blRet = m_blPause;
		m_blPause = blPause;
		
		return blRet;
	};

	// Attributes
public:
	CLoopRate	m_HostMsgPostRate;
	CLoopRate	m_HostMsgSendRate;
	CLoopRate	m_HostMsgFifoOverflowRate;

	CLoopRate	m_Z80MsgReceiveRate;

protected:
	MicrocomputerID			m_MicrocomputerID;
	USB_SEM_CommPortState		m_CommPortState;

	CFIFO<USB_SEM_SIO_Package, USB_SEM_SIO_Package&>	m_fifoHostMSG;
	CFIFO<USB_SEM_SIO_Package, USB_SEM_SIO_Package&>	m_fifoZ80MSG;
	CFIFO<CString, CString&>					m_fifoError;

	HANDLE					m_hEventKill;
	HANDLE					m_hEventDead;
	volatile BOOL			m_blClose;

	IN LONG					m_dwLocalReadIOAddress;
	IN LONG					m_dwLocalWriteIOAddress;

	BOOL					m_bNeedReadFIFO;

protected:
	// Operations
	void	InitParameter( MicrocomputerID mcID );

	BOOL	CopyMSGfromHost( USB_SEM_ControlMessage& hostMSG );
	BOOL	GetMSGfromHost( USB_SEM_ControlMessage& hostMSG );
	BOOL	PostErrorInformation( CString& csError );

	BOOL	NotifyMSGInfo( USB_SEM_SIO_Package& sspTemp, UINT nFlag);

	USB_TransceiverState	MainLoop();
	USB_TransceiverState	Listen();

public:
	void	KillThread();

	MicrocomputerID	GetMicrocomputerID(){ return m_MicrocomputerID; };

	BOOL	PostMSGtoHost( USB_SEM_ControlMessage& mcMSG );
	BOOL	PostMSGtoMicrocomputer( USB_SEM_ControlMessage& hostMSG );
	BOOL	PostMSGtoFPGA( USB_SEM_ControlMessage& hostMSG );
	BOOL	GetMSGfromMicrocomputer( USB_SEM_ControlMessage& mcMSG );
	BOOL	GetLastErrorInformation( CString& csError );

	BOOL	SetCoadjustParam( double dHV );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUSB_TransceiverNew)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
public:
	virtual ~CUSB_TransceiverNew();
	virtual void Delete();

	// Generated message map functions
	//{{AFX_MSG(CUSB_TransceiverNew)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USB_TRANSCEIVERNEW_H__9A022AD5_3D3A_4951_AF07_31FD70742E70__INCLUDED_)
