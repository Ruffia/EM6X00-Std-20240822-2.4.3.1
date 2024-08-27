#if !defined(AFX_SEMCORECTRLPANEL_STAND8_H__48DE3004_9E64_4DA6_BA95_02A8F14D1232__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND8_H__48DE3004_9E64_4DA6_BA95_02A8F14D1232__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand8.h : main header file for SEMCORECTRLPANEL_STAND8.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "..\..\Language\LanguagePackage\resource.h"	// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand8App : See SemCoreCtrlPanel_Stand8.cpp for implementation.

class CSemCoreCtrlPanel_Stand8App : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

const CString g_csRegistryKey		= _T("KYKY");
const CString g_csSettingSection	= _T("Settings");

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND8_H__48DE3004_9E64_4DA6_BA95_02A8F14D1232__INCLUDED)
