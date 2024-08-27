// DlgSEMLog.cpp : 实现文件
//
#include "stdafx.h"
#include "xxx2.h"
#include "DlgSEMLog_Data.h"
#include "DlgSEMLog_ShowALLData.h"
#include "DlgSEMLog_ShowOperation.h"
#include "DlgSEMLog_ShowAlarm.h"


// CDlgSEMLog 对话框

IMPLEMENT_DYNAMIC(CDlgSEMLog_Data, CDlgTab)

CDlgSEMLog_Data::CDlgSEMLog_Data(CWnd* pParent /*=NULL*/)
	: CDlgTab( pParent)
{
}

BEGIN_MESSAGE_MAP(CDlgSEMLog_Data, CDlgTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDlgTab::OnTcnSelchangeTabLog)
END_MESSAGE_MAP()


void CDlgSEMLog_Data::InitPage() 
{
	SetWindowText(GetResString(IDS_SEMLog_Show));
	m_Tab.InsertItem(0,GetResString(IDS_SEMLog_LogData ));
	m_Tab.InsertItem(1,GetResString(IDS_SEMLog_AlarmLogs));
	m_Tab.InsertItem(2,GetResString(IDS_SEMLog_UserOperationLogs));

	//设定在Tab内显示的范围
	CRect rcTab;
	m_Tab.GetClientRect(rcTab);

	CRect rcHold;
	rcHold.top = rcTab.top + 20;
	rcHold.left = rcTab.left;
	rcHold.right = rcTab.right;
	rcHold.bottom = rcTab.bottom;

	CDlgSEMLog_ShowALLData* pDlgLogDataAll = new CDlgSEMLog_ShowALLData;
	pDlgLogDataAll->Create(CDlgSEMLog_ShowALLData::IDD, &m_Tab);
	pDlgLogDataAll->SetParent(&m_Tab);
	pDlgLogDataAll->MoveWindow(&rcHold);
	m_vPage.push_back(pDlgLogDataAll);

	CDlgSEMLog_ShowAlarm* pDlgAlarm = new CDlgSEMLog_ShowAlarm;
	pDlgAlarm->Create(CDlgSEMLog_ShowAlarm::IDD, &m_Tab);
	pDlgAlarm->SetParent(&m_Tab);
	rcHold.bottom = rcTab.bottom + 130;
	pDlgAlarm->MoveWindow(&rcHold);
	m_vPage.push_back(pDlgAlarm);

	CDlgSEMLog_ShowOperation* pDlgOperation = new CDlgSEMLog_ShowOperation;
	pDlgOperation->Create(CDlgSEMLog_ShowOperation::IDD, &m_Tab);
	pDlgOperation->SetParent(&m_Tab);
	rcHold.bottom = rcTab.bottom + 130;
	pDlgOperation->MoveWindow(&rcHold);
	m_vPage.push_back(pDlgOperation);

	//显示初始页面
	ShowPage(0);

	//保存当前选择  
	m_CurSelTab = 0;
}