//---------------------------
// ExportXYStageFunc.cpp
// ��������ĺ���
//---------------------------

/////////////////////////////////////////////////
// ���ܣ�DLL���������
//		������Ӧ�ó�����ظ�DLL�󣬵����������
//		������ʾ��DLL�ڽ��ĶԻ���
// �������1��HWND hMainWnd���Ի��򸸴��ڵľ��
/////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Options_XYStage.h"

#include "PanelStage.h"

void ShowXYStageDlg(HWND hMainWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
	{
		if( !(theApp.m_pPanelStage->m_Stage.xy.bReady) )
			theApp.m_pPanelStage->Stage_XY_Init();
		theApp.m_pPanelStage->ShowWindow(SW_SHOW);
	}
	else
	{
		if( theApp.m_pPanelStage != NULL )
		{
			delete theApp.m_pPanelStage;
			theApp.m_pPanelStage = NULL;
		}
		theApp.m_pPanelStage = new CPanelStage;
		CWnd* pMainWnd = CWnd::FromHandle(hMainWnd);
		ASSERT(pMainWnd);
		BOOL ret = theApp.m_pPanelStage->Create(IDD_PANEL_Stage, pMainWnd);
		if( !ret )
		{
			MessageBox(pMainWnd->m_hWnd, "Create dialog in DLL failed", "XYStage", MB_OK);
		}
		else
		{
			theApp.m_pPanelStage->Stage_XY_Init();
			theApp.m_pPanelStage->ShowWindow(SW_SHOW);
		}
	}
}

void Options_XYStage_FindIndex()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
		theApp.m_pPanelStage->Stage_XY_FindIndex();
}