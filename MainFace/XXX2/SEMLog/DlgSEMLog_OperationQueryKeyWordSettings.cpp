// DlgSEMLog.cpp : 实现文件
//
#include "stdafx.h"
#include "xxx2.h"
#include "DlgSEMLog_OperationQueryKeyWordSettings.h"
#include "Factory.h"
#include "FieldMeta.h"
#include "DBDataManager.h"
#include <algorithm>
#include "Util.h"
#include "Message.h"


IMPLEMENT_DYNAMIC(CDlgSEMLog_OperationQueryKeyWordSettings, CDlgGridPage)

CDlgSEMLog_OperationQueryKeyWordSettings::CDlgSEMLog_OperationQueryKeyWordSettings(CWnd* pParent /*=NULL*/)
	: CDlgGridPage(pParent)
{
	m_bHorzLines = TRUE;
	m_bVertLines = TRUE;
	m_bVertical = FALSE;
	m_bEditable  = TRUE;
}

CDlgSEMLog_OperationQueryKeyWordSettings::~CDlgSEMLog_OperationQueryKeyWordSettings()
{
	m_ds.Clear();
}


BEGIN_MESSAGE_MAP(CDlgSEMLog_OperationQueryKeyWordSettings, CDlgGridPage)
	ON_BN_CLICKED(IDC_BUTTON_Add, &CDlgSEMLog_OperationQueryKeyWordSettings::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_Delete, &CDlgSEMLog_OperationQueryKeyWordSettings::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_Save, &CDlgSEMLog_OperationQueryKeyWordSettings::OnBnClickedButtonSave)
	ON_WM_SIZE()	
END_MESSAGE_MAP()


void CDlgSEMLog_OperationQueryKeyWordSettings::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDC_BUTTON_Add)->SetWindowText(GetResString(IDC_BUTTON_Add));
	GetDlgItem(IDC_BUTTON_Delete)->SetWindowText(GetResString(IDC_BUTTON_Delete));
	GetDlgItem(IDC_BUTTON_Save)->SetWindowText(GetResString(IDC_BUTTON_Save));
}

BOOL CDlgSEMLog_OperationQueryKeyWordSettings::OnInitDialog()
{
	CDlgGridPage::OnInitDialog();
	_InitializeGrid();
	m_Grid.EnableSelection(TRUE);
	m_Grid.SetSingleRowSelection(TRUE);

	const string& sTableName = "Settings";
	_LoadData(sTableName);
	map<string,CFieldDesc*> &mapFieldName2FieldDesc = CDBDataManager::Instance().GetTableMeta(sTableName);
	ShowData(mapFieldName2FieldDesc,&m_ds);

	m_Grid.SetEditable(m_bEditable);

	Localize();
	return TRUE;
}


void CDlgSEMLog_OperationQueryKeyWordSettings::OnBnClickedButtonAdd()
{
	string sAdd = "请输入内容";
	m_sKeyWord += ";";
	m_sKeyWord += sAdd;
	m_Grid.SetRowCount(m_nRows + 1);
	for (int col = 0; col < m_Grid.GetColumnCount(); col++)
	{ 
		CString str;
		GV_ITEM Item;

		Item.mask = GVIF_TEXT;
		Item.row = m_nRows;
		Item.col = col;

		if (col >= m_nFixCols)
		{
			if (0 == col)
			{
				str.Format(_T("%d"),m_nRows);
			}
			else if (1 == col)
			{
				str.Format(_T("%s"),sAdd.c_str());
			}
		}

		Item.strText = str;
		m_Grid.SetItemState(m_nRows,col, m_Grid.GetItemState(m_nRows,col) & ~GVIS_READONLY);			

		m_Grid.SetItem(&Item);
	}
	m_nRows++;
	m_Grid.Invalidate();
	UpdateData(FALSE);
}


void CDlgSEMLog_OperationQueryKeyWordSettings::OnBnClickedButtonDelete()
{
	int nRow = m_Grid.GetFocusCell().row;
	if (nRow >= 0)
	{
		m_Grid.DeleteRow(nRow);	
		m_nRows--;
		m_Grid.SetRowCount(m_nRows);
		m_Grid.Invalidate();
	}
}


void CDlgSEMLog_OperationQueryKeyWordSettings::_LoadData(const string& sTableName) 
{
	char szSQL[256] = {0};
	sprintf_s(szSQL,256,"select * from %s where subject = 'OperationKeyWord'",sTableName.c_str());
	CDBDataManager::Instance().LoadData(string(szSQL),sTableName,m_ds);
}

