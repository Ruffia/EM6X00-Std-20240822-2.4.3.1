#if !defined(AFX_PAGEVS_H__53656FFD_F4BC_4061_A2AF_E8D76FB607A2__INCLUDED_)
#define AFX_PAGEVS_H__53656FFD_F4BC_4061_A2AF_E8D76FB607A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageVS.h : header file
//

#include "MacButtons.h"

/////////////////////////////////////////////////////////////////////////////
// CPageVS dialog

class CPageVS : public CDialog
{
	CBitmap		m_bmpON;
	CBitmap		m_bmpOFF;

// Construction
public:
	CPageVS(CWnd* pParent = NULL);   // standard constructor

	void	Init();
	long	BoardRead0816( int nRsize, int nModID, BYTE bCktAddr, WORD wWdata );
	long	BoardRead2432( int nRsize, int nModID, BYTE bCktAddr, DWORD dwWdata );
	void	ADCvconv();

	void	VACDACgroupOptionChanged();
	void	VACDACselOptionChanged();
	void	VACDACvalUpdate( int nPos );
	void	VACDACupdate( int nVACDACvalue );
	void	IOXguide( int nPos );
	void	CheckSoftEnableVacuum( int nCheck );
	void	InitVacuumControls( BOOL bCtrlBySoft );
	void	LegHVUpdate( int nPos );
	void	SPRDACselOptionChanged();
	void	SPRDACsValUpdae( int nPos );
	void	SPRDACupdate();

	int		m_nDACTCvalue[12];
	int		m_nSPRDACvalues[4];

	int		m_nSPRDACchannel, m_nSPRDACnewVal;
	BYTE	m_byteSPRDACselBits;

	int		m_nVACDACselected, m_nVACDACchannel;	// VAC board addr & chan selects for TC gauge DAC's (U44,45,46)
	BYTE	m_byteVACDACaddr, m_byteVACDACselBits;
	CString	m_strVACDACRefDes, m_strVACDACchan;		// (Reference Designnator and channel # in string form)

	BOOL	m_bVACtestLED4, m_bSPRtestLED4, m_bWDTSelfReset;
	BOOL	m_bFansOnFPGA, m_bFansOnPLD, m_bSPRRelay1En, m_bSPRRelay2En, m_bSPRRelay3En, m_bSPRRelay4En, m_bHVOn;
	BOOL	m_bInh20V, m_bInh13V, m_bInh8V;
	void	SwitchAnalogPowerSupplies( int nCheck );

	void	StartScan();
	void	StopScan();
	void	SetPxlClkto2();

	void	ReadComprs();
	CString	ReadComprs_Logging();
	CString	ReadComprs_Logging( int nData );
	void	ReadSelectedADC_Finish(BOOL bSingle = TRUE);
	BOOL	m_bGraph, m_bRecord;

	WORD	m_wposHV;

	void	SetTimers( UINT nIDEvent, BOOL bSet );
	void	InitDiagnose( BOOL bEnd );

	void	HVOnAutoTest();
	void	SPRRelay1EnAutoTest();
	WORD	m_dwV1stat;

// Dialog Data
	//{{AFX_DATA(CPageVS)
	enum { IDD = IDD_PAGE_VAC_SPR };
	CMacButton	m_btnSPRRelay4En;
	CMacButton	m_btnSPRRelay3En;
	CMacCheckBox	m_btnSoftEnVac;
	CMacButton	m_btnSPRreadBoardID;
	CMacButton	m_btnVACreadBoardID;
	CMacButton	m_btnSPRsoftReset;
	CMacButton	m_btnVACsoftReset;
	CMacButton	m_btnHVOn;
	CMacButton	m_btnSPRRelay2En;
	CMacCheckBox	m_btnAnalogPS;
	CScrollBar	m_scrollLegHV;
	CScrollBar	m_scrollSPRDACsVal;
	CMacButton	m_btnWDTSelfReset;
	CMacButton	m_btnSPRtestLED4;
	CMacButton	m_btnInh8V;
	CMacButton	m_btnInh13V;
	CMacButton	m_btnInh20V;
	CMacButton	m_btnSPRRelay1En;
	CMacButton	m_btnFansOnPLD;
	CMacButton	m_btnFansOnFPGA;
	CMacButton	m_btnVACtestLED4;
	CScrollBar	m_scrollIOexp;
	CScrollBar	m_scrollVACDACsVal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageVS)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageVS)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVACDACupdateCmd();
	afx_msg void OnReadComprsCmd();
	afx_msg void OnIOXstbHiCmd();
	afx_msg void OnIOXstbLoCmd();
	afx_msg void OnReadVACextSwitchStates();
	afx_msg void OnVACtestLED4();
	afx_msg void OnVACsoftReset();
	afx_msg void OnLocalLoopback();
	afx_msg void OnStartScan();
	afx_msg void OnStopScan();
	afx_msg void OnSetPxlClkto2();
	afx_msg void OnFansOnFPGA();
	afx_msg void OnFansOnPLD();
	afx_msg void OnSPRsoftReset();
	afx_msg void OnSPRRelay1En();
	afx_msg void OnHVOn();
	afx_msg void OnLegHVOnOff();
	afx_msg void OnInh20V();
	afx_msg void OnInh13V();
	afx_msg void OnInh8V();
	afx_msg void OnSPRtestLED4();
	afx_msg void OnTestR8();
	afx_msg void OnTestReadSPRswitches();
	afx_msg void OnTestR16();
	afx_msg void OnTestR32();
	afx_msg void OnReadSPRADC();
	afx_msg void OnReadVACADC();
	afx_msg void OnReadSelectedADC();
	afx_msg void OnWDTSelfResetToggle();
	afx_msg void OnSPRDACupdate();
	afx_msg void OnCHECKReadVACmprsAuto();
	afx_msg void OnBoardCommRx();
	afx_msg void OnSwitchAnalogPowerSupplies();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSPRRelay2En();
	afx_msg void OnVACreadBoardID();
	afx_msg void OnSPRreadBoardID();
	afx_msg void OnCHECKSoftEnableVacuum();
	afx_msg void OnCHECKReadSelectedADCAuto();
	afx_msg void OnBUTTONOnlySendReadSelectedADCcmd();
	afx_msg void OnSPRRelay3En();
	afx_msg void OnSPRRelay4En();
	afx_msg void OnHVOnAutoTest();
	afx_msg void OnSPRRelay1EnAutoTest();
	//}}AFX_MSG
	afx_msg void OnVACDACgroupOptionChanged( UINT nID );
	afx_msg void OnVACDACselOptionChanged( UINT nID );
	afx_msg void OnFPGArelayEnChanged( UINT nID );
	afx_msg void OnADCselectOBChanged( UINT nID );
	afx_msg void OnADCsChSelectChanged( UINT nID );
	afx_msg void OnSPRDACselOptionChanged( UINT nID );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEVS_H__53656FFD_F4BC_4061_A2AF_E8D76FB607A2__INCLUDED_)
