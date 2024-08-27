#if !defined(AFX_PANELSTAGE_H__F0DC6552_40D4_4596_A37D_27FC78043081__INCLUDED_)
#define AFX_PANELSTAGE_H__F0DC6552_40D4_4596_A37D_27FC78043081__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelStage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPanelStage dialog

class CPanelStage : public CDialog
{
	HICON		m_hIconStage1;
	HICON		m_hIconStage2;
	HICON		m_hIconStage3;
	HICON		m_hIconStage4;

// Construction
public:
	CPanelStage(CWnd* pParent = NULL);   // standard constructor
	~CPanelStage();

	void	Localize(void);
	void	SetupDebugParam( BOOL bDebugStage );

	// XY×Ô¶¯Ì¨
	void	StagePosLoad();
	void	StagePosListUpdate();
	void	StagePosUpdate();
	void	StagePosGoto( int nItem );
	void	StagePosGotoX( double dValue );
	void	StagePosGotoY( double dValue );
	void	StageMoveX( int nScaleX, BOOL bRelative, BOOL bDir );
	void	StageMoveY( int nScaleY, BOOL bRelative, BOOL bDir );
	CString	m_strName;
	BOOL	m_bStageRelative;

// Dialog Data
	//{{AFX_DATA(CPanelStage)
	enum { IDD = IDD_PANEL_Stage };
	CListCtrl		m_listCtrlStagePos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelStage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPanelStage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnStageAbsolute();
	afx_msg void OnStageRelative();
	afx_msg void OnStagePosGoto();
	afx_msg void OnStagePosAdd();
	afx_msg void OnStagePosDelete();
	afx_msg void OnStagePosUpdate();
	afx_msg void OnStageCalibrate();
	afx_msg void OnStageXLeft();
	afx_msg void OnStageXRight();
	afx_msg void OnStageYTop();
	afx_msg void OnStageYBottom();
	afx_msg void OnStagePositionsDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStagePositionsBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStagePositionsEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStagePositionsClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStageReset();
	afx_msg void OnStageDP();
	afx_msg void OnStageStop();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANELSTAGE_H__F0DC6552_40D4_4596_A37D_27FC78043081__INCLUDED_)
