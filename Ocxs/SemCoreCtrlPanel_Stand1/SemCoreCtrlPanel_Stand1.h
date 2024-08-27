#if !defined(AFX_SEMCORECTRLPANEL_STAND1_H__D9CB9466_4D49_4CEF_9810_E01DC11CE17D__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND1_H__D9CB9466_4D49_4CEF_9810_E01DC11CE17D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand1.h : main header file for SEMCORECTRLPANEL_STAND1.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "..\..\Language\LanguagePackage\resource.h"	// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand1App : See SemCoreCtrlPanel_Stand1.cpp for implementation.

class CSemCoreCtrlPanel_Stand1App : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND1_H__D9CB9466_4D49_4CEF_9810_E01DC11CE17D__INCLUDED)
