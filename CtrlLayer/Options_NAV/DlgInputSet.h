#pragma once

#include "resource.h"
// CDlgInputSet 对话框

class CDlgInputSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgInputSet)

public:
	CDlgInputSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInputSet();
	void	Localize();

	char	m_cPathLocalize[256];
	CString m_strInput;

// 对话框数据
	enum { IDD = IDD_INPUT_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
};
