// DlgSEMLog.cpp : 实现文件
//
#include "stdafx.h"
#include "xxx2.h"
#include "DlgSEMLog_DeleteData.h"
#include "DBDataManager.h"


IMPLEMENT_DYNAMIC(CDlgSEMLog_DeleteData, CDialog)

CDlgSEMLog_DeleteData::CDlgSEMLog_DeleteData(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSEMLog_DeleteData::IDD, pParent)
{
}

CDlgSEMLog_DeleteData::~CDlgSEMLog_DeleteData()
{

}

void CDlgSEMLog_DeleteData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DateBegin, m_DayBegin);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TimeBegin, m_TimeBegin);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DateEnd, m_DayEnd);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TimeEnd, m_TimeEnd);
}


BEGIN_MESSAGE_MAP(CDlgSEMLog_DeleteData, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_Delete, &CDlgSEMLog_DeleteData::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


BOOL CDlgSEMLog_DeleteData::OnInitDialog()
{
	CDialog::OnInitDialog();

	CTime timeNow = CTime::GetCurrentTime();
	CTimeSpan sp(-1,0,0,0);
	CTime timeYesterday = timeNow + sp;
	m_DayBegin.SetTime(&timeYesterday);
	UpdateData(FALSE);

	return TRUE;
}


__int64 CDlgSEMLog_DeleteData::_GetTimeStamp(bool bBeginDateTime)
{
	CTime tDay, tTime;
	__int64 timestamp = 0;
	DWORD dwResult = 0;
	if (bBeginDateTime)
	{
		dwResult = m_DayBegin.GetTime(tDay);
		dwResult = m_TimeBegin.GetTime(tTime);
	}
	else
	{
		DWORD dwResult = m_DayEnd.GetTime(tDay);
		dwResult = m_TimeEnd.GetTime(tTime);
	}

	COleDateTime tDateTime(tDay.GetYear(), tDay.GetMonth(), tDay.GetDay(), 
		tTime.GetHour(), tTime.GetMinute(), tTime.GetSecond());

	SYSTEMTIME systime;
	VariantTimeToSystemTime(tDateTime, &systime);
	//时间戳最小值为北京时间:1970-01-01 08:00:00
	if (systime.wYear <= 1970 && systime.wMonth <= 1 && systime.wDay <= 1 && systime.wHour <= 7 
		&& systime.wMinute <= 59 && systime.wSecond <= 59)
		return -1;
	CTime cTimeFromDB(systime);
	timestamp = cTimeFromDB.GetTime() * pow(10,3);//CTime->时间戳
	return timestamp;
}


void CDlgSEMLog_DeleteData::OnClose()
{

	CDialog::OnClose();
}


void CDlgSEMLog_DeleteData::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	UINT nRet = MessageBox("数据删除后不可恢复，确实要进行该操作吗？","操作提醒",MB_OKCANCEL|MB_ICONERROR);
	if(IDCANCEL == nRet) return;

	__int64 tTimeStampBegin = _GetTimeStamp(true);
	__int64 tTimeStampEnd   = _GetTimeStamp(false);
	char szTimeCondition[128] = {0};
	sprintf_s(szTimeCondition,128,"Time >= %I64d and Time <= %I64d",tTimeStampBegin,tTimeStampEnd);

	char szSQL[256] = {0};
	const string& strTableName = CDBDataManager::Instance().GetLogTableName();
	sprintf_s(szSQL,256,"delete from %s where %s",strTableName.c_str(),szTimeCondition);

	CDBDataManager::Instance().Exec(szSQL);
}
