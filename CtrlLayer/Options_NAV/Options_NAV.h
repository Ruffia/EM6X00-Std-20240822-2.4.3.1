// Options_NAV.h : Options_NAV DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "PanelNAV.h"

// COptions_NAVApp
// �йش���ʵ�ֵ���Ϣ������� Options_NAV.cpp
//

class COptions_NAVApp : public CWinApp
{
public:
	COptions_NAVApp();

	CPanelNAV*	m_pPanelNAV;

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
extern COptions_NAVApp theApp;
