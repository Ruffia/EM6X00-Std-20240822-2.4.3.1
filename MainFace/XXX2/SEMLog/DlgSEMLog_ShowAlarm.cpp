// DlgSEMLog.cpp : 实现文件
//
#include "stdafx.h"
#include "xxx2.h"
#include "DlgSEMLog_ShowAlarm.h"
#include "Factory.h"
#include "GridViewField.h"
#include "FieldMeta.h"
#include "DBDataManager.h"
#include <algorithm>
#include "GridCellBehaviour.h"
#include "Tools/CheckComboBox.h"
#include "FormatConvertor.h"
#include "Util.h"
#include "Message.h"


// CDlgSEMLog_ShowAlarm 对话框

IMPLEMENT_DYNAMIC(CDlgSEMLog_ShowAlarm, CDlgGridPage)

CDlgSEMLog_ShowAlarm::CDlgSEMLog_ShowAlarm(CWnd* pParent /*=NULL*/)
	: CDlgGridPage(pParent)
	, m_strPageNO(_T(""))
	, m_nCurrentPage(0)
	, m_nTotalPage(1)
	, m_nRecordCountPerPage(100)
{

}

CDlgSEMLog_ShowAlarm::~CDlgSEMLog_ShowAlarm()
{
	if (m_pMultiSelectCombo)
	{
		//m_pMultiSelectCombo->DestroyWindow();
		//delete m_pMultiSelectCombo;
	}

	m_ds.Clear();
}

void CDlgSEMLog_ShowAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Grid, m_Grid); 
	DDX_Text(pDX, IDC_STATIC_PageNO, m_strPageNO);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DateBegin, m_DayBegin);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TimeBegin, m_TimeBegin);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DateEnd, m_DayEnd);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TimeEnd, m_TimeEnd);
}


BEGIN_MESSAGE_MAP(CDlgSEMLog_ShowAlarm, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PreviousPage, &CDlgSEMLog_ShowAlarm::OnBnClickedButtonPreviouspage)
	ON_BN_CLICKED(IDC_BUTTON_NextPage, &CDlgSEMLog_ShowAlarm::OnBnClickedButtonNextpage)
	ON_BN_CLICKED(IDC_BUTTON_Query, &CDlgSEMLog_ShowAlarm::OnBnClickedButtonQuery)
	ON_MESSAGE(WM_MsgAsynLoadPageData,OnLoadPageData)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CDlgSEMLog_ShowAlarm::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDC_STATIC_TimeBegin)->SetWindowText(GetResString(IDC_STATIC_TimeBegin));
	GetDlgItem(IDC_STATIC_TimeEnd)->SetWindowText(GetResString(IDC_STATIC_TimeEnd));
	GetDlgItem(IDC_BUTTON_Query)->SetWindowText(GetResString(IDC_BUTTON_Query));
	GetDlgItem(IDC_BUTTON_PreviousPage)->SetWindowText(GetResString(IDC_BUTTON_PreviousPage));
	GetDlgItem(IDC_BUTTON_NextPage)->SetWindowText(GetResString(IDC_BUTTON_NextPage));
	
}

// CDlgSEMLog 消息处理程序
BOOL CDlgSEMLog_ShowAlarm::OnInitDialog()
{
	CDialog::OnInitDialog();

	_CreateCheckCombox();
	_InitializeGrid();

	CTime timeNow = CTime::GetCurrentTime();
	CTimeSpan sp(-1,0,0,0);
	CTime timeYesterday = timeNow + sp;
	m_DayBegin.SetTime(&timeYesterday);

	const string& strTableName = "AlarmLog";
	map<string,CFieldDesc*>& meta = CDBDataManager::Instance().GetTableMeta(strTableName);
	for (map<string,CFieldDesc*>::iterator it = meta.begin(); it != meta.end();it++)
	{
		CFieldDesc* pFieldDesc = it->second;
		if (pFieldDesc)
		{
			const string& sDisplayName = pFieldDesc->GetAttributeString("DisplayName");
			m_pMultiSelectCombo->AddString(sDisplayName.c_str());
		}
	}

    PostMessage(WM_MsgAsynLoadPageData,WPARAM(0),LPARAM(0));

	UpdateData(FALSE);

	Localize();
	return TRUE;
}

