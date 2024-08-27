// DllTest.h : main header file for the DLLTEST application
//

#if !defined(AFX_DLLTEST_H__4AADE952_635A_423C_A83D_69DE4A9E703A__INCLUDED_)
#define AFX_DLLTEST_H__4AADE952_635A_423C_A83D_69DE4A9E703A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "SemCtrl.h"
#include "SemCtrlPanelManager.h"
#include "SemVirtualMCICard.h"
#include <afxmt.h>
#include <USB_Functions.h>

#include "ExportXYStageFunc.h"
//#pragma comment(lib, "..\\..\\Bin\\Options_XYStage.lib")

/////////////////////////////////////////////////////////////////////////////
// CDllTestApp:
// See DllTest.cpp for the implementation of this class
//

class CDllTestApp : public CWinApp
{
public:
	CDllTestApp();

	CSemCtrl				m_SemCtrl;
	CSemVirtualMCICard		m_ImageCard;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDllTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDllTestApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CDllTestApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLTEST_H__4AADE952_635A_423C_A83D_69DE4A9E703A__INCLUDED_)
