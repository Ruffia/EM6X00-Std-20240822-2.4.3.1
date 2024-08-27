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

class CDlgSEMLog_ShowAlarm : public CDlgGridPage
{
	DECLARE_DYNAMIC(CDlgSEMLog_ShowAlarm)

public:
	CDlgSEMLog_ShowAlarm(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSEMLog_ShowAlarm();
	void	Localize(void);

// 对话框数据
	enum { IDD = IDD_DLG_SEMLog_Alarm };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	//创建多选下拉框
	void _CreateCheckCombox();

	//移动Grid
	void _MoveGrid();

	//设计控件布局
	void _DesignLayout();


	//将UI上的时间转换成时间戳(毫秒),便于数据库查询
	__int64 _GetTimeStamp(bool bBeginDateTime);

	//获取过滤字段
	//strFieldSelect,ui上勾选的字段名称
	//vSelectFieldID, 输出参数，保存字段id
	void _GetFieldID4Filter(const string strFieldSelect, vector<string> &vSelectFieldID );

	void _QueryData(const string strFieldSelect, const string sTableName );

	//加载页面数据
	//nType = 0, 加载；nType = 1, 前一页；nType = 1, 下一页。
	void _LoadPageData(const int nType);

	//根据过滤条件显示数据
	void ShowData( map<string,CFieldDesc*> &mapFieldName2FieldDesc, CDataSet* pDs );

	//获取单元格数据
	void GetCellData( CDataSet* pDs,vector<CFieldDesc*>& vColumn,int row, int col, CString &str );

protected:
	afx_msg void OnBnClickedButtonPreviouspage();
	afx_msg void OnBnClickedButtonNextpage();
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnLoadPageData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	//多选框
	CCheckComboBox*	m_pMultiSelectCombo;
	//时间控件
	CDateTimeCtrl m_DayBegin;
	CDateTimeCtrl m_TimeBegin;
	CDateTimeCtrl m_DayEnd;
	CDateTimeCtrl m_TimeEnd;

public:
	CDataSet m_ds;
	CString m_strPageNO;
	int  m_nCurrentPage;  //当前页码
	int  m_nTotalPage;    //总页码
	const int  m_nRecordCountPerPage;
};
