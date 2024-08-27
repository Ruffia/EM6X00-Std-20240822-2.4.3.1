#pragma once
#include "afxcmn.h"
#include "Tools/DlgTab.h"

class CDlgSEMLog_Data : public CDlgTab
{
	DECLARE_DYNAMIC(CDlgSEMLog_Data)

public:
	CDlgSEMLog_Data(CWnd* pParent = NULL);   // 标准构造函数

	virtual void InitPage(); 

// 对话框数据
	enum { IDD = IDD_DLG_Tab };

protected:

	DECLARE_MESSAGE_MAP()
};