void CDlgSEMLog_ShowAlarm::OnBnClickedButtonPreviouspage()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_nCurrentPage < 1) return;
	PostMessage(WM_MsgAsynLoadPageData,WPARAM(1),LPARAM(0));
}

void CDlgSEMLog_ShowAlarm::OnBnClickedButtonNextpage()
{
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_MsgAsynLoadPageData,WPARAM(2),LPARAM(0));
}


void CDlgSEMLog_ShowAlarm::_LoadPageData(const int nType) 
{
	__int64 tTimeStampBegin = _GetTimeStamp(true);
	__int64 tTimeStampEnd   = _GetTimeStamp(false);
	char szTimeCondition[128] = {0};
	sprintf_s(szTimeCondition,128,"Time >= %I64d and Time <= %I64d",tTimeStampBegin,tTimeStampEnd);

	char szSQL[256] = {0};
	const char* sField4Cluster = "Time";     // 聚集索引字段名
	const char* sCompareCondition = "<=";    // 比较符号
	string strTableName = "AlarmLog";
	int nPage = 0;
	if (0 == nType)
	{
		nPage = m_nCurrentPage;
	}
	else if (1 == nType)
	{
		nPage = m_nCurrentPage - 1;
	}
	else if (2 == nType)
	{
		nPage = m_nCurrentPage + 1;
	}

	sprintf_s(szSQL,256,"select * from %s where %s order by %s desc limit %d offset %d",
		strTableName.c_str(),szTimeCondition,sField4Cluster,m_nRecordCountPerPage, nPage*m_nRecordCountPerPage);

	string sSQL = szSQL;
	CDBDataManager::Instance().LoadData(sSQL,strTableName,m_ds);
	if (m_ds.Size() > 0)
	{
		if (1 == nType)
		{
			m_nCurrentPage--;
		}
		else if (2 == nType)
		{
			m_nCurrentPage++;
		}	
	}

	//将数据显示在UI上
	ShowData(CDBDataManager::Instance().GetTableMeta(strTableName), &m_ds);
	m_strPageNO.Format(GetResString(IDS_SEMLog_Page), m_nCurrentPage + 1);
}

void CDlgSEMLog_ShowAlarm::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType,cx,cy);
	_DesignLayout();
}

void CDlgSEMLog_ShowAlarm::_DesignLayout()
{
	_MoveControl(IDC_STATIC_TimeBegin,-12,0);
	_MoveControl(IDC_DATETIMEPICKER_DateBegin,-12,0);
	_MoveControl(IDC_DATETIMEPICKER_TimeBegin,-12,0);

	_MoveControl(IDC_STATIC_TimeEnd,-12,0);
	_MoveControl(IDC_DATETIMEPICKER_DateEnd,-12,0);
	_MoveControl(IDC_DATETIMEPICKER_TimeEnd,-12,0);

	_MoveControl(IDC_BUTTON_Query,-12,200);

	_MoveControl(IDC_CHECKCOMBOBOX_SHOWLOG,8,480);

	_MoveControl(IDC_BUTTON_PreviousPage,200,260);
	_MoveControl(IDC_BUTTON_NextPage,200,260);
	_MoveControl(IDC_STATIC_PageNO,200,260);

	_MoveGrid();
}

void CDlgSEMLog_ShowAlarm::_MoveGrid()
{
	CWnd* pCtrl = GetDlgItem(IDC_Grid);
	if(!pCtrl) return;

	CRect rc;
	pCtrl->GetWindowRect(rc);
	rc.left += -12;
	rc.top +=  -12;
	rc.right += 280;
	rc.bottom += 200;
	ScreenToClient(rc);
	pCtrl->MoveWindow(rc);
}
static bool Comp(CFieldDesc* pLeft,CFieldDesc* pRight)
{
	if(!pLeft || !pRight) return false;
	return pLeft->GetAttributeInt("SortID") < pRight->GetAttributeInt("SortID");
}

