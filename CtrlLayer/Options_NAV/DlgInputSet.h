#pragma once

#include "resource.h"
// CDlgInputSet �Ի���

class CDlgInputSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgInputSet)

public:
	CDlgInputSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInputSet();
	void	Localize();

	char	m_cPathLocalize[256];
	CString m_strInput;

// �Ի�������
	enum { IDD = IDD_INPUT_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
};
