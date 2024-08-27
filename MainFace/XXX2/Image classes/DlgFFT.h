#if !defined(AFX_DLGFFT_H__B3EB1107_B044_11D6_BB83_EA3D5554237A__INCLUDED_)
#define AFX_DLGFFT_H__B3EB1107_B044_11D6_BB83_EA3D5554237A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFFT.h : header file
//

#include "xTargetButton.h"

class CImageDoc;
/////////////////////////////////////////////////////////////////////////////
// CDlgFFT dialog
class CDlgFFT : public CDialog
{
// Construction
public:
	CImageDoc* pDocReal;
	CImageDoc* pDocImag;

	long bForceFFT;
	long bMagnitude;
	long bInverse;

	CDlgFFT(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFFT)
	enum { IDD = IDD_DLG_FFT };
	CButton	m_cinverse;
	CButton	m_cmagnitude;
	CButton	m_cforcefft;
	CComboBox	m_imag;
	CComboBox	m_real;
	CxTargetButton	m_canc;
	CxTargetButton	m_ok;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFFT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFFT)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFFT_H__B3EB1107_B044_11D6_BB83_EA3D5554237A__INCLUDED_)
