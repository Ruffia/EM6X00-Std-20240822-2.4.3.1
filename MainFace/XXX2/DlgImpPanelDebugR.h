#if !defined(AFX_DLGIMPPANELDEBUGR_H__4C4BA726_7E0C_4233_B30E_15A1177A7DCD__INCLUDED_)
#define AFX_DLGIMPPANELDEBUGR_H__4C4BA726_7E0C_4233_B30E_15A1177A7DCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImpPanelDebugR.h : header file
//

#include "MacButtons.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelDebugR dialog

class CDlgImpPanelDebugR : public CDialog
{
// Construction
public:
	CDlgImpPanelDebugR(CWnd* pParent = NULL);   // standard constructor

	void	Init();
	/////////////////////////////////////////////////////////////////
	void	ScanStart();
	void	ScanStop();

	CSize	GetSizes();
	void	CheckIndexs( int nLineCounts, int nFrameCounts, int& nX, int& nY );

	void	PixelRateSB_Change( int nPos );
	void	SegDShiftXSB_Change( int nPos );
	void	SegDShiftYSB_Change( int nPos );
	void	SegValleyXSB_Change( int nPos );
	void	SegValleyYSB_Change( int nPos );
	void	SegRiseXSB_Change( int nPos );
	void	SegRiseYSB_Change( int nPos );
	void	SegDRiseXSB_Change( int nPos );
	void	SegDRiseYSB_Change( int nPos );
	void	SegPeakXSB_Change( int nPos );
	void	SegPeakYSB_Change( int nPos );
	void	SegFallXSB_Change( int nPos );
	void	SegFallYSB_Change( int nPos );
	void	SegDFallXSB_Change( int nPos );
	void	SegDFallYSB_Change( int nPos );
	void	SpotXSB_Change( int nPos );
	void	SpotYSB_Change( int nPos );
	void	UpdateSpotX( int nPos, BOOL bUpdate = TRUE );
	void	UpdateSpotY( int nPos, BOOL bUpdate = TRUE );
	int		UpdateOffsetX();
	int		UpdateOffsetY();
	void	UpdateLineRate();
	void	UpdateFrameRate();

	void	PixClkChanged();

	void	DetGainSB_Change( int nPos );
	void	DetGain2SB_Change( int nPos );
	void	BWselSB_Change( int nPos );
	void	MCI_SyncParams();
	void	MCI_UpdateCfg( int nIndex );

	unsigned short	m_nusbPixRate;
	DebugOSC	m_debugOSC;

	BOOL	m_bTrialParam1;
	BOOL	m_bTrialParam2;
	BOOL	m_bTrialParam3;
	void	SetupDebugParam( BOOL bDebugHV, BOOL bDebugVac, BOOL bDebugStage );
	void	ShowGenerate();

	/////////////////////////////////////////////
	// 2020.10 ¨º¦Ì?¨¦
	void	SPRDACsValUpdae( int nPos );
	void	SPRDACupdate();
	int		m_nSPRDACnewVal;
	/////////////////////////////////////////////

// Dialog Data
	//{{AFX_DATA(CDlgImpPanelDebugR)
	enum { IDD = IDD_IMP_PANEL_DEBUGR };
	CScrollBar	m_scrollSpotY;
	CScrollBar	m_scrollSpotX;
	CMacCheckBox	m_btnMainsLock;
	CScrollBar	m_scrollBWsel;
	CScrollBar	m_scrollDetGain;
	CScrollBar	m_scrollDetGain2;
	CScrollBar	m_scrollPixelRate;
	CScrollBar	m_scrollSegDShiftX;
	CScrollBar	m_scrollSegDShiftY;
	CScrollBar	m_scrollSegValleyX;
	CScrollBar	m_scrollSegValleyY;
	CScrollBar	m_scrollSegRiseX;
	CScrollBar	m_scrollSegRiseY;
	CScrollBar	m_scrollSegDRiseX;
	CScrollBar	m_scrollSegDRiseY;
	CScrollBar	m_scrollSegPeakX;
	CScrollBar	m_scrollSegPeakY;
	CScrollBar	m_scrollSegFallX;
	CScrollBar	m_scrollSegFallY;
	CScrollBar	m_scrollSegDFallX;
	CScrollBar	m_scrollSegDFallY;
	int		m_nReso;
	int		m_nPixClk;
	CScrollBar	m_scrollSPRDACsVal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImpPanelDebugR)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImpPanelDebugR)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSetPixelRate();
	afx_msg void OnScanStart();
	afx_msg void OnScanStop();
	afx_msg void OnUpdateX();
	afx_msg void OnUpdateY();
	afx_msg void OnCHECKMainsLock();
	afx_msg void OnUpdateRamp();
	afx_msg void OnSpotSetX();
	afx_msg void OnSpotSetY();
	afx_msg void OnCHECKDet2nd();
	afx_msg void OnCHECKDebugScan();
	afx_msg void OnCHECKDebugZ();
	afx_msg void OnCHECKDebugOsc();
	afx_msg void OnCHECKDebugModeOsc90();
	afx_msg void OnCHECKNeedMon();
	afx_msg void OnGenerateKey();
	afx_msg void OnSPRDACupdate();
	//}}AFX_MSG
	afx_msg void OnResolutionChanged(UINT uID);
	afx_msg void OnPixClkChanged(UINT uID);
	afx_msg void OnDetPairSelect(UINT uID);
	afx_msg void OnDeltaposSPINVideoGraph(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSPINVideoGraphOffset(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMPPANELDEBUGR_H__4C4BA726_7E0C_4233_B30E_15A1177A7DCD__INCLUDED_)
