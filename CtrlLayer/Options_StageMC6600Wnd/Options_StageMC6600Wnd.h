// Options_StageMC6600Wnd.h : main header file for the OPTIONS_STAGEMC6600WND DLL
//

#if !defined(AFX_OPTIONS_STAGEMC6600WND_H__94BC0DE4_64B7_45E7_B2A6_5B9F62FF3AB4__INCLUDED_)
#define AFX_OPTIONS_STAGEMC6600WND_H__94BC0DE4_64B7_45E7_B2A6_5B9F62FF3AB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "PanelStage.h"

/////////////////////////////////////////////////////////////////////////////
// COptions_StageMC6600WndApp
// See Options_StageMC6600Wnd.cpp for the implementation of this class
//

class COptions_StageMC6600WndApp : public CWinApp
{
public:
	COptions_StageMC6600WndApp();

	CPanelStage*	m_pPanelStage;
	CCommMC6600		m_StageMC6600;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptions_StageMC6600WndApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(COptions_StageMC6600WndApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
extern COptions_StageMC6600WndApp theApp;


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONS_STAGEMC6600WND_H__94BC0DE4_64B7_45E7_B2A6_5B9F62FF3AB4__INCLUDED_)
