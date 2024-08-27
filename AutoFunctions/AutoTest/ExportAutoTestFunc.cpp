//---------------------------
// ExportAutoTestFunc.cpp
// 定义输出的函数
//---------------------------

/////////////////////////////////////////////////
// 功能：DLL的输出函数
//		当其它应用程序加载该DLL后，调用这个函数
//		可以显示该DLL内建的对话框
// 输入参数1：HWND hMainWnd，对话框父窗口的句柄
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