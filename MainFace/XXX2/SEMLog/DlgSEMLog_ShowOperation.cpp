// DlgSEMLog.cpp : 实现文件
//
#include "stdafx.h"
#include "xxx2.h"
#include "DlgSEMLog_ShowOperation.h"
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

// CDlgSEMLog_ShowOperation 对话框

IMPLEMENT_DYNAMIC(CDlgSEMLog_ShowOperation, CDlgGridPage)

CDlgSEMLog_ShowOperation::CDlgSEMLog_ShowOperation(CWnd* pParent /*=NULL*/)
	: CDlgGridPage(pParent)
	, m_strPageNO(_T(""))
	, m_nCurrentPage(0)
	, m_nTotalPage(1)
	, m_nRecordCountPerPage(600)
	, m_strKeyWord(_T(""))
{
}

CDlgSEMLog_ShowOperation::~CDlgSEMLog_ShowOperation()
{
	m_ds.Clear();
}

void CDlgSEMLog_ShowOperation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Grid, m_Grid); 
	DDX_Text(pDX, IDC_STATIC_PageNO, m_strPageNO);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DateBegin, m_DayBegin);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TimeBegin, m_TimeBegin);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DateEnd, m_DayEnd);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TimeEnd, m_TimeEnd);
	DDX_Text(pDX, IDC_EDIT_KeyWord, m_strKeyWord);
}


BEGIN_MESSAGE_MAP(CDlgSEMLog_ShowOperation, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PreviousPage, &CDlgSEMLog_ShowOperation::OnBnClickedButtonPreviouspage)
	ON_BN_CLICKED(IDC_BUTTON_NextPage, &CDlgSEMLog_ShowOperation::OnBnClickedButtonNextpage)
	ON_BN_CLICKED(IDC_BUTTON_Query, &CDlgSEMLog_ShowOperation::OnBnClickedButtonQuery)
	ON_MESSAGE(WM_MsgAsynLoadPageData,OnLoadPageData)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CDlgSEMLog_ShowOperation::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDC_STATIC_TimeBegin)->SetWindowText(GetResString(IDC_STATIC_TimeBegin));
	GetDlgItem(IDC_STATIC_TimeEnd)->SetWindowText(GetResString(IDC_STATIC_TimeEnd));
	GetDlgItem(IDC_BUTTON_Query)->SetWindowText(GetResString(IDC_BUTTON_Query));
	GetDlgItem(IDC_BUTTON_PreviousPage)->SetWindowText(GetResString(IDC_BUTTON_PreviousPage));
	GetDlgItem(IDC_BUTTON_NextPage)->SetWindowText(GetResString(IDC_BUTTON_NextPage));
	GetDlgItem(IDC_STATIC_Subject)->SetWindowText(GetResString(IDC_STATIC_Subject));
	GetDlgItem(IDC_STATIC_KeyWord)->SetWindowText(GetResString(IDC_STATIC_KeyWord));
}

// CDlgSEMLog 消息处理程序
BOOL CDlgSEMLog_ShowOperation::OnInitDialog()
{
	CDialog::OnInitDialog();

	_InitializeGrid();
	_CreateCheckCombox();
	_AddItem2MultiSelectCheckBox();

	CTime timeNow = CTime::GetCurrentTime();
	CTimeSpan sp(-1,0,0,0);
	CTime timeYesterday = timeNow + sp;
	m_DayBegin.SetTime(&timeYesterday);

    PostMessage(WM_MsgAsynLoadPageData,WPARAM(5),LPARAM(0));

	UpdateData(FALSE);

	Localize();
	return TRUE;
}


void CDlgSEMLog_ShowOperation::_CreateCheckCombox()
{
	CRect rc;
	GetClientRect(rc);
	rc.left += (rc.Width() - 250);
	rc.top += 8;
	rc.right = (rc.left + 120);
	rc.bottom = (rc.top + 200);
	m_pMultiSelectCombo = new CCheckComboBox;
	DWORD dwStyle = CBS_DROPDOWNLIST | CBS_OWNERDRAWVARIABLE | CBS_HASSTRINGS | WS_VSCROLL | WS_TABSTOP;
	m_pMultiSelectCombo->Create(dwStyle,rc,this,IDC_CHECKCOMBOBOX_SHOWLOG);
	m_pMultiSelectCombo->ShowWindow(SW_SHOW);
}

