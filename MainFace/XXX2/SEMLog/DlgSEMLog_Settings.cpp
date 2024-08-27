#include "stdafx.h"
#include "xxx2.h"
#include "DlgSEMLog_Settings.h"
#include "DlgSEMLog_GridHeaderSettings.h"
#include "DlgSEMLog_OperationQueryKeyWordSettings.h"


IMPLEMENT_DYNAMIC(CDlgSEMLog_Settings, CDlgTab)
CDlgSEMLog_Settings::CDlgSEMLog_Settings(CWnd* pParent /*=NULL*/)
: CDlgTab( pParent)
{
}

BEGIN_MESSAGE_MAP(CDlgSEMLog_Settings, CDlgTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDlgTab::OnTcnSelchangeTabLog)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//void CDlgSEMLog_Settings::Localize(void)
//{
//	if( m_hWnd == NULL )
//		return;
//
//	GetDlgItem(IDD_DLG_Tab)->SetWindowText(GetResString(IDC_BTN_AutoRefresh));
//}

void CDlgSEMLog_Settings::InitPage() 
{
//	SetWindowText(_T("日志配置"));
	SetWindowText(GetResString(IDS_SEMLog_Settings));

	//缩放Tab对话框的大小
	CRect rc;
	GetClientRect(rc);
	MoveWindow(rc.left,rc.top,rc.Width() - 530,rc.Height());

	//缩放Tab控件的大小
	GetClientRect(rc);
	m_Tab.MoveWindow(rc.left + 10,rc.top,rc.Width() - 20,rc.Height() - 10);
	
	m_Tab.InsertItem(0,GetResString(IDS_SEMLog_UserOperationQuery));
	m_Tab.InsertItem(1,GetResString(IDS_SEMLog_DataDiaplay));
//	m_Tab.InsertItem(0,"用户操作查询关键词配置");
//	m_Tab.InsertItem(1,"数据显示列标题配置");
	//m_Tab.InsertItem(2,"用户操作记录");

	//设定在Tab内显示的范围
	CRect rcTab;
	m_Tab.GetClientRect(rcTab);

	CRect rcHold;
	rcHold.top = rcTab.top + 20;
	rcHold.left = rcTab.left;
	rcHold.right = rcTab.right;
	rcHold.bottom = rcTab.bottom;

	CDlgSEMLog_OperationQueryKeyWordSettings* pDlgOperationKeyWord = new CDlgSEMLog_OperationQueryKeyWordSettings;
	pDlgOperationKeyWord->Create(CDlgSEMLog_OperationQueryKeyWordSettings::IDD, &m_Tab);
	pDlgOperationKeyWord->SetParent(&m_Tab);
	pDlgOperationKeyWord->MoveWindow(&rcHold);
	m_vPage.push_back(pDlgOperationKeyWord);

	CDlgSEMLog_GridHeaderSettings* pDlgGridHeader = new CDlgSEMLog_GridHeaderSettings;
	pDlgGridHeader->Create(CDlgSEMLog_GridHeaderSettings::IDD, &m_Tab);
	pDlgGridHeader->SetParent(&m_Tab);
	pDlgGridHeader->MoveWindow(&rcHold);
	m_vPage.push_back(pDlgGridHeader);

	//显示初始页面
	ShowPage(0);

	//保存当前选择  
	m_CurSelTab = 0;
}


void CDlgSEMLog_Settings::OnSize(UINT nType, int cx, int cy)
{
	CDlgTab::OnSize(nType,cx,cy);
}