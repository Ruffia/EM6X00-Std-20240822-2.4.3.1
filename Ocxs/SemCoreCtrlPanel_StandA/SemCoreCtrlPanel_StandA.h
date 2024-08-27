#if !defined(AFX_SEMCORECTRLPANEL_STANDA_H__336388A3_28F6_4322_8A11_B7DE5C8C4A44__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STANDA_H__336388A3_28F6_4322_8A11_B7DE5C8C4A44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_StandA.h : main header file for SEMCORECTRLPANEL_STANDA.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "..\..\Language\LanguagePackage\resource.h"	// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_StandAApp : See SemCoreCtrlPanel_StandA.cpp for implementation.

class CSemCoreCtrlPanel_StandAApp : public COleControlModule
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

#endif // !defined(AFX_SEMCORECTRLPANEL_STANDA_H__336388A3_28F6_4322_8A11_B7DE5C8C4A44__INCLUDED)
