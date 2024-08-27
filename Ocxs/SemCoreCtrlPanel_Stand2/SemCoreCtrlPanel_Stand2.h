#if !defined(AFX_SEMCORECTRLPANEL_STAND2_H__0ED10FA1_F5FA_4192_AC08_7E892E185EA2__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND2_H__0ED10FA1_F5FA_4192_AC08_7E892E185EA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand2.h : main header file for SEMCORECTRLPANEL_STAND2.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "..\..\Language\LanguagePackage\resource.h"	// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand2App : See SemCoreCtrlPanel_Stand2.cpp for implementation.

class CSemCoreCtrlPanel_Stand2App : public COleControlModule
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

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND2_H__0ED10FA1_F5FA_4192_AC08_7E892E185EA2__INCLUDED)
