// AutoBC.h : main header file for the AUTOBC DLL
//

#if !defined(AFX_AUTOBC_H__94C47646_A832_444C_808D_D2BC97283638__INCLUDED_)
#define AFX_AUTOBC_H__94C47646_A832_444C_808D_D2BC97283638__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAutoBCApp
// See AutoBC.cpp for the implementation of this class
//

class CAutoBCApp : public CWinApp
{
public:
	CAutoBCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoBCApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAutoBCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOBC_H__94C47646_A832_444C_808D_D2BC97283638__INCLUDED_)
