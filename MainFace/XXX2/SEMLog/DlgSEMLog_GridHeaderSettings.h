#pragma once
#include "afxcmn.h"
#include "Tools/DlgGridPage.h"
#include "DataSet.h"
#include "afxdtctl.h"

class CField;
class CFieldDesc;
class CCheckComboBox;

#if _MSC_VER > 1000
#pragma once
#endif 

class CDlgSEMLog_GridHeaderSettings : public CDlgGridPage
{
	DECLARE_DYNAMIC(CDlgSEMLog_GridHeaderSettings)

public:
	CDlgSEMLog_GridHeaderSettings(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSEMLog_GridHeaderSettings();

// 对话框数据
	enum { IDD = IDD_DLG_Grid };

protected:
	virtual BOOL OnInitDialog();

	//移动Grid
	void _MoveGrid();

	//设计控件布局
	void _DesignLayout();

	//加载页面数据
	void _LoadPageData();

	//根据过滤条件显示数据
	void ShowData(const vector<tuple<int,string,string>>& vHeader);

	//获取单元格数据
	void GetCellData( const vector<tuple<int,string,string>>& vHeader,const int row, const int col, CString &str );

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnLoadPageData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	CDataSet m_ds;
};