void CDlgSEMLog_ShowOperation::_AddItem2MultiSelectCheckBox()
{
	char szSQL[256] = {0};
	const string& sTableName = "Settings";
	sprintf_s(szSQL,256,"select * from %s where subject = 'OperationKeyWord'",sTableName.c_str());
	CDataSet dsSetting;
	CDBDataManager::Instance().LoadData(string(szSQL),sTableName,dsSetting);
	if(dsSetting.Size() != 1) return;

	CRecord* pRecord = dsSetting.GetAt(0);
	if(!pRecord) return;
	CField* pField = pRecord->GetField("Content");
	if(!pField)  return;
	const string& sSetting = pField->GetValueAsString();

	vector<string> vKeyWord;
	Split(sSetting, ";",vKeyWord);

	for (int i = 0; i < vKeyWord.size();i++)
	{
		m_pMultiSelectCombo->AddString(LPCTSTR(vKeyWord[i].c_str()));
	}
}

void CDlgSEMLog_ShowOperation::OnBnClickedButtonQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
    PostMessage(WM_MsgAsynLoadPageData,WPARAM(0),LPARAM(0));
}


void CDlgSEMLog_ShowOperation::OnBnClickedButtonPreviouspage()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_nCurrentPage < 1) return;
	PostMessage(WM_MsgAsynLoadPageData,WPARAM(1),LPARAM(0));
}

void CDlgSEMLog_ShowOperation::OnBnClickedButtonNextpage()
{
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_MsgAsynLoadPageData,WPARAM(2),LPARAM(0));
}


void CDlgSEMLog_ShowOperation::_LoadPageData(const int nType) 
{
	const string sTableName = "OperationLog";
	const string strKeyWord = m_strKeyWord;

	string sSQL = "select * ";
	sSQL += "from ";

	sSQL += sTableName;

	sSQL += " where ";

	const __int64 tTimeStampBegin = _GetTimeStamp(true);
	const __int64 tTimeStampEnd   = _GetTimeStamp(false);

	if (tTimeStampBegin > tTimeStampEnd)
	{
		MessageBox("结束时间大于开始时间!");
		return;
	}

	if (tTimeStampEnd - tTimeStampBegin > 24 * 3600 * 1000)
	{
		MessageBox("查询区间不能超过24小时!");
		return;
	}

	char szTimeCondition[128] = {0};
	sprintf_s(szTimeCondition,128," Time >= %I64d and Time <= %I64d ",tTimeStampBegin,tTimeStampEnd);

	sSQL += szTimeCondition;

	if ("" != strKeyWord)
	{
		char szContentCondition[128] = {0};
		sprintf_s(szContentCondition,128," and  Content like '%s%s%s' ","%",strKeyWord.c_str(),"%");
		sSQL += szContentCondition;
	}
	else
	{
		string strFieldSelect = m_pMultiSelectCombo->GetSelectText();
		if ("" != strFieldSelect)
		{
			vector<string> vKeyWord;
			Split(strFieldSelect,",",vKeyWord);
			string sKeyWordQuery = " and (";
			for (int i = 0; i < vKeyWord.size();i++)
			{
				char sz[128] = {0};
				if (vKeyWord.size() - 1 != i)
				{
					sprintf_s(sz,128,"  Content like '%s%s%s' or ","%",vKeyWord[i].c_str(),"%");
				}
				else
				{
					sprintf_s(sz,128,"  Content like '%s%s%s' ","%",vKeyWord[i].c_str(),"%");
				}

				sKeyWordQuery += sz;
			}
			sKeyWordQuery += ")";
			sSQL += sKeyWordQuery;
		}
	}

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
	else if (5 ==  nType)
	{
		nPage = -1;
	}

	char szSQL[256] = {0};
	const char* sField4Cluster = "Time";     // 聚集索引字段名

	if (-1 != nPage)
	{
		sprintf_s(szSQL,256,"order by %s asc limit %d offset %d", sField4Cluster, m_nRecordCountPerPage, nPage*m_nRecordCountPerPage);
	}
	else
	{
		sprintf_s(szSQL,256," and Time = 0");
	}

	sSQL += szSQL;


	CDBDataManager::Instance().LoadData(sSQL,sTableName,m_ds);
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
	ShowData(CDBDataManager::Instance().GetTableMeta("OperationLog"), &m_ds);
	m_strPageNO.Format(GetResString(IDS_SEMLog_Page), m_nCurrentPage + 1);
}

void CDlgSEMLog_ShowOperation::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType,cx,cy);
	_DesignLayout();
}

