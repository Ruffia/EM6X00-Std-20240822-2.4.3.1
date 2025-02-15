#pragma once

#include "Resource.h"
// CDlgProgress 对话框

class CDlgProgress : public CDialog
{
	DECLARE_DYNAMIC(CDlgProgress)

public:
	CDlgProgress();
	virtual ~CDlgProgress();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROGRESS };
	CProgressCtrl m_ProgCtrl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

private:
	int m_nTimer;
	BOOL m_bStopProgress;
	CString m_strTitle;
	CString m_strText;

public:
	void SetTitle(LPCTSTR szTitle);
	void SetText(LPCTSTR szText);
	void StopProgress();

	void SetProgressInfo(CString strProgressInfo);
	void SetRange(const int nStepCount);

	void ProgressStep()
	{
		m_ProgCtrl.StepIt();
	}
};
