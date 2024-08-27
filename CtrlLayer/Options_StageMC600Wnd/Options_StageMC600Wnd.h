// Options_StageMC600Wnd.h : main header file for the OPTIONS_STAGEMC600WND DLL
//

#if !defined(AFX_OPTIONS_STAGEMC600WND_H__85325FD0_2D15_47FA_B915_04D1FF9A3181__INCLUDED_)
#define AFX_OPTIONS_STAGEMC600WND_H__85325FD0_2D15_47FA_B915_04D1FF9A3181__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "PanelStage.h"

/////////////////////////////////////////////////////////////////////////////
// COptions_StageMC600WndApp
// See Options_StageMC600Wnd.cpp for the implementation of this class
//

class COptions_StageMC600WndApp : public CWinApp
{
public:
	COptions_StageMC600WndApp();

	CPanelStage*	m_pPanelStage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptions_StageMC600WndApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(COptions_StageMC600WndApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
extern COptions_StageMC600WndApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONS_STAGEMC600WND_H__85325FD0_2D15_47FA_B915_04D1FF9A3181__INCLUDED_)
