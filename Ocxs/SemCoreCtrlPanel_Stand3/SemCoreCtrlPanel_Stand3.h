#if !defined(AFX_SEMCORECTRLPANEL_STAND3_H__022E79CA_6A17_471A_85C7_E92BE447315E__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND3_H__022E79CA_6A17_471A_85C7_E92BE447315E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand3.h : main header file for SEMCORECTRLPANEL_STAND3.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "..\..\Language\LanguagePackage\resource.h"	// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand3App : See SemCoreCtrlPanel_Stand3.cpp for implementation.

class CSemCoreCtrlPanel_Stand3App : public COleControlModule
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

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND3_H__022E79CA_6A17_471A_85C7_E92BE447315E__INCLUDED)