void CDlgSEMLog_ShowAlarm::ShowData( map<string,CFieldDesc*> &mapFieldName2FieldDesc, CDataSet* pDs )
{
	vector<CFieldDesc*> vColumn;
	for (map<string,CFieldDesc*>::iterator it = mapFieldName2FieldDesc.begin(); it != mapFieldName2FieldDesc.end();it++)
	{
		CFieldDesc* pDesc = it->second;
		if (pDesc && pDesc->GetAttributeInt("Show"))
		{
			vColumn.push_back(it->second);
		}	
	}

	::sort(vColumn.begin(),vColumn.end(),Comp);

	//m_Grid.ClearCells(CCellRange(0,0,m_Grid.GetRowCount(),m_Grid.GetColumnCount()));
	m_nFixCols = 0;
	m_nFixRows = 1;
	m_nCols = vColumn.size();
	m_nRows = pDs->Size() + 1;

	m_Grid.SetAutoSizeStyle();

	TRY {
		m_Grid.SetRowCount(m_nRows);
		m_Grid.SetColumnCount(m_nCols);
		m_Grid.SetFixedRowCount(m_nFixRows);
		m_Grid.SetFixedColumnCount(m_nFixCols);
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		return;
	}
	END_CATCH

		//m_Grid.DeleteAllItems();
		// fill rows/cols with text
		for (int row = 0; row < m_Grid.GetRowCount(); row++)
		{
			for (int col = 0; col < m_Grid.GetColumnCount(); col++)
			{ 
				CString str;
				GV_ITEM Item;

				Item.mask = GVIF_TEXT;
				Item.row = row;
				Item.col = col;

				if (row < m_nFixRows && col < m_nFixCols)
				{
					str.Format(_T("%s"),_T(""));
				}
				else if (row < m_nFixRows && col >= m_nFixCols)
				{
					CFieldDesc* pFieldDesc = vColumn[col];
					if (pFieldDesc)
					{
						str.Format(_T("%s"),pFieldDesc->GetAttributeString("DisplayName").c_str());
					}
				}
				else if (col < m_nFixCols && row >= m_nFixRows)
				{
					str.Format(_T("%d"), row);
				}
				else if (row >= m_nFixRows && col >= m_nFixCols)
				{
					GetCellData(pDs,vColumn, row, col, str);
				}

				Item.strText = str;
				m_Grid.SetItemState(row,col, m_Grid.GetItemState(row,col) & ~GVIS_READONLY);			

				m_Grid.SetItem(&Item);
			}
		}

		m_Grid.Invalidate();
}



void CDlgSEMLog_ShowAlarm::GetCellData( CDataSet* pDs, vector<CFieldDesc*>& vColumn,int row, int col, CString &str )
{
	CFieldDesc* pFieldDesc = vColumn[col];
	if(!pFieldDesc) return;

	CRecord* pRecord = pDs->GetAt(row - 1);
	if(!pRecord) return;
	CField* pField = pRecord->GetField(pFieldDesc->GetFieldID());
	if(!pField) return;

	GridCellBehaviour* pBehaviour = Factory<GridCellBehaviour,string>::Instance().BuildProduct(pFieldDesc->GetFieldID());
	if (pBehaviour)
	{
		pBehaviour->Attach(&m_Grid);
		pBehaviour->Behave(row,col,pField,str);
	}
	else
	{
		pBehaviour = new GridCellBehaviour();
		pBehaviour->Attach(&m_Grid);
		pBehaviour->Behave(row,col,pField,str);
	}

	delete pBehaviour;
	pBehaviour= NULL;
}

void CDlgSEMLog_ShowAlarm::_CreateCheckCombox()
{
	CRect rtCombo;
	GetDlgItem(IDC_BUTTON_Query)->GetClientRect(rtCombo);
	rtCombo.right += 100;
	rtCombo.bottom += 200;
	m_pMultiSelectCombo = new CCheckComboBox;
	DWORD dwStyle = CBS_DROPDOWNLIST | CBS_OWNERDRAWVARIABLE | CBS_SORT | CBS_HASSTRINGS | WS_VSCROLL | WS_TABSTOP;
	m_pMultiSelectCombo->Create(dwStyle,rtCombo,this,IDC_CHECKCOMBOBOX_SHOWLOG);
	m_pMultiSelectCombo->ShowWindow(SW_SHOW);
}


