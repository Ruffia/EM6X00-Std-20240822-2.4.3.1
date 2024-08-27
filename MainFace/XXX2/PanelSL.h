#if !defined(AFX_PANELSL_H__38929603_6548_42F3_A3DF_004D8772E697__INCLUDED_)
#define AFX_PANELSL_H__38929603_6548_42F3_A3DF_004D8772E697__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelSL.h : header file
//

#include "MacButtons.h"

/////////////////////////////////////////////////////////////////////////////
// CPanelSL dialog

class CPanelSL : public CDialog
{
// Construction
public:
	CPanelSL(CWnd* pParent = NULL);   // standard constructor

	void	Init();
	void	UpdateReadbackData( unsigned short* pnData );

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

	void	SyncXOverPos( int nPosX, int nPosY );

	BYTE	m_byteXYgainFBbits, m_byteXYlowFBbits, m_byteXYlowMuxbits;
	BYTE	m_byteAmuxAddrX, m_byteAmuxAddrY;
	BOOL	m_bSCNtestLED4;
	int		m_nOldAMPselOption;
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// LSN
	void	L1DACSB_Change( int nPos );
	void	L2SDACSB_Change( int nPos );
	void	L2DDACSB_Change( int nPos );
	void	XalDACSB_Change( int nPos );
	void	YalDACSB_Change( int nPos );
	void	WobDACSB_Change( int nPos );
	int		GetADCselOption();

	int		m_nAutoWobblerMin;
	int		m_nAutoWobblerMax;
	BOOL	m_bAutoWobblerOn;
	BOOL	m_bAutoWobblerDir;	// direction
	int		m_nAutoWobblerLast;

	BOOL	m_bLNStestLED4;
	///////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////
	// Diag & Logging
	void	InitDiagnose( BOOL bEnd );
	void	SetTimers( UINT nIDEvent, BOOL bSet );

// Dialog Data
	//{{AFX_DATA(CPanelSL)
	enum { IDD = IDD_PAGE_SCN_LNS };
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
	CScrollBar	m_scrollL2DDAC;
	CScrollBar	m_scrollL2SDAC;
	CScrollBar	m_scrollL1DAC;
	CMacCheckBox	m_btnYalCompDisable;
	CMacCheckBox	m_btnXalCompDisable;
	CMacCheckBox	m_btnL2CompDisable;
	CMacCheckBox	m_btnL1CompDisable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelSL)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPanelSL)
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
	afx_msg void OnCHECKReadLNSadcAuto();
	afx_msg void OnReadSelectedADC();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSCNreadBoardID();
	afx_msg void OnLNSreadBoardID();
	afx_msg void OnCHECKStigCtrl();
	afx_msg void OnWrMuxClear();
	afx_msg void OnSCNExpChipClearCurrent();
	afx_msg void OnSCNExpChipClearAll();
	afx_msg void OnWriteRateX();
	afx_msg void OnWriteRateY();
	//}}AFX_MSG
	afx_msg void OnAmuxSelOptionChanged(UINT nID);
	afx_msg void OnADCselOptionChanged(UINT nID);
	afx_msg void OnAMPselOptionChanged(UINT nID);
	afx_msg void OnWobblerselOptionChanged(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANELSL_H__38929603_6548_42F3_A3DF_004D8772E697__INCLUDED_)
