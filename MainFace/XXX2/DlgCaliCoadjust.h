#if !defined(AFX_DLGCALICOADJUST_H__4DB0A3EE_6413_42A7_9E6E_D9D558F8D1E8__INCLUDED_)
#define AFX_DLGCALICOADJUST_H__4DB0A3EE_6413_42A7_9E6E_D9D558F8D1E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCaliCoadjust.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliCoadjust dialog

class CDlgCaliCoadjust : public CDialog
{
// Construction
public:
	CDlgCaliCoadjust(CWnd* pParent = NULL);   // standard constructor
	void	Localize(void);
	void	InitParams();
	void	UpdateCaliData();
	void	UpdateCaliData_OLWDMag();
	void	UpdateCaliData_HV();

	void	CalculateOLWDMag();
	void	CalculateHV();

	void	AddRecord_OLWDMag( double dWD );
	void	AddRecord_HV( double dWD );

	void	Coadjust_Old6200( char* path );
	void	Coadjust_62006900( char* path );
	void	Coadjust_New6X00( char* path );
	void	Coadjust_HV( char* path );

	int		m_nType, m_nCoadjustMode;

	/////////////////////////////////////////////
	void	MCI_Det45SB_Change( int nPos );
	void	MCI_Det45SBY_Change( int nPos );
	void	MCI_SyncDet45( int nPosX, int nPosY );
	/////////////////////////////////////////////

	int		m_nApproxMag;
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	void	SCN_XxoverDACSB_Change( int nPos );
	void	SCN_YxoverDACSB_Change( int nPos );
	void	SCN_SyncXOverPos( int nPosX, int nPosY );
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// 2021.11: 最小放大倍数档超时保护
	void	UpdateMagStage( int nStage );
	void    KillMagMinTimer( BOOL bHVon );
	void	CompareMagTimeout();
	UINT	m_nMag_MinStageTimer;
	DWORD	m_dwMagTickCountStart;
	int		m_nMagMinStageTimeoutCounts;
	void	InitMagMinLogFile();
	void	MessageMagMin(LPCTSTR lpszMessage);
	CFile	m_fMagMinLogFile;
	/////////////////////////////////////////////

// Dialog Data
	//{{AFX_DATA(CDlgCaliCoadjust)
	enum { IDD = IDD_Cali_Coadjust };
	CScrollBar	m_scrollDet45;
	CScrollBar	m_scrollDet45Y;
	CScrollBar	m_scrollXxoverDAC;
	CScrollBar	m_scrollYxoverDAC;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCaliCoadjust)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCaliCoadjust)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBUTTONATCaliCoadjustApply();
	afx_msg void OnBUTTONATCaliCoadjustReset();
	afx_msg void OnBUTTONATCaliCoadjustRecord();
	afx_msg void OnBUTTONATCaliCoadjustLoad();
	afx_msg void OnBUTTONATCaliCoadjustAdd();
	afx_msg void OnBUTTONATCaliCoadjustSetLength();
	afx_msg void OnCHECKDebugModeCoadjust();
	afx_msg void OnClearAllChannels();
	afx_msg void OnWriteRateX();
	afx_msg void OnWriteRateY();
	afx_msg void OnWriteRateX2();
	afx_msg void OnWriteRateY2();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnCancel();
	afx_msg void OnShowGraph1();
	afx_msg void OnShowGraph2();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRadioAtCaliCoadjustModeOL();
	afx_msg void OnRadioAtCaliCoadjustModeHV();
	//}}AFX_MSG
	afx_msg void OnCheckChanged(UINT nID);
	afx_msg void OnApproxMagChanged( UINT nID );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCALICOADJUST_H__4DB0A3EE_6413_42A7_9E6E_D9D558F8D1E8__INCLUDED_)
