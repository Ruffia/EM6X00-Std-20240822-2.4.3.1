// Options_BSD.h : main header file for the OPTIONS_BSD DLL
//

#if !defined(AFX_OPTIONS_BSD_H__C18C595F_9194_49D0_830B_9533C9CA77E7__INCLUDED_)
#define AFX_OPTIONS_BSD_H__C18C595F_9194_49D0_830B_9533C9CA77E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "PanelBSD.h"

/////////////////////////////////////////////////////////////////////////////
// COptions_BSDApp
// See Options_BSD.cpp for the implementation of this class
//

class COptions_BSDApp : public CWinApp
{
public:
	COptions_BSDApp();

	CPanelBSD*	m_pPanelBSD;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptions_BSDApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(COptions_BSDApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
extern COptions_BSDApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONS_BSD_H__C18C595F_9194_49D0_830B_9533C9CA77E7__INCLUDED_)
