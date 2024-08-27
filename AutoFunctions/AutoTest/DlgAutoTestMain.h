#if !defined(AFX_DLGAUTOTESTMAIN_H__548C5D55_0247_4B98_89B7_A38BFDC6EED4__INCLUDED_)
#define AFX_DLGAUTOTESTMAIN_H__548C5D55_0247_4B98_89B7_A38BFDC6EED4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAutoTestMain.h : header file
//

#include "MacButtons.h"

#include "DlgAutoTestTab.h"
#include "DlgAutoTestPower.h"
#include "DlgAutoTestVAC.h"
#include "DlgAutoTestLNS.h"
#include "DlgAutoTestSCN.h"
#include "DlgAutoTestMCI.h"
#include "DlgAutoTestSPR.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestMain dialog

class CDlgAutoTestMain : public CDialog
{
// Construction
public:
	CDlgAutoTestMain(CWnd* pParent = NULL);   // standard constructor
	~CDlgAutoTestMain();

	void InitParams();

// Dialog Data
	//{{AFX_DATA(CDlgAutoTestMain)
	enum { IDD = IDD_AutoTest_Main };
	CStatic	m_staticTabPos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAutoTestMain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAutoTestMain)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
private:
	BOOL InitTabDialog();
	
	BOOL AddPagesToTabDialog();
	
private:
	CDlgAutoTestTab*	m_pTabDialog;
	CMacRadioButton*	m_pBtnPower;
	CMacRadioButton*	m_pBtnVac;
	CMacRadioButton*	m_pBtnLns;
	CMacRadioButton*	m_pBtnScn;
	CMacRadioButton*	m_pBtnMci;
	CMacRadioButton*	m_pBtnSpr;
	CDlgAutoTestPower*	m_pPagePower;
	CDlgAutoTestVAC*	m_pPageVAC;
	CDlgAutoTestLNS*	m_pPageLNS;
	CDlgAutoTestSCN*	m_pPageSCN;
	CDlgAutoTestMCI*	m_pPageMCI;
	CDlgAutoTestSPR*	m_pPageSPR;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTOTESTMAIN_H__548C5D55_0247_4B98_89B7_A38BFDC6EED4__INCLUDED_)
