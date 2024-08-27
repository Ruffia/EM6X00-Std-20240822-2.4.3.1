// DlgSEMLog.cpp : 实现文件
//
#include "stdafx.h"
#include "xxx2.h"
#include "DlgSEMLog_ShowALLData.h"
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

//比较条件
static const char* sCompare[6] = {"<","<=","=","!=",">=",">"};

// CDlgSEMLog_ShowALLData 对话框
IMPLEMENT_DYNAMIC(CDlgSEMLog_ShowALLData, CDialog)

CDlgSEMLog_ShowALLData::CDlgSEMLog_ShowALLData(CWnd* pParent /*=NULL*/)
	: CDlgGridPage(pParent)
	, m_strPageNO(_T(""))
	, m_nCurrentPage(0)
	, m_nTotalPage(1)
	, m_nRecordCountPerPage(600)
	, m_dCompareValue(0)
	, m_strQueryByCompare(_T(""))
{
	m_vCompareFiled.clear();
}

CDlgSEMLog_ShowALLData::~CDlgSEMLog_ShowALLData()
{
	if (m_pMultiSelectCombo)
	{
		//m_pMultiSelectCombo->DestroyWindow();
		//delete m_pMultiSelectCombo;
	}

	m_ds.Clear();
	m_vCompareFiled.clear();
}

void CDlgSEMLog_ShowALLData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Grid, m_Grid); 
	DDX_Text(pDX, IDC_STATIC_PageNO, m_strPageNO);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DateBegin, m_DayBegin);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TimeBegin, m_TimeBegin);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DateEnd, m_DayEnd);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TimeEnd, m_TimeEnd);
	DDX_Control(pDX, IDC_COMBOBOXEX_Field, m_comboField);
	DDX_Control(pDX, IDC_COMBOBOXEX_Compare, m_comboCompare);
	DDX_Text(pDX, IDC_EDIT_CompareValue, m_dCompareValue);
}


BEGIN_MESSAGE_MAP(CDlgSEMLog_ShowALLData, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PreviousPage, &CDlgSEMLog_ShowALLData::OnBnClickedButtonPreviouspage)
	ON_BN_CLICKED(IDC_BUTTON_NextPage, &CDlgSEMLog_ShowALLData::OnBnClickedButtonNextpage)
	ON_BN_CLICKED(IDC_BUTTON_Query, &CDlgSEMLog_ShowALLData::OnBnClickedButtonQuery)
	ON_MESSAGE(WM_MsgAsynLoadPageData,OnLoadPageData)
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CDlgSEMLog_ShowALLData::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDC_STATIC_TimeBegin)->SetWindowText(GetResString(IDC_STATIC_TimeBegin));
	GetDlgItem(IDC_STATIC_TimeEnd)->SetWindowText(GetResString(IDC_STATIC_TimeEnd));
	GetDlgItem(IDC_BUTTON_Query)->SetWindowText(GetResString(IDC_BUTTON_Query));
	GetDlgItem(IDC_BUTTON_PreviousPage)->SetWindowText(GetResString(IDC_BUTTON_PreviousPage));
	GetDlgItem(IDC_BUTTON_NextPage)->SetWindowText(GetResString(IDC_BUTTON_NextPage));
	GetDlgItem(IDC_STATIC_FieldCompare)->SetWindowText(GetResString(IDC_STATIC_FieldCompare));
}

// CDlgSEMLog 消息处理程序
BOOL CDlgSEMLog_ShowALLData::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	_InitializeGrid();

	_CreateCheckCombox();
	_AddItem2MultiSelectCheckBox();

	CTime timeNow = CTime::GetCurrentTime();
	CTimeSpan sp(0,0,-15,0);
	CTime timeYesterday = timeNow + sp;
	m_TimeBegin.SetTime(&timeYesterday);

    PostMessage(WM_MsgAsynLoadPageData,WPARAM(5),LPARAM(0));

	m_AsynProcessor.Initial(this);
	CString strTitle = _T("加载日志数据");
	m_AsynProcessor.SetTitle(strTitle);
	CString strText(_T("处理中......"));
	m_AsynProcessor.SetText(strText);

	UpdateData(FALSE);

	Localize();
	return TRUE;
}

void CDlgSEMLog_ShowALLData::OnBnClickedButtonQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_strQueryByCompare = _T("");
	const int nFieldSel = m_comboField.GetCurSel();
	const int nCompareSel = m_comboCompare.GetCurSel();
	if (-1 == nCompareSel || -1 == nFieldSel)
	{
		m_strQueryByCompare = "";
	}
	else
	{
		CFieldDesc* &pFieldDesc = m_vCompareFiled[nFieldSel];
		string strFieldID = pFieldDesc->m_strFieldID;
		const char* sCompareSelect = sCompare[nCompareSel];
		m_strQueryByCompare.Format(_T("%s%s%f"),pFieldDesc->m_strFieldID.c_str(),sCompare[nCompareSel],m_dCompareValue);

	}
	m_nCurrentPage = 0;
    PostMessage(WM_MsgAsynLoadPageData,WPARAM(0),LPARAM(0));
}

