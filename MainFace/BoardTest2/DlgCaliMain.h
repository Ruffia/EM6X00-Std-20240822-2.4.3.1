#if !defined(AFX_DLGCALIMAIN_H__B3E72994_33A3_4BDE_B0F9_3F25C20EE1BE__INCLUDED_)
#define AFX_DLGCALIMAIN_H__B3E72994_33A3_4BDE_B0F9_3F25C20EE1BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCaliMain.h : header file
//

#include "MacButtons.h"

#include "DlgAutoTestTab.h"
#include "DlgCaliLNS.h"
#include "DlgCaliSCN.h"
#include "DlgCaliMCI.h"
#include "DlgCaliSPR.h"
#include "DlgCaliCoadjust.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliMain dialog

class CDlgCaliMain : public CDialog
{
// Construction
public:
	CDlgCaliMain(CWnd* pParent = NULL);   // standard constructor
	~CDlgCaliMain();
	
	void	InitParams();
	void	UpdateCaliData( int nIndex );

// Dialog Data
	//{{AFX_DATA(CDlgCaliMain)
	enum { IDD = IDD_Cali_Main };
	CStatic	m_staticTabPos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCaliMain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCaliMain)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL InitTabDialog();

	BOOL AddPagesToTabDialog();

private:
	CDlgAutoTestTab*	m_pTabDialog;
	CMacRadioButton*	m_pBtnLns;
	CMacRadioButton*	m_pBtnScn;
	CMacRadioButton*	m_pBtnMci;
	CMacRadioButton*	m_pBtnSpr;
	CMacRadioButton*	m_pBtnCo;
	CDlgCaliLNS*		m_pPageLNS;
	CDlgCaliSCN*		m_pPageSCN;
	CDlgCaliMCI*		m_pPageMCI;
	CDlgCaliSPR*		m_pPageSPR;
	CDlgCaliCoadjust*	m_pPageCo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCALIMAIN_H__B3E72994_33A3_4BDE_B0F9_3F25C20EE1BE__INCLUDED_)
