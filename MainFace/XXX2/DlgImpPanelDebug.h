#if !defined(AFX_DLGIMPPANELDEBUG_H__60A9AC65_5960_4A5B_8D2C_104952088BB5__INCLUDED_)
#define AFX_DLGIMPPANELDEBUG_H__60A9AC65_5960_4A5B_8D2C_104952088BB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImpPanelDebug.h : header file
//

#include "MacButtons.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelDebug dialog

class CDlgImpPanelDebug : public CDialog
{
// Construction
public:
	CDlgImpPanelDebug(CWnd* pParent = NULL);   // standard constructor
	~CDlgImpPanelDebug();
	void	Localize(void);

	void	SetupDebugParam( BOOL bDebugVac );
	void	VAC_SyncStates( int nIndex );
	BOOL	m_bSPRRelay1En;
	void	SPR_En( BOOL bEn );

	void	PLC_UpdateStatusValve();

// Dialog Data
	//{{AFX_DATA(CDlgImpPanelDebug)
	enum { IDD = IDD_IMP_PANEL_DEBUG };
	CMacCheckBox	m_btnTestLEDs;
	CMacButton	m_btnSPRRelay1En;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImpPanelDebug)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImpPanelDebug)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnCHECKTestLEDs();
	afx_msg void OnSPRRelay1En();
	afx_msg void OnReadBoardIDs();
	afx_msg void OnBUTTONResetSubBoards();
	afx_msg void OnBUTTONResetFT232();
	afx_msg void OnBUTTONResetDI16A();
	afx_msg void OnBUTTONSetupStatus();
	afx_msg void OnCHECKDebugV1();
	afx_msg void OnCHECKDebugV2();
	afx_msg void OnCHECKDebugV3();
	afx_msg void OnCHECKDebugV4();
	afx_msg void OnCHECKDebugTMP();
	afx_msg void OnCHECKDebugMP();
	afx_msg void OnCHECKVacuumTCf();
	afx_msg void OnCHECKVacuumTCc();
	afx_msg void OnCHECKVacuumPauseRead();
	afx_msg void OnCHECKDebugCCGvalueLock();
	afx_msg void OnCHECKDebugCCGvacuum();
	afx_msg void OnCHECKOff();
	afx_msg void OnCHECKDrawCircle();
	afx_msg void OnSelchangeCCD();
	afx_msg void OnSelchangeStage();
	afx_msg void OnSelchangeEDS();
	afx_msg void OnSelchangeBSD();
	afx_msg void OnSelchangeHVType();
	afx_msg void OnSelchangeVacType();
	afx_msg void OnSelchangeGaugeType();
	afx_msg void OnSelchangeNAV();
	afx_msg void OnSelchangeBeam();
	afx_msg void OnBUTTONPLCvacBrdThresholdsSet();
	afx_msg void OnBUTTONPLCvacBrdThresholdsGet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMPPANELDEBUG_H__60A9AC65_5960_4A5B_8D2C_104952088BB5__INCLUDED_)
