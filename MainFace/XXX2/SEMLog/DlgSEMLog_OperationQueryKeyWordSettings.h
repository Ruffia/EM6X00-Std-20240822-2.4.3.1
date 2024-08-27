#pragma once
#include "afxcmn.h"
#include "DataSet.h"
#include "afxdtctl.h"
#include "Tools/DlgGridPage.h"

class CField;
class CFieldDesc;

#if _MSC_VER > 1000
#pragma once
#endif 

//操作记录查询，常用关键词
class CDlgSEMLog_OperationQueryKeyWordSettings : public CDlgGridPage
{
	DECLARE_DYNAMIC(CDlgSEMLog_OperationQueryKeyWordSettings)

public:
	CDlgSEMLog_OperationQueryKeyWordSettings(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSEMLog_OperationQueryKeyWordSettings();
	void Localize();

// 对话框数据
	enum { IDD = IDD_DLG_SEMLog_OperationKeyWord };

	void SaveSettings();

protected:
	virtual BOOL OnInitDialog();

	//移动Grid
	void _MoveGrid();

	//设计控件布局
	void _DesignLayout();

	//加载数据
	void _LoadData(const string& sTableName);

	//根据过滤条件显示数据
	void ShowData( map<string,CFieldDesc*> &mapFieldName2FieldDesc, CDataSet* pDs );

protected:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
afx_msg void OnBnClickedButtonSave();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
	CDataSet m_ds;
	string   m_sKeyWord;
};
