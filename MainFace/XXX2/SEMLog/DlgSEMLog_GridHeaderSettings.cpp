// DlgSEMLog.cpp : 实现文件
//
#include "stdafx.h"
#include "xxx2.h"
#include "DlgSEMLog_GridHeaderSettings.h"
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

//本页面中的列表展示3列，分别是"序号，FieldID, DisplayName"
static const int nColumnCount = 3;

IMPLEMENT_DYNAMIC(CDlgSEMLog_GridHeaderSettings, CDlgGridPage)
CDlgSEMLog_GridHeaderSettings::CDlgSEMLog_GridHeaderSettings(CWnd* pParent /*=NULL*/)
	: CDlgGridPage(pParent)
{
	m_bHorzLines = TRUE;
	m_bVertLines = TRUE;
	m_bVertical = FALSE;
}

CDlgSEMLog_GridHeaderSettings::~CDlgSEMLog_GridHeaderSettings()
{
	m_ds.Clear();
}


BEGIN_MESSAGE_MAP(CDlgSEMLog_GridHeaderSettings, CDlgGridPage)
	ON_MESSAGE(WM_MsgAsynLoadPageData,OnLoadPageData)
	ON_WM_SIZE()
END_MESSAGE_MAP()


BOOL CDlgSEMLog_GridHeaderSettings::OnInitDialog()
{
	CDlgGridPage::OnInitDialog();
	_InitializeGrid();
	m_Grid.EnableSelection(TRUE);
	m_Grid.SetSingleRowSelection(TRUE);

    PostMessage(WM_MsgAsynLoadPageData,WPARAM(0),LPARAM(0));

	UpdateData(FALSE);

	return TRUE;
}



void CDlgSEMLog_GridHeaderSettings::_LoadPageData() 
{
	vector<tuple<int,string,string>> vColumnDisplayName;
	const string& strTableName = CDBDataManager::Instance().GetLogTableName();
	map<string,CFieldDesc*>& meta =  CDBDataManager::Instance().GetTableMeta(strTableName);

	vector<pair<string,CFieldDesc*>> metaV(meta.begin(),meta.end());//用map对该容器初始化
	sort(metaV.begin(),metaV.end(),CompareBySequenceID);//用sort排序

	for (vector<pair<string,CFieldDesc*>>::iterator it = metaV.begin(); it != metaV.end();it++)
	{
		CFieldDesc* pFieldDesc = it->second;
		if(!pFieldDesc) continue;

		const string& sDisplayName =  pFieldDesc->GetAttributeString("DisplayName");
		vColumnDisplayName.push_back(make_tuple(pFieldDesc->m_nSequenceID,pFieldDesc->m_strFieldID,sDisplayName));	
	}

	//将数据显示在UI上
	ShowData(vColumnDisplayName);
}

void CDlgSEMLog_GridHeaderSettings::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType,cx,cy);
	_DesignLayout();
}

void CDlgSEMLog_GridHeaderSettings::_DesignLayout()
{
	_MoveGrid();
}

void CDlgSEMLog_GridHeaderSettings::_MoveGrid()
{
	CRect rc;
	GetClientRect(rc);

	CWnd* pCtrl = GetDlgItem(IDC_Grid);
	if(!pCtrl) return;

	rc.left += 5;
	rc.top +=  5;
	rc.right -= 5;
	rc.bottom -= 5;
	pCtrl->MoveWindow(rc);
}

static bool Comp(CFieldDesc* pLeft,CFieldDesc* pRight)
{
	if(!pLeft || !pRight) return false;
	return pLeft->GetAttributeInt("SortID") < pRight->GetAttributeInt("SortID");
}

void CDlgSEMLog_GridHeaderSettings::ShowData(const vector<tuple<int,string,string>>& vHeader)
{
	//m_Grid.ClearCells(CCellRange(0,0,m_Grid.GetRowCount(),m_Grid.GetColumnCount()));
	const string vColumn[nColumnCount] = {"列序号","列ID","显示名称"};
	const int vColumnWidth[nColumnCount] = {60,120,240};

	m_nFixCols = 0;
	m_nFixRows = 1;
	m_nCols = nColumnCount;
	m_nRows = vHeader.size() + 1;

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
				str.Format(_T("%s"),vColumn[col].c_str());
				m_Grid.SetColumnWidth(col,vColumnWidth[col]);
			}
			else if (col < m_nFixCols && row >= m_nFixRows)
			{
				str.Format(_T("%d"), row);
			}
			else if (row >= m_nFixRows && col >= m_nFixCols)
			{
				GetCellData(vHeader,row, col, str);
			}

			Item.strText = str;
			m_Grid.SetItemState(row,col, m_Grid.GetItemState(row,col) & ~GVIS_READONLY);			

			m_Grid.SetItem(&Item);
		}
	}

	m_Grid.Invalidate();
}



void CDlgSEMLog_GridHeaderSettings::GetCellData(const vector<tuple<int,string,string>>& vHeader, int row, int col, CString &str )
{
	tuple<int,string,string> header = vHeader[row - 1];
	int ID = get<0>(header);
	string sFieldID = get<1>(header);
	string sDisplayName = get<2>(header);
	if (0 == col)
	{
		str.Format(_T("%d"),get<0>(header));
	}
	else if (1 == col)
	{
		str.Format(_T("%s"),get<1>(header).c_str());
	}
	else if (2 ==  col)
	{
		str.Format(_T("%s"),get<2>(header).c_str());
	}

}


HRESULT CDlgSEMLog_GridHeaderSettings::OnLoadPageData(WPARAM wParam, LPARAM lParam)
{
	_LoadPageData();	
	UpdateData(FALSE);
	return 0L;
}