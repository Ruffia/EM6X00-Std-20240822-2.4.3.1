#if !defined(AFX_DLGDEBUGMON3_H__487764CB_1736_423C_9CB8_F5AA32E48503__INCLUDED_)
#define AFX_DLGDEBUGMON3_H__487764CB_1736_423C_9CB8_F5AA32E48503__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDebugMon3.h : header file
//

#include "TransparentImage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgDebugMon3 dialog

class CDlgDebugMon3 : public CDialog
{
	CBitmap		m_bmpON, m_bmpOFF, m_bmpERR;
	CBitmap		m_bmpPneumaticDiagram;
	CBrush		m_brushHLcyan, m_brushHLgreen, m_brushHLred;

// Construction
public:
	CDlgDebugMon3(CWnd* pParent = NULL);   // standard constructor
	~CDlgDebugMon3();
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

	void	UpdateStatus_BoardStatus();
	void	UpdateStatus_BoardTCs(WPARAM wParam, LPARAM lParam);

	void	PLC_UpdateStatusVac();
	void	PLC_UpdateStatusValve();
//	void	PLC_UpdateThresholds();
	void	PLC_UpdateStatus( BYTE bUpdate );

	void	UpdateStatus_01( WPARAM wParam, LPARAM lParam );
	void	UpdateStatus_25( WPARAM wParam, LPARAM lParam );
	void	UpdateInquire_01( WPARAM wParam, LPARAM lParam );
	void	UpdateInquire_25( WPARAM wParam, LPARAM lParam );
	void	UpdateMon_VT( WPARAM wParam, LPARAM lParam );
	void	UpdateMon_FB( WPARAM wParam , LPARAM lParam );
	void	UpdateMon_Given( WPARAM wParam , LPARAM lParam );

	void	Display_Status_INT( CDC* pDC, HBRUSH& hbr, int nID );
	void	Display_Status_Output( CDC* pDC, HBRUSH& hbr, int nID );
	void	Display_Status_Protect01( CDC* pDC, HBRUSH& hbr, int nID );
	void	Display_Status_Protect25( CDC* pDC, HBRUSH& hbr, int nID );
	void	Display_Status_ProtectOV( CDC* pDC, HBRUSH& hbr, int nID );
	void	Display_Status_ProtectOC( CDC* pDC, HBRUSH& hbr, int nID );

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////

	void	UpdateStatus2( WPARAM wParam, LPARAM lParam );
	void	UpdateMon_VT2( WPARAM wParam, LPARAM lParam );
	void	UpdateMon_FB2( WPARAM wParam , LPARAM lParam );
	void	UpdateMon_Given2( WPARAM wParam , LPARAM lParam );

	void	Display_Status_INT2( CDC* pDC, HBRUSH& hbr, int nID );
	void	Display_Status_Output2( CDC* pDC, HBRUSH& hbr, int nID );
	void	Display_Status_Protect2( CDC* pDC, HBRUSH& hbr, int nID );
	void	Display_Status_ProtectOV2( CDC* pDC, HBRUSH& hbr, int nID );
	void	Display_Status_ProtectOC2( CDC* pDC, HBRUSH& hbr, int nID );

// Dialog Data
	//{{AFX_DATA(CDlgDebugMon3)
	enum { IDD = IDD_Debug_Mon3 };
	CTransparentImage	m_staticPneumaticDiagram;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDebugMon3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDebugMon3)
	virtual BOOL OnInitDialog();
	afx_msg void OnReLinkCCG();
	afx_msg void OnReLinkHVpower();
	afx_msg void OnResetHVPower();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg LRESULT OnSMvacChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBoardVACUpdateStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWSM30KVUpdateMon(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWSM30KVUpdateStatus(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEBUGMON3_H__487764CB_1736_423C_9CB8_F5AA32E48503__INCLUDED_)
