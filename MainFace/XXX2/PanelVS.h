#if !defined(AFX_PANELVS_H__8CC9D607_31CB_4E7E_859C_9123CADEE9EB__INCLUDED_)
#define AFX_PANELVS_H__8CC9D607_31CB_4E7E_859C_9123CADEE9EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelVS.h : header file
//

#include "MacButtons.h"

/////////////////////////////////////////////////////////////////////////////
// CPanelVS dialog

class CPanelVS : public CDialog
{
// Construction
public:
	CPanelVS(CWnd* pParent = NULL);   // standard constructor

	void	Init();
	void	UpdateReadbackData( unsigned short* pnData );
	void	VAC_ReadComprs();

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
	void	ADCvconv();

	int		m_nSPRDACchannel, m_nSPRDACnewVal;

	int		m_nVACDACselected, m_nVACDACchannel;	// VAC board addr & chan selects for TC gauge DAC's (U44,45,46)
	BYTE	m_byteVACDACaddr, m_byteVACDACselBits;
	CString	m_strVACDACRefDes, m_strVACDACchan;		// (Reference Designnator and channel # in string form)

	BOOL	m_bVACtestLED4, m_bSPRtestLED4, m_bWDTSelfReset;
	BOOL	m_bFansOnFPGA, m_bFansOnPLD, m_bSPRRelay1En, m_bSPRRelay2En, m_bHVOn;
	BOOL	m_bInh20V, m_bInh13V, m_bInh8V;
	void	SwitchAnalogPowerSupplies( int nCheck );

	void	InitDiagnose( BOOL bEnd );
	void	SetTimers( UINT nIDEvent, BOOL bSet );

	WORD	m_wposHV;

// Dialog Data
	//{{AFX_DATA(CPanelVS)
	enum { IDD = IDD_PAGE_VAC_SPR };
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
	//{{AFX_VIRTUAL(CPanelVS)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPanelVS)
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
	afx_msg void OnCHECKReadSelectedADCAuto();
	afx_msg void OnWDTSelfResetToggle();
	afx_msg void OnSPRDACupdate();
	afx_msg void OnCHECKReadVACmprsAuto();
	afx_msg void OnBoardCommRx();
	afx_msg void OnClearFIFO();
	afx_msg void OnClearList();
	afx_msg void OnSwitchAnalogPowerSupplies();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSPRRelay2En();
	afx_msg void OnVACreadBoardID();
	afx_msg void OnSPRreadBoardID();
	afx_msg void OnCHECKSoftEnableVacuum();
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

#endif // !defined(AFX_PANELVS_H__8CC9D607_31CB_4E7E_859C_9123CADEE9EB__INCLUDED_)
