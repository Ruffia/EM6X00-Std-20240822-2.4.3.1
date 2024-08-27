// Options_XYStage.h : main header file for the OPTIONS_XYSTAGE DLL
//

#if !defined(AFX_OPTIONS_XYSTAGE_H__BFC5AF43_547C_4AD5_AC74_C4BD6900D782__INCLUDED_)
#define AFX_OPTIONS_XYSTAGE_H__BFC5AF43_547C_4AD5_AC74_C4BD6900D782__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "PanelStage.h"

/////////////////////////////////////////////////////////////////////////////
// COptions_XYStageApp
// See Options_XYStage.cpp for the implementation of this class
//

class COptions_XYStageApp : public CWinApp
{
public:
	COptions_XYStageApp();

	CPanelStage*	m_pPanelStage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptions_XYStageApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(COptions_XYStageApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
extern COptions_XYStageApp theApp;


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONS_XYSTAGE_H__BFC5AF43_547C_4AD5_AC74_C4BD6900D782__INCLUDED_)
