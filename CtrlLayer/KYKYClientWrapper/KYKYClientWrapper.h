// KYKYClientWrapper.h : main header file for the KYKYCLIENTWRAPPER DLL
//

#if !defined(AFX_KYKYCLIENTWRAPPER_H__09CB9028_AEA8_48FC_A2CD_7031D34D88DE__INCLUDED_)
#define AFX_KYKYCLIENTWRAPPER_H__09CB9028_AEA8_48FC_A2CD_7031D34D88DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "SEMCommClient.h"

/////////////////////////////////////////////////////////////////////////////
// CKYKYClientWrapperApp
// See KYKYClientWrapper.cpp for the implementation of this class
//

CSEMCommClient SEMCommClient;

class CKYKYClientWrapperApp : public CWinApp
{
public:
	CKYKYClientWrapperApp();

	/////////////////////////////////////////////
	// 初始化
	void	KYKYSEM_Init(HWND hWnd);
	// 与客户端进行连接
	BOOL	KYKYSEM_Connect(BYTE IP1, BYTE IP2, BYTE IP3, BYTE IP4);
	// 断开连接，根据bActive的状态有主动断开和被动断开
	void	KYKYSEM_Disconnect(BOOL bActive);
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// 获取参数
	BOOL	KYKYSEM_QueryMagnification();
	BOOL	KYKYSEM_QueryWorkDistance();
	BOOL	KYKYSEM_QueryScanRotAngle();
	BOOL	KYKYSEM_QueryHighVoltage();
	BOOL	KYKYSEM_QueryEMI();
	/////////////////////////////////////////////

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKYKYClientWrapperApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CKYKYClientWrapperApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KYKYCLIENTWRAPPER_H__09CB9028_AEA8_48FC_A2CD_7031D34D88DE__INCLUDED_)
