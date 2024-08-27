// Options_NAV.h : Options_NAV DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "PanelNAV.h"

// COptions_NAVApp
// 有关此类实现的信息，请参阅 Options_NAV.cpp
//

class COptions_NAVApp : public CWinApp
{
public:
	COptions_NAVApp();

	CPanelNAV*	m_pPanelNAV;

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
extern COptions_NAVApp theApp;
