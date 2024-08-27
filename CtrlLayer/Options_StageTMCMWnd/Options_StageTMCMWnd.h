// Options_StageTMCMWnd.h : main header file for the OPTIONS_STAGETMCMWND DLL
//

#if !defined(AFX_OPTIONS_STAGETMCMWND_H__59BFCB1D_501C_460E_88A5_8AF38CB337CC__INCLUDED_)
#define AFX_OPTIONS_STAGETMCMWND_H__59BFCB1D_501C_460E_88A5_8AF38CB337CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "PanelStage.h"

/////////////////////////////////////////////////////////////////////////////
// COptions_StageTMCMWndApp
// See Options_StageTMCMWnd.cpp for the implementation of this class
//

class COptions_StageTMCMWndApp : public CWinApp
{
public:
	COptions_StageTMCMWndApp();
	CPanelStage*	m_pPanelStage;
	CCommTMCM		m_StageTMCM;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptions_StageTMCMWndApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(COptions_StageTMCMWndApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern COptions_StageTMCMWndApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONS_STAGETMCMWND_H__59BFCB1D_501C_460E_88A5_8AF38CB337CC__INCLUDED_)
