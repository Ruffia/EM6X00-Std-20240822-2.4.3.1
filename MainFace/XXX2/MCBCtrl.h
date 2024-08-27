#if !defined(AFX_MCBCtrl_H__19BA9847_B0D1_42E4_8F69_021DA483D535__INCLUDED_)
#define AFX_MCBCtrl_H__19BA9847_B0D1_42E4_8F69_021DA483D535__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MCBCtrl.h : header file
//

//#include "SemCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CMCB thread

class CMCBCtrl : public CWinThread
{
	DECLARE_DYNCREATE(CMCBCtrl)
public:
	CMCBCtrl();           // protected constructor used by dynamic creation

protected:
	HANDLE		m_hEventKill;
	HANDLE		m_hEventDead;

// Attributes
public:
	CString		m_strMCB_DeviceName;						// The system name for the remote button device
	BOOL		m_bMCB_Openned;								// Is the ButtonsAndLights device operational?

	HANDLE		m_hMCB_Write;								// Needed to write to a system device
	HANDLE		m_hMCB_Read;								// Needed to read a system device
	HANDLE		m_hMCB_ReadEvent;							// Needed to do overlapped ( asynchronous ) IO
	OVERLAPPED	m_MCB_overlapRead;

	BYTE		g_byteMCB_ReadReport[3];					// Data read from ButtonsAdnLights device, changed to 8 by DD 6-24-10
	//BYTE		byteMCB_WriteReport[2];						// Data written to ButtonsAndLights device
	BYTE		g_byteMCB_Port2RBdata, g_byteMCB_Port2RBdataBak;// Comes from ButtonReport data upon detection of Port2 data change

	BOOL		m_bMCB_ExpectLowByte, m_bMCB_ExpectHighByte;// Keeps track of which USB Rx byte is which
	int			g_nMCB_Channel;								// Which channels's data is being sent (Need to decode from data)
	int			g_nMCB_RotDir;								// CCW or CW rotation direction detected?
	int			g_nMCB_SwPos;								// switch position of corresponding channel
	int			g_nMCB_TicksABS, g_nMCB_TicksHi;			// Low 2 bits of High Byte + Low Byte (10 bits total)
	//long		lMCB_USBwriteTotals, lMCB_USBreadTotals;	// For determining whether any Port3 transitions were missed
	int			g_nMCB_PollCounter;							// A local timer
	int			g_nMCB_CountStamp;

	DWORD		m_lastTickCount;

// Operations
public:
	void KillThread();
	void Delete();

	BOOL InitData();
	void MainLoop();

	CString	MCB_OpenHIDDevice();
	BOOL	MCB_InitHIDDevice( CString strDeviceName );
	void	MCB_CloseHIDDevice();
	DWORD	MCB_StartRead( BYTE* pBuf, int nCount );
	void	MCB_SendReport( BYTE byteBuf, int nCount );
	BOOL	MCB_ReadData( BYTE &bData, BOOL &bLow );
	int		MCB_ReceiveCountStamp( BYTE bData );
	BOOL	MCB_ReceiveHighByte( BYTE bData );
	BOOL	MCB_ReceiveLowByte( BYTE bData );
	BOOL	MCB_CheckHighByte( BYTE bData );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMCBCtrl)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMCBCtrl();
protected:

	// Generated message map functions
	//{{AFX_MSG(CMCBCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCBCtrl_H__19BA9847_B0D1_42E4_8F69_021DA483D535__INCLUDED_)