void CDlgSEMLog_OperationQueryKeyWordSettings::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType,cx,cy);
	_DesignLayout();
}

void CDlgSEMLog_OperationQueryKeyWordSettings::_DesignLayout()
{
	_MoveControl(IDC_BUTTON_Add,150,0);
	_MoveControl(IDC_BUTTON_Delete,150,0);
	_MoveControl(IDC_BUTTON_Save,150,0);

	_MoveGrid();
}

void CDlgSEMLog_OperationQueryKeyWordSettings::_MoveGrid()
{
	CRect rc;
	GetClientRect(rc);

	CWnd* pCtrl = GetDlgItem(IDC_Grid);
	if(!pCtrl) return;

	rc.left += 5;
	rc.top +=  5;
	rc.right += 70;
	rc.bottom -= 80;
	pCtrl->MoveWindow(rc);
}


static bool Comp(CFieldDesc* pLeft,CFieldDesc* pRight)
{
	if(!pLeft || !pRight) return false;
	return pLeft->GetAttributeInt("SortID") < pRight->GetAttributeInt("SortID");
}

void CDlgSEMLog_OperationQueryKeyWordSettings::ShowData( map<string,CFieldDesc*> &mapFieldName2FieldDesc, CDataSet* pDs )
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


	//只有一行数据
	if(pDs->Size() != 1) return;

	CFieldDesc* pFieldDesc = vColumn[1];
	if(!pFieldDesc) return;

	CRecord* pRecord = pDs->GetAt(0);
	if(!pRecord) return;

	CField* pField = pRecord->GetField(pFieldDesc->GetFieldID());
	if(!pField) return;

	CString strValue;
	vector<string> vKeyWord;
	const string strDataType = pFieldDesc->m_strDataType;
	const string strDisplayType = pFieldDesc->GetAttributeString("DisplayType");
	if (strDataType.find("string") != string::npos)
	{
		m_sKeyWord = pField->GetValueAsString();
	    Split(m_sKeyWord, ";",vKeyWord);
	}

	vector<pair<int,string>> vShowContent; 
	for (int i = 0; i < vKeyWord.size();i++)
	{
		vShowContent.push_back(make_pair(i + 1,vKeyWord[i]));
	}

	const int   nColumnWidth[2] = {120,322};
	const string sGridHeader[2] = {"序号","关键词"};

	m_nFixCols = 0;
	m_nFixRows = 1;
	m_nCols = vColumn.size();
	m_nRows = vKeyWord.size() + 1;

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
				m_Grid.SetColumnWidth(col,nColumnWidth[col]);
				str.Format(_T("%s"),sGridHeader[col].c_str());
			}
			else if (col < m_nFixCols && row >= m_nFixRows)
			{
				str.Format(_T("%d"), row);
			}
			else if (row >= m_nFixRows && col >= m_nFixCols)
			{
				pair<int,string>& rowInfo = vShowContent[row - 1];
				if (0 == col)
				{
					str.Format(_T("%d"),rowInfo.first);
				}
				else if (1 == col)
				{
					str.Format(_T("%s"),rowInfo.second.c_str());
				}
			
			}

			Item.strText = str;
			m_Grid.SetItemState(row,col, m_Grid.GetItemState(row,col) & ~GVIS_READONLY);			

			m_Grid.SetItem(&Item);
		}
	}

	m_Grid.Invalidate();
	UpdateData(FALSE);
}


void CDlgSEMLog_OperationQueryKeyWordSettings::SaveSettings()
{
	CString sContent = _T("");
	for (int row = 0; row < m_Grid.GetRowCount(); row++)
	{
		for (int col = 0; col < m_Grid.GetColumnCount(); col++)
		{
			if (1 == col)
			{
				CGridCellBase* pCell = m_Grid.GetCell(row,col);
				CString sCellText = pCell->GetText();
				if (m_Grid.GetRowCount() -1 == row)
				{
					sContent += sCellText;
				}
				else
				{

					sContent += sCellText;
					sContent += ";";
				}
			}
		}
	}

	char szSQL[256] = {0};
	sprintf_s(szSQL,256,"Update Settings set content = '%s' where subject = 'OperationKeyWord'",sContent );
	CDBDataManager::Instance().Exec(string(szSQL));
	MessageBox("保存成功！");
}

void CDlgSEMLog_OperationQueryKeyWordSettings::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	SaveSettings();
}
