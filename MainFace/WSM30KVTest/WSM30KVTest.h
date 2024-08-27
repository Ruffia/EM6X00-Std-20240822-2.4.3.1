// WSM30KVTest.h : main header file for the WSM30KVTEST application
//

#if !defined(AFX_WSM30KVTEST_H__83D7F23C_7389_4112_993C_6B91C5636AA0__INCLUDED_)
#define AFX_WSM30KVTEST_H__83D7F23C_7389_4112_993C_6B91C5636AA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWSM30KVTestApp:
// See WSM30KVTest.cpp for the implementation of this class
//

class CWSM30KVTestApp : public CWinApp
{
public:
	CWSM30KVTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWSM30KVTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWSM30KVTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSM30KVTEST_H__83D7F23C_7389_4112_993C_6B91C5636AA0__INCLUDED_)
