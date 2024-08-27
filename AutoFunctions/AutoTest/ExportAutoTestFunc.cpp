//---------------------------
// ExportAutoTestFunc.cpp
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
#include "AutoTest.h"

#include "DlgAutoSum.h"

void ShowAutoTestDlg(HWND hMainWnd, void* pSemCtrl)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDlgAutoSum *pAutoSumDlg = new CDlgAutoSum;
	CWnd* pMainWnd = CWnd::FromHandle(hMainWnd);
	ASSERT(pMainWnd);
	BOOL ret = pAutoSumDlg->Create(IDD_Auto_Sum, pMainWnd);
	if( !ret )
	{
		AfxMessageBox("Create dialog in DLL failed");
	}
	else
	{
		pAutoSumDlg->Bind(pSemCtrl);
		pAutoSumDlg->ShowWindow(SW_SHOW);
	}
}