void CDlgSEMLog_ShowOperation::_DesignLayout()
{
	_MoveControl(IDC_STATIC_TimeBegin,-12,0);
	_MoveControl(IDC_DATETIMEPICKER_DateBegin,-12,0);
	_MoveControl(IDC_DATETIMEPICKER_TimeBegin,-12,0);

	_MoveControl(IDC_STATIC_TimeEnd,-12,0);
	_MoveControl(IDC_DATETIMEPICKER_DateEnd,-12,0);
	_MoveControl(IDC_DATETIMEPICKER_TimeEnd,-12,0);
	_MoveControl(IDC_STATIC_Subject,-12,4);

	_MoveControl(IDC_STATIC_KeyWord,-12,122);
	_MoveControl(IDC_EDIT_KeyWord,-12,122);

	_MoveControl(IDC_BUTTON_Query,-12,122);

	_MoveControl(IDC_BUTTON_PreviousPage,200,260);
	_MoveControl(IDC_BUTTON_NextPage,200,260);
	_MoveControl(IDC_STATIC_PageNO,200,260);

	_MoveGrid();
}

void CDlgSEMLog_ShowOperation::_MoveGrid()
{
	CWnd* pCtrl = GetDlgItem(IDC_Grid);
	if(!pCtrl) return;

	CRect rc;
	pCtrl->GetWindowRect(rc);
	rc.left += -12;
	rc.top +=  -12;
	rc.right += 130;
	rc.bottom += 200;
	ScreenToClient(rc);
	pCtrl->MoveWindow(rc);
}


static bool Comp(CFieldDesc* pLeft,CFieldDesc* pRight)
{
	if(!pLeft || !pRight) return false;
	return pLeft->GetAttributeInt("SortID") < pRight->GetAttributeInt("SortID");
}

void CDlgSEMLog_ShowOperation::ShowData( map<string,CFieldDesc*> &mapFieldName2FieldDesc, CDataSet* pDs )
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
						const string& sDisplayName = pFieldDesc->GetAttributeString("DisplayName");
						str.Format(_T("%s"),sDisplayName.c_str());
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



void CDlgSEMLog_ShowOperation::GetCellData( CDataSet* pDs, vector<CFieldDesc*>& vColumn,int row, int col, CString &str )
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

__int64 CDlgSEMLog_ShowOperation::_GetTimeStamp(bool bBeginDateTime)
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


void CDlgSEMLog_ShowOperation::_GetFieldID4Filter(const string strFieldSelect, vector<string> &vSelectFieldID )
{
	map<string,CFieldDesc*>& meta = CDBDataManager::Instance().GetTableMeta("OperationLog");

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

void CDlgSEMLog_ShowOperation::_QueryData(const string strFieldSelect, const string sTableName, const string strKeyWord )
{
	vector<string> vSelectFieldID;
	vSelectFieldID.push_back("Time");
	vSelectFieldID.push_back(strFieldSelect);

	string sSQL = "select * ";
	sSQL += "from ";

	sSQL += sTableName;

	sSQL += " where ";

	const __int64 tTimeStampBegin = _GetTimeStamp(true);
	const __int64 tTimeStampEnd   = _GetTimeStamp(false);

	if (tTimeStampBegin > tTimeStampEnd)
	{
		MessageBox("结束时间大于开始时间!");
		return;
	}

	if (tTimeStampEnd - tTimeStampBegin > 24 * 3600 * 1000)
	{
		MessageBox("查询区间不能超过24小时!");
		return;
	}

	char szTimeCondition[128] = {0};
	sprintf_s(szTimeCondition,128," Time >= %I64d and Time <= %I64d ",tTimeStampBegin,tTimeStampEnd);

	sSQL += szTimeCondition;

	if ("" != strKeyWord)
	{
		char szContentCondition[128] = {0};
		sprintf_s(szContentCondition,128," and  Content like '%s%s%s' ","%",strKeyWord.c_str(),"%");
		sSQL += szContentCondition;
	}

	char szSQL[256] = {0};
	const char* sField4Cluster = "Time";     // 聚集索引字段名
	sprintf_s(szSQL,256,"order by %s asc limit %d offset %d", sField4Cluster, m_nRecordCountPerPage, m_nCurrentPage*m_nRecordCountPerPage);

	sSQL += szSQL;

	m_ds.Clear();

	CDBDataManager::Instance().GetFieldMetaData(sTableName,vSelectFieldID);

	CDBDataManager::Instance().LoadData(sSQL,sTableName,m_ds);
}


HRESULT CDlgSEMLog_ShowOperation::OnLoadPageData(WPARAM wParam, LPARAM lParam)
{
	int nType = int(wParam);
	_LoadPageData(nType);
    UpdateData(FALSE);
	return 0L;
}