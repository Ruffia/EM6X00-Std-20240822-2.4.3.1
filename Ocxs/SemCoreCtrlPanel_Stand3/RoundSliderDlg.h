// RoundSliderDlg.h : header file
//
// Last Modify : 2009.03.11

#if !defined(AFX_ROUNDSLIDERDLG_H__4F5A06A8_6FC3_42D3_9929_45DA548C667E__INCLUDED_)
#define AFX_ROUNDSLIDERDLG_H__4F5A06A8_6FC3_42D3_9929_45DA548C667E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RoundSliderCtrlEx.h"

class CSemCoreCtrlPanel_Stand3Ctrl;

/////////////////////////////////////////////////////////////////////////////
// CRoundSliderDlg dialog

class CRoundSliderDlg : public CDialog
{

	BOOL							m_blReady;
	CSemCoreCtrlPanel_Stand3Ctrl*	m_pCoreCtrlPanel;
	void	ReportPos();

	long	m_lMin, m_lMax, m_lPos;
	int		m_nRoundCtrlGrade;

public:
	BOOL	m_bGradeVisible;
	BOOL	m_bDebugInfoVisible;
	CString	m_csRoundCtrl;

	BOOL	Bind( CSemCoreCtrlPanel_Stand3Ctrl* pCoreCtrlPanel );
	
	void	SetPos( long lPos );
	void	SetRange( long lMin, long lMax );
	// 09.02.17 当属性页中有设置改变时更新显示
	void	SettingChanged( BOOL bGradeChanged );
	// 09.02.17 将档位的更新单做一个函数
	void	UpdateGrade();

// Construction
public:
	CRoundSliderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRoundSliderDlg)
	enum { IDD = IDD_DIALOG_SEMCORECTRLPANEL_STAND3_DLG_RoundSlider };
	CRoundSliderCtrlEx	m_ctrlRoundSlider;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRoundSliderDlg)
	public:
	virtual void OnSetFont(CFont* pFont);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	void AdjustRoundSlider();
	
	// Generated message map functions
	//{{AFX_MSG(CRoundSliderDlg)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioCoarse();
	afx_msg void OnRadioFine();
	afx_msg void OnRadioTiny();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROUNDSLIDERDLG_H__4F5A06A8_6FC3_42D3_9929_45DA548C667E__INCLUDED_)
