#pragma once
#include "resource.h"

// CDlgProgress 对话框

class CDlgProgress : public CDialog
{
	DECLARE_DYNAMIC(CDlgProgress)

public:
	CDlgProgress(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProgress();

// 对话框数据
	enum { IDD = IDD_PROGRESS_CTRL };
	CProgressCtrl	m_progressCtrl;
	CStatic			m_staticPrompt;

	BOOL			m_bIsCancelled;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void OnCancel();
};