void CDlgSEMLog_ShowAlarm::OnBnClickedButtonQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	const string sTableName = "AlarmLog";
	string strFieldSelect = m_pMultiSelectCombo->GetSelectText();
	
	_QueryData(strFieldSelect, sTableName);

	//将数据显示在UI上
	vector<string> vSelectFieldID;
	_GetFieldID4Filter(strFieldSelect, vSelectFieldID);
	map<string,CFieldDesc*> mapFieldName2FieldDesc;
	CDBDataManager::Instance().GetMetaFilter(sTableName,vSelectFieldID,mapFieldName2FieldDesc);
	ShowData(mapFieldName2FieldDesc, &m_ds);
}

__int64 CDlgSEMLog_ShowAlarm::_GetTimeStamp(bool bBeginDateTime)
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


void CDlgSEMLog_ShowAlarm::_GetFieldID4Filter(const string strFieldSelect, vector<string> &vSelectFieldID )
{
	const string& strTableName = "AlarmLog";
	map<string,CFieldDesc*>& meta = CDBDataManager::Instance().GetTableMeta(strTableName);

	if (strFieldSelect != "")
	{
		vector<string> vSelectField;
		Split(strFieldSelect,",",vSelectField);
		for (map<string,CFieldDesc*>::iterator it = meta.begin(); it != meta.end();it++)
		{
			CFieldDesc* pFieldDesc = it->second;
			if(!pFieldDesc) continue;
			const string& sDisplayName = pFieldDesc->GetAttributeString("DisplayName");
			for (int i = 0; i< vSelectField.size();i++)
			{
				string& sDisplayNameSelect = trim(vSelectField[i]); 
				if (0 == sDisplayName.compare(sDisplayNameSelect))
				{
					vSelectFieldID.push_back(pFieldDesc->GetFieldID());
					break;
				}	
			}
		}
	}
	else
	{
		for (map<string,CFieldDesc*>::iterator it = meta.begin(); it != meta.end();it++)
		{
			CFieldDesc* pFieldDesc = it->second;
			if(!pFieldDesc) continue;
			vSelectFieldID.push_back(pFieldDesc->GetFieldID());
		}
	}
}

void CDlgSEMLog_ShowAlarm::_QueryData(const string strFieldSelect, const string sTableName )
{
	vector<string> vSelectFieldID;
	_GetFieldID4Filter(strFieldSelect, vSelectFieldID);

	string sSQL = "select ";
	for (int i = 0; i< vSelectFieldID.size();i++)
	{
		const string& sFieldID = vSelectFieldID[i]; 
		sSQL += sFieldID; 
		if(vSelectFieldID.size() - 1 != i)
		{
			sSQL += ",";
		}
		else
		{
			sSQL += "  ";
		}
	}
	sSQL += "from ";

	sSQL += sTableName;

	sSQL += " where ";

	const __int64 tTimeStampBegin = _GetTimeStamp(true);
	const __int64 tTimeStampEnd   = _GetTimeStamp(false);

	//if (tTimeStampBegin > tTimeStampEnd)
	//{
	//	MessageBox("结束时间大于开始时间!");
	//	return;
	//}

	//if (tTimeStampEnd - tTimeStampBegin > 24 * 3600 * 1000)
	//{
	//	MessageBox("查询区间不能超过24小时!");
	//	return;
	//}

	char szTimeCondition[128] = {0};
	sprintf_s(szTimeCondition,128," Time >= %I64d and Time <= %I64d ",tTimeStampBegin,tTimeStampEnd);

	sSQL += szTimeCondition;

	char szSQL[256] = {0};
	const char* sField4Cluster = "Time";     // 聚集索引字段名
	sprintf_s(szSQL,256,"order by %s asc limit %d offset %d", sField4Cluster, m_nRecordCountPerPage, m_nCurrentPage*m_nRecordCountPerPage);

	sSQL += szSQL;

	m_ds.Clear();

	CDBDataManager::Instance().GetFieldMetaData(sTableName,vSelectFieldID);

	CDBDataManager::Instance().LoadData(sSQL,sTableName,m_ds);
}


HRESULT CDlgSEMLog_ShowAlarm::OnLoadPageData(WPARAM wParam, LPARAM lParam)
{
	int nType = int(wParam);
	_LoadPageData(nType);	
	UpdateData(FALSE);
	return 0L;
}