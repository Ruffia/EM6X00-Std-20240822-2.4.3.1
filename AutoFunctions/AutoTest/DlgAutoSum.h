#if !defined(AFX_DLGAUTOSUM_H__55164AD8_6320_4C4E_A46D_D5C1F73B4005__INCLUDED_)
#define AFX_DLGAUTOSUM_H__55164AD8_6320_4C4E_A46D_D5C1F73B4005__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAutoSum.h : header file
//

#include "DlgAutoTestMain.h"
#include "DlgCaliMain.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoSum dialog

class CDlgAutoSum : public CDialog
{
// Construction
public:
	CDlgAutoSum(CWnd* pParent = NULL);   // standard constructor
	void	Bind( void* pSemCtrl );

	CSemCtrl*			m_pSemCtrl;
	CDlgAutoTestMain	m_dlgAutotest;
	CDlgCaliMain		m_dlgCali;

// Dialog Data
	//{{AFX_DATA(CDlgAutoSum)
	enum { IDD = IDD_Auto_Sum };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAutoSum)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAutoSum)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnOpenAutoTest();
	afx_msg void OnOpenAutoCalibration();
	afx_msg void OnOpenDiagLogSetup();
	afx_msg void OnStartDiagnose();
	afx_msg void OnStartLogging();
	afx_msg void OnScanStart();
	afx_msg void OnScanStop();
	afx_msg void OnHVto30KV();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTOSUM_H__55164AD8_6320_4C4E_A46D_D5C1F73B4005__INCLUDED_)
