// AutoFilament.h : main header file for the AUTOFILAMENT DLL
//

#if !defined(AFX_AUTOFILAMENT_H__AC200FC6_90C1_404A_A0A5_1525ECB4AD17__INCLUDED_)
#define AFX_AUTOFILAMENT_H__AC200FC6_90C1_404A_A0A5_1525ECB4AD17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAutoFilamentApp
// See AutoFilament.cpp for the implementation of this class
//

class CAutoFilamentApp : public CWinApp
{
public:
	CAutoFilamentApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoFilamentApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAutoFilamentApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOFILAMENT_H__AC200FC6_90C1_404A_A0A5_1525ECB4AD17__INCLUDED_)
