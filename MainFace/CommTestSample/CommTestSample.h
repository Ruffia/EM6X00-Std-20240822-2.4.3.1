// CommTestSample.h : main header file for the COMMTESTSAMPLE application
//

#if !defined(AFX_COMMTESTSAMPLE_H__974E2829_C56B_4F67_B74B_A178692AF66C__INCLUDED_)
#define AFX_COMMTESTSAMPLE_H__974E2829_C56B_4F67_B74B_A178692AF66C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCommTestSampleApp:
// See CommTestSample.cpp for the implementation of this class
//

class CCommTestSampleApp : public CWinApp
{
public:
	CCommTestSampleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommTestSampleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCommTestSampleApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMTESTSAMPLE_H__974E2829_C56B_4F67_B74B_A178692AF66C__INCLUDED_)
