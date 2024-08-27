#if !defined(AFX_PANELMCI_H__622496D3_08C7_49E1_8226_35AF56E69078__INCLUDED_)
#define AFX_PANELMCI_H__622496D3_08C7_49E1_8226_35AF56E69078__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelMCI.h : header file
//

#include "MacButtons.h"

/////////////////////////////////////////////////////////////////////////////
// CPanelMCI dialog

class CPanelMCI : public CDialog
{
// Construction
public:
	CPanelMCI(CWnd* pParent = NULL);   // standard constructor
	~CPanelMCI();

	void	Init();
	/////////////////////////////////////////////////////////////////
	void	UpdatePortState( unsigned char OutMask );

	void	ReadBackdata( int nBits = 8 );
	void	PwrVconV( BYTE bData1, BYTE bData2, CDialog* pParent );
	double	m_dPwrReadback[8];
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	void	ScanStart();
	void	ScanStop();

	CSize	GetSizes();

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

	void	Det03SB_Change( int nPos );
	void	Det45SB_Change( int nPos );
	void	DetGainSB_Change( int nPos );
	void	BWselSB_Change( int nPos );
	void	MCI_SyncParams();

	unsigned short	m_nusbPixRate;
	BOOL	m_bVideoMode;		// 0: XYZ mode;	1: Raster mode
	BOOL	m_bDetMode;			// 0: Single;	1: Duel
	BOOL	m_bDispMode;
	BOOL	m_bRotate;

	void	InitDiagnose( BOOL bEnd );
	void	SetTimers( UINT nIDEvent, BOOL bSet );

// Dialog Data
	//{{AFX_DATA(CPanelMCI)
	enum { IDD = IDD_PAGE_MCI };
	CMacCheckBox	m_btnTestLED16State;
	CScrollBar	m_scrollSpotY;
	CScrollBar	m_scrollSpotX;
	CMacCheckBox	m_btnFakeMains;
	CMacCheckBox	m_btnINTtest;
	CMacCheckBox	m_btnVideo;
	CMacCheckBox	m_btnMainsLock;
	CMacButton	m_btnRotate;
	CScrollBar	m_scrollBWsel;
	CScrollBar	m_scrollDetGain;
	CScrollBar	m_scrollDet45;
	CScrollBar	m_scrollDet03;
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
	CMacButton	m_btnDefaultRamp;
	CStatic	m_dlgPos;
	CMacButton	m_btnCancel;
	CMacButton	m_btnOK;
	CString	m_strPortA;
	CString	m_strPortB;
	CString	m_strPortC;
	CString	m_strPortD;
	CListBox	m_listReport;
	CString	m_strBWsel;
	int		m_nReso;
	int		m_nPixClk;
	UINT	m_nDet03Chn2;
	UINT	m_nDet45Chn2;
	UINT	m_nDetGainNo2;
	UINT	m_nDetPair2;
	int		m_nDispType;
	int		m_nBoardID;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelMCI)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPanelMCI)
	virtual BOOL OnInitDialog();
	afx_msg void OnReadAll();
	afx_msg void OnSetPixelRate();
	afx_msg void OnSetPixClkEq250KHz();
	afx_msg void OnSetPixClkEq1MHz();
	afx_msg void OnScanStart();
	afx_msg void OnScanStop();
	afx_msg void OnReceive();
	afx_msg void OnInHexCheck();
	afx_msg void OnUpdateX();
	afx_msg void OnUpdateY();
	afx_msg void OnUpdateDefaultRamps();
	afx_msg void OnDet03Load();
	afx_msg void OnDet45Load();
	afx_msg void OnScanModeNormal();
	afx_msg void OnScanModeLine();
	afx_msg void OnScanModeSpot();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDeltaposDet03(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposDet45(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposDetGain(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposDetPair(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnScanRotation();
	afx_msg void OnScanRotationUpdate();
	afx_msg void OnReadVersion();
	afx_msg void OnScanStartWithParams();
	afx_msg void OnINT_ClearDB();
	afx_msg void OnINT_SetIMR();
	afx_msg void OnINT_INTtest();
	afx_msg void OnCHECKMainsLock();
	afx_msg void OnBUTTONMainsLockLineSync();
	afx_msg void OnBUTTONMainsLockFrameSync();
	afx_msg void OnCHECKVideo();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnFreeze();
	afx_msg void OnResetCommunicate();
	afx_msg void OnReadINTSourceRegister();
	afx_msg void OnWriteINTEnableRegister();
	afx_msg void OnBUTTONFakeMainslock();
	afx_msg void OnUpdateRamp();
	afx_msg void OnSpotSetX();
	afx_msg void OnSpotSetY();
	afx_msg void OnSetPxlClkto2Cmd();
	afx_msg void OnCHECKDet2nd();
	afx_msg void OnCHECKDebugScan();
	afx_msg void OnCHECKDebugZ();
	afx_msg void OnCHECKDebugOsc();
	afx_msg void OnCHECKDebugModeOsc90();
	afx_msg void OnTestLED16on();
	//}}AFX_MSG
	afx_msg void OnResolutionChanged(UINT nID);
	afx_msg void OnPixClkChanged(UINT nID);
	afx_msg void OnDetPairSelect(UINT nID);
	BOOL memberFxn( UINT id, NMHDR * pTTTStruct, LRESULT * pResult );		//在线提示
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANELMCI_H__622496D3_08C7_49E1_8226_35AF56E69078__INCLUDED_)
