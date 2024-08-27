#if !defined(AFX_DLGDEBUGMAIN_H__FF2FCF3C_1179_4EE7_968F_23D80BAE58E8__INCLUDED_)
#define AFX_DLGDEBUGMAIN_H__FF2FCF3C_1179_4EE7_968F_23D80BAE58E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDebugMain.h : header file
//

#include "MacButtons.h"

#include "DlgDebugTab.h"
#include "DlgDebugMon.h"
#include "DlgDebugMon2.h"
#include "DlgDebugMon3.h"
#include "DlgDebugAdvance.h"
//#include "SemCtrlDlgDebug.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgDebugMain dialog

class CDlgDebugMain : public CDialog
{
// Construction
public:
	CDlgDebugMain(CWnd* pParent = NULL);   // standard constructor
	~CDlgDebugMain();
	void	Localize(void);

// Dialog Data
	//{{AFX_DATA(CDlgDebugMain)
	enum { IDD = IDD_Debug_Main };
	CStatic	m_staticTabPos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDebugMain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDebugMain)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL	InitTabDialog();

	BOOL	AddPagesToTabDialog();

private:
	CDlgDebugTab*		m_pTabDialog;
	CMacRadioButton*	m_pBtnMon;
	CMacRadioButton*	m_pBtnAdvance;
//	CMacRadioButton*	m_pBtnDebug;
public:
	CDlgDebugMon*		m_pPageMon;
	CDlgDebugMon2*		m_pPageMon2;
	CDlgDebugMon3*		m_pPageMon3;
	CDlgDebugAdvance*	m_pPageAdvance;
//	CSemCtrlDlgDebug*	m_pPageDebug;

	CFont				m_Font;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEBUGMAIN_H__FF2FCF3C_1179_4EE7_968F_23D80BAE58E8__INCLUDED_)
