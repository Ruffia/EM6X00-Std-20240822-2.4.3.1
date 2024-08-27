#if !defined(AFX_SEMCORECTRLPANEL_STAND4_H__E2AFAB31_803C_41B1_82DB_86DB5819BEBE__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND4_H__E2AFAB31_803C_41B1_82DB_86DB5819BEBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand4.h : main header file for SEMCORECTRLPANEL_STAND4.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "..\..\Language\LanguagePackage\resource.h"	// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand4App : See SemCoreCtrlPanel_Stand4.cpp for implementation.

class CSemCoreCtrlPanel_Stand4App : public COleControlModule
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

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND4_H__E2AFAB31_803C_41B1_82DB_86DB5819BEBE__INCLUDED)
