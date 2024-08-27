#if !defined(AFX_PANELSTAGE_H__EC8ED88A_57D1_4CA2_B714_4B8FF60FB2BD__INCLUDED_)
#define AFX_PANELSTAGE_H__EC8ED88A_57D1_4CA2_B714_4B8FF60FB2BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelStage.h : header file
//

#include "dmcwin.h"
#include <USB_Functions.h>

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

	CString	GetResString( UINT nID );
	void	Localize(void);
	void	SetupDebugParam( BOOL bDebugStage );
	void	DisplayMsg(LPCTSTR lpszMessage);

	// XY自动台
	void	Stage_XY_Init();
	void	Stage_XY_FindIndex();
	int		Stage_XY_GetPosition( long& lX, long& lY );
	void	Stage_XY_Command( CString strCmd, int nValue, BOOL bIsX, BOOL bDir );
	void	Stage_XY_Command2( CString strCmd, int nValueX, int nValueY, BOOL bDirX, BOOL bDirY, BOOL bSerial );
	
	void	StagePosLoad();
	void	StagePosUpdate();
	void	StagePosGoto( int nItem );
	void	StagePosGotoX();
	void	StagePosGotoY();
	void	StageMoveX( int nScaleX, BOOL bRelative, BOOL bDir );
	void	StageMoveY( int nScaleY, BOOL bRelative, BOOL bDir );

	CString	CalculateUScale2( int nScreenWidth, double dfMag, int& nScalePixels );
	CString CalculateStageUScale( BOOL	bIsX,		// 是X或Y：1 is X, 0 is Y
							 BOOL	bDir,			// 方向：1 is positive, 0 is negative
							 int	nScalePixels,	// 像素长度
							 int	&nScale );		// 实际移动步数
	void	CoherenceCheck();
	void	MagChange();

	CDMCWin m_DMCWin;
	ConfigStage	m_Stage;
	ListStagePos*	m_plistStagePos;
	int		m_nStagePosTotal;
	CString	m_strName;
	BOOL	m_bStageRelative;
	CSize	m_sizeReso;

	void	ManuallyAdjustStage( UINT nChar );
	void	SetStagePointParam(CPoint point);
	void	SetStageTrackParam(CPoint point);

	CPoint	m_ptPosSave, m_ptPosNew;
	BOOL	m_bCatch;
	BOOL	m_bStageOrigin;
	BOOL	m_bStageOriginHitTest;
	BOOL	m_bStageTrack;
	BOOL	m_bShiftDown;
	CRect	m_rcStageDragRectLast;
	CPoint	m_ptStagePointParam, m_ptStageTrackParam;
	int		m_nStageTrackStepX, m_nStageTrackStepY;
	DWORD		m_lastStage;
	CDialog*	m_pAutoProgressDlg;

	void	AutoStageXY_FI_Init();
	void	AutoStageXY_FI_Process11();
	void	AutoStageXY_FI_Process12();
	void	AutoStageXY_FI_Process21();
	void	AutoStageXY_FI_Process22();
	void	AutoStageXY_FI_Process31();
	void	AutoStageXY_FI_Process32();
	void	AutoStageXY_FI2_Process11();
	void	AutoStageXY_FI2_Process12();
	void	AutoStageXY_FI2_Process21();
	void	AutoStageXY_FI2_Process22();
	void	AutoStageXY_FI2_Process31();
	void	AutoStageXY_FI2_Process32();
	void	AutoStageXY_FI_XQ();
	void	AutoStageXY_FI_Process4();
	void	AutoStageXY_FI_Finish( BOOL bAutoFinish );
	void	AutoStageXY_FI_TrackSecurity();
	BOOL	m_bStageFinishX, m_bStageFinishY;

	void	AutoStage_Clear( BOOL bZRT );
	void	AutoStage_Clear_XY();
	void	AutoStage_Clear_Finish( BOOL bStep, BOOL bAutoFinish );

	void	GraphDrawCircle( CPoint ptCenter, CPoint ptEnd, COLORREF color, BOOL bDrawArrow = TRUE, 
							int nRadius = nSTAGE_TRACE_POINT_RADIUS );	// 12.10.11
	void	GraphClear( const CRect rcDest,	const COLORREF color );

// Dialog Data
	//{{AFX_DATA(CPanelStage)
	enum { IDD = IDD_PANEL_Stage };
	CSpinButtonCtrl	m_spinMag;
	CListCtrl		m_listCtrlStagePos;
	double	m_dMag;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelStage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CString m_csUScaleUnit;
	double	m_dfMobileUScaleParameter;

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
	afx_msg void OnDeltaposStageMagAdj(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStageAdvance();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANELSTAGE_H__EC8ED88A_57D1_4CA2_B714_4B8FF60FB2BD__INCLUDED_)
