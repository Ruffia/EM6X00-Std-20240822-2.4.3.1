#pragma once
#include "afxcmn.h"

#if _MSC_VER > 1000
#pragma once
#endif 

class CDlgSEMLog_DeleteData : public CDialog
{
	DECLARE_DYNAMIC(CDlgSEMLog_DeleteData)

public:
	CDlgSEMLog_DeleteData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSEMLog_DeleteData();

// 对话框数据
	enum { IDD = IDD_DLG_SEMLog_DeleteData };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonDelete();
	DECLARE_MESSAGE_MAP()

protected:
	//将UI上的时间转换成时间戳(毫秒),便于数据库查询
	__int64 _GetTimeStamp(bool bBeginDateTime);

protected:
	//时间控件
	CDateTimeCtrl m_DayBegin;
	CDateTimeCtrl m_TimeBegin;
	CDateTimeCtrl m_DayEnd;
	CDateTimeCtrl m_TimeEnd;
};
