// DlgSEMLog.cpp : 实现文件
//
#include "stdafx.h"
#include "xxx2.h"
#include "DlgSEMLog_DataCurve.h"
#include "DlgSEMLog_ShowHistoryTrend.h"
#include "DlgSEMLog_ShowRealTimeTrend.h"

// CDlgSEMLog 对话框

IMPLEMENT_DYNAMIC(CDlgSEMLog_DataCurve, CDlgTab)

CDlgSEMLog_DataCurve::CDlgSEMLog_DataCurve(CWnd* pParent /*=NULL*/)
	: CDlgTab( pParent)
{
}

BEGIN_MESSAGE_MAP(CDlgSEMLog_DataCurve, CDlgTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDlgTab::OnTcnSelchangeTabLog)
END_MESSAGE_MAP()


void CDlgSEMLog_DataCurve::InitPage() 
{
	//重新设置对话框标题
	SetWindowText(GetResString(IDS_SEMLog_Chart));
	m_Tab.InsertItem(0,GetResString(IDS_SEMLog_RealTimeChart));
	m_Tab.InsertItem(1,GetResString(IDS_SEMLog_HistoricalChart));
	//设定在Tab内显示的范围
	CRect rcTab;
	m_Tab.GetClientRect(rcTab);

	CRect rcHold;
	rcHold.top = rcTab.top + 20;
	rcHold.left = rcTab.left;
	rcHold.right = rcTab.right;
	rcHold.bottom = rcTab.bottom;

	//实时数据
	CDlgSEMLog_ShowRealTimeTrend* pDlgLogRealTimeData = new CDlgSEMLog_ShowRealTimeTrend;
	pDlgLogRealTimeData->Create(CDlgSEMLog_ShowRealTimeTrend::IDD, &m_Tab);
	pDlgLogRealTimeData->SetParent(&m_Tab);
	rcHold.bottom = rcTab.bottom + 130;
	pDlgLogRealTimeData->MoveWindow(&rcHold);
	m_vPage.push_back(pDlgLogRealTimeData);

	//历史数据
	CDlgSEMLog_ShowHistoryTrend* pDlgLogHistoryData = new CDlgSEMLog_ShowHistoryTrend;
	pDlgLogHistoryData->Create(CDlgSEMLog_ShowHistoryTrend::IDD, &m_Tab);
	pDlgLogHistoryData->SetParent(&m_Tab);
	rcHold.bottom = rcTab.bottom + 130;
	pDlgLogHistoryData->MoveWindow(&rcHold);
	m_vPage.push_back(pDlgLogHistoryData);

	ShowPage(0);

	//保存当前选择  
	m_CurSelTab = 0;
}