#if !defined(AFX_DLGDISPLAY_H__72930B25_8FFD_4CDC_9C5C_5E9F5955E0A6__INCLUDED_)
#define AFX_DLGDISPLAY_H__72930B25_8FFD_4CDC_9C5C_5E9F5955E0A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDisplay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDisplay dialog

class CDlgDisplay : public CDialog
{
// Construction
public:
	CDlgDisplay(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDisplay)
	enum { IDD = IDD_DISPLAY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDisplay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDisplay)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDISPLAY_H__72930B25_8FFD_4CDC_9C5C_5E9F5955E0A6__INCLUDED_)
