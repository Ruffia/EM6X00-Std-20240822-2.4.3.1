// AutoStigmator.h : main header file for the AUTOSTIGMATOR DLL
//

#if !defined(AFX_AUTOSTIGMATOR_H__1352DEB2_47B3_4987_8404_AD27681AB8F2__INCLUDED_)
#define AFX_AUTOSTIGMATOR_H__1352DEB2_47B3_4987_8404_AD27681AB8F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAutoStigmatorApp
// See AutoStigmator.cpp for the implementation of this class
//

class CAutoStigmatorApp : public CWinApp
{
public:
	CAutoStigmatorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoStigmatorApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAutoStigmatorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOSTIGMATOR_H__1352DEB2_47B3_4987_8404_AD27681AB8F2__INCLUDED_)
