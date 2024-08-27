#pragma once
#include "resource.h"

// CDlgProgress �Ի���

class CDlgProgress : public CDialog
{
	DECLARE_DYNAMIC(CDlgProgress)

public:
	CDlgProgress(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgProgress();

// �Ի�������
	enum { IDD = IDD_PROGRESS_CTRL };
	CProgressCtrl	m_progressCtrl;
	CStatic			m_staticPrompt;

	BOOL			m_bIsCancelled;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void OnCancel();
};