void CDlgSEMLog_ShowALLData::OnBnClickedButtonPreviouspage()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_nCurrentPage < 1) return;
	PostMessage(WM_MsgAsynLoadPageData,WPARAM(1),LPARAM(0));
}

void CDlgSEMLog_ShowALLData::OnBnClickedButtonNextpage()
{
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_MsgAsynLoadPageData,WPARAM(2),LPARAM(0));
}


void CDlgSEMLog_ShowALLData::_LoadPageData(const int nType) 
{
	const string& sTableName = CDBDataManager::Instance().GetLogTableName();
	string strFieldSelect = m_pMultiSelectCombo->GetSelectText();

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

	__int64 tTimeStampBegin = _GetTimeStamp(true);
	__int64 tTimeStampEnd   = _GetTimeStamp(false);

	char szTimeCondition[128] = {0};
	sprintf_s(szTimeCondition,128,"Time >= %I64d and Time <= %I64d ",tTimeStampBegin,tTimeStampEnd);

	sSQL += szTimeCondition;

	if (m_strQueryByCompare != "")
	{
		sSQL += " and ";		
		sSQL +=	m_strQueryByCompare;
	}

	char szSQLPage[256] = {0};
	const char* sField4Cluster = "Time";     // 聚集索引字段名
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

	if (-1 != nPage)
	{
		sprintf_s(szSQLPage,256," order by %s desc limit %d offset %d",
			sField4Cluster,m_nRecordCountPerPage, nPage*m_nRecordCountPerPage);
	}
	else
	{
		sprintf_s(szSQLPage,256," and Time = 0");
	}

	sSQL += szSQLPage;

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

	//获取过滤的字段
	m_mapFieldID2Desc.clear();
	CDBDataManager::Instance().GetMetaFilter(sTableName,vSelectFieldID,m_mapFieldID2Desc);

//	m_strPageNO.Format(_T("第 %d 页"), m_nCurrentPage + 1);
	m_strPageNO.Format(GetResString(IDS_SEMLog_Page), m_nCurrentPage + 1);
	UpdateData(FALSE);

	//将数据显示在UI上
	m_AsynProcessor.StartDoJob();
}

void CDlgSEMLog_ShowALLData::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType,cx,cy);
	_DesignLayout();
}

void CDlgSEMLog_ShowALLData::_DesignLayout()
{
	_MoveControl(IDC_STATIC_TimeBegin,-12,-10);
	_MoveControl(IDC_DATETIMEPICKER_DateBegin,-12,-10);
	_MoveControl(IDC_DATETIMEPICKER_TimeBegin,-12,-10);

	_MoveControl(IDC_STATIC_TimeEnd,-12,-20);
	_MoveControl(IDC_DATETIMEPICKER_DateEnd,-12,-20);
	_MoveControl(IDC_DATETIMEPICKER_TimeEnd,-12,-20);
	_MoveControl(IDC_BUTTON_Query,-12,170);

	_MoveControl(IDC_CHECKCOMBOBOX_SHOWLOG,8,460);
	_MoveControl(IDC_STATIC_FieldCompare,-12,168);
	_MoveControl(IDC_COMBOBOXEX_Field,-12,165);
	_MoveControl(IDC_COMBOBOXEX_Compare,-12,198);
	_MoveControl(IDC_EDIT_CompareValue,-12,192);

	
	_MoveControl(IDC_BUTTON_PreviousPage,200,260);
	_MoveControl(IDC_BUTTON_NextPage,200,260);
	_MoveControl(IDC_STATIC_PageNO,200,260);

	_MoveGrid();
}

void CDlgSEMLog_ShowALLData::_MoveGrid()
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

void CDlgSEMLog_ShowALLData::OnVirtualMode() 
{
	UpdateData();
	m_Grid.SetEditable(m_bEditable);

	m_bHeaderSort = m_Grid.GetHeaderSort();

	m_nFixCols = 1;
	m_nFixRows = 1;
	m_nCols = 6;
	m_nRows = 3;

	m_Grid.SetAutoSizeStyle();

	m_bEditable = m_Grid.IsEditable();
	m_Grid.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);

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

				if (row < m_nFixRows)
					str.Format(_T("Column %d"),col);
				else if (col < m_nFixCols) 
					str.Format(_T("Row %d"), row);
				else 
					str.Format(_T("%d"),row*col);
				Item.strText = str;

				if (col < m_Grid.GetFixedColumnCount())
				{
					//Item.iImage = rand()%m_ImageList.GetImageCount();
					Item.mask  |= (GVIF_IMAGE);
				}

				m_Grid.SetItem(&Item);
			}
		}


	// Having weird Autosize problems with CE 2.11 - possibly just an emulation problem
#if !defined(_WIN32_WCE) || (_WIN32_WCE < 211)
	m_Grid.AutoSize();
