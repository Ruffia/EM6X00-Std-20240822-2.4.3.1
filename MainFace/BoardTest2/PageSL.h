#if !defined(AFX_PAGESL_H__5F739268_CE5D_4561_A4A8_70F742FFF50E__INCLUDED_)
#define AFX_PAGESL_H__5F739268_CE5D_4561_A4A8_70F742FFF50E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageSL.h : header file
//

#include "MacButtons.h"

/////////////////////////////////////////////////////////////////////////////
// CPageSL dialog

class CPageSL : public CDialog
{
// Construction
public:
	CPageSL(CWnd* pParent = NULL);   // standard constructor

	void	Init();
	long	BoardRead0816( int nRsize, int nModID, BYTE bCktAddr, WORD wWdata );
	long	BoardRead2432( int nRsize, int nModID, BYTE bCktAddr, DWORD dwWdata );
	void	ADCvconv();

	///////////////////////////////////////////////////////
	// SCN
	void	XstigDACSB_Change( int nPos );
	void	YstigDACSB_Change( int nPos );
	void	XshiftDACSB_Change( int nPos );
	void	YshiftDACSB_Change( int nPos );
	void	XxoverDACSB_Change( int nPos );
	void	YxoverDACSB_Change( int nPos );
	void	IOexpSB_Change( int nPos );
	int		GetAmuxSelOption();

	BOOL	UpdateStrobe( BYTE bID, BOOL bHigh );
	BOOL	ClearAnalogSwitches();
	BOOL	UpdateStage15();
	BOOL	UpdateStage150();
	BOOL	UpdateStage1500();
	BOOL	UpdateStage15000();
	BOOL	UpdateStage150000();

	BYTE	m_byteXYgainFBbits, m_byteXYlowFBbits, m_byteXYlowMuxbits;
	BYTE	m_byteAmuxAddrX, m_byteAmuxAddrY;
	BOOL	m_bSCNtestLED4;
	int		m_nOldAMPselOption;
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// LNS
	void	L1DACSB_Change( int nPos );
	void	L2SDACSB_Change( int nPos );
	void	XalDACSB_Change( int nPos );
	void	YalDACSB_Change( int nPos );
	void	WobDACSB_Change( int nPos );
	void	DFStartSB_Change( int nPos );
	void	DFEndSB_Change( int nPos );
	void	DFStepsSB_Change( int nPos );

	int		GetADCselOption();
	void	ChangeDFselectOB( int nChan );

	int		m_nAutoWobblerMin;
	int		m_nAutoWobblerMax;
	BOOL	m_bAutoWobblerOn;
	BOOL	m_bAutoWobblerDir;	// direction
	int		m_nAutoWobblerLast;

	BOOL	m_bLNStestLED4;
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// Diagnose / Logging
	void	SetTimers( UINT nIDEvent, BOOL bSet );
	void	InitDiagnose( BOOL bEnd );
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// MCI
	void	StartScan();
	void	StopScan();
	void	SetPxlClkto2();
	///////////////////////////////////////////////////////

// Dialog Data
	//{{AFX_DATA(CPageSL)
	enum { IDD = IDD_PAGE_SCN_LNS };
	CScrollBar	m_scrollDFSteps;
	CScrollBar	m_scrollDFEnd;
	CScrollBar	m_scrollDFStart;
	CMacButton	m_btnModeSpot;
	CMacButton	m_btnModeLineVertical;
	CMacButton	m_btnModeLineHorizontal;
	CMacButton	m_btnModeNormal;
	CMacCheckBox	m_btnVideo;
	CMacCheckBox	m_btnMainsLock;
	CMacButton	m_btnLNSreadBoardID;
	CMacButton	m_btnSCNreadBoardID;
	CMacButton	m_btnSCNtestLED4;
	CMacButton	m_btnSCNsoftReset;
	CScrollBar	m_scrollIOexp;
	CScrollBar	m_scrollYxoverDAC;
	CScrollBar	m_scrollXxoverDAC;
	CScrollBar	m_scrollYshiftDAC;
	CScrollBar	m_scrollXshiftDAC;
	CScrollBar	m_scrollYstigDAC;
	CScrollBar	m_scrollXstigDAC;
	CMacButton	m_btnLNStestLED4;
	CMacButton	m_btnAutoWob;
	CMacButton	m_btnLNSsoftReset;
	CScrollBar	m_scrollWobDAC;
	CScrollBar	m_scrollYalDAC;
	CScrollBar	m_scrollXalDAC;
	CScrollBar	m_scrollL2SDAC;
	CScrollBar	m_scrollL1DAC;
	CMacCheckBox	m_btnYalCompDisable;
	CMacCheckBox	m_btnXalCompDisable;
	CMacCheckBox	m_btnL2CompDisable;
	CMacCheckBox	m_btnL1CompDisable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageSL)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageSL)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnWrMuxCtl();
	afx_msg void OnIOXstbHi();
	afx_msg void OnIOXstbLo();
	afx_msg void OnSCNsoftReset();
	afx_msg void OnSCNtestLED4Cmd();
	afx_msg void OnTestW8();
	afx_msg void OnTestW16();
	afx_msg void OnTestW24();
	afx_msg void OnTestW32();
	afx_msg void OnL1CompDisable();
	afx_msg void OnL2CompDisable();
	afx_msg void OnXalCompDisable();
	afx_msg void OnYalCompDisable();
	afx_msg void OnLNSsoftReset();
	afx_msg void OnReadADC();
	afx_msg void OnSetWobMinCmd();
	afx_msg void OnSetAutoWobbleCmd();
	afx_msg void OnSetWobMaxCmd();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLNStestLED4();
	afx_msg void OnReadSelectedADC();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSCNreadBoardID();
	afx_msg void OnLNSreadBoardID();
	afx_msg void OnCHECKStigCtrl();
	afx_msg void OnWrMuxClear();
	afx_msg void OnSCNExpChipClearCurrent();
	afx_msg void OnSCNExpChipClearAll();
	afx_msg void OnCHECKReadSelectedADCAuto();
	afx_msg void OnScanModeNormal();
	afx_msg void OnScanModeLineHorizontal();
	afx_msg void OnScanModeLineVertical();
	afx_msg void OnScanModeSpot();
	afx_msg void OnFreeze();
	afx_msg void OnFreezeX();
	afx_msg void OnFreezeY();
	afx_msg void OnCHECKMainsLock();
	afx_msg void OnCHECKVideo();
	afx_msg void OnStartScan();
	afx_msg void OnStopScan();
	afx_msg void OnSetPxlClkto2();
	afx_msg void OnDFInit();
	afx_msg void OnCHECKDFNegative();
	//}}AFX_MSG
	afx_msg void OnAmuxSelOptionChanged(UINT nID);
	afx_msg void OnADCselectOBChanged(UINT nID);
	afx_msg void OnADCsChSelectChanged(UINT nID);
	afx_msg void OnAMPselOptionChanged(UINT nID);
	afx_msg void OnDFselectOBChanged(UINT nID);
	afx_msg void OnMagOBChanged(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESL_H__5F739268_CE5D_4561_A4A8_70F742FFF50E__INCLUDED_)
