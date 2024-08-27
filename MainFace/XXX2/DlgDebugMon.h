#if !defined(AFX_DLGDEBUGMON_H__F7AFEF7F_2A29_4BAA_AA9D_E8D824AA9E27__INCLUDED_)
#define AFX_DLGDEBUGMON_H__F7AFEF7F_2A29_4BAA_AA9D_E8D824AA9E27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDebugMon.h : header file
//

#include "TransparentImage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgDebugMon dialog

class CDlgDebugMon : public CDialog
{
	CBitmap		m_bmpON;
	CBitmap		m_bmpOFF;
	HICON		m_hIconON;
	HICON		m_hIconOFF;
	CBrush		m_brushVacNotGood;
	CBrush		m_brushVacGood;
	CBitmap		m_bmpPneumaticDiagram;

// Construction
public:
	CDlgDebugMon(CWnd* pParent = NULL);   // standard constructor
	~CDlgDebugMon();
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

	void	dlgprintf(int id, char *fmt, ... );
	WORD	m_wVer_a, m_wVer_e, m_wVer_f;

	void	UpdateStatus_BoardStatus();
	void	UpdateStatus_BoardTCs(WPARAM wParam, LPARAM lParam);

// Dialog Data
	//{{AFX_DATA(CDlgDebugMon)
	enum { IDD = IDD_Debug_Mon };
	CTransparentImage	m_staticPneumaticDiagram;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDebugMon)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDebugMon)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnReLinkCCG();
	afx_msg void OnReLinkHVpower();
	//}}AFX_MSG
	afx_msg LRESULT OnSMvacChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBoardVACUpdateStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCPS3603statusChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCPS3603msgChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCPS3603paramChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEBUGMON_H__F7AFEF7F_2A29_4BAA_AA9D_E8D824AA9E27__INCLUDED_)
