#if !defined(AFX_DLGDEBUGMON2_H__8BE6EEC0_E15F_4610_A31D_B5F36E79BF91__INCLUDED_)
#define AFX_DLGDEBUGMON2_H__8BE6EEC0_E15F_4610_A31D_B5F36E79BF91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDebugMon2.h : header file
//

#include "TransparentImage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgDebugMon2 dialog

class CDlgDebugMon2 : public CDialog
{
	CBitmap		m_bmpON, m_bmpOFF, m_bmpERR;
	CBitmap		m_bmpPneumaticDiagram;

// Construction
public:
	CDlgDebugMon2(CWnd* pParent = NULL);   // standard constructor
	~CDlgDebugMon2();
	void	Localize(void);

	void	AdjustCtrlsPos();
	void	AdjustCtrlsPos_ValveLamp( int nID, int nLeftPD, int nTopPD, int nWidthPD, int nHeightPD );
	CRect	GetControlRect( int nID );

	BOOL	VAC_UpdatePneumaticDiagram();
	void	VAC_UpdatePneumaticDiagram_Old();
	void	VAC_UpdatePneumaticDiagram_New();
	void	VAC_UpdatePneumaticDiagram_PLC();

	void	VAC_UpdateSMvacTCAmpUpdate( int nType, double dValue );
	void	VAC_UpdateTCAmpUpdate( int nType, double dValue );

	void	VAC_ShowBrdCtrls();
	void	VAC_ShowPLCvacBrdCtrls();

	void	HV_UpdateStatusSwitches();
	void	HV_UpdateStatusRegister();

	void	UpdateStatus_BoardStatus();
	void	UpdateStatus_BoardTCs(WPARAM wParam, LPARAM lParam);

	void	PLC_UpdateStatusVac();
	void	PLC_UpdateStatusValve();
//	void	PLC_UpdateThresholds();
	void	PLC_UpdateStatus( BYTE bUpdate );

// Dialog Data
	//{{AFX_DATA(CDlgDebugMon2)
	enum { IDD = IDD_Debug_Mon2 };
	CTransparentImage	m_staticPneumaticDiagram;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDebugMon2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDebugMon2)
	virtual BOOL OnInitDialog();
	afx_msg void OnReLinkCCG();
	afx_msg void OnReLinkHVpower();
	afx_msg void OnReset771();
	//}}AFX_MSG
	afx_msg LRESULT OnSMvacChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBoardVACUpdateStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSpellmanStatusChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSpellmanParamChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSpellmanParam2Changed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSpellmanParam3Changed(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEBUGMON2_H__8BE6EEC0_E15F_4610_A31D_B5F36E79BF91__INCLUDED_)