#endif

	UpdateData(FALSE);
}

static bool Comp(CFieldDesc* pLeft,CFieldDesc* pRight)
{
	if(!pLeft || !pRight) return false;
	return pLeft->GetAttributeInt("SortID") < pRight->GetAttributeInt("SortID");
}


void CDlgSEMLog_ShowALLData::GetCellData( CDataSet* pDs, vector<CFieldDesc*>& vColumn,int row, int col, CString &str )
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


void CDlgSEMLog_ShowALLData::_CreateCheckCombox()
{
	CRect rtCombo;
	GetDlgItem(IDC_BUTTON_Query)->GetClientRect(rtCombo);
	rtCombo.right += 100;
	rtCombo.bottom += 200;
	m_pMultiSelectCombo = new CCheckComboBox;
	DWORD dwStyle = CBS_DROPDOWNLIST | CBS_OWNERDRAWVARIABLE | CBS_HASSTRINGS | WS_VSCROLL | WS_TABSTOP;
	m_pMultiSelectCombo->Create(dwStyle,rtCombo,this,IDC_CHECKCOMBOBOX_SHOWLOG);
	m_pMultiSelectCombo->ShowWindow(SW_SHOW);
}


__int64 CDlgSEMLog_ShowALLData::_GetTimeStamp(bool bBeginDateTime)
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


void CDlgSEMLog_ShowALLData::_GetFieldID4Filter(const string strFieldSelect, vector<string> &vSelectFieldID )
{
	const string& strTableName = CDBDataManager::Instance().GetLogTableName();
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


HRESULT CDlgSEMLog_ShowALLData::OnLoadPageData(WPARAM wParam, LPARAM lParam)
{
	int nType = int(wParam);
	_LoadPageData(nType);
	UpdateData(FALSE);
	return 0L;
}

void CDlgSEMLog_ShowALLData::DoJob(AsynProcessorWithProgress* pProgress)
{
	vector<CFieldDesc*> vColumn;
	for (map<string,CFieldDesc*>::iterator it = m_mapFieldID2Desc.begin(); it != m_mapFieldID2Desc.end();it++)
	{
		CFieldDesc* pDesc = it->second;
		if (pDesc && pDesc->GetAttributeInt("Show"))
		{
			vColumn.push_back(it->second);
		}	
	}

	::sort(vColumn.begin(),vColumn.end(),Comp);

	CDataSet* pDs = &m_ds;
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
	pProgress->SetProgressRange(pDs->Size());
	for (int row = 0; row < m_Grid.GetRowCount(); row++)
	{
		CString strText = _T("");
		strText.Format(_T("正在加载第 %d 页数据"),m_nCurrentPage + 1);
		pProgress->SetText(strText);

		CString strProgress = _T("");
		strProgress.Format(_T("当前进度：%d/%d"),row,pDs->Size());
		pProgress->SetProgressInfo(strProgress);
		pProgress->DoProgress();

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
		Sleep(1);
	}

	m_Grid.Invalidate();
}

void CDlgSEMLog_ShowALLData::_AddItem2MultiSelectCheckBox()
{
	const string& strTableName = CDBDataManager::Instance().GetLogTableName();
	map<string,CFieldDesc*>& meta = CDBDataManager::Instance().GetTableMeta(strTableName);

	vector<CFieldDesc*> vFieldDesc;
	for (map<string,CFieldDesc*>::iterator it = meta.begin(); it != meta.end();it++)
	{
		CFieldDesc* pFieldDesc = it->second;
		if (pFieldDesc)
		{
			vFieldDesc.push_back(pFieldDesc);
		}
	}

	sort(vFieldDesc.begin(),vFieldDesc.end(),CompareFieldBySequenceID);

	for (int i = 0; i < vFieldDesc.size();i++)
	{
		CFieldDesc* &pFieldDesc = vFieldDesc[i];
		const string& sFieldID = pFieldDesc->m_strFieldID;
		const string& sDisplayName = pFieldDesc->GetAttributeString("DisplayName");
		m_pMultiSelectCombo->AddString(sDisplayName.c_str());
		if (sFieldID.find("_s") != -1 || sFieldID.find("_m2") != -1 || sFieldID.find("_set") != -1 || 
			sFieldID == "Power" || sFieldID == "Chamb" || sFieldID == "Gun" || 
			sFieldID == "Vf" || sFieldID == "Vc" || sFieldID == "TMP" ||
			sFieldID == "Va" || sFieldID == "TCf"|| sFieldID == "TCc" ||
			sFieldID == "TCs")
		{

		}
		else
		{
			m_vCompareFiled.push_back(pFieldDesc);
			m_pMultiSelectCombo->SetCheck(i,TRUE);
			m_comboField.AddString(LPCTSTR(sDisplayName.c_str()));
		}
	}

	for (int i = 0; i < 6; i++)
	{
		m_comboCompare.AddString(sCompare[i]);
	}
}